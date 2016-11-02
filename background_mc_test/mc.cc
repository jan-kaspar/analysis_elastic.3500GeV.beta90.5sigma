#include "../Profile.h"

#include "TH1D.h"
#include "TFile.h"
#include "TRandom2.h"

//----------------------------------------------------------------------------------------------------

double th_x_dist(double th_x)
{
	double sum = 0;

	double a1 = 8.23453e+03, mu1 = 0., si1 = 8.77724e-05;
	sum += a1 * exp(- (th_x - mu1)*(th_x - mu1) / 2. /si1/si1);
	
	double a2 = 4.40674e+02, mu2 = 4.20886e-04, si2 = 3.47555e-04;
	sum += a2 * exp(- (th_x - mu2)*(th_x - mu2) / 2. /si2/si2);
	
	double a3 = 3.12198e+00, mu3 = 3.82434e-04, si3 = 3.34936e-03;
	sum += a3 * exp(- (th_x - mu3)*(th_x - mu3) / 2. /si3/si3);

	return sum;
}

//----------------------------------------------------------------------------------------------------

double th_y_dist(double th_y)
{
	double be = 2.5E4;
	return exp(-be * th_y);
}

//----------------------------------------------------------------------------------------------------

int main()
{
	TFile *f = new TFile("mc.root", "recreate");

	unsigned long N_ev = 1E8;

	double th_x_min = -4E-3, th_x_max = +4E-3;
	double th_y_min = 0E-6, th_y_max = +110E-6;

	double si_de_th_x = 11E-6, si_de_th_y = 3.6E-6;
	double n_si = 3.;

	Profile *p_th_x_L_full = new Profile("p_th_x_L_full", ";#theta_{x}^{L}", 100, th_x_min, th_x_max);
	Profile *p_th_x_L_sig = new Profile("p_th_x_L_sig", ";#theta_{x}^{L}", 100, th_x_min, th_x_max);
	Profile *p_th_x_L_bckg = new Profile("p_th_x_L_bckg", ";#theta_{x}^{L}", 100, th_x_min, th_x_max);
	
	Profile *p_th_x_R_full = new Profile("p_th_x_R_full", ";#theta_{x}^{R}", 100, th_x_min, th_x_max);
	Profile *p_th_x_R_sig = new Profile("p_th_x_R_sig", ";#theta_{x}^{R}", 100, th_x_min, th_x_max);
	Profile *p_th_x_R_bckg = new Profile("p_th_x_R_bckg", ";#theta_{x}^{R}", 100, th_x_min, th_x_max);
	
	Profile *p_th_y_L_full = new Profile("p_th_y_L_full", ";#theta_{y}^{L}", 100, th_y_min, th_y_max);
	Profile *p_th_y_L_sig = new Profile("p_th_y_L_sig", ";#theta_{y}^{L}", 100, th_y_min, th_y_max);
	Profile *p_th_y_L_bckg = new Profile("p_th_y_L_bckg", ";#theta_{y}^{L}", 100, th_y_min, th_y_max);
	
	Profile *p_th_y_R_full = new Profile("p_th_y_R_full", ";#theta_{y}^{R}", 100, th_y_min, th_y_max);
	Profile *p_th_y_R_sig = new Profile("p_th_y_R_sig", ";#theta_{y}^{R}", 100, th_y_min, th_y_max);
	Profile *p_th_y_R_bckg = new Profile("p_th_y_R_bckg", ";#theta_{y}^{R}", 100, th_y_min, th_y_max);
	
	Profile *p_cq_x = new Profile("p_cq_x", ";#Delta^{R-L} #theta_{x}", 100, -30., +30.);
	Profile *p_cq_y = new Profile("p_cq_y", ";#Delta^{R-L} #theta_{y}", 300, -30., +30.);
	
	Profile *p_cq_x_all = new Profile("p_cq_x_all", ";#Delta^{R-L} #theta_{x}", 100, -30., +30.);
	Profile *p_cq_y_all = new Profile("p_cq_y_all", ";#Delta^{R-L} #theta_{y}", 300, -30., +30.);

	for (unsigned long ev = 0; ev < N_ev; ++ev) {
		double th_x_L = gRandom->Rndm() * (th_x_max - th_x_min) + th_x_min;
		double th_x_R = gRandom->Rndm() * (th_x_max - th_x_min) + th_x_min;
		
		double th_y_L = gRandom->Rndm() * (th_y_max - th_y_min) + th_y_min;
		double th_y_R = gRandom->Rndm() * (th_y_max - th_y_min) + th_y_min;

		double w = th_x_dist(-th_x_L) * th_x_dist(th_x_R) * th_y_dist(th_y_L) * th_y_dist(th_y_R);

		double cq_x = (th_x_R - th_x_L) / si_de_th_x;
		double cq_y = (th_y_R - th_y_L) / si_de_th_y;

		bool sig = (fabs(cq_x) < n_si && fabs(cq_y) < n_si);
		bool bckg = !sig;

		p_th_x_L_full->Fill(th_x_L, w);
		p_th_x_R_full->Fill(th_x_R, w);
		p_th_y_L_full->Fill(th_y_L, w);
		p_th_y_R_full->Fill(th_y_R, w);

		if (sig) {
			p_th_x_L_sig->Fill(th_x_L, w);
			p_th_x_R_sig->Fill(th_x_R, w);
			p_th_y_L_sig->Fill(th_y_L, w);
			p_th_y_R_sig->Fill(th_y_R, w);
		}
		
		if (bckg) {
			p_th_x_L_bckg->Fill(th_x_L, w);
			p_th_x_R_bckg->Fill(th_x_R, w);
			p_th_y_L_bckg->Fill(th_y_L, w);
			p_th_y_R_bckg->Fill(th_y_R, w);
			
			p_cq_x_all->Fill(cq_x, w);
			p_cq_y_all->Fill(cq_y, w);
		}

		if (fabs(cq_y) < n_si)
			p_cq_x->Fill(cq_x, w);
		
		if (fabs(cq_x) < n_si)
			p_cq_y->Fill(cq_y, w);
	}

	// get histograms
	TH1D *h_th_x_L_full = p_th_x_L_full->GetHist(N_ev);
	TH1D *h_th_x_R_full = p_th_x_R_full->GetHist(N_ev);
	TH1D *h_th_y_L_full = p_th_y_L_full->GetHist(N_ev);
	TH1D *h_th_y_R_full = p_th_y_R_full->GetHist(N_ev);
	
	TH1D *h_th_x_L_sig = p_th_x_L_sig->GetHist(N_ev);
	TH1D *h_th_x_R_sig = p_th_x_R_sig->GetHist(N_ev);
	TH1D *h_th_y_L_sig = p_th_y_L_sig->GetHist(N_ev);
	TH1D *h_th_y_R_sig = p_th_y_R_sig->GetHist(N_ev);
	
	TH1D *h_th_x_L_bckg = p_th_x_L_bckg->GetHist(N_ev);
	TH1D *h_th_x_R_bckg = p_th_x_R_bckg->GetHist(N_ev);
	TH1D *h_th_y_L_bckg = p_th_y_L_bckg->GetHist(N_ev);
	TH1D *h_th_y_R_bckg = p_th_y_R_bckg->GetHist(N_ev);

	TH1D *h_cq_x = p_cq_x->GetHist(N_ev);
	TH1D *h_cq_y = p_cq_y->GetHist(N_ev);
	
	TH1D *h_cq_x_all = p_cq_x_all->GetHist(N_ev);
	TH1D *h_cq_y_all = p_cq_y_all->GetHist(N_ev);
	
	// scale
	double sc = 4.1E-6;
	h_th_x_L_full->Scale(sc);
	h_th_x_L_sig->Scale(sc);
	h_th_x_L_bckg->Scale(sc);

	h_th_x_R_full->Scale(sc);
	h_th_x_R_sig->Scale(sc);
	h_th_x_R_bckg->Scale(sc);

	h_th_y_L_full->Scale(sc);
	h_th_y_L_sig->Scale(sc);
	h_th_y_L_bckg->Scale(sc);

	h_th_y_R_full->Scale(sc);
	h_th_y_R_sig->Scale(sc);
	h_th_y_R_bckg->Scale(sc);

	// colors
	h_th_x_L_full->SetLineColor(1);
	h_th_x_L_sig->SetLineColor(4);
	h_th_x_L_bckg->SetLineColor(2);

	h_th_x_R_full->SetLineColor(1);
	h_th_x_R_sig->SetLineColor(4);
	h_th_x_R_bckg->SetLineColor(2);

	h_th_y_L_full->SetLineColor(1);
	h_th_y_L_sig->SetLineColor(4);
	h_th_y_L_bckg->SetLineColor(2);

	h_th_y_R_full->SetLineColor(1);
	h_th_y_R_sig->SetLineColor(4);
	h_th_y_R_bckg->SetLineColor(2);
	
	// write
	h_th_x_L_full->Write();
	h_th_x_L_sig->Write();
	h_th_x_L_bckg->Write();

	h_th_x_R_full->Write();
	h_th_x_R_sig->Write();
	h_th_x_R_bckg->Write();

	h_th_y_L_full->Write();
	h_th_y_L_sig->Write();
	h_th_y_L_bckg->Write();

	h_th_y_R_full->Write();
	h_th_y_R_sig->Write();
	h_th_y_R_bckg->Write();

	h_cq_x->Write();
	h_cq_y->Write();
	
	h_cq_x_all->Write();
	h_cq_y_all->Write();

	delete f;

	return 0;
}
