#include "TF1.h"
#include "TRandom2.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TGraphErrors.h"

namespace CF {

double t_middle = 0.;

void MakeFit(TH1D *input, TH1D *corr, TF1 *ff, TH1D *guide)
{
	// correct the input with current correction
	TH1D *input_corr = new TH1D(*input);
	input_corr->Multiply(corr);
	
	/*
	input_corr->Fit(ff, "Q",  "", t_min, t_max);
	input_corr->Fit(ff, "Q",  "", t_min, t_max);
	input_corr->Fit(ff, "IQ",  "", t_min, t_max);
	input_corr->Fit(ff, "I",  "", t_min, t_max);

	printf("residual chi^2 = %.1E, number of points = %i, chi^2/npx = %.2f\n",
		ff->GetChisquare(), ff->GetNpx(), ff->GetChisquare() / ff->GetNpx());

	input_corr->SetName("input_corr");
	input_corr->Write();
	*/

	// build a graph from input and guide
	TGraphErrors *merged = new TGraphErrors();
	merged->SetName("merged");
	for (int bi = 1; bi <= input_corr->GetNbinsX(); bi++) {
		double x = input_corr->GetBinCenter(bi), y = input_corr->GetBinContent(bi), ye = input_corr->GetBinError(bi);
		if (x < t_middle) {
			int idx = merged->GetN();
			merged->SetPoint(idx, x, y);
			merged->SetPointError(idx, 0, ye);
		}
	}
	
	for (int bi = 1; bi <= guide->GetNbinsX(); bi++) {
		double x = guide->GetBinCenter(bi), y = guide->GetBinContent(bi), ye = guide->GetBinError(bi);
		if (x > t_middle && x <= t_max_full) {
			int idx = merged->GetN();
			merged->SetPoint(idx, x, y);
			merged->SetPointError(idx, 0, ye);
		}
	}
	
	merged->Fit(ff, "Q",  "", t_min, t_max_full);
	merged->Fit(ff, "Q",  "", t_min, t_max_full);

	merged->Write();
	ff->Write();
}

//----------------------------------------------------------------------------------------------------

void CalculateCorrection(TF1 *ff, TH1D *corr)
{
	TH1D *h_t, *h_t_p;

	Profile *p_t, *p_t_p;
	if (corr->GetXaxis()->GetXbins()->fN > 0) {
		p_t = new Profile("h_t", ";t", corr->GetNbinsX(), corr->GetXaxis()->GetXbins()->fArray);
		p_t_p = new Profile("h_t_p", ";t'", corr->GetNbinsX(), corr->GetXaxis()->GetXbins()->fArray);
	} else {
		p_t = new Profile("h_t", ";t", corr->GetNbinsX(), corr->GetXaxis()->GetXmin(), corr->GetXaxis()->GetXmax());
		p_t_p = new Profile("h_t_p", ";t'", corr->GetNbinsX(), corr->GetXaxis()->GetXmin(), corr->GetXaxis()->GetXmax());
	}
	
	// make simulation
	unsigned long N_anal_cf = 1E9;
	for (unsigned long ev = 0; ev < N_anal_cf; ++ev) {
		double t = gRandom->Rndm() * (t_max_full - t_min_full) + t_min_full;
		double phi = gRandom->Rndm() * M_PI; // just upper half
		double w = ff->Eval(t) * (t_max_full - t_min_full);

		double th = sqrt(t) / p;
		double th_x = th * cos(phi);
		double th_y = th * sin(phi);

		double th_x_L_sm = th_x + gRandom->Gaus() * si_th_x_os;
		double th_x_R_sm = th_x + gRandom->Gaus() * si_th_x_os;
		double th_y_L_sm = th_y + gRandom->Gaus() * si_th_y_os;
		double th_y_R_sm = th_y + gRandom->Gaus() * si_th_y_os;

		double th_x_sm = (th_x_L_sm + th_x_R_sm) / 2.;
		double th_y_sm = (th_y_L_sm + th_y_R_sm) / 2.;

		double th_sm = sqrt(th_x_sm*th_x_sm + th_y_sm*th_y_sm);
		double t_sm = p*p * (th_x_sm*th_x_sm + th_y_sm*th_y_sm);
	
		p_t->Fill(t, w);

		double phi_corr = 0., div_corr = 0.;
		bool skip = CalculateAcceptanceCorrections(+1., th_y_L_sm, th_y_R_sm, th_y_sm, th_sm, si_th_y_os, phi_corr, div_corr);
		if (!skip)
			p_t_p->Fill(t_sm, w * div_corr * phi_corr / 2.); // only upper half in generation
	}
	
	h_t = p_t->GetHist(N_anal_cf);
	h_t_p = p_t_p->GetHist(N_anal_cf);

	h_t->Write();
	h_t_p->Write();

	// extract correction histogram
	for (int i = 1; i <= h_t->GetNbinsX(); i++) {
		double t = h_t->GetBinCenter(i);
		if (t < t_min || t > t_max) {
			corr->SetBinContent(i, 0.);
			corr->SetBinError(i, 0.);
			continue;
		}

		double c = h_t->GetBinContent(i), e = h_t->GetBinError(i);
		double c_p = h_t_p->GetBinContent(i), e_p = h_t_p->GetBinError(i);

		double v = (c_p > 0.) ? c / c_p : 1.;
		double u = (c_p > 0.) ? sqrt(e*e/c_p/c_p + c*c/c_p/c_p/c_p/c_p * e_p*e_p) : 1.;

		corr->SetBinContent(i, v);
		corr->SetBinError(i, u);
	}

	corr->Write();
}

//----------------------------------------------------------------------------------------------------

TF1* InitFitFunction(double norm = 1e7)
{
	TF1 *ff = new TF1("ff", "[0]*exp([1]*x + [2]*x*x) + [3]*exp([4]*x + [5]*x*x + [6]*x*x*x + [7]*x*x*x*x)");
	ff->SetParameters(norm, -20., 0., norm/1E3, -3., 0., 0., 0);
	
	//TF1 *ff = new TF1("ff", "[0]*exp([1]*x) + [2]*exp([3]*x + [4]*x*x)");
	//ff->SetParameters(norm, -4., norm/1E3, -0.5, 0.);
	
	//TF1 *ff = new TF1("ff", "[0]*exp([1]*x + [2]*x*x)");
	//ff->SetParameters(norm, -20., 0);
	
	//TF1 *ff = new TF1("ff", "[0]*exp([1]*x)");
	//ff->SetParameters(norm, -4.);

	ff->SetRange(t_min_full, t_max_full);

	return ff;
}

//----------------------------------------------------------------------------------------------------

void DoUnfolding(TH1D *input, TH1D *guide)
{
	printf(">> CF::DoUnfolding\n");

	// make initial correction histogram
	TH1D *corr = new TH1D(*input);
	corr->SetName("corr");
	for (int i = 1; i <= corr->GetNbinsX(); i++) {
		corr->SetBinContent(i, 1.);
		corr->SetBinError(i, 0.);
	}

	// initialize fit function
	TF1 *ff = InitFitFunction();

	TDirectory *baseDir = gDirectory;

	// run iterations
	for (unsigned int it = 1; it <= 3; it++) {
		printf("* iteration: %i\n", it);
	
		char buf[100];
		sprintf(buf, "iteration %i", it);
		gDirectory = baseDir->mkdir(buf);

		MakeFit(input, corr, ff, guide);
		CalculateCorrection(ff, corr);
	}

	gDirectory = baseDir;

	// save final correction
	corr->SetName("corr_final");
	corr->Write();

	// save unsmeared result
	TH1D *output = new TH1D(*input);
	output->SetName("output");
	output->SetLineColor(8);
	output->Multiply(corr);
	output->Write();

	// distribution comparison
	TCanvas *c = new TCanvas("dist cmp");
	c->SetLogy(1);
	input->SetLineColor(1);
	input->Draw();
	output->Draw("same");
	c->Write();
}

} // namespace
