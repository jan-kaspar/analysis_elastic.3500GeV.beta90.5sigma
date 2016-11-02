#include "parameters.h"
#include "../common.h"

#include "TFile.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TH1D.h"
#include "TF1.h"
#include "TMath.h"
#include "TProfile.h"

#include <vector>
#include <map>
#include <cmath>
#include <algorithm>

using namespace std;

//----------------------------------------------------------------------------------------------------

bool saveDetails = false;

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

void DoHorizontalProfile(TGraph *g_t, TGraph *g_b)
{
	printf(">> DoHorizontalProfile\n");

	TProfile *p = new TProfile("p", ";y   (mm);mean x   (mm)", 200, -40., +40.);

	for (int i = 0; i < g_t->GetN(); i++) {
		double x, y;
		g_t->GetPoint(i, x, y);
		if (fabs(y) > 8. && fabs(y) < 35.)
			p->Fill(y, x);
	}
	
	for (int i = 0; i < g_b->GetN(); i++) {
		double x, y;
		g_b->GetPoint(i, x, y);
		if (fabs(y) > 8. && fabs(y) < 35.)
			p->Fill(y, x);
	}

	TF1 *ff = new TF1("ff", "[0] + x*[1]");
	p->Fit(ff, "Q", "", -25., +25.);
	p->Write();

	printf("\ta = %.2f +- %.2f mrad\n", ff->GetParameter(1)*1E3, ff->GetParError(1)*1E3);
	printf("\tb = %.1f +- %.1f um\n", ff->GetParameter(0)*1E3, ff->GetParError(0)*1E3);
}

//----------------------------------------------------------------------------------------------------

double a, b, ae, be;

/**
	\par sigma_w sigma for weight calculation
	\par sigma_u sigma for uncertainty calculation
**/
void OneFitIteration(TGraph *g, vector<double> &ws, double sigma_u, double sigma_w)
{
	// transformation to xi-eta space
	double al = atan(a);
	double C = cos(al), S = sin(al);

	// fit in xi-eta space
	double Sw = 0., Swx = 0., Swe = 0., Swxe = 0., Swxx = 0.;
	for (int i = 0; i < g->GetN(); i++) {
		double x = 0., y = 0.;
		g->GetPoint(i, x, y);
		double w = ws[i];

		double xi = C*x + S*(y-b);
		double eta = -S*x + C*(y-b);

		Sw += w;
		Swx += w*xi;
		Swe += w*eta;
		Swxe += w*xi*eta;
		Swxx += w*xi*xi;
	}

	// calculate corrections to a and b
	double det = - Swx*Swx + Swxx*Sw;
	double ap = (Sw*Swxe - Swx*Swe) / det;
	double bp = (-Swx*Swxe + Swxx*Swe) / det;

	double alp = atan(ap);
	a = tan(al + alp);
	b += bp * (cos(al) + sin(al)*tan(al+alp));

	ae = sqrt(Sw / det) * sigma_u;
	be = sqrt(Swxx / det) * sigma_u;
	
	// recompute weights
	for (int i = 0; i < g->GetN(); i++) {
		double x = 0., y = 0.;
		g->GetPoint(i, x, y);

		//double xi = C*x + S*(y-b);
		double eta = -S*x + C*(y-b);

		double si = sigma_w;
		double nd = eta/si;
		double cH = 1.345;
		double w = (nd <= cH) ? 1. : cH / nd;
		w *= w;
		ws[i] = w;
	}
}

//----------------------------------------------------------------------------------------------------

void DoHorizontalGraphFit(TGraph *g_t, TGraph *g_b)
{
	printf(">> DoHorizontalGraphFit\n");

	double y_min = 8., y_max = 25.;

	// merge graphs, invert x <--> y
	TGraph *g = new TGraph();
	TH1D *h_x = new TH1D("h_x", ";x   (mm)", 100, 0., 0.);
	g->SetName("merged");
	g->SetTitle(";y   (mm);x   (mm)");
	for (int i = 0; i < g_t->GetN(); i++) {
		double x, y;
		g_t->GetPoint(i, x, y);
		if (fabs(y) >= y_min && fabs(y) <= y_max) {
			g->SetPoint(g->GetN(), y, x);
			h_x->Fill(x);
		}
	}

	for (int i = 0; i < g_b->GetN(); i++) {
		double x, y;
		g_b->GetPoint(i, x, y);
		if (fabs(y) >= y_min && fabs(y) <= y_max) {
			g->SetPoint(g->GetN(), y, x);
			h_x->Fill(x);
		}
	}

	h_x->Write();
	double si_x = h_x->GetRMS();

	// reset fit parameters
	a = b = ae = be = 0.;

	// reset weights
	vector<double> w;
	for (int i = 0; i < g->GetN(); i++)
		w.push_back(1.);

	// run iterations
	unsigned int iterations = 2;
	for (unsigned int i = 0; i < iterations; i++) {
		//printf("    iteration %u\n", i);
		OneFitIteration(g, w, si_x, 10.);
		//printf("	a = %.2E +- %.2E, b = %.2E +- %.2E\n", a, ae, b, be);
	}
	
	printf("\ta = %.2f +- %.2f mrad\n", a*1E3, ae*1E3);
	printf("\tb = %.1f +- %.1f um\n", b*1E3, be*1E3);

	if (true) {
		TCanvas *c = new TCanvas();
		c->SetName("horizontal fit");
		g->Draw("ap");
		TF1 *ff = new TF1("ff", "[0]*x + [1]", -10, +10);
		ff->SetParameters(a, b);
		ff->Draw("same");
		c->Write();
	}


	delete g;
}

//----------------------------------------------------------------------------------------------------

void DoHorizontalAlignment(TGraph *g_t, TGraph *g_b)
{
	TDirectory *baseDir = gDirectory;
	
	gDirectory = baseDir->mkdir("horizontal profile");
	DoHorizontalProfile(g_t, g_b);

	gDirectory = baseDir->mkdir("horizontal graph fit");
	DoHorizontalGraphFit(g_t, g_b);
}

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

void DoVerticalAlignment(TGraph *g_t, double corr_t, TGraph *g_b, double corr_b)
{
	printf(">> DoVerticalAlignment\n");
	printf("\tcorr_t = %.3f\n", corr_t);
	printf("\tcorr_b = %.3f\n", corr_b);

	double bs_y_cut = 2.;	// 3 * si_th_y * L_y
	printf("\tbs_y_cut = %.3f mm\n", bs_y_cut);

	// prepare samples, determine ranges, 
	double y_max = 25.;

	TH1D *y_hist = new TH1D("y_hist", "", 500, -50, +50);
	
	double y_min_b = 1E100, y_min_t = 1E100;
	double y_max_b = -1E100, y_max_t = -1E100;
	
	vector<double> sample_t, sample_b;	// in mm

	double *ya = g_t->GetY();
	for (int i = 0; i < g_t->GetN(); i++) {
		//if ((i % 2) == 0)
		//.	continue;

		double y = ya[i];

		if (y <= 0.)
			printf("ERROR top\n");

		if (fabs(y) > y_max)
			continue;
		
		y_min_t = min(y_min_t, y);
		y_max_t = max(y_max_t, y);
		y_hist->Fill(y, corr_t);
		
		sample_t.push_back(y);
	}
	
	ya = g_b->GetY();
	for (int i = 0; i < g_b->GetN(); i++) {
		//.if ((i % 2) == 0)
		//.	continue;

		double y = ya[i];

		if (y >= 0.)
			printf("ERROR bottom\n");

		if (fabs(y) > y_max)
			continue;
		
		y_min_b = min(y_min_b, -y);
		y_max_b = max(y_max_b, -y);
		y_hist->Fill(y, corr_b);
		
		sample_b.push_back(-y);
	}

	y_hist->Write();
	delete y_hist;

	printf("\tbefore cut\n");
	printf("\t\ty_min_b = %.3f mm, y_min_t = %.3f mm\n", y_min_b, y_min_t);

	y_min_b += bs_y_cut;
	y_min_t += bs_y_cut;

	y_max_t = min(y_max_t, 40.);
	y_max_b = min(y_max_b, 40.);

	printf("\tafter cut\n");
	printf("\t\ty_min_b = %.3f mm, y_min_t = %.3f mm\n", y_min_b, y_min_t);
	printf("\t\ty_max_b = %.3f mm, y_max_t = %.3f mm\n", y_max_b, y_max_t);

	//printf("\t\t\t - sorting\n");
	sort(sample_t.begin(), sample_t.end());
	sort(sample_b.begin(), sample_b.end());

	// determine shift range
	double de_w = (y_max_t - y_min_t) - (y_max_b - y_min_b);
	double s_min = min(0., de_w), s_max = max(0., de_w);
	s_min = -1.5; s_max = +1.5;
	double s_step = 0.02;
	printf("\tshift range: %.2E to %.2E, shift step = %.2E\n", s_min, s_max, s_step);

	// result variables
	double bsh_min_diff=0., bsh_prob=0., bsh_mean_diff_sq=0., bsh_hist_chi_sq=0.;
	double de_y_min_diff=0., de_y_prob=0., de_y_mean_diff_sq=0., de_y_hist_chi_sq=0.;

	// find the best match
	TGraph *g_max_diff = new TGraph(); g_max_diff->SetName("g_max_diff");
	TGraph *g_prob = new TGraph(); g_prob->SetName("g_prob");
	TGraph *g_z = new TGraph(); g_z->SetName("g_z");
	TGraph *g_mean_diff_sq = new TGraph(); g_mean_diff_sq->SetName("g_mean_diff_sq");
	TGraph *g_hist_chi_sq = new TGraph(); g_hist_chi_sq->SetName("g_hist_chi_sq");
	double min_diff = 1E100, max_prob = 0.;
	TDirectory *baseDir = gDirectory;

	for (double sh = s_min; sh <= s_max; sh += s_step) {
		char buf[100];

		bool saveExtra = (sh > -0.5*s_step && sh < 0.5*s_step);

		if (saveDetails || saveExtra) {
			sprintf(buf, "shift = %.3f", sh);
			gDirectory = baseDir->mkdir(buf);
		}

		//printf("\tsh = %.3f mm\n", sh);

		// determine actual boundaries
		double bot_sh = - y_min_b + y_min_t + sh;
		double y_min = max(y_min_t, y_min_b + bot_sh), y_max = min(y_max_t, y_max_b + bot_sh);
		//printf("\t\ty_min = %.3f mm, y_max = %.3f mm\n", y_min, y_max);

		// build increase maps
		map<double, unsigned int> m_t;
		unsigned int norm_t = 0;
		for (unsigned int i = 0; i < sample_t.size(); i++) {
			double y = sample_t[i];
			if (y >= y_min && y < y_max) {
				//printf("y1: %E\n", y);
				m_t[y]++;
				norm_t++;
			}
		}

		map<double, unsigned int> m_b;
		unsigned int norm_b = 0;
		for (unsigned int i = 0; i < sample_b.size(); i++) {
			double y = sample_b[i] + bot_sh;
			//printf("y2: %E\n", y);
			if (y >= y_min && y < y_max) {
				m_b[y]++;
				norm_b++;
			}
		}
		
		//printf("\t\tnorm_t = %u, norm_b = %u\n", norm_t, norm_b);
	
		// build y histograms
		//unsigned int N_bins = (norm_t + norm_b) / 1000;
		unsigned int N_bins = 150;

		//printf("\t\tN_bins = %u\n", N_bins);

		TH1D *y_hist_t = new TH1D("y_hist_t", "", N_bins, y_min, y_max);
		for (map<double, unsigned int>::iterator it = m_t.begin(); it != m_t.end(); ++it)
			y_hist_t->Fill(it->first, double(it->second) * corr_t);
		y_hist_t->SetLineColor(2);

		TH1D *y_hist_b = new TH1D("y_hist_b", "", N_bins, y_min, y_max);
		for (map<double, unsigned int>::iterator it = m_b.begin(); it != m_b.end(); ++it)
			y_hist_b->Fill(it->first, double(it->second) * corr_b);
		y_hist_b->SetLineColor(4);
		
		// histogram chi^2 dist
		double hist_chi_sq = 0.;
		int hist_n = 0;
		for (int i = 1; i <= y_hist_t->GetNbinsX(); i++) {
			double diff = y_hist_t->GetBinContent(i) - y_hist_b->GetBinContent(i);
			double eff_err_sq = y_hist_t->GetBinError(i) * y_hist_b->GetBinError(i);
			if (eff_err_sq <= 0)
				continue;
			hist_chi_sq += diff*diff / eff_err_sq;
			hist_n++;
		}

		if (hist_n > 0) {
			hist_chi_sq /= hist_n;
			g_hist_chi_sq->SetPoint(g_hist_chi_sq->GetN(), sh, hist_chi_sq);
		}
		//printf("\t\tmean hist chi^2 = %E, contribution points %u\n", hist_chi_sq, hist_n);
		
		// save debug plots
		if (saveDetails || saveExtra) {
			y_hist_t->Write();
			y_hist_b->Write();

			TCanvas *c = new TCanvas();
			c->SetName("y hist cmp");
			y_hist_t->Draw();
			y_hist_b->Draw("sames");
			c->Write();
			delete c;

			// build cumulative graphs
			TGraph *g_y_cumul_t = new TGraph(); g_y_cumul_t->SetLineColor(2);
			TGraph *g_y_cumul_b = new TGraph(); g_y_cumul_b->SetLineColor(4);
			double sum = 0.;
			for (int i = 1; i <= y_hist_t->GetNbinsX(); i++) {
				sum += y_hist_t->GetBinContent(i);
				g_y_cumul_t->SetPoint(g_y_cumul_t->GetN(), y_hist_t->GetBinCenter(i), sum);
			}
			sum = 0.;
			for (int i = 1; i <= y_hist_b->GetNbinsX(); i++) {
				sum += y_hist_b->GetBinContent(i);
				g_y_cumul_b->SetPoint(g_y_cumul_b->GetN(), y_hist_b->GetBinCenter(i), sum);
			}
			c = new TCanvas();
			c->SetName("y cumul cmp");
			g_y_cumul_t->Draw("al");
			g_y_cumul_b->Draw("l");
			c->Write();

			delete g_y_cumul_t;
			delete g_y_cumul_b;
			delete c;
		}

		// build sum vectors S
		double norm_m = (norm_t + norm_b) / 2.;
		double sum = 0;
		vector<double> S_t;
		S_t.reserve(m_t.size());
		S_t.push_back(0.);
		TGraph *g_t = new TGraph();
		for (map<double, unsigned int>::iterator it = m_t.begin(); it != m_t.end(); ++it) {
			sum += double(it->second) / norm_m * corr_t;
			S_t.push_back(sum);
			g_t->SetPoint(g_t->GetN(), it->first, sum);
		}
		
		vector<double> S_b;
		S_b.reserve(m_b.size());
		S_b.push_back(0.);
		TGraph *g_b = new TGraph();
		sum = 0.;
		for (map<double, unsigned int>::iterator it = m_b.begin(); it != m_b.end(); ++it) {
			sum += double(it->second) / norm_m * corr_b;
			S_b.push_back(sum);
			g_b->SetPoint(g_b->GetN(), it->first, sum);
		}

		if (saveDetails || saveExtra) {
			TCanvas *c = new TCanvas();
			c->SetName("F cmp");
			g_t->SetLineColor(2);
			g_t->Draw("al");
			g_b->SetLineColor(4);
			g_b->Draw("l");
			c->Write();	
			delete c;
		}

		// build sorted array of all increase points
		map<double, unsigned int> ipa;
		for (map<double, unsigned int>::iterator it = m_t.begin(); it != m_t.end(); ++it)
			ipa[it->first] += 1;
		for (map<double, unsigned int>::iterator it = m_b.begin(); it != m_b.end(); ++it)
			ipa[it->first] += 2;

		// find the maximum vertical difference
		unsigned int idx_t = 0, idx_b = 0;
		double max_diff = 0.;
		double sum_diff_sq = 0.;
		unsigned int n_diff = 0;
		TGraph *g_F_diff = new TGraph(); g_F_diff->SetName("g_F_diff"); g_F_diff->SetLineColor(6);
		for (map<double, unsigned int>::iterator it = ipa.begin(); it != ipa.end(); ++it) {
			unsigned int num = it->second;
			double y = it->first;

			if ((num & 0x1) > 0)
				idx_t++;
			if ((num & 0x2) > 0)
				idx_b++;

			double v_t = S_t[idx_t];
			double v_b = S_b[idx_b];
			double diff = v_b - v_t;
			max_diff = max(max_diff, fabs(diff));
			n_diff++;
			sum_diff_sq += diff*diff;

			g_F_diff->SetPoint(g_F_diff->GetN(), y, diff);
			//printf("\t\t\tnum=%u, idx_t=%u, idx_b=%u | x=%E | v_t=%E, v_b=%E, diff=%E, max_diff after=%E\n", num, idx_t, idx_b, x, v_t, v_b, diff, max_diff);
		}

		if (saveDetails || saveExtra)
			g_F_diff->Write();


		//double kCorr = sqrt(double(norm_t) * double(norm_b) / (norm_t + norm_b));
		double kCorr = sqrt(double(norm_m) / 2.);

		double z = max_diff * kCorr;
		double prob = TMath::KolmogorovProb(z);

		if (n_diff > 0) {
			double mean_diff_sq = sum_diff_sq / n_diff;
			g_mean_diff_sq->SetPoint(g_mean_diff_sq->GetN(), sh, mean_diff_sq);
		}

		if (norm_t > 0 && norm_b > 0) {
			g_prob->SetPoint(g_prob->GetN(), sh, prob);
			g_z->SetPoint(g_z->GetN(), sh, z);
			g_max_diff->SetPoint(g_max_diff->GetN(), sh, max_diff);

			if (min_diff > max_diff) {
				min_diff = max_diff;
				bsh_min_diff = sh;
			}
	
			if (max_prob < prob) {
				max_prob = prob;
				bsh_prob = sh;
			}
		}

		//printf("\t\tmax_diff = %E, z = %E (corr %E), prob = %E\n", max_diff, z, kCorr, prob);

		// clean
		delete y_hist_t;
		delete y_hist_b;
		delete g_t;
		delete g_b;
		delete g_F_diff;
	}

	TF1 *ff = new TF1("ff", "[0] + [1]*x + [2]*x*x");
	
	double fit_margin = 0.3;
	g_mean_diff_sq->Fit(ff, "Q", "", bsh_prob-fit_margin, bsh_prob+fit_margin);
	bsh_mean_diff_sq = - ff->GetParameter(1) / 2. / ff->GetParameter(2);

	fit_margin = 0.7;
	g_hist_chi_sq->Fit(ff, "Q", "", bsh_prob-fit_margin, bsh_prob+fit_margin);
	bsh_hist_chi_sq = - ff->GetParameter(1) / 2. / ff->GetParameter(2);
	

	gDirectory = baseDir;
	g_max_diff->Write();
	g_z->Write();
	g_prob->Write();
	g_mean_diff_sq->Write();
	g_hist_chi_sq->Write();

	printf("\tbest shifts: min_diff = %.3f, prop = %.3f, mean_diff_sq = %.3f, hist_chi_sq = %.3f\n",
		bsh_min_diff, bsh_prob, bsh_mean_diff_sq, bsh_hist_chi_sq);

	de_y_min_diff = - (-y_min_b + y_min_t + bsh_min_diff) / 2.;
	de_y_prob = - (-y_min_b + y_min_t + bsh_prob) / 2.;
	de_y_mean_diff_sq = - (-y_min_b + y_min_t + bsh_mean_diff_sq) / 2.;
	de_y_hist_chi_sq = - (-y_min_b + y_min_t + bsh_hist_chi_sq) / 2.;
	
	printf("\tde_y:\n\t\tmin_diff :     %.0f um\n\t\tprop :         %.0f um\n\t\tmean_diff_sq : %.0f um\n\t\thist_chi_sq :  %.0f um\n",
		de_y_min_diff*1E3, de_y_prob*1E3, de_y_mean_diff_sq*1E3, de_y_hist_chi_sq*1E3);

	double center = - (de_y_min_diff + de_y_mean_diff_sq) / 2.;
	printf("\tcenter:\n\t\t%.0f um\n", center*1E3);
}

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

void DoVerticalRelNFAlignment(TGraph *g_t_n, TGraph *g_t_f, TGraph *g_b_n, TGraph *g_b_f)
{
	printf(">> DoVerticalRelNFAlignment\n");

	TProfile *p = new TProfile("p", ";y^{N}   (mm);y^{F}-y^{N}   (mm)", 300, -30., +30.);

	for (int i = 0; i < g_t_n->GetN(); i++) {
		double xn, yn, xf, yf;
		g_t_n->GetPoint(i, xn, yn);
		g_t_f->GetPoint(i, xf, yf);

		if (fabs(yn) > 7.)
			p->Fill(yn, yf-yn);
	}
	
	for (int i = 0; i < g_b_n->GetN(); i++) {
		double xn, yn, xf, yf;
		g_b_n->GetPoint(i, xn, yn);
		g_b_f->GetPoint(i, xf, yf);

		if (fabs(yn) > 7.)
			p->Fill(yn, yf-yn);
	}
	
	TF1 *ff = new TF1("ff", "[0] + x*[1]");
	ff->SetLineColor(2);
	ff->SetLineWidth(1);
	p->Fit(ff, "Q", "", -25., +25.);
	p->Write();

	printf("\ta = %.2f +- %.2f mrad\n", ff->GetParameter(1)*1E3, ff->GetParError(1)*1E3);
	printf("\tb = %.1f +- %.1f um\n", ff->GetParameter(0)*1E3, ff->GetParError(0)*1E3);
}

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{
	if (argc != 2)
		return 1;

	// init diagonal
	Init(argv[1]);
	if (diagonal == d45t)
		return 0;

	// get per-diagonal efficiency corrections
	Init("45b_56t");
	double eff_corr_45b = full_eff_corr;
	Init("45t_56b");
	double eff_corr_45t = full_eff_corr;

	// input files
	TFile *inF_45b = new TFile("distributions_45b_56t.root");
	TFile *inF_45t = new TFile("distributions_45t_56b.root");
	
	// prepare output
	TFile *outF = new TFile("alignment.root", "recreate");

	char buf[1000];

	vector<string> units;
	units.push_back("L_F");
	units.push_back("L_N");
	units.push_back("R_N");
	units.push_back("R_F");

	for (unsigned int ui = 0; ui < units.size(); ui++) {
		printf("\n---------- unit %s ----------\n\n", units[ui].c_str());

		TGraph *g_t = (TGraph *) inF_45t->Get((string("hit distributions/g_y_")+units[ui]+"_vs_x_"+units[ui]+"_sel").c_str());
		TGraph *g_b = (TGraph *) inF_45b->Get((string("hit distributions/g_y_")+units[ui]+"_vs_x_"+units[ui]+"_sel").c_str());

		double corr_t = eff_corr_45t, corr_b = eff_corr_45b;

		if (units[ui][0] == 'R') {
			swap(g_t, g_b);
			swap(corr_t, corr_b);
		}

		sprintf(buf, "unit %s", units[ui].c_str());
		TDirectory *unitDir = outF->mkdir(buf);
		
		gDirectory = unitDir->mkdir("horizontal");
		DoHorizontalAlignment(g_t, g_b);
		
		gDirectory = unitDir->mkdir("vertical");
		DoVerticalAlignment(g_t, corr_t, g_b, corr_b);
	}

	vector<string> arms;
	arms.push_back("L");
	arms.push_back("R");

	for (unsigned int ai = 0; ai < arms.size(); ai++) {
		printf("\n---------- arm %s ----------\n\n", arms[ai].c_str());
		
		sprintf(buf, "arm %s", arms[ai].c_str());
		TDirectory *armDir = outF->mkdir(buf);

		sprintf(buf, "hit distributions/g_y_%s_N_vs_x_%s_N_sel", arms[ai].c_str(), arms[ai].c_str()); TGraph *g_t_n = (TGraph *) inF_45b->Get(buf);
		sprintf(buf, "hit distributions/g_y_%s_F_vs_x_%s_F_sel", arms[ai].c_str(), arms[ai].c_str()); TGraph *g_t_f = (TGraph *) inF_45b->Get(buf);
		sprintf(buf, "hit distributions/g_y_%s_N_vs_x_%s_N_sel", arms[ai].c_str(), arms[ai].c_str()); TGraph *g_b_n = (TGraph *) inF_45t->Get(buf);
		sprintf(buf, "hit distributions/g_y_%s_F_vs_x_%s_F_sel", arms[ai].c_str(), arms[ai].c_str()); TGraph *g_b_f = (TGraph *) inF_45t->Get(buf);

		gDirectory = armDir->mkdir("vertical rel NF");
		DoVerticalRelNFAlignment(g_t_n, g_t_f, g_b_n, g_b_f);
	}

	delete outF;
	return 0;
}
