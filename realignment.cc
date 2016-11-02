#include "parameters.h"
#include "../common.h"

#include "TFile.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TH1D.h"
#include "TF1.h"
#include "TMath.h"

#include <vector>
#include <map>
#include <cmath>

using namespace std;

//----------------------------------------------------------------------------------------------------

bool saveDetails = true;

void DoVerticalRealignment(TGraph *g_p, TGraph *g_n)
{
	TDirectory *baseDir = gDirectory;

	char buf[100];

	double *th_y_p = g_p->GetX(), *w_p = g_p->GetY();
	double *th_y_n = g_n->GetX(), *w_n = g_n->GetY();

	// selected th_y region
	double th_y_min = 45E-6, th_y_max = 100E-6;

	// result variables
	double bsh_min_diff=0., bsh_prob=0., bsh_mean_diff_sq=0., bsh_hist_chi_sq=0.;

	// match graphs
	TGraph *g_max_diff = new TGraph(); g_max_diff->SetName("g_max_diff");
	TGraph *g_prob = new TGraph(); g_prob->SetName("g_prob");
	TGraph *g_z = new TGraph(); g_z->SetName("g_z");
	TGraph *g_mean_diff_sq = new TGraph(); g_mean_diff_sq->SetName("g_mean_diff_sq");
	TGraph *g_hist_chi_sq = new TGraph(); g_hist_chi_sq->SetName("g_hist_chi_sq");	

	// find the best match (loop over shifts)
	double sh_min = -4E-6, sh_max = +2E-6;
	double min_diff = 1E100, max_prob = 0.;
	for (double sh = sh_min; sh <= sh_max; sh += 0.2E-6) {
		if (saveDetails) {
			sprintf(buf, "shift = %+.1f um", sh*1E6);
			gDirectory = baseDir->mkdir(buf);
		}

		// build th_y histograms
		double bound_low = max(th_y_min, th_y_min + sh);
		double bound_high = min(th_y_max, th_y_max + sh);

		unsigned int N_bins = 100;
		TH1D *h_th_y_p = new TH1D("h_th_y_p", "", N_bins, bound_low, bound_high);
		for (int i = 0; i < g_p->GetN(); i++) {
			double th_y = th_y_p[i];
			if (th_y > bound_low && th_y < bound_high)
				h_th_y_p->Fill(th_y, w_p[i]);
		}
		h_th_y_p->Sumw2();
		h_th_y_p->SetLineColor(2);
		
		TH1D *h_th_y_n = new TH1D("h_th_y_n", "", N_bins, bound_low, bound_high);
		for (int i = 0; i < g_n->GetN(); i++) {
			double th_y = -th_y_n[i] + sh;
			if (th_y > bound_low && th_y < bound_high)
				h_th_y_n->Fill(th_y, w_n[i]);
		}
		h_th_y_n->Sumw2();
		h_th_y_n->SetLineColor(4);

		// histogram chi^2 dist
		double hist_chi_sq = 0.;
		int hist_n = 0;
		for (int i = 1; i <= h_th_y_p->GetNbinsX(); i++) {
			double diff = h_th_y_p->GetBinContent(i) - h_th_y_n->GetBinContent(i);
			double eff_err_sq = h_th_y_p->GetBinError(i) * h_th_y_n->GetBinError(i);
			if (eff_err_sq <= 0.)
				continue;
			hist_chi_sq += diff*diff / eff_err_sq;
			hist_n++;
		}

		if (hist_n > 0) {
			hist_chi_sq /= hist_n;
			g_hist_chi_sq->SetPoint(g_hist_chi_sq->GetN(), sh, hist_chi_sq);
		}

		// save debug plots
		if (saveDetails) {
			TCanvas *c = new TCanvas("th_y cmp");
			h_th_y_p->Draw();
			h_th_y_n->Draw("same");
			c->Write();
		}

		// build increase maps
		map<double, unsigned int> m_p;
		unsigned int norm_p = 0;
		for (int i = 0; i < g_p->GetN(); i++) {
			double th_y = th_y_p[i];
			if (th_y > bound_low && th_y < bound_high) {
				m_p[th_y] += w_p[i];
				norm_p++;
			}
		}
		
		map<double, unsigned int> m_n;
		unsigned int norm_n = 0;
		for (int i = 0; i < g_n->GetN(); i++) {
			double th_y = -th_y_n[i] + sh;
			if (th_y > bound_low && th_y < bound_high) {
				m_n[th_y] += w_n[i];
				norm_n++;
			}
		}

		// build th_y experimental CDF

		//--------------------------------------------------
		// build sum vectors S
		double norm_m = (norm_p + norm_n) / 2.;
		double sum = 0.;
		vector<double> S_p;
		S_p.reserve(m_p.size());
		S_p.push_back(0.);
		TGraph *g_p = new TGraph();
		for (map<double, unsigned int>::iterator it = m_p.begin(); it != m_p.end(); ++it) {
			//sum += double(it->second) / norm_p;
			sum += double(it->second) / norm_m;
			S_p.push_back(sum);
			g_p->SetPoint(g_p->GetN(), it->first, sum);
		}
		
		vector<double> S_n;
		S_n.reserve(m_n.size());
		S_n.push_back(0.);
		TGraph *g_n = new TGraph();
		sum = 0.;
		for (map<double, unsigned int>::iterator it = m_n.begin(); it != m_n.end(); ++it) {
			//sum += double(it->second) / norm_n;
			sum += double(it->second) / norm_m;
			S_n.push_back(sum);
			g_n->SetPoint(g_n->GetN(), it->first, sum);
		}

		if (saveDetails) {
			TCanvas *c = new TCanvas("F cmp");
			g_p->SetLineColor(2);
			g_p->Draw("al");
			g_n->SetLineColor(4);
			g_n->Draw("l");
			c->Write();	
			delete c;
		}

		// build sorted array of all increase points
		map<double, unsigned int> ipa;
		for (map<double, unsigned int>::iterator it = m_p.begin(); it != m_p.end(); ++it)
			ipa[it->first] += 1;
		for (map<double, unsigned int>::iterator it = m_n.begin(); it != m_n.end(); ++it)
			ipa[it->first] += 2;

		// find the maximum vertical difference
		unsigned int idx_p = 0, idx_n = 0;
		double max_diff = 0.;
		double sum_diff_sq = 0.;
		unsigned int n_diff = 0;
		TGraph *g_F_diff = new TGraph(); g_F_diff->SetName("g_F_diff"); g_F_diff->SetLineColor(6);
		for (map<double, unsigned int>::iterator it = ipa.begin(); it != ipa.end(); ++it) {
			unsigned int num = it->second;
			double y = it->first;

			if ((num & 0x1) > 0)
				idx_p++;
			if ((num & 0x2) > 0)
				idx_n++;

			double v_p = S_p[idx_p];
			double v_n = S_n[idx_n];
			double diff = v_n - v_p;
			max_diff = max(max_diff, fabs(diff));
			n_diff++;
			sum_diff_sq += diff*diff;

			g_F_diff->SetPoint(g_F_diff->GetN(), y, diff);
			//printf("\t\t\tnum=%u, idx_p=%u, idx_n=%u | x=%E | v_p=%E, v_n=%E, diff=%E, max_diff after=%E\n", num, idx_p, idx_n, x, v_p, v_n, diff, max_diff);
		}

		if (saveDetails)
			g_F_diff->Write();

		//double kCorr = sqrt(double(norm_p) * double(norm_n) / (norm_p + norm_n));
		double kCorr = sqrt(double(norm_m) / 2.);

		double z = max_diff * kCorr;
		double prob = TMath::KolmogorovProb(z);

		if (n_diff > 0) {
			double mean_diff_sq = sum_diff_sq / n_diff;
			g_mean_diff_sq->SetPoint(g_mean_diff_sq->GetN(), sh, mean_diff_sq);
		}

		if (norm_p > 0 && norm_n > 0) {
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

		// clean
		delete h_th_y_p;
		delete h_th_y_n;
		delete g_p;
		delete g_n;
		delete g_F_diff;
	}

	// fit and save match graphs
	TF1 *ff = new TF1("ff", "[0] + [1]*x + [2]*x*x");
	g_hist_chi_sq->Fit(ff, "Q", "", bsh_prob-0.5, bsh_prob+0.5);
	bsh_hist_chi_sq = - ff->GetParameter(1) / 2. / ff->GetParameter(2);
	
	g_mean_diff_sq->Fit(ff, "Q", "", bsh_prob-0.5, bsh_prob+0.5);
	bsh_mean_diff_sq = - ff->GetParameter(1) / 2. / ff->GetParameter(2);

	if (saveDetails) {
		gDirectory = baseDir;
		g_max_diff->Write();
		g_z->Write();
		g_prob->Write();
		g_mean_diff_sq->Write();
		g_hist_chi_sq->Write();
	}
}

//----------------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{
	if (argc != 2)
		return 1;

	// init diagonal
	Init(argv[1]);
	if (diagonal == d45t)
		return 0;

	// get input
	TFile *inF_45b = new TFile("distributions_45b_56t.root");
	TGraph *g_45b = (TGraph *) inF_45b->Get("acceptance correction/g_weight_vs_th_y");
	
	TFile *inF_45t = new TFile("distributions_45t_56b.root");
	TGraph *g_45t = (TGraph *) inF_45t->Get("acceptance correction/g_weight_vs_th_y");

	// prepare output
	TFile *outF = new TFile("realignment.root", "recreate");

	DoVerticalRealignment(g_45b, g_45t);

	delete outF;
	return 0;
}
