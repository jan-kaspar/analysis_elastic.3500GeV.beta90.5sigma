#include "TFile.h"
#include "TH1D.h"
#include "TRandom2.h"

#include <cmath>

double pdf_rho_sq(double rho_sq, double rho_abs)
{
	double si = 0.056;
	return exp(- pow(rho_sq - rho_abs * rho_abs, 2.) /2./si/si );
}

//----------------------------------------------------------------------------------------------------

double pdf_si_lmd(double si_lmd_meas, double si_lmd_deri)
{
	double si = 2.17;
	return exp(- pow(si_lmd_meas - si_lmd_deri, 2.) /2./si/si );
}

//----------------------------------------------------------------------------------------------------

void PrintCentralConfidenceInterval(TH1D *h, double level)
{
	printf("\tcentral confidence interval at level %.1f \%\n", level*100.);
	
	double al = (1. - level)/2.;

	double N=0.;
	for (int i = 1; i <= h->GetNbinsX(); i++)
		N += h->GetBinContent(i);

	// left part
	double f_min;
	double S_min = 0.;
	for (unsigned int i = 1; i < h->GetNbinsX(); i++) {
		double v = h->GetBinContent(i) / N;
		S_min += v;
		
		//printf("%i => %E\n", i, S_min);

		if (S_min > al) {
			if (i > 1)
				f_min = (h->GetBinCenter(i) + h->GetBinCenter(i-1)) / 2.;
			else
				f_min = h->GetBinCenter(i);
			break;
		}
	}

	// right part
	double f_plu;
	double S_plu = 0.;
	for (unsigned int i = h->GetNbinsX(); i > 0; i--) {
		double v = h->GetBinContent(i) / N;
		S_plu += v;
		if (S_plu > al) {
			if (i < h->GetNbinsX())
				f_plu = (h->GetBinCenter(i) + h->GetBinCenter(i-1)) / 2.;
			else
				f_plu = h->GetBinCenter(i);
			break;
		}
	}

	printf("\t\tfrom %.3f to %.3f     [bin size = %.3f]\n", f_min, f_plu, h->GetBinWidth(1));
}

//----------------------------------------------------------------------------------------------------

void PrintLeftConfidenceInterval(TH1D *h, double level)
{
	printf("\tleft confidence interval at level %.1f \%\n", level*100.);

	double al = level;

	double N=0.;
	for (int i = 1; i <= h->GetNbinsX(); i++)
		N += h->GetBinContent(i);

	// left part
	double f_min;
	double S_min = 0.;
	for (unsigned int i = 1; i < h->GetNbinsX(); i++) {
		double v = h->GetBinContent(i) / N;
		S_min += v;
		
		//printf("%i => %E\n", i, S_min);

		if (S_min > al) {
			if (i > 1)
				f_min = (h->GetBinCenter(i) + h->GetBinCenter(i-1)) / 2.;
			else
				f_min = h->GetBinCenter(i);
			break;
		}
	}
	
	printf("\t\tto %.3f     [bin size = %.3f]\n", f_min, h->GetBinWidth(1));
}

//----------------------------------------------------------------------------------------------------

void PrintDistProperties(TH1D *h)
{
	printf(">> PrintDistProperties: %s\n", h->GetName());

	double Sw=0., Sxw=0., Sxxw=0.;
	for (int i = 1; i <= h->GetNbinsX(); i++) {
		double x = h->GetBinCenter(i);
		double w = h->GetBinContent(i);

		Sw += w;
		Sxw += x*w;
		Sxxw += x*x*w;
	}

	double mean = Sxw/Sw;
	double var = Sxxw/Sw - mean*mean;
	double stddev = sqrt(var);

	printf("\tmean = %.3f, std.dev. = %.3f\n", mean, stddev);
	
	PrintCentralConfidenceInterval(h, 0.683);
	PrintCentralConfidenceInterval(h, 0.950);
	
	PrintLeftConfidenceInterval(h, 0.683);
	PrintLeftConfidenceInterval(h, 0.8415);
	PrintLeftConfidenceInterval(h, 0.950);
}

//----------------------------------------------------------------------------------------------------

int main()
{
	TFile *outF = new TFile("bayes.root", "recreate");

	unsigned int N_ev = 300000000;
	unsigned int bins = 10000;

	//------------------------------ rho ------------------------------

	TH1D *h_rho_sq = new TH1D("h_rho_sq", ";#rho^{2}_{m}", bins, 0., 0.);
	TH1D *h_rho_abs = new TH1D("h_rho_abs", ";|#rho|", bins, 0., 0.);
	TH1D *h_rho = new TH1D("h_rho", ";#rho", bins, 0., 0.);

	double rho_true = 0.2;

	for (unsigned int i = 0; i < N_ev; i++) {
		double rho_sq = (gRandom->Rndm() - 0.5) * 1.;
		double w = pdf_rho_sq(rho_sq, rho_true);
		h_rho_sq->Fill(rho_sq, w);
	}
	h_rho_sq->Write();

	PrintDistProperties(h_rho_sq);
	
	double rho_sq_m = 0.0084;
	
	for (unsigned int i = 0; i < N_ev; i++) {
		double rho = (gRandom->Rndm() - 0.5) * 3.;
		double w = pdf_rho_sq(rho_sq_m, rho);

		if (rho > 0.)
			h_rho_abs->Fill(rho, w/2.);

		h_rho->Fill(rho, w);
	}


	h_rho_abs->Write();
	h_rho->Write();
	
	PrintDistProperties(h_rho_abs);
	PrintDistProperties(h_rho);
	
	
	//------------------------------ sigma_lmd ------------------------------

	TH1D *h_si_lmd_meas = new TH1D("h_si_lmd_meas", ";#sigma_{lmd,meas}", bins, 0., 0.);
	TH1D *h_si_lmd_deri = new TH1D("h_si_lmd_deri", ";#sigma_{lmd,deri}", bins, 0., 0.);

	double si_lmd_true = 3.;
	
	for (unsigned int i = 0; i < N_ev; i++) {
		double si_lmd_meas = (gRandom->Rndm() - 0.5) * 25.;
		double w = pdf_si_lmd(si_lmd_meas, si_lmd_true);
		h_si_lmd_meas->Fill(si_lmd_meas, w);
	}
	h_si_lmd_meas->Write();
	PrintDistProperties(h_si_lmd_meas);

	double si_lmd_meas = 2.62;
	for (unsigned int i = 0; i < N_ev; i++) {
		double si_lmd_deri = (gRandom->Rndm() - 0.5) * 25.;
		double w = pdf_si_lmd(si_lmd_meas, si_lmd_deri);

		if (si_lmd_deri > 0.)
			h_si_lmd_deri->Fill(si_lmd_deri, w);
	}
	h_si_lmd_deri->Write();
	PrintDistProperties(h_si_lmd_deri);

	delete outF;
}
