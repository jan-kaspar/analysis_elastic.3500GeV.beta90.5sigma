#include "../Profile.h"

#include "TRandom2.h"
#include "TFile.h"
#include "TProfile.h"

//----------------------------------------------------------------------------------------------------
	
// t-systematics
double be_x_L = 0E-6, al_x_L = 0.;
double be_y_L = 0E-6, al_y_L = 0.;
double be_x_R = 1E-6, al_x_R = 0.013;
double be_y_R = 0.1E-6, al_y_R = 0.004;

// simulation range
double t_min_full = 0., t_max_full = +0.5;
double t_min = 0., t_max = +0.4;

// beam smearing (one-side) parameters: for simulation (true)
double si_th_x_sim = 6.36E-6, si_th_x_unc = 0.21E-6;
double si_th_y_sim = 2.47E-6, si_th_y_unc = 0.07E-6;

// beam smearing (one-side) parameters: for reconstruction (assumed)
double si_th_y_ass = si_th_y_sim;
double si_th_x_ass = si_th_x_sim;

// hardware cuts
double th_y_lcut_hw_L = 18E-6, th_y_hcut_hw_L = 110E-6;
double th_y_lcut_hw_R = 18E-6, th_y_hcut_hw_R = 110E-6;

// analysis cuts
double th_y_lcut_L = 20E-6, th_y_hcut_L = 100E-6;
double th_y_lcut_R = 20E-6, th_y_hcut_R = 100E-6;

// energy
double p = 3500.;

//----------------------------------------------------------------------------------------------------

void Init_45b_56t() {}
void Init_45t_56b() {}

#include "../common.h"

//----------------------------------------------------------------------------------------------------

double t_dist(double t)
{
	double B = 20.;
	return exp(-B * t);
}

//----------------------------------------------------------------------------------------------------

void PrintUsage()
{
	printf("options: \n");
}


//----------------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{
	if (argc != 2) {
		PrintUsage();
		return 1;
	}

	// what systematics scenario?
	TFile *f = NULL;

	if (strcmp(argv[1], "none") == 0) {
		al_x_L = al_x_R = al_y_L = al_y_R = 0.;
		be_x_L = be_x_R = be_y_L = be_y_R = 0.;
		f = new TFile("mc_none.root", "recreate");
	}
	
	if (strcmp(argv[1], "none,smp") == 0) {
		al_x_L = al_x_R = al_y_L = al_y_R = 0.;
		be_x_L = be_x_R = be_y_L = be_y_R = 0.;
		si_th_y_ass += si_th_y_unc;
		si_th_x_ass += si_th_x_unc;
		f = new TFile("mc_none,smp.root", "recreate");
	}
	
	if (strcmp(argv[1], "none,smm") == 0) {
		al_x_L = al_x_R = al_y_L = al_y_R = 0.;
		be_x_L = be_x_R = be_y_L = be_y_R = 0.;
		si_th_y_ass -= si_th_y_unc;
		si_th_x_ass -= si_th_x_unc;
		f = new TFile("mc_none,smm.root", "recreate");
	}

	if (strcmp(argv[1], "misal") == 0) {
		al_x_L = al_x_R = al_y_L = al_y_R = 0.;
		f = new TFile("mc_misal.root", "recreate");
	}
	
	if (strcmp(argv[1], "optics:yp") == 0) {
		al_x_L = al_x_R = 0.;
		be_x_L = be_x_R = be_y_L = be_y_R = 0.;
		f = new TFile("mc_optics:yp.root", "recreate");
	}
	
	if (strcmp(argv[1], "optics:xp") == 0) {
		al_y_L = al_y_R = 0.;
		be_x_L = be_x_R = be_y_L = be_y_R = 0.;
		f = new TFile("mc_optics:xp.root", "recreate");
	}
	
	if (strcmp(argv[1], "optics:ypxp") == 0) {
		be_x_L = be_x_R = be_y_L = be_y_R = 0.;
		f = new TFile("mc_optics:ypxp.root", "recreate");
	}
	
	if (strcmp(argv[1], "optics:ypxp,smp") == 0) {
		be_x_L = be_x_R = be_y_L = be_y_R = 0.;
		si_th_y_ass += si_th_y_unc;
		si_th_x_ass += si_th_x_unc;
		f = new TFile("mc_optics:ypxp,smp.root", "recreate");
	}
	
	if (strcmp(argv[1], "optics:ypxp,smm") == 0) {
		be_x_L = be_x_R = be_y_L = be_y_R = 0.;
		si_th_y_ass -= si_th_y_unc;
		si_th_x_ass -= si_th_x_unc;
		f = new TFile("mc_optics:ypxp,smm.root", "recreate");
	}
	
	if (strcmp(argv[1], "optics:ypxm") == 0) {
		be_x_L = be_x_R = be_y_L = be_y_R = 0.;
		al_x_L = - al_x_L; al_x_R = - al_x_R;
		f = new TFile("mc_optics:ypxm.root", "recreate");
	}
	
	if (strcmp(argv[1], "misal_optics:ypxp") == 0) {
		f = new TFile("mc_misal_optics:ypxp.root", "recreate");
	}
	
	if (strcmp(argv[1], "enoff") == 0) {
		al_x_L = al_x_R = al_y_L = al_y_R = 0.001;	// energy offset of 1 permille
		be_x_L = be_x_R = be_y_L = be_y_R = 0.;
		f = new TFile("mc_enoff.root", "recreate");
	}

	if (!f) {
		PrintUsage();
		return 1;
	}
	
	// book histograms
	unsigned int N_bins = 150;
	Profile *p_t_tr = new Profile("p_t_tr", ";t", N_bins, t_min, t_max);
	Profile *p_t_sm = new Profile("p_t_sm", ";t", N_bins, t_min, t_max);
	Profile *p_t_sma = new Profile("p_t_sma", ";t", N_bins, t_min, t_max);
	Profile *p_t_re = new Profile("p_t_re", ";t'", N_bins, t_min, t_max);

	TProfile *de_th_x_vs_th_x = new TProfile("de_th_x_vs_th_x", ";#theta_{x};mean of #Delta #theta_{x}", N_bins, 0., 0.);
	TProfile *de_th_y_vs_th_y = new TProfile("de_th_y_vs_th_y", ";#theta_{y};mean of #Delta #theta_{y}", N_bins, 0., 0.);
	TProfile *de_th_vs_th = new TProfile("de_th_vs_th", ";#theta;#Delta#theta (mean and variance)", N_bins, 0., 0., "s");
	
	TProfile *de_t_sm_vs_t = new TProfile("de_t_sm_vs_t", ";t;#Delta t (mean and variance)", N_bins, t_min, t_max, "s");
	TProfile *de_t_sm_rel_vs_t = new TProfile("de_t_sm_rel_vs_t", ";t;mean of #Delta t / t", N_bins, t_min, t_max);

	TProfile *de_t_re_vs_t = new TProfile("de_t_re_vs_t", ";t;#Delta t (mean and variance)", N_bins, t_min, t_max, "s");
	TProfile *de_t_re_rel_vs_t = new TProfile("de_t_re_rel_vs_t", ";t;mean of #Delta t / t", N_bins, t_min, t_max);

	TProfile *acc_corr_vs_t = new TProfile("acc_corr_vs_t", ";t;acc_corr", N_bins, t_min, t_max);
	
	// simulation
	unsigned long N_ev = 1E9;
	
	for (unsigned long ev = 0; ev < N_ev; ev++) {
		// ----- generate elastic event -----

		double t_tr = gRandom->Rndm() * (t_max_full - t_min_full) + t_min_full;
		double phi_tr = gRandom->Rndm() * M_PI;	// just the upper half

		double w = t_dist(t_tr);

		double th_tr = sqrt(t_tr) / p;
		double th_x_tr = th_tr * cos(phi_tr);
		double th_y_tr = th_tr * sin(phi_tr);
		
		p_t_tr->Fill(t_tr, w);
		
		// ----- beam smearing: with true sigma -----

		// generate beam smearing
		double de_stat_th_x_L = gRandom->Gaus() * si_th_x_sim;
		double de_stat_th_x_R = gRandom->Gaus() * si_th_x_sim;
		double de_stat_th_y_L = gRandom->Gaus() * si_th_y_sim;
		double de_stat_th_y_R = gRandom->Gaus() * si_th_y_sim;

		double th_x_R_sm = th_x_tr + de_stat_th_x_R;
		double th_x_L_sm = th_x_tr + de_stat_th_x_L;
		double th_y_R_sm = th_y_tr + de_stat_th_y_R;
		double th_y_L_sm = th_y_tr + de_stat_th_y_L;
		
		double th_x_sm = (th_x_R_sm + th_x_L_sm)/2.;
		double th_y_sm = (th_y_R_sm + th_y_L_sm)/2.;

		double th_sm = sqrt(th_x_sm*th_x_sm + th_y_sm*th_y_sm);
		double t_sm = p*p * (th_x_sm*th_x_sm + th_y_sm*th_y_sm);
		
		de_t_sm_vs_t->Fill(t_tr, t_sm - t_tr, w);
		if (t_tr > 1E-5)
			de_t_sm_rel_vs_t->Fill(t_tr, (t_sm - t_tr)/t_tr, w);
		
		// acceptance correction for smearing correction
		double phi_corr = 0., div_corr = 0.;
		bool skip = CalculateAcceptanceCorrections(+1., th_y_L_sm, th_y_R_sm, th_y_sm, th_sm, si_th_y_sim, phi_corr, div_corr);
		phi_corr /= 2.; // only upper half in simulation 
		if (!skip) {
			p_t_sm->Fill(t_sm, w * div_corr * phi_corr);
		}
		
		// ----- beam smearing: with assumed sigma -----
		de_stat_th_x_L *= si_th_x_ass / si_th_x_sim;
		de_stat_th_x_R *= si_th_x_ass / si_th_x_sim;
		de_stat_th_y_L *= si_th_y_ass / si_th_y_sim;
		de_stat_th_y_R *= si_th_y_ass / si_th_y_sim;
		
		double th_x_R_sma = th_x_tr + de_stat_th_x_R;
		double th_x_L_sma = th_x_tr + de_stat_th_x_L;
		double th_y_R_sma = th_y_tr + de_stat_th_y_R;
		double th_y_L_sma = th_y_tr + de_stat_th_y_L;
		
		double th_x_sma = (th_x_R_sma + th_x_L_sma)/2.;
		double th_y_sma = (th_y_R_sma + th_y_L_sma)/2.;

		double th_sma = sqrt(th_x_sma*th_x_sma + th_y_sma*th_y_sma);
		double t_sma = p*p * (th_x_sma*th_x_sma + th_y_sma*th_y_sma);
		
		skip = CalculateAcceptanceCorrections(+1., th_y_L_sma, th_y_R_sma, th_y_sma, th_sma, si_th_y_ass, phi_corr, div_corr);
		phi_corr /= 2.; // only upper half in simulation 
		if (!skip) {
			p_t_sma->Fill(t_sma, w * div_corr * phi_corr);
		}
		
		// ----- discard events outside HW acceptance -----
		if (th_y_L_sm < th_y_lcut_hw_L || th_y_L_sm > th_y_hcut_hw_L || th_y_R_sm < th_y_lcut_hw_R || th_y_R_sm > th_y_hcut_hw_R)
			continue;
		
		// ----- mimic reconstruction -----

		// determine systematic shift
		double de_syst_th_x_L = be_x_L + al_x_L * th_x_L_sm;
		double de_syst_th_x_R = be_x_R + al_x_R * th_x_R_sm;
		double de_syst_th_y_L = be_y_L + al_y_L * th_y_L_sm;
		double de_syst_th_y_R = be_y_R + al_y_R * th_y_R_sm;

		double th_x_L_re = th_x_L_sm + de_syst_th_x_L;
		double th_x_R_re = th_x_R_sm + de_syst_th_x_R;
		double th_y_L_re = th_y_L_sm + de_syst_th_y_L;
		double th_y_R_re = th_y_R_sm + de_syst_th_y_R;

		double th_x_re = (th_x_R_re + th_x_L_re)/2.;
		double th_y_re = (th_y_R_re + th_y_L_re)/2.;

		double th_re = sqrt(th_x_re*th_x_re + th_y_re*th_y_re);
		double t_re = p*p * (th_x_re*th_x_re + th_y_re*th_y_re);

		// ----- acceptance correction for reco -----

		phi_corr = div_corr = 0.;
		skip = CalculateAcceptanceCorrections(+1., th_y_L_re, th_y_R_re, th_y_re, th_re, si_th_y_ass, phi_corr, div_corr);
		phi_corr /= 2.; // only upper half in simulation 
		if (skip)
			continue;
		
		acc_corr_vs_t->Fill(t_re, div_corr * phi_corr);

		double wc = w * div_corr * phi_corr;
		
		// ----- fill histograms -----

		//if (w > 1E3)
		//.	printf("ev %lu: w = %E, div_corr=%E, phi_corr=%E, th=%E (%E, %E)\n", ev, w, div_corr, phi_corr, th, th_y_lcut, th_y_hcut);

		p_t_re->Fill(t_re, wc);
		
		de_th_x_vs_th_x->Fill(th_x_tr, th_x_re - th_x_tr, w);
		de_th_y_vs_th_y->Fill(th_y_tr, th_y_re - th_y_tr, w);
		de_th_vs_th->Fill(th_tr, th_re - th_tr, w);

		de_t_re_vs_t->Fill(t_tr, t_re - t_tr, w);
		if (t_tr > 1E-5)
			de_t_re_rel_vs_t->Fill(t_tr, (t_re - t_tr)/t_tr, w);
	}

	TH1D *h_t_tr = p_t_tr->GetHist(N_ev);
	TH1D *h_t_sm = p_t_sm->GetHist(N_ev);
	TH1D *h_t_sma = p_t_sma->GetHist(N_ev);
	TH1D *h_t_re = p_t_re->GetHist(N_ev);
		
	de_th_x_vs_th_x->Write();
	de_th_y_vs_th_y->Write();

	de_t_sm_vs_t->Write();
	de_t_sm_rel_vs_t->Write();
	de_t_re_vs_t->Write();
	de_t_re_rel_vs_t->Write();

	acc_corr_vs_t->Write();

	h_t_tr->Write();
	h_t_sm->Write();
	h_t_re->Write();
	
	TH1D *unsm_corr_true = new TH1D(*h_t_tr);
	unsm_corr_true->SetName("unsm_corr_true");
	unsm_corr_true->Divide(h_t_sm);
	unsm_corr_true->SetLineColor(2);
	unsm_corr_true->Write();
	
	TH1D *unsm_corr_ass = new TH1D(*h_t_tr);
	unsm_corr_ass->SetName("unsm_corr_ass");
	unsm_corr_ass->Divide(h_t_sma);
	unsm_corr_ass->SetLineColor(4);
	unsm_corr_ass->Write();
	
	TH1D *h_t_uct = new TH1D(*h_t_re);
	h_t_uct->SetName("h_t_uct");
	h_t_uct->Multiply(unsm_corr_true);
	h_t_uct->SetLineColor(2);
	h_t_uct->Write();
	
	TH1D *h_t_uca = new TH1D(*h_t_re);
	h_t_uca->SetName("h_t_uca");
	h_t_uca->Multiply(unsm_corr_ass);
	h_t_uca->SetLineColor(4);
	h_t_uca->Write();
	
	TH1D *ratio_uct = new TH1D(*h_t_uct);
	ratio_uct->SetName("ratio_uct");
	ratio_uct->Divide(h_t_tr);
	ratio_uct->SetLineColor(2);
	ratio_uct->Write();
	
	TH1D *ratio_uca = new TH1D(*h_t_uca);
	ratio_uca->SetName("ratio_uca");
	ratio_uca->Divide(h_t_tr);
	ratio_uca->SetLineColor(4);
	ratio_uca->Write();
	
	delete f;
	printf(">> done\n");
	return 0;
}
