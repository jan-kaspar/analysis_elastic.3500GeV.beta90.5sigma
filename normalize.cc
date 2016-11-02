#include "parameters.h"
#include "../common.h"

#include "TFile.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TF1.h"
#include "TFitResultPtr.h"
#include "TFitResult.h"

#include <cmath>

using namespace std;

void NormalizeAndFit(TH1D *h, const char *label, double scale)
{
	printf(">> %s\n", label);

	TDirectory *baseDir = gDirectory;
	gDirectory = baseDir->mkdir(label);

	// normalize histogram
	TH1D *h_norm = new TH1D(*h);
	h_norm->Scale(scale);
	h_norm->Write();

	// fit 1
	double t_max_fit = 0.20;
	TF1 *ff1 = new TF1("ff1", "[0]*exp([1]*x)");
	h_norm->Fit(ff1, "Q", "", t_min_fit, t_max_fit);
	TFitResultPtr frp = h_norm->Fit(ff1, "QIS", "", t_min_fit, t_max_fit);
	h_norm->SetName("fit1");
	h_norm->Write();

	frp->GetCorrelationMatrix().Print();

	double a = ff1->GetParameter(0), a_err = ff1->GetParError(0);
	double B = ff1->GetParameter(1), B_err = ff1->GetParError(1);
	double chi2 = ff1->GetChisquare(), ndf = ff1->GetNDF();

	printf("* fit 1\n");
	printf("\trange: %.4E to %.4E\n", t_min_fit, t_max_fit);
	printf("\tchi^2 = %.3E, ndf = %.3E, chi^2/ndf = %.3E\n", chi2, ndf, chi2/ndf);
	printf("\ta = %.3E +- %.3E\n", a, a_err);
	printf("\tB = %.3E +- %.3E\n", B, B_err);
	printf("\n");

	// integrate cross section
	double t_min_integ = t_min_fit;
	double t_max_integ = 0.4;
	int bi_min = h_norm->FindBin(t_min_integ), bi_max = h_norm->FindBin(t_max_integ);

	printf("* summing cross section over bins from %i (left edge %.2E) to %i (right edge %.2E)\n",
		bi_min, h_norm->GetBinLowEdge(bi_min), bi_max, h_norm->GetBinLowEdge(bi_max) + h_norm->GetBinWidth(bi_max));

	double integ_sum = 0., integ_sum_err_sq = 0.;
	for (int bi = bi_min; bi <= bi_max; bi++)
	{
		double w = h_norm->GetBinWidth(bi);
		integ_sum += h_norm->GetBinContent(bi) * w;
		integ_sum_err_sq += pow(h_norm->GetBinError(bi) * w, 2.);
	}

	printf("\t%.2f +- %.2f mb\n", integ_sum, sqrt(integ_sum_err_sq));

	double t_max_extra = h_norm->GetBinLowEdge(bi_min);
	printf("* integrating between %.2E, %.2E\n", 0., t_max_extra);
	
	double integ_extra = a/B * (exp(B*t_max_extra) - 1.);
	double cB = a/B*t_max_extra*exp(B*t_max_extra) - integ_extra/B;
	double integ_extra_err = sqrt(integ_extra*integ_extra/a/a*a_err*a_err + cB*cB*B_err*B_err);
	printf("\t%.2f +- %.2f mb\n", integ_extra, integ_extra_err);

	double integ_full = integ_sum + integ_extra;
	double integ_full_err = sqrt(integ_sum_err_sq + integ_extra_err*integ_extra_err);
	printf("* full cross section\n\t%.2f +- %.2f mb\n", integ_full, integ_full_err);
	
	// fit 2
	/*
	t_max_fit = 0.4;
	TF1 *ff2 = new TF1("ff2", "[0]*exp([1]*x + [2]*x*x)");
	h_norm->Fit(ff2, "Q", "", t_min_fit, t_max_fit);
	h_norm->Fit(ff2, "QI", "", t_min_fit, t_max_fit);
	h_norm->SetName("fit2");
	h_norm->Write();

	a = ff2->GetParameter(0); a_err = ff2->GetParError(0);
	B = ff2->GetParameter(1); B_err = ff2->GetParError(1);
	chi2 = ff2->GetChisquare(); ndf = ff2->GetNDF();

	printf("* fit 2\n");
	printf("\tchi^2 = %.3E, ndf = %.3E, chi^2/ndf = %.3E\n", chi2, ndf, chi2/ndf);
	printf("\ta = %.3E +- %.3E\n", a, a_err);
	printf("\tB = %.3E +- %.3E\n", B, B_err);
	printf("\n");
	*/

	gDirectory = baseDir;
}

//----------------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{
	if (argc != 2)
		return 1;

	// init diagonal
	Init(argv[1]);

	// get input
	TFile *inF = new TFile((string("unfolding_") + argv[1] + ".root").c_str());
	TH1D *h_t = (TH1D *) inF->Get("cf,eb/output");

	// prepare output
	TFile *outF = new TFile((string("normalize_") + argv[1] + ".root").c_str(), "recreate");

	NormalizeAndFit(h_t, "cross section", bckg_correction * full_eff_corr / L_int_eff);
	//NormalizeAndFit(h_t, "rate", bckg_correction * full_eff_corr / daq_eff);

	delete outF;
	delete inF;

	//------------------------------

	// merge diagonals
	if (diagonal == d45t)
		return 0;

	// get input
	TFile *inF_45b = new TFile("unfolding_45b_56t.root");
	TH1D *h_t_45b = (TH1D *) inF_45b->Get("cf,eb/output");
	
	TFile *inF_45t = new TFile("unfolding_45t_56b.root");
	TH1D *h_t_45t = (TH1D *) inF_45t->Get("cf,eb/output");

	// get scaling factors
	Init("45b_56t");
	double sf_45b = bckg_correction * full_eff_corr / L_int_eff;
	
	Init("45t_56b");
	double sf_45t = bckg_correction * full_eff_corr / L_int_eff;

	// prepare output
	outF = new TFile("normalize_merged.root", "recreate");

	printf("\n");
	printf("\n");
	printf("\n");
	printf("--------------------------------------------------\n");
	printf("                     MERGED                       \n");
	printf("--------------------------------------------------\n");

	// merge
	h_t = new TH1D(*h_t_45b);
	h_t->SetName("output");
	for (int i = 1; i <= h_t->GetNbinsX(); i++) {
		double v1 = sf_45b * h_t_45b->GetBinContent(i), e1 = sf_45b * h_t_45b->GetBinError(i);
		double v2 = sf_45t * h_t_45t->GetBinContent(i), e2 = sf_45t * h_t_45t->GetBinError(i);

		double w1 = (e1 > 0.) ? 1./e1/e1 : 0.;
		double w2 = (e2 > 0.) ? 1./e2/e2 : 0.;

		double v = (w1+w2 > 0.) ? (w1*v1 + w2*v2) / (w1 + w2) : 0.;
		double e = (w1+w2 > 0.) ? 1. / sqrt(w1 + w2) : 0.;

		h_t->SetBinContent(i, v);
		h_t->SetBinError(i, e);

		//printf("t=%E, v1=%E, v2=%E, v=%E\n", h_t_45b->GetBinCenter(i), v1, v2, v);
	}

	NormalizeAndFit(h_t, "cross section", 1.);

	delete outF;

	return 0;
}
