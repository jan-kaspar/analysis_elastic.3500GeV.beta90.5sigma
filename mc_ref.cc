#include "TFile.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TRandom2.h"
#include "TMath.h"
#include "TVectorD.h"
#include "TMatrixD.h"

#include "../Profile.h"
#include "parameters.h"
#include "../common.h"

#include <vector>
#include <string>

using namespace std;

//----------------------------------------------------------------------------------------------------

double ReferenceModel(double mt)
{
	return exp(-4. * mt);
}

//----------------------------------------------------------------------------------------------------

int main()
{
	gRandom->SetSeed(123);

	TFile *outF = new TFile("mc_ref.root", "recreate");

	unsigned long N_mc_ref = 1E8;

	printf("t_min_full = %.3E, t_max_full = %.3E\n", t_min_full, t_max_full);
	printf("t_min = %.3E, t_max = %.3E\n", t_min, t_max);
	
	// do it for exp and no exp binning
	vector<string> binning;
	binning.push_back("uni");
	binning.push_back("exp");

	for (vector<string>::iterator it = binning.begin(); it != binning.end(); ++it) {
		printf("* %s\n", it->c_str());

		gDirectory = outF->mkdir(it->c_str());
		
		bool expBins = (it->compare("exp") == 0);
	
		// prepare histograms
		Profile *p_t, *p_t_p, *p_t_p2;
		Profile2D *p_t_p_t, *p_t_p_t2;

		unsigned int N_bins_full = N_bins + N_low + N_high;

		double *binEdges = BuildBinning(expBins, N_bins, N_low, N_high);
		p_t = new Profile("h_t", ";t", N_bins_full, binEdges);
		p_t_p = new Profile("h_t_p", ";t'", N_bins_full, binEdges);
		p_t_p2 = new Profile("h_t_p2", ";t'", N_bins_full, binEdges);
		p_t_p_t = new Profile2D("h_t_p_t", ";t';t", N_bins_full, binEdges, N_bins_full, binEdges);
		p_t_p_t2 = new Profile2D("h_t_p_t2", ";t';t", N_bins_full, binEdges, N_bins_full, binEdges);
	
		// make simulation
		for (unsigned long ev = 0; ev < N_mc_ref; ++ev) {
			double t = gRandom->Rndm() * (t_max_full - t_min_full) + t_min_full;
			double th = sqrt(t) / p;
	
			double phi = gRandom->Rndm() * 2.*M_PI;
			double th_x = th * cos(phi);
			double th_y = th * sin(phi);
			double th_x_p = th_x + gRandom->Gaus() * si_th_x;
			double th_y_p = th_y + gRandom->Gaus() * si_th_y;
			double t_p = p*p * (th_x_p*th_x_p + th_y_p*th_y_p);
	
			double v = ReferenceModel(t) * (t_max_full - t_min_full);
	
			p_t->Fill(t, v);
			p_t_p->Fill(t_p, v);
	
			p_t_p_t->Fill(t_p, t, v);
			
			if (t_p >= t_min && t_p <= t_max) {
				p_t_p2->Fill(t_p, v);
				p_t_p_t2->Fill(t_p, t, v);
			}
		}
	
		TH1D *h_t = p_t->GetHist(N_mc_ref);
		TH1D *h_t_p = p_t_p->GetHist(N_mc_ref);
		TH1D *h_t_p2 = p_t_p2->GetHist(N_mc_ref);
		TH2D *h_t_p_t = p_t_p_t->GetHist(N_mc_ref);
		TH2D *h_t_p_t2 = p_t_p_t2->GetHist(N_mc_ref);
		
		// save raw histograms
		h_t->Write();
		h_t_p->Write();
		h_t_p2->Write();
		h_t_p_t->Write();
		h_t_p_t2->Write();
	
		// normalize the resolution-matrix histogram
		TH2D *h_res_mat = new TH2D(*h_t_p_t); h_res_mat->SetName("h_res_mat");
		TH2D *h_res_mat2 = new TH2D(*h_t_p_t2); h_res_mat2->SetName("h_res_mat2");
		for (int x_i = 1; x_i <= h_res_mat->GetNbinsX(); x_i++) {
			double wx = h_res_mat->GetXaxis()->GetBinWidth(x_i);
			for (int y_i = 1; y_i <= h_res_mat->GetNbinsY(); y_i++) {
				double wy = h_res_mat->GetYaxis()->GetBinWidth(y_i);
	
				double n_v = h_res_mat->GetBinContent(x_i, y_i);
				double n_e = h_res_mat->GetBinError(x_i, y_i);
				double d_v = h_t->GetBinContent(y_i) * wx;
				double d_e = h_t->GetBinError(y_i) * wx;
	
				double v = (d_v != 0.) ? n_v/d_v : 0.;
				double e = (d_v != 0.) ? sqrt(n_e*n_e/d_v/d_v + n_v*n_v /d_v/d_v/d_v/d_v * d_e*d_e): 0.;
				h_res_mat->SetBinContent(x_i, y_i, v);
				h_res_mat->SetBinError(x_i, y_i, e);
				
				n_v = h_res_mat2->GetBinContent(x_i, y_i);
				n_e = h_res_mat2->GetBinError(x_i, y_i);
				v = (d_v != 0.) ? n_v/d_v : 0.;
				e = (d_v != 0.) ? sqrt(n_e*n_e/d_v/d_v + n_v*n_v /d_v/d_v/d_v/d_v * d_e*d_e): 0.;
				h_res_mat2->SetBinContent(x_i, y_i, v);
				h_res_mat2->SetBinError(x_i, y_i, e);
			}
		}
		h_res_mat->Write();
		h_res_mat2->Write();
	
		// resolution-matrix diagonal
		TH1D *h_res_mat_dgn = new TH1D(*h_t);
		h_res_mat_dgn->SetName("h_res_mat_dgn");
		for (int x_i = 1; x_i <= h_res_mat->GetNbinsX(); x_i++) {
			h_res_mat_dgn->SetBinContent(x_i, h_res_mat->GetBinContent(x_i, x_i));
			h_res_mat_dgn->SetBinError(x_i, h_res_mat->GetBinError(x_i, x_i));
		}
		h_res_mat_dgn->Write();
	
		// extract correction histogram
		TH1D *h_corr = new TH1D(*h_t); h_corr->SetName("h_corr"); h_corr->SetLineColor(2);
		for (int i = 1; i <= h_t->GetNbinsX(); i++) {
			double t = h_t->GetBinCenter(i);
			if (t < t_min || t > t_max) {
				h_corr->SetBinContent(i, 0.);
				h_corr->SetBinError(i, 0.);
				continue;
			}
		
			double c = h_t->GetBinContent(i), e = h_t->GetBinError(i);
			double c_p = h_t_p->GetBinContent(i), e_p = h_t_p->GetBinError(i);
	
			double v = (c_p > 0.) ? c / c_p : 1.;
			double u = (c_p > 0.) ? sqrt(e*e/c_p/c_p + c*c/c_p/c_p/c_p/c_p * e_p*e_p) : 1.;
		
			h_corr->SetBinContent(i, v);
			h_corr->SetBinError(i, u);
		}
		h_corr->Write();
	
		// overlay the histograms with the original function
		TCanvas *c = new TCanvas();
		c->SetName("plot");
		c->SetLogy(1);
		h_t->SetLineColor(4);
		h_t_p->SetLineColor(2);
		h_t->Draw();
		h_t_p->Draw("same");
		c->Write();
	}
}
