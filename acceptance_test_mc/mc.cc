#include "../Profile.h"

#include "TH1D.h"
#include "TFile.h"
#include "TRandom2.h"

double t_dist(double t)
{
	double B = 20.;
	return exp(-B * t);
}

//----------------------------------------------------------------------------------------------------

int main()
{
	TFile *f = new TFile("mc.root", "recreate");

	unsigned long N_ev = 1E8;

	double t_min = 0., t_max = +1.;

	double th_y_lcut_L = 24.5E-6, th_y_lcut_R = 23.5E-6;	
	double th_y_hcut_L = 107E-6, th_y_hcut_R = 103E-6;

	double si_th_y_os = 2.47E-6, si_th_y_os_unc = 0.07E-6;

	double p = 3500.;

	Profile *p_th_y_full = new Profile("p_th_y_full", ";#theta_{y}", 300, 0E-6, 300E-6);
	Profile *p_th_y_meas = new Profile("p_th_y_meas", ";#theta_{y}", 300, 0E-6, 300E-6);
	Profile *p_th_y_corr = new Profile("p_th_y_corr", ";#theta_{y}", 300, 0E-6, 300E-6);

	for (unsigned long ev = 0; ev < N_ev; ++ev) {
		double t = gRandom->Rndm() * (t_max - t_min) + t_min;
		double phi = gRandom->Rndm() * M_PI;

		double th = sqrt(t) / p;
		double th_x = th * cos(phi);
		double th_y = th * sin(phi);

		double de_th_y_L = gRandom->Gaus() * si_th_y_os;
		double de_th_y_R = gRandom->Gaus() * si_th_y_os;
		
		double th_y_L = th_y + de_th_y_L;
		double th_y_R = th_y + de_th_y_R;

		double th_y_p = (th_y_L + th_y_R) / 2.;

		double w = t_dist(t);

		p_th_y_full->Fill(th_y_p, w);
		
		if (th_y_L > th_y_lcut_L && th_y_L < th_y_hcut_L && th_y_R > th_y_lcut_R && th_y_R < th_y_hcut_R) {
			p_th_y_meas->Fill(th_y_p, w);
			
			double th_y_lcut_eff = (th_y_lcut_L + th_y_lcut_R) / 2.;
			double th_y_hcut_eff = (th_y_hcut_L + th_y_hcut_R) / 2.;
			double th_y_abs = th_y_p;	// always positive
	
			double div_corr_l = (th_y_abs > th_y_lcut_eff) ?
				2. / (TMath::Erf((th_y_abs - th_y_lcut_L) / si_th_y_os) - TMath::Erf((th_y_lcut_R - th_y_abs) / si_th_y_os)) : 0.;
	
			double div_corr_h = (th_y_abs < th_y_hcut_eff) ?
				2. / (TMath::Erf((th_y_hcut_L - th_y_abs) / si_th_y_os) - TMath::Erf((th_y_abs - th_y_hcut_R) / si_th_y_os)) : 0.;
	
			double div_corr = div_corr_l * div_corr_h;
			
			p_th_y_corr->Fill(th_y_p, w * div_corr);
		}
	}

	// get histograms
	TH1D *h_th_y_full = p_th_y_full->GetHist(N_ev);
	TH1D *h_th_y_meas = p_th_y_meas->GetHist(N_ev);
	TH1D *h_th_y_corr = p_th_y_corr->GetHist(N_ev);
	
	h_th_y_full->SetLineColor(1);
	h_th_y_meas->SetLineColor(2);
	h_th_y_corr->SetLineColor(4);
	
	// write
	h_th_y_full->Write();
	h_th_y_meas->Write();
	h_th_y_corr->Write();

	delete f;

	return 0;
}
