#include "parameters.h"
#include "../common.h"

#include "TFile.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TChain.h"
#include "TH1D.h"
#include "TF1.h"

#include <cmath>

using namespace std;

//----------------------------------------------------------------------------------------------------

double sign;
double ep, ep_err;

//----------------------------------------------------------------------------------------------------

void TestOne(TTree *tree, unsigned int pot, double n_si = 3.)
{
	char tag[10], buf[500];
	sprintf(tag, "%i", pot);
	
	printf("* TestOne(%s | %.1f)\n", tag, n_si);

	char th_y_L[500], th_y_R[500], th_y[500], th_y_diff[500], cut_with[500], cut_without[500];

	bool L_F = (pot == 1) ? false : true;
	bool L_N = (pot == 2) ? false : true;
	bool R_N = (pot == 3) ? false : true;
	bool R_F = (pot == 4) ? false : true;

	double norm_L = 0.;
	sprintf(buf, "0");
	if (L_F) { sprintf(buf, "%s - y_L_F/%E", buf, L_y_F); norm_L++; }
	if (L_N) { sprintf(buf, "%s - y_L_N/%E", buf, L_y_N); norm_L++; }
	sprintf(th_y_L, "(%s)/%.0f", buf, norm_L);
	printf("\tth_y_L: %s\n", th_y_L);
	
	double norm_R = 0.;
	sprintf(buf, "0");
	if (R_F) { sprintf(buf, "%s + y_R_F/%E", buf, L_y_F); norm_R++; }
	if (R_N) { sprintf(buf, "%s + y_R_N/%E", buf, L_y_N); norm_R++; }
	sprintf(th_y_R, "(%s)/%.0f", buf, norm_R);
	printf("\tth_y_R: %s\n", th_y_R);

	sprintf(th_y_diff, "%s - %s", th_y_R, th_y_L);
	//sprintf(th_y, "(%s + %s)/2", th_y_R, th_y_L);
	if (pot == 1) sprintf(th_y, "-y_L_N/%E*%E", L_y_N, sign);
	if (pot == 2) sprintf(th_y, "-y_L_F/%E*%E", L_y_F, sign);
	if (pot == 3) sprintf(th_y, " y_R_F/%E*%E", L_y_F, sign);
	if (pot == 4) sprintf(th_y, " y_R_N/%E*%E", L_y_N, sign);

	sprintf(cut_without, "(run_num < 69300000 || run_num > 69309999) && (run_num < 69400000 || run_num > 69409999) && (run_num < 69430000 || run_num > 69439999)");
	if (L_F) sprintf(cut_without, "%s && v_L_F", cut_without);
	if (L_N) sprintf(cut_without, "%s && v_L_N", cut_without);
	if (R_N) sprintf(cut_without, "%s && v_R_N", cut_without);
	if (R_F) sprintf(cut_without, "%s && v_R_F", cut_without);
	
	sprintf(cut_with, "(run_num < 69300000 || run_num > 69309999) && (run_num < 69400000 || run_num > 69409999) && (run_num < 69430000 || run_num > 69439999)");
	sprintf(cut_with, "%s && v_L_F", cut_with);
	sprintf(cut_with, "%s && v_L_N", cut_with);
	sprintf(cut_with, "%s && v_R_N", cut_with);
	sprintf(cut_with, "%s && v_R_F", cut_with);

	sprintf(cut_without, "%s && abs(%s) < %E*%E", cut_without, th_y_diff, n_si, cut2_si);
	sprintf(cut_with, "%s && abs(%s) < %E*%E", cut_with, th_y_diff, n_si, cut2_si);

	printf("\tcut_without: %s\n", cut_without);
	printf("\tcut_with: %s\n", cut_with);

	TH1D *h_without = new TH1D("h_without", "", 100, 0, 150E-6);
	sprintf(buf, "%s >> h_without", th_y);
	tree->Draw(buf, cut_without);
	h_without->Sumw2();
	h_without->Write();
	
	TH1D *h_with = new TH1D("h_with", "", 100, 0, 150E-6);
	sprintf(buf, "%s >> h_with", th_y);
	tree->Draw(buf, cut_with);
	h_with->Sumw2();
	h_with->Write();

	TH1D *h_ratio = new TH1D(*h_with);
	h_ratio->SetName("h_ratio");
	h_ratio->Divide(h_without);

	TF1 *ff = new TF1("ff", "[0]");
	h_ratio->Fit(ff, "QW", "", 25E-6, 100E-6);
	ep = ff->GetParameter(0);
	
	// improve error calculation: si(y)^2 = N ep (1-ep)
	for (int i = 1; i <= h_ratio->GetNbinsX(); i++) {
		double n = h_with->GetBinContent(i);
		double d = h_without->GetBinContent(i);
		double r = (d > 0.) ? n / d : 0.;
		double v = d * ep * (1. - ep);
		double e = (v >= 0. && d > 0.) ? sqrt(v) / d : 0.;
		h_ratio->SetBinContent(i, r);
		h_ratio->SetBinError(i, e);
	}
	
	h_ratio->Fit(ff, "Q", "", 25E-6, 100E-6);
	h_ratio->Write();

	ep = ff->GetParameter(0);
	ep_err = ff->GetParError(0);
	printf(">> %.4f +- %.4f\n\n", ep, ep_err);

	fflush(stdout);
}

//----------------------------------------------------------------------------------------------------

void TestTwo(TTree *tree, unsigned int pot1, unsigned int pot2, double n_si = 3.)
{
	char buf[500];
	printf("* TestTwo(%u, %u | %.1f)\n", pot1, pot2, n_si);

	char th_y_L[500], th_y_R[500], th_y[500], th_y_diff[500], cut_with[500], cut_without[500];

	bool L_F = (pot1 == 1 || pot2 == 1) ? false : true;
	bool L_N = (pot1 == 2 || pot2 == 2) ? false : true;
	bool R_N = (pot1 == 3 || pot2 == 3) ? false : true;
	bool R_F = (pot1 == 4 || pot2 == 4) ? false : true;

	double norm_L = 0.;
	sprintf(buf, "0");
	if (L_F) { sprintf(buf, "%s - y_L_F/%E", buf, L_y_F); norm_L++; }
	if (L_N) { sprintf(buf, "%s - y_L_N/%E", buf, L_y_N); norm_L++; }
	sprintf(th_y_L, "(%s)/%.0f", buf, norm_L);
	printf("\tth_y_L: %s\n", th_y_L);
	
	double norm_R = 0.;
	sprintf(buf, "0");
	if (R_F) { sprintf(buf, "%s + y_R_F/%E", buf, L_y_F); norm_R++; }
	if (R_N) { sprintf(buf, "%s + y_R_N/%E", buf, L_y_N); norm_R++; }
	sprintf(th_y_R, "(%s)/%.0f", buf, norm_R);
	printf("\tth_y_R: %s\n", th_y_R);

	sprintf(th_y_diff, "%s - %s", th_y_R, th_y_L);
	sprintf(th_y, "(%s + %s)/2", th_y_R, th_y_L);

	sprintf(cut_without, "(run_num < 69300000 || run_num > 69309999) && (run_num < 69430000 || run_num > 69439999)");
	if (L_F) sprintf(cut_without, "%s && v_L_F", cut_without);
	if (L_N) sprintf(cut_without, "%s && v_L_N", cut_without);
	if (R_N) sprintf(cut_without, "%s && v_R_N", cut_without);
	if (R_F) sprintf(cut_without, "%s && v_R_F", cut_without);
	
	sprintf(cut_with, "(run_num < 69300000 || run_num > 69309999) && (run_num < 69430000 || run_num > 69439999)");
	sprintf(cut_with, "%s && v_L_F", cut_with);
	sprintf(cut_with, "%s && v_L_N", cut_with);
	sprintf(cut_with, "%s && v_R_N", cut_with);
	sprintf(cut_with, "%s && v_R_F", cut_with);

	sprintf(cut_without, "%s && abs(%s) < %E*%E", cut_without, th_y_diff, n_si, cut2_si);
	sprintf(cut_with, "%s && abs(%s) < %E*%E", cut_with, th_y_diff, n_si, cut2_si);

	printf("\tcut_without: %s\n", cut_without);
	printf("\tcut_with: %s\n", cut_with);

	TH1D *h_without = new TH1D("h_without", "", 100, 0, 150E-6);
	sprintf(buf, "%s >> h_without", th_y);
	tree->Draw(buf, cut_without);
	h_without->Sumw2();
	h_without->Write();
	
	TH1D *h_with = new TH1D("h_with", "", 100, 0, 150E-6);
	sprintf(buf, "%s >> h_with", th_y);
	tree->Draw(buf, cut_with);
	h_with->Sumw2();
	h_with->Write();

	TH1D *h_ratio = new TH1D(*h_with);
	h_ratio->SetName("h_ratio");
	h_ratio->Divide(h_without);

	TF1 *ff = new TF1("ff", "[0]");
	h_ratio->Fit(ff, "Q", "", 25E-6, 100E-6);
	h_ratio->Write();

	ep = ff->GetParameter(0);
	ep_err = ff->GetParError(0);
	printf(">> %.4f +- %.4f\n\n", ep, ep_err);

	fflush(stdout);
}

//----------------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{
	if (argc != 2)
		return 1;

	// init diagonal
	Init(argv[1]);
	sign = (strcmp(argv[1], "45b_56t") == 0) ? +1. : -1.;

	// get input
	TFile *inF = new TFile((string("distill_") + argv[1] + ".root").c_str());
	TTree *inT = (TTree *) inF->Get("distilled");

	// prepare output
	TFile *outF = new TFile((string("efficiency_") + argv[1] + ".root").c_str(), "recreate");

	// run 3-out-of-4 analysis
	map<unsigned int, double> m_ep, m_ep_err;
	for (unsigned int i = 1; i <= 4; i++) {
		double S1 = 0., Sep = 0., Sep2 = 0., Serr = 0.;
		for (double n_si = 1.; n_si <= 3.; n_si++) {
			char buf[50];
			sprintf(buf, "rp=%i, n_si=%.1f", i, n_si);
			gDirectory = outF->mkdir(buf);
			TestOne(inT, i, n_si);

			S1 += 1;
			Sep += ep;
			Sep2 += ep * ep;
			Serr += ep_err;
		}

		printf("SUMMARY for RP %i:\n", i);
		printf("\tep mean          : %.4f\n", Sep / S1);
		double si_sq = (Sep2 - Sep*Sep/S1) / (S1-1.);
		printf("\tep RMS           : %.4f\n", sqrt(si_sq));
		printf("\tep mean stat. err: %.4f\n", Serr / S1);

		m_ep[i] = Sep/S1;
		m_ep_err[i] = Serr/S1;

		printf("\n--------------------------------------------------\n");
		fflush(stdout);
	}
	
	// run 2-out-of-4 analysis
	vector< pair<unsigned int, unsigned int> > pairs;
	pairs.push_back(pair<unsigned int, unsigned int>(1, 3));
	pairs.push_back(pair<unsigned int, unsigned int>(1, 4));
	pairs.push_back(pair<unsigned int, unsigned int>(2, 3));
	pairs.push_back(pair<unsigned int, unsigned int>(2, 4));

	map<unsigned int, double> mp_ep, mp_ep_err;

	for (unsigned int i = 0; i < pairs.size(); i++) {
		double S1 = 0., Sep = 0., Sep2 = 0., Serr = 0.;
		for (double n_si = 1.; n_si <= 3.; n_si++) {
			char buf[50];
			sprintf(buf, "rps=%i and %i, n_si=%.1f", pairs[i].first, pairs[i].second, n_si);
			gDirectory = outF->mkdir(buf);
			TestTwo(inT, pairs[i].first, pairs[i].second, n_si);

			S1 += 1;
			Sep += ep;
			Sep2 += ep * ep;
			Serr += ep_err;
		}

		printf("SUMMARY for RP pair %i and %i:\n", pairs[i].first, pairs[i].second);
		printf("\tep mean          : %.4f\n", Sep / S1);
		double si_sq = (Sep2 - Sep*Sep/S1) / (S1-1.);
		printf("\tep RMS           : %.4f\n", sqrt(si_sq));
		printf("\tep mean stat. err: %.4f\n", Serr / S1);

		mp_ep[i] = Sep/S1;
		mp_ep_err[i] = Serr/S1;

		printf("\n--------------------------------------------------\n");
		fflush(stdout);
	}

	// pair comparison

	for (unsigned int i = 0; i < pairs.size(); i++) {
		printf("rps=%i and %i: extracted efficiency = %.3f +- %.3f, calculated from single efficiencies = %.3f\n", pairs[i].first, pairs[i].second,
			mp_ep[i], mp_ep_err[i], m_ep[pairs[i].first] * m_ep[pairs[i].second]);
	}


	// final print
	printf("\n\nFINAL RESULTS\n");
	for (unsigned int i = 1; i <= 4; i++) {
		printf("%.2f    %.2f\n", m_ep[i]*100., m_ep_err[i]*100.);
	}


	delete outF;
	return 0;
}
