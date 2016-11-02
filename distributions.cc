#include "parameters.h"
#include "../common.h"

#include "TFile.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TChain.h"
#include "TH2D.h"
#include "TProfile.h"
#include "TMath.h"
#include "TF1.h"
#include "TTree.h"

#include <cmath>

using namespace std;

//----------------------------------------------------------------------------------------------------

map<unsigned int, double> cqa, cqb, cca, ccb, ccc, csi;
vector<unsigned int> cuts;	// list of active cuts

void BuildCuts()
{
	// cut structure:
	//	| a*qa + b*qb + c| < n_si * si

	// a: th_x_R, b: th_x_L
	cca[1] = 1.;
	ccb[1] = -1.;
	ccc[1] = -cut1_c;
	csi[1] = cut1_si;
	cuts.push_back(1);
	
	// a: th_y_R, b: th_y_L
	cca[2] = 1.;
	ccb[2] = -1.;
	ccc[2] = 0.;
	csi[2] = cut2_si;
	cuts.push_back(2);
	
	// a: th_x_R, b: vtx_x_R
	cca[3] = 0.;
	ccb[3] = 1.;
	ccc[3] = 0.;
	csi[3] = cut34_si;
	cuts.push_back(3);
	
	// a: th_x_L, b: vtx_x_L
	cca[4] = 0.;
	ccb[4] = 1.;
	ccc[4] = 0.;
	csi[4] = cut34_si;
	cuts.push_back(4);
	
	// a: y_R_N, b: y_R_F - y_R_N
	cca[5] = -cut5_a;
	ccb[5] = 1.;
	ccc[5] = cut5_b;
	csi[5] = cut5_si;
	cuts.push_back(5);
	
	// a: y_L_N, b: y_L_F - y_L_N
	cca[6] = -cut6_a;
	ccb[6] = 1.;
	ccc[6] = cut6_b;
	csi[6] = cut6_si;
	cuts.push_back(6);
	
	// a: th_x, b: vtx_x_R - vtx_x_L
	cca[7] = -cut7_al;
	ccb[7] = 1.;
	ccc[7] = -cut7_c;
	csi[7] = cut7_si;
	cuts.push_back(7);
	
	// fake cut !!
	// a: vtx_x_R, b: vtx_x_L
	cca[8] = +1;
	ccb[8] = -1.;
	ccc[8] = 0;
	csi[8] = 0;
}

//----------------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{
	if (argc != 2)
		return 1;

	// init diagonal settings
	Init(argv[1]);
	double sign = (diagonal == d45t) ? -1. : +1.;
	BuildCuts();
	double n_si = 3.0;

	// init files
	TFile *inF = new TFile((string("distill_") + argv[1] + ".root").c_str());
	TFile *outF = new TFile((string("distributions_") + argv[1] + ".root").c_str(), "recreate");

	// get input data
	TTree *inT = (TTree *) inF->Get("distilled");
	EventRed ev;
	inT->SetBranchAddress("v_L_F", &ev.v_L_F); inT->SetBranchAddress("x_L_F", &ev.x_L_F); inT->SetBranchAddress("y_L_F", &ev.y_L_F);
	inT->SetBranchAddress("v_L_N", &ev.v_L_N); inT->SetBranchAddress("x_L_N", &ev.x_L_N); inT->SetBranchAddress("y_L_N", &ev.y_L_N);
	inT->SetBranchAddress("v_R_N", &ev.v_R_N); inT->SetBranchAddress("x_R_N", &ev.x_R_N); inT->SetBranchAddress("y_R_N", &ev.y_R_N);
	inT->SetBranchAddress("v_R_F", &ev.v_R_F); inT->SetBranchAddress("x_R_F", &ev.x_R_F); inT->SetBranchAddress("y_R_F", &ev.y_R_F);

	inT->SetBranchAddress("timestamp", &ev.timestamp);
	inT->SetBranchAddress("run_num", &ev.run_num);
	inT->SetBranchAddress("bunch_num", &ev.bunch_num);
	inT->SetBranchAddress("event_num", &ev.event_num);
	inT->SetBranchAddress("trigger_num", &ev.trigger_num);
	inT->SetBranchAddress("trigger_bits", &ev.trigger_bits);
	
	// book metadata histograms
	TH1D *h_timestamp_dgn = new TH1D("h_timestamp_dgn", ";timestamp;rate   (Hz)", 40001, 40000-0.5, 80000+0.5);
	TH1D *h_timestamp_B0 = new TH1D("h_timestamp_B0", ";timestamp;rate   (Hz)", 40001, 40000-0.5, 80000+0.5);
	TH1D *h_timestamp_sel = new TH1D("h_timestamp_sel", ";timestamp;rate   (Hz)", 40001, 40000-0.5, 80000+0.5);

	TGraph *g_run_vs_timestamp = new TGraph(); g_run_vs_timestamp->SetName("g_run_vs_timestamp"); g_run_vs_timestamp->SetTitle(";timestamp;run");
	TGraph *g_ev_num_vs_timestamp = new TGraph(); g_ev_num_vs_timestamp->SetName("g_ev_num_vs_timestamp"); g_ev_num_vs_timestamp->SetTitle(";timestamp;ev_num");
	TGraph *g_tr_num_vs_timestamp = new TGraph(); g_tr_num_vs_timestamp->SetName("g_tr_num_vs_timestamp"); g_tr_num_vs_timestamp->SetTitle(";timestamp;tr_num");
	
	TGraph *g_timestamp_vs_ev_idx_sel = new TGraph(); g_timestamp_vs_ev_idx_sel->SetName("g_timestamp_vs_ev_idx_sel"); g_timestamp_vs_ev_idx_sel->SetTitle(";event index in distilled TTree;timestamp");

	// book hit-distribution histograms
	TH2D *h_y_L_N_vs_x_L_N_raw = new TH2D("h_y_L_N_vs_x_L_N_raw", ";x^{L,N};y^{L,N}", 100, -10., 10., 400, -40., 40.);
	TH2D *h_y_L_F_vs_x_L_F_raw = new TH2D("h_y_L_F_vs_x_L_F_raw", ";x^{L,F};y^{L,F}", 100, -10., 10., 400, -40., 40.);
	TH2D *h_y_R_N_vs_x_R_N_raw = new TH2D("h_y_R_N_vs_x_R_N_raw", ";x^{R,N};y^{R,N}", 100, -10., 10., 400, -40., 40.);
	TH2D *h_y_R_F_vs_x_R_F_raw = new TH2D("h_y_R_F_vs_x_R_F_raw", ";x^{R,F};y^{R,F}", 100, -10., 10., 400, -40., 40.);
	
	TH2D *h_y_L_N_vs_x_L_N_sel = new TH2D("h_y_L_N_vs_x_L_N_sel", ";x^{L,N};y^{L,N}", 100, -10., 10., 400, -40., 40.);
	TH2D *h_y_L_F_vs_x_L_F_sel = new TH2D("h_y_L_F_vs_x_L_F_sel", ";x^{L,F};y^{L,F}", 100, -10., 10., 400, -40., 40.);
	TH2D *h_y_R_N_vs_x_R_N_sel = new TH2D("h_y_R_N_vs_x_R_N_sel", ";x^{R,N};y^{R,N}", 100, -10., 10., 400, -40., 40.);
	TH2D *h_y_R_F_vs_x_R_F_sel = new TH2D("h_y_R_F_vs_x_R_F_sel", ";x^{R,F};y^{R,F}", 100, -10., 10., 400, -40., 40.);
	
	TGraph *g_y_L_N_vs_x_L_N_sel = new TGraph(); g_y_L_N_vs_x_L_N_sel->SetName("g_y_L_N_vs_x_L_N_sel"); g_y_L_N_vs_x_L_N_sel->SetTitle(";x^{L,N};y^{L,N}");
	TGraph *g_y_L_F_vs_x_L_F_sel = new TGraph(); g_y_L_F_vs_x_L_F_sel->SetName("g_y_L_F_vs_x_L_F_sel"); g_y_L_F_vs_x_L_F_sel->SetTitle(";x^{L,F};y^{L,F}");
	TGraph *g_y_R_N_vs_x_R_N_sel = new TGraph(); g_y_R_N_vs_x_R_N_sel->SetName("g_y_R_N_vs_x_R_N_sel"); g_y_R_N_vs_x_R_N_sel->SetTitle(";x^{R,N};y^{R,N}");
	TGraph *g_y_R_F_vs_x_R_F_sel = new TGraph(); g_y_R_F_vs_x_R_F_sel->SetName("g_y_R_F_vs_x_R_F_sel"); g_y_R_F_vs_x_R_F_sel->SetTitle(";x^{R,F};y^{R,F}");

	// book cut histograms
	map<unsigned int, TH1D *> h_cq;
	map<unsigned int, TH2D *> h2_cq, h2_cq_full;
	map<unsigned int, TGraph *> g_cq;
	for (map<unsigned int, double>::iterator cit = csi.begin(); cit != csi.end(); ++cit) {
		unsigned i = cit->first;
		char name[100], title[100];
		sprintf(name, "h_cq%i", i); sprintf(title, ";cq%i", i); h_cq[i] = new TH1D(name, title, 300, 0, 0);
		sprintf(name, "h2_cq%i", i); sprintf(title, ";cqa%i;cqb%i", i, i); h2_cq[i] = new TH2D(name, title, 100, 0, 0, 100, 0, 0);
		sprintf(name, "h2_cq_full%i", i); sprintf(title, ";cqa%i;cqb%i", i, i); h2_cq_full[i] = new TH2D(name, title, 200, 0, 0, 200, 0, 0);
		//sprintf(name, "g_cq%i", i); sprintf(title, ";cqa%i;cqb%i", i, i); g_cq[i] = new TGraph(); g_cq[i]->SetName(name); g_cq[i]->SetTitle(title);
	}
	
	// book alignment histograms
	TProfile *p_x_vs_y_L_F = new TProfile("p_x_vs_y_L_F", ";y^{L,F};x^{L,F};", 100, 0., 0.);
	TProfile *p_x_vs_y_L_N = new TProfile("p_x_vs_y_L_N", ";y^{L,N};x^{L,N};", 100, 0., 0.);
	TProfile *p_x_vs_y_R_N = new TProfile("p_x_vs_y_R_N", ";y^{R,N};x^{R,N};", 100, 0., 0.);
	TProfile *p_x_vs_y_R_F = new TProfile("p_x_vs_y_R_F", ";y^{R,F};x^{R,F};", 100, 0., 0.);

	// book raw (= before acceptance correction) histograms
	TH1D *th_y_L_diffNF = new TH1D("th_y_L_diffNF", ";#theta_{y}^{L,F} - #theta_{y}^{L,N}", 100, 0, 0);
	TH1D *th_y_R_diffNF = new TH1D("th_y_R_diffNF", ";#theta_{y}^{R,F} - #theta_{y}^{R,N}", 100, 0, 0);
	TProfile *th_y_L_diffNF_vs_th_y_L = new TProfile("th_y_L_diffNF_vs_th_y_L", ";#theta_{y}^{L};#theta_{y}^{L,F} - #theta_{y}^{L,N};", 100, 0., 0.);
	TProfile *th_y_R_diffNF_vs_th_y_R = new TProfile("th_y_R_diffNF_vs_th_y_R", ";#theta_{y}^{R};#theta_{y}^{R,F} - #theta_{y}^{R,N};", 100, 0., 0.);
	
	TH1D *th_y_diffLR = new TH1D("th_y_diffLR", ";#theta_{y}^{R} - #theta_{y}^{L}", 100, 0, 0);
	TH1D *th_x_diffLR = new TH1D("th_x_diffLR", ";#theta_{x}^{R} - #theta_{x}^{L}", 100, 0, 0);
	
	TProfile *th_y_diffLR_vs_th_y = new TProfile("th_y_diffLR_vs_th_y", ";#theta_{y};#theta_{y}^{R} - #theta_{y}^{L}", 100, 0., 0.);
	TProfile *th_x_diffLR_vs_th_x = new TProfile("th_x_diffLR_vs_th_x", ";#theta_{x};#theta_{x}^{R} - #theta_{x}^{L}", 100, 0., 0.);

	TH1D *th_y_diffLR_safe = new TH1D("th_y_diffLR_safe", ";#theta_{y}^{R} - #theta_{y}^{L}", 100, 0, 0);
	TH1D *th_x_diffLR_safe = new TH1D("th_x_diffLR_safe", ";#theta_{x}^{R} - #theta_{x}^{L}", 100, 0, 0);

	TProfile *p_th_x_L_vs_th_y_L_noYCorr = new TProfile("p_th_x_L_vs_th_y_L_noYCorr", ";#theta_{y}^{L};#theta_{x}^{L}", 30, 0., 0.);
	TProfile *p_th_x_R_vs_th_y_R_noYCorr = new TProfile("p_th_x_R_vs_th_y_R_noYCorr", ";#theta_{y}^{R};#theta_{x}^{R}", 30, 0., 0.);
	TProfile *p_th_x_L_vs_th_y_L = new TProfile("p_th_x_L_vs_th_y_L", ";#theta_{y}^{L};#theta_{x}^{L}", 30, 0., 0.);
	TProfile *p_th_x_R_vs_th_y_R = new TProfile("p_th_x_R_vs_th_y_R", ";#theta_{y}^{R};#theta_{x}^{R}", 30, 0., 0.);

	TH2D *h_th_y_L_vs_th_x_L = new TH2D("h_th_y_L_vs_th_x_L", ";#theta_{x}^{L};#theta_{y}^{L}", 100, 0, 0, 100, 0, 0);
	TH2D *h_th_y_R_vs_th_x_R = new TH2D("h_th_y_R_vs_th_x_R", ";#theta_{x}^{R};#theta_{y}^{R}", 100, 0, 0, 100, 0, 0);
	TH2D *h_th_y_vs_th_x = new TH2D("h_th_y_vs_th_x", ";#theta_{x};#theta_{y}", 100, -250E-6, +250E-6, 100, -250E-6, +250E-6);
	
	TGraph *g_th_y_L_vs_th_x_L = new TGraph(); g_th_y_L_vs_th_x_L->SetName("g_th_y_L_vs_th_x_L"); g_th_y_L_vs_th_x_L->SetTitle(";#theta_{x}^{L};#theta_{y}^{L}");
	TGraph *g_th_y_R_vs_th_x_R = new TGraph(); g_th_y_R_vs_th_x_R->SetName("g_th_y_R_vs_th_x_R"); g_th_y_R_vs_th_x_R->SetTitle(";#theta_{x}^{R};#theta_{y}^{R}");
	TGraph *g_th_y_vs_th_x = new TGraph(); g_th_y_vs_th_x->SetName("g_th_y_vs_th_x"); g_th_y_vs_th_x->SetTitle(";#theta_{x}^{L};#theta_{y}^{L}");
	
	TH2D *h_th_y_L_vs_th_y_R = new TH2D("h_th_y_L_vs_th_y_R", ";#theta_{y}^{R};#theta_{y}^{L}", 300, -150E-6, +150E-6, 300, -150E-6, +150E-6);
	TGraph *g_th_y_L_vs_th_y_R = new TGraph(); g_th_y_L_vs_th_y_R->SetName("g_th_y_L_vs_th_y_R"); g_th_y_L_vs_th_y_R->SetTitle(";#theta_{y}^{R};#theta_{y}^{L}");
	
	TH1D *h_th_x_raw = new TH1D("h_th_x_raw", ";#theta_{x}", 100, 0, 0);
	TH1D *h_th_y_raw = new TH1D("h_th_y_raw", ";#theta_{y}", 100, 0, 0);
	TH1D *h_th_y_raw_flipped = new TH1D("h_th_y_raw_flipped", ";#theta_{y}", 100, 0, 0);
	
	TH1D *h_th_x_L = new TH1D("h_th_x_L", ";#theta_{x}^{L}", 100, 0, 0);
	TH1D *h_th_x_R = new TH1D("h_th_x_R", ";#theta_{x}^{R}", 100, 0, 0);
	
	TProfile *p_nu_L_F_vs_th_x_L = new TProfile("p_nu_L_F_vs_th_x_L", ";#theta_{x}^{L};#nu^{L,F}", 100, 0., 0.);
	TProfile *p_nu_L_N_vs_th_x_L = new TProfile("p_nu_L_N_vs_th_x_L", ";#theta_{x}^{L};#nu^{L,N}", 100, 0., 0.);
	TProfile *p_nu_R_F_vs_th_x_R = new TProfile("p_nu_R_F_vs_th_x_R", ";#theta_{x}^{R};#nu^{R,F}", 100, 0., 0.);
	TProfile *p_nu_R_N_vs_th_x_R = new TProfile("p_nu_R_N_vs_th_x_R", ";#theta_{x}^{R};#nu^{R,N}", 100, 0., 0.);

	TProfile *p_vtx_x_L_vs_th_x = new TProfile("p_vtx_x_L_vs_th_x", ";#theta_{x};x^{*,L}", 100, 0., 0.);
	TProfile *p_vtx_x_L_vs_th_x_L = new TProfile("p_vtx_x_L_vs_th_x_L", ";#theta_{x}^{L};x^{*,L}", 100, 0., 0.);
	TProfile *p_vtx_x_R_vs_th_x = new TProfile("p_vtx_x_R_vs_th_x", ";#theta_{x};x^{*,R}", 100, 0., 0.);
	TProfile *p_vtx_x_R_vs_th_x_R = new TProfile("p_vtx_x_R_vs_th_x_R", ";#theta_{x}^{R};x^{*,R}", 100, 0., 0.);
	
	TProfile *p_vtx_x_L_vs_th_y_L = new TProfile("p_vtx_x_L_vs_th_y_L", ";#theta_{y}^{L};x^{*,L}", 100, 0., 0.);
	TProfile *p_vtx_x_R_vs_th_y_R = new TProfile("p_vtx_x_R_vs_th_y_R", ";#theta_{y}^{R};x^{*,R}", 100, 0., 0.);
	
	TH2D *h_vtx_x_L_vs_th_x_L = new TH2D("h_vtx_x_L_vs_th_x_L", ";#theta_{x}^{L};x^{*,L}", 100, 0., 0., 100, 0., 0.);
	TH2D *h_vtx_x_R_vs_th_x_R = new TH2D("h_vtx_x_R_vs_th_x_R", ";#theta_{x}^{R};x^{*,R}", 100, 0., 0., 100, 0., 0.);

	TH1D *h_vtx_x_diffLR = new TH1D("h_vtx_x_diffLR", ";x^{*,R} - x^{*,L}", 100, 0, 0);
	TH2D *h_vtx_x_L_vs_vtx_x_R = new TH2D("h_vtx_x_L_vs_vtx_x_R", ";x^{*,R};x^{*,L}", 100, 0, 0, 100, 0, 0);
	TProfile *p_vtx_x_diffLR_vs_th_y = new TProfile("p_vtx_x_diffLR_vs_th_y", ";#theta_{y};x^{*,R} - x^{*,L}", 100, 0., 0.);
	TProfile *p_vtx_x_diffLR_vs_th_x = new TProfile("p_vtx_x_diffLR_vs_th_x", ";#theta_{x};x^{*,R} - x^{*,L}", 100, 0., 0.);
	TH2D *h_vtx_x_diffLR_vs_th_y = new TH2D("h_vtx_x_diffLR_vs_th_y", ";#theta_{y};x^{*,R} - x^{*,L}", 100, 0., 0., 100, 0., 0.);
	TH2D *h_vtx_x_diffLR_vs_th_x = new TH2D("h_vtx_x_diffLR_vs_th_x", ";#theta_{x};x^{*,R} - x^{*,L}", 100, 0., 0., 100, 0., 0.);
	
	// book acceptance-correction histograms
	TH1D *h_th_before = new TH1D("h_th_before", ";#theta", N_bins, 0., 0.); h_th_before->Sumw2();
	TH1D *h_th_after = new TH1D("h_th_after", ";#theta", N_bins, 0., 0.); h_th_after->Sumw2();
	TProfile *p_th_div_corr = new TProfile("p_th_div_corr", ";#theta_{y}^{*}", N_bins, 0., 0.);
	TProfile *p_th_phi_corr = new TProfile("p_th_phi_corr", ";#theta^{*}", N_bins, 0., 0.);
	TProfile *p_th_full_corr = new TProfile("p_th_full_corr", ";#theta^{*}", N_bins, 0., 0.);

	unsigned int N_bins_full = N_bins + N_low + N_high;
	double *unifBE = BuildBinning(false, N_bins, N_low, N_high);
	TH1D *h_t_ub_before = new TH1D("h_t_ub_before", ";|t|", N_bins_full, unifBE); h_t_ub_before->Sumw2();
	TH1D *h_t_ub_after = new TH1D("h_t_ub_after", ";|t|", N_bins_full, unifBE); h_t_ub_after->Sumw2();
	TProfile *p_t_ub_div_corr = new TProfile("p_t_ub_div_corr", ";t_ub_{y}", N_bins_full, unifBE);
	TProfile *p_t_ub_phi_corr = new TProfile("p_t_ub_phi_corr", ";t", N_bins_full, unifBE);
	TProfile *p_t_ub_full_corr = new TProfile("p_t_ub_full_corr", ";t", N_bins_full, unifBE);
	
	double *expBE = BuildBinning(true, N_bins, N_low, N_high);
	TH1D *h_t_eb_before = new TH1D("h_t_eb_before", ";|t|", N_bins_full, expBE); h_t_eb_before->Sumw2();
	TH1D *h_t_eb_after = new TH1D("h_t_eb_after", ";|t|", N_bins_full, expBE); h_t_eb_after->Sumw2();
	TProfile *p_t_eb_div_corr = new TProfile("p_t_eb_div_corr", ";t_eb_{y}", N_bins_full, expBE);
	TProfile *p_t_eb_phi_corr = new TProfile("p_t_eb_phi_corr", ";t", N_bins_full, expBE);
	TProfile *p_t_eb_full_corr = new TProfile("p_t_eb_full_corr", ";t", N_bins_full, expBE);

	TH1D *h_th_x_before = new TH1D("h_th_x_before", ";#theta_{x}", 300, -250E-6, +250E-6); h_th_x_before->Sumw2();
	TH1D *h_th_x_after = new TH1D("h_th_x_after", ";#theta_{x}", 300, -250E-6, +250E-6); h_th_x_after->Sumw2();
	TH1D *h_th_y_before = new TH1D("h_th_y_before", ";#theta_{y}", 300, -250E-6, +250E-6); h_th_y_before->Sumw2();
	TH1D *h_th_y_before_flipped = new TH1D("h_th_y_before_flipped", ";#theta_{y}", 300, -250E-6, +250E-6); h_th_y_before_flipped->Sumw2();
	TH1D *h_th_y_after = new TH1D("h_th_y_after", ";#theta_{y}", 300, -250E-6, +250E-6); h_th_y_after->Sumw2();
	TH1D *h_th_y_after_flipped = new TH1D("h_th_y_after_flipped", ";#theta_{y}", 300, -250E-6, +250E-6); h_th_y_after_flipped->Sumw2();

	TH2D *h_th_y_vs_th_x_before = new TH2D("h_th_y_vs_th_x_before", ";#theta_{x};#theta_{y}", 400, -400E-6, +400E-6, 400, -400E-6, +400E-6); h_th_y_vs_th_x_before->Sumw2();
	TH2D *h_th_y_vs_th_x_after = new TH2D("h_th_y_vs_th_x_after", ";#theta_{x};#theta_{y}", 400, -400E-6, +400E-6, 400, -400E-6, +400E-6); h_th_y_vs_th_x_after->Sumw2();
	TH2D *h_th_vs_phi_after = new TH2D("h_th_vs_phi_after", ";#phi;#theta", 100, 0, 0, 100, 0, 0); h_th_vs_phi_after->Sumw2();
	
	TGraph *g_weight_vs_th_y = new TGraph(); g_weight_vs_th_y->SetName("g_weight_vs_th_y"); g_weight_vs_th_y->SetTitle(";#theta_{y};weight");

	// book background histograms
	map<unsigned int, TH1D *> hb_cq;
	for (map<unsigned int, double>::iterator cit = csi.begin(); cit != csi.end(); ++cit) {
		unsigned i = cit->first;
		char name[100], title[100];
		sprintf(name, "hb_cq%i", i); sprintf(title, ";cq%i", i); hb_cq[i] = new TH1D(name, title, 1500, -30, +30);
	}

	TH1D *hb_th_x_L = new TH1D("hb_th_x_L", ";-#theta_{x}^{L}", 100, 0, 0); hb_th_x_L->Sumw2();
	TH1D *hb_th_x_R = new TH1D("hb_th_x_R", ";#theta_{x}^{R}", 100, 0, 0); hb_th_x_R->Sumw2();
	TH1D *hb_th_y_L = new TH1D("hb_th_y_L", ";#theta_{y}^{L}", 100, 0, 0); hb_th_y_L->Sumw2();
	TH1D *hb_th_y_R = new TH1D("hb_th_y_R", ";#theta_{y}^{R}", 100, 0, 0); hb_th_y_R->Sumw2();

	TH2D *hb_th_y_L_vs_th_x_L = new TH2D("hb_th_y_L_vs_th_x_L", ";-#theta_{x}^{L};#theta_{y}^{L}", 100, -2E-3, +2E-3, 200, -120E-6, 120E-6);
	TH2D *hb_th_y_R_vs_th_x_R = new TH2D("hb_th_y_R_vs_th_x_R", ";#theta_{x}^{R};#theta_{y}^{R}", 100, -2E-3, +2E-3, 200, -120E-6, 120E-6);
	
	TH1D *hb_th_y_diffLR = new TH1D("hb_th_y_diffLR", ";#theta_{y}^{R} - #theta_{y}^{L}", 500, 0, 0);
	TH1D *hb_th_x_diffLR = new TH1D("hb_th_x_diffLR", ";#theta_{x}^{R} - #theta_{x}^{L}", 500, 0, 0);
	
	TH2D *hb_th_y_diffLR_vs_th_y = new TH2D("hb_th_y_diffLR_vs_th_y", ";#theta_{y};#theta_{y}^{R} - #theta_{y}^{L}", 100, 0, 0, 100, 0, 0);
	TH2D *hb_th_x_diffLR_vs_th_y = new TH2D("hb_th_x_diffLR_vs_th_y", ";#theta_{y};#theta_{x}^{R} - #theta_{x}^{L}", 100, 0, 0, 100, 0, 0);
	
	TH2D *hb_th_y_L_vs_th_y_R = new TH2D("hb_th_y_L_vs_th_y_R", ";-#theta_{y}^{R};#theta_{y}^{L}", 100, 0, 0, 100, 0, 0);
	TH2D *hb_th_x_L_vs_th_x_R = new TH2D("hb_th_x_L_vs_th_x_R", ";-#theta_{x}^{R};#theta_{x}^{L}", 100, 0, 0, 100, 0, 0);

	// zero counters
	unsigned long n_ev_full = 0, n_ev_cuts12 = 0, n_ev_cuts3t6 = 0, n_ev_cut7 = 0, n_ev_all_cuts = 0;
	map<unsigned int, unsigned long> n_ev_cut;
	for (map<unsigned int, double>::iterator it = cca.begin(); it != cca.end(); ++it)
		n_ev_cut[it->first] = 0;

	double th_min = 1E100;

	unsigned long n_BX_tlr = 0, n_BX_RP_tlr = 0;
	
	// build histograms
	for (int ev_idx = 0; ev_idx < inT->GetEntries(); ++ev_idx) {
		inT->GetEntry(ev_idx);
		
		// for trigger efficiency test
		if ((ev.trigger_bits & 512) > 0) {
			n_BX_tlr++;

			if ((ev.trigger_bits & 3) > 0)
				n_BX_RP_tlr++;
		}
		
		// choose the desired trigger
		if ((ev.trigger_bits & 3) == 0)	// RP trigger only
			continue;

		// remove troublesome runs
		unsigned int run = ev.run_num / 10000;
		if (run == 6930 || run == 6940 || run == 6943)
			continue;

		// diagonal cut
		bool allDiagonalRPs = (ev.v_L_F && ev.v_L_N && ev.v_R_N && ev.v_R_F);
		if (!allDiagonalRPs)
			continue;
		
		h_timestamp_dgn->Fill(ev.timestamp);

		// select big bunch only
		if (ev.bunch_num != 0)
			continue;
			
		/*
		// cut out the beam-halo region
		th_y_lcut_L = 40E-6;
		th_y_lcut_R = 40E-6;
		bool outsideBeamHalo = (fabs(ev.y_L_N) > 8 && fabs(ev.y_L_F) > 8 && fabs(ev.y_R_N) > 8 && fabs(ev.y_R_F) > 8);
		if (!outsideBeamHalo)
			continue;
		*/

		// apply fine alignment
		double x_L_F = ev.x_L_F - al_a_L_F * ev.y_L_F - al_b_L_F, y_L_F = ev.y_L_F - al_c_L_F;
		double x_L_N = ev.x_L_N - al_a_L_N * ev.y_L_N - al_b_L_N, y_L_N = ev.y_L_N - al_c_L_N;
		double x_R_N = ev.x_R_N - al_a_R_N * ev.y_R_N - al_b_R_N, y_R_N = ev.y_R_N - al_c_R_N;
		double x_R_F = ev.x_R_F - al_a_R_F * ev.y_R_F - al_b_R_F, y_R_F = ev.y_R_F - al_c_R_F;
		
		// fill pre-selection histograms
		h_y_L_N_vs_x_L_N_raw->Fill(x_L_N, y_L_N);
		h_y_L_F_vs_x_L_F_raw->Fill(x_L_F, y_L_F);
		h_y_R_N_vs_x_R_N_raw->Fill(x_R_N, y_R_N);
		h_y_R_F_vs_x_R_F_raw->Fill(x_R_F, y_R_F);

		h_timestamp_B0->Fill(ev.timestamp);
		g_run_vs_timestamp->SetPoint(g_run_vs_timestamp->GetN(), ev.timestamp, ev.run_num);
		g_ev_num_vs_timestamp->SetPoint(g_ev_num_vs_timestamp->GetN(), ev.timestamp, ev.event_num);
		g_tr_num_vs_timestamp->SetPoint(g_tr_num_vs_timestamp->GetN(), ev.timestamp, ev.trigger_num);

		// vertex reconstruction
		//double det_x = L_x_N * v_x_F - L_x_F * v_x_N;
		//double vtx_x_L = (x_L_F * L_x_N - x_L_N * L_x_F) / det_x;
		//double vtx_x_R = (x_R_F * L_x_N - x_R_N * L_x_F) / det_x;

		double la_R_F = 229., la_R_N = -1129.;	// experimental L_x / v_x in mm
		double la_L_F = -360., la_L_N = +1013.;	
		
		double nu_L_F = x_L_F / v_x_F;	// in mm
		double nu_R_F = x_R_F / v_x_F;
		double nu_L_N = x_L_N / v_x_N;
		double nu_R_N = x_R_N / v_x_N;

		double vtx_x_L = ( x_L_F / v_x_F * la_L_N - x_L_N / v_x_N * la_L_F ) / (la_L_N - la_L_F); // in mm
		double vtx_x_R = ( x_R_F / v_x_F * la_R_N - x_R_N / v_x_N * la_R_F ) / (la_R_N - la_R_F);
		double vtx_x = (vtx_x_L + vtx_x_R) / 2.;

		// theta_y reconstruction
		double th_y_L_F = y_L_F / L_y_F / 1.004;	// in rad
		double th_y_L_N = y_L_N / L_y_N / 1.004;
		double th_y_L = - (th_y_L_N + th_y_L_F) / 2.;

		double th_y_R_F = y_R_F / L_y_F / 0.996;
		double th_y_R_N = y_R_N / L_y_N / 0.996;
		double th_y_R = + (th_y_R_N + th_y_R_F) / 2.;

		double th_y = (th_y_R + th_y_L) / 2.;

		// theta_x reconstruction
		//double th_x_R = + (x_R_N - v_x_rat * x_R_F) / L_x_N + th_x_al_R + th_x_be_R * (th_y_R - th_y_shift);

		double th_x_R = + ( (x_R_F - x_R_N) / d_RP - dvds_x * vtx_x_R ) / dLds_x * 0.994 - (th_x_al_R + th_x_be_R * th_y_R); // in rad
		double th_x_L = - ( (x_L_F - x_L_N) / d_RP - dvds_x * vtx_x_L ) / dLds_x * 1.006 - (th_x_al_L + th_x_be_L * th_y_L);
		
		double th_x_R_noYCorr = + ( (x_R_F - x_R_N) / d_RP - dvds_x * vtx_x_R ) / dLds_x;
		double th_x_L_noYCorr = - ( (x_L_F - x_L_N) / d_RP - dvds_x * vtx_x_L ) / dLds_x;

		double th_x = (th_x_R + th_x_L) / 2.;

		// theta reconstruction
		double th_sq = th_x*th_x + th_y*th_y;
		double th = sqrt(th_sq);
		double phi = atan2(th_y, th_x);
		
		// t reconstruction
		double t_x = p*p * th_x * th_x;
		double t_y = p*p * th_y * th_y;
		double t = t_x + t_y;

		// cut evaluation
		cqa[1] = th_x_R;	cqb[1] = th_x_L;
		cqa[2] = th_y_R;	cqb[2] = th_y_L;
		cqa[3] = th_x_R;	cqb[3] = vtx_x_R;
		cqa[4] = th_x_L;	cqb[4] = vtx_x_L;
		cqa[5] = y_R_N;		cqb[5] = y_R_F - y_R_N;
		cqa[6] = y_L_N;		cqb[6] = y_L_F - y_L_N;
		cqa[7] = th_x;		cqb[7] = vtx_x_R - vtx_x_L;
		cqa[8] = vtx_x_R;	cqb[8] = vtx_x_L;
	
		map<unsigned int, double> cv; // cut value
		map<unsigned int, bool> ct; // cut true or false
		for (map<unsigned int, double>::iterator cit = csi.begin(); cit != csi.end(); ++cit) {
			unsigned ci = cit->first;
			cv[ci] = cca[ci]*cqa[ci] + ccb[ci]*cqb[ci] + ccc[ci];
			ct[ci] = (fabs(cv[ci]) <= n_si * csi[ci]);
		}

		// increment counters
		n_ev_full++;
		for (map<unsigned int, double>::iterator cit = csi.begin(); cit != csi.end(); ++cit) {
			unsigned ci = cit->first;
			if (ct[ci])
				n_ev_cut[ci]++;
		}
		if (ct[1] && ct[2])
			n_ev_cuts12++;
		if (ct[2] && ct[4] && ct[5] && ct[6])
			n_ev_cuts3t6++;
		if (ct[7])
			n_ev_cut7++;
		
		// flags
		bool select = true;	// and between all cuts
		for (unsigned int i = 0; i < cuts.size(); i++)
			select &= ct[cuts[i]];

		bool bckg = !select; //ct[1] && (!ct[2]);
		
		// fill background distributions
		for (map<unsigned int, double>::iterator cit = csi.begin(); cit != csi.end(); ++cit) {
			unsigned ci = cit->first;
			bool add = true;
			for (map<unsigned int, double>::iterator citt = csi.begin(); citt != csi.end(); ++citt) {
				if (cit->first != citt->first)
					add &= ct[citt->first];
			}

			if (add)
				hb_cq[ci]->Fill(cv[ci] / csi[ci]);
		}

		if (bckg) {
			hb_th_y_L->Fill(th_y_L);
			hb_th_y_R->Fill(th_y_R);
			hb_th_x_L->Fill(-th_x_L);
			hb_th_x_R->Fill(th_x_R);
		
			hb_th_y_L_vs_th_x_L->Fill(-th_x_L, th_y_L);
			hb_th_y_R_vs_th_x_R->Fill(th_x_R, th_y_R);

			hb_th_y_diffLR->Fill(th_y_R - th_y_L);
			hb_th_x_diffLR->Fill(th_x_R - th_x_L);
	
			hb_th_y_diffLR_vs_th_y->Fill(th_y, th_y_R - th_y_L);
			hb_th_x_diffLR_vs_th_y->Fill(th_y, th_x_R - th_x_L);
	
			hb_th_y_L_vs_th_y_R->Fill(th_y_R, th_y_L);
			hb_th_x_L_vs_th_x_R->Fill(th_x_R, th_x_L);
		}
		
		// fill no-cut histograms
		for (map<unsigned int, double>::iterator cit = csi.begin(); cit != csi.end(); ++cit) {
			unsigned ci = cit->first;
			//h2_cq_full[ci]->Fill(ccb[ci]*cqa[ci] - cca[ci]*cqb[ci], cca[ci]*cqa[ci] + ccb[ci]*cqb[ci] + ccc[ci]);
			h2_cq_full[ci]->Fill(cqa[ci], cqb[ci]);
		}

		// elastic cut
		if (!select)
			continue;

		n_ev_all_cuts++;
		h_timestamp_sel->Fill(ev.timestamp);
		g_timestamp_vs_ev_idx_sel->SetPoint(g_timestamp_vs_ev_idx_sel->GetN(), ev_idx, ev.timestamp);
		
		h_y_L_N_vs_x_L_N_sel->Fill(x_L_N, y_L_N);
		h_y_L_F_vs_x_L_F_sel->Fill(x_L_F, y_L_F);
		h_y_R_N_vs_x_R_N_sel->Fill(x_R_N, y_R_N);
		h_y_R_F_vs_x_R_F_sel->Fill(x_R_F, y_R_F);

		g_y_L_N_vs_x_L_N_sel->SetPoint(g_y_L_N_vs_x_L_N_sel->GetN(), ev.x_L_N, ev.y_L_N);
		g_y_L_F_vs_x_L_F_sel->SetPoint(g_y_L_F_vs_x_L_F_sel->GetN(), ev.x_L_F, ev.y_L_F);
		g_y_R_N_vs_x_R_N_sel->SetPoint(g_y_R_N_vs_x_R_N_sel->GetN(), ev.x_R_N, ev.y_R_N);
		g_y_R_F_vs_x_R_F_sel->SetPoint(g_y_R_F_vs_x_R_F_sel->GetN(), ev.x_R_F, ev.y_R_F);

		p_x_vs_y_L_F->Fill(y_L_F, x_L_F);
		p_x_vs_y_L_N->Fill(y_L_N, x_L_N);
		p_x_vs_y_R_N->Fill(y_R_N, x_R_N);
		p_x_vs_y_R_F->Fill(y_R_F, x_R_F);

		// fill raw histograms
		th_y_L_diffNF->Fill(th_y_L_F - th_y_L_N);
		th_y_R_diffNF->Fill(th_y_R_F - th_y_R_N);
		th_y_L_diffNF_vs_th_y_L->Fill(th_y_L, th_y_L_F - th_y_L_N);
		th_y_R_diffNF_vs_th_y_R->Fill(th_y_R, th_y_R_F - th_y_R_N);

		th_y_diffLR->Fill(th_y_R - th_y_L);
		th_x_diffLR->Fill(th_x_R - th_x_L);
		th_y_diffLR_vs_th_y->Fill(th_y, th_y_R - th_y_L);
		th_x_diffLR_vs_th_x->Fill(th_x, th_x_R - th_x_L);
		
		if (fabs(th_y) > 35E-6 && fabs(th_y) < 95E-6) {
			th_y_diffLR_safe->Fill(th_y_R - th_y_L);
			th_x_diffLR_safe->Fill(th_x_R - th_x_L);
		}
	
		p_th_x_L_vs_th_y_L_noYCorr->Fill(th_y_L, th_x_L_noYCorr);
		p_th_x_R_vs_th_y_R_noYCorr->Fill(th_y_R, th_x_R_noYCorr);

		p_th_x_L_vs_th_y_L->Fill(th_y_L, th_x_L);
		p_th_x_R_vs_th_y_R->Fill(th_y_R, th_x_R);
	
		h_th_y_L_vs_th_x_L->Fill(th_x_L, th_y_L);
		h_th_y_R_vs_th_x_R->Fill(th_x_R, th_y_R);
		h_th_y_vs_th_x->Fill(th_x, th_y);
	
		g_th_y_L_vs_th_x_L->SetPoint(g_th_y_L_vs_th_x_L->GetN(), th_x_L, th_y_L);
		g_th_y_R_vs_th_x_R->SetPoint(g_th_y_R_vs_th_x_R->GetN(), th_x_R, th_y_R);
		g_th_y_vs_th_x->SetPoint(g_th_y_vs_th_x->GetN(), th_x, th_y);
	
		h_th_y_L_vs_th_y_R->Fill(th_y_R, th_y_L);
		g_th_y_L_vs_th_y_R->SetPoint(g_th_y_L_vs_th_y_R->GetN(), th_y_R, th_y_L);

		h_th_x_raw->Fill(th_x);
		h_th_y_raw->Fill(th_y);
		h_th_y_raw_flipped->Fill(-th_y);
		
		h_th_x_L->Fill(th_x_L);
		h_th_x_R->Fill(th_x_R);
	
		p_nu_L_F_vs_th_x_L->Fill(th_x_L, nu_L_F);
		p_nu_L_N_vs_th_x_L->Fill(th_x_L, nu_L_N);
		p_nu_R_F_vs_th_x_R->Fill(th_x_R, nu_R_F);
		p_nu_R_N_vs_th_x_R->Fill(th_x_R, nu_R_N);
	
		p_vtx_x_L_vs_th_x->Fill(th_x, vtx_x_L);
		p_vtx_x_L_vs_th_x_L->Fill(th_x_L, vtx_x_L);
		p_vtx_x_R_vs_th_x->Fill(th_x, vtx_x_R);
		p_vtx_x_R_vs_th_x_R->Fill(th_x_R, vtx_x_R);
		
		p_vtx_x_L_vs_th_y_L->Fill(th_y_L, vtx_x_L);
		p_vtx_x_R_vs_th_y_R->Fill(th_y_R, vtx_x_R);
	
		h_vtx_x_L_vs_th_x_L->Fill(th_x_L, vtx_x_L);
		h_vtx_x_R_vs_th_x_R->Fill(th_x_R, vtx_x_R);

		h_vtx_x_diffLR->Fill(vtx_x_R - vtx_x_L);
		h_vtx_x_L_vs_vtx_x_R->Fill(vtx_x_R, vtx_x_L);
		p_vtx_x_diffLR_vs_th_y->Fill(th_y, vtx_x_R - vtx_x_L);
		p_vtx_x_diffLR_vs_th_x->Fill(th_x, vtx_x_R - vtx_x_L);
		h_vtx_x_diffLR_vs_th_y->Fill(th_y, vtx_x_R - vtx_x_L);
		h_vtx_x_diffLR_vs_th_x->Fill(th_x, vtx_x_R - vtx_x_L);
		
		for (map<unsigned int, double>::iterator cit = csi.begin(); cit != csi.end(); ++cit) {
			unsigned ci = cit->first;
			h_cq[ci]->Fill(cv[ci]);
			h2_cq[ci]->Fill(cqa[ci], cqb[ci]);
			//h2_cq[ci]->Fill(ccb[ci]*cqa[ci] - cca[ci]*cqb[ci], cca[ci]*cqa[ci] + ccb[ci]*cqb[ci] + ccc[ci]);
			//g_cq[ci]->SetPoint(g_cq[ci]->GetN(), cqa[ci], cqb[ci]);
		}


		// calculate acceptance divergence correction
		double phi_corr = 0., div_corr = 0.;
		bool skip = CalculateAcceptanceCorrections(sign, th_y_L, th_y_R, th_y, th, si_th_y_os, phi_corr, div_corr);
		if (skip)
			continue;

		double corr = div_corr * phi_corr;

		th_min = min(th_min, th);

		// fill acceptance histograms
		h_th_before->Fill(th, 1.);
		h_th_after->Fill(th, corr);
		p_th_div_corr->Fill(th_y, div_corr);
		p_th_phi_corr->Fill(th, phi_corr);
		p_th_full_corr->Fill(th, corr);		
	
		h_t_ub_before->Fill(t, 1.);
		h_t_ub_after->Fill(t, corr);
		p_t_ub_div_corr->Fill(t_y, div_corr);
		p_t_ub_phi_corr->Fill(t, phi_corr);
		p_t_ub_full_corr->Fill(t, corr);
		
		h_t_eb_before->Fill(t, 1.);
		h_t_eb_after->Fill(t, corr);
		p_t_eb_div_corr->Fill(t_y, div_corr);
		p_t_eb_phi_corr->Fill(t, phi_corr);
		p_t_eb_full_corr->Fill(t, corr);

		h_th_x_before->Fill(th_x, 1.);
		h_th_x_after->Fill(th_x, 1.);
		h_th_y_before->Fill(th_y, 1.);
		h_th_y_before_flipped->Fill(-th_y, 1.);
		h_th_y_after->Fill(th_y, div_corr);
		h_th_y_after_flipped->Fill(-th_y, div_corr);

		h_th_y_vs_th_x_before->Fill(th_x, th_y, 1.);
		h_th_y_vs_th_x_after->Fill(th_x, th_y, div_corr);

		h_th_vs_phi_after->Fill(phi, th, div_corr);

		g_weight_vs_th_y->SetPoint(g_weight_vs_th_y->GetN(), th_y, div_corr);
	}

	printf("n_BX_tlr = %lu\n", n_BX_tlr);
	printf("n_BX_RP_tlr = %lu\n", n_BX_RP_tlr);

	printf(">> th_min = %E\n", th_min);

	// derived plots
	TGraph *th_y_sigmaLR_vs_th_y = new TGraph(); th_y_sigmaLR_vs_th_y->SetName("th_y_sigmaLR_vs_th_y"); th_y_sigmaLR_vs_th_y->SetTitle(";#theta_{y};RMS of #Delta^{R-L} #theta_{y}");
	TGraph *th_x_sigmaLR_vs_th_x = new TGraph(); th_x_sigmaLR_vs_th_x->SetName("th_x_sigmaLR_vs_th_x"); th_x_sigmaLR_vs_th_x->SetTitle(";#theta_{x};RMS of #Delta^{R-L} #theta_{x}");

	for (int bi = 1; bi <= th_y_diffLR_vs_th_y->GetNbinsX(); ++bi) {
		double c = th_y_diffLR_vs_th_y->GetBinCenter(bi);

		double N = th_y_diffLR_vs_th_y->GetBinEntries(bi);
		double Sy = th_y_diffLR_vs_th_y->GetBinContent(bi) * N;
		double Syy = th_y_diffLR_vs_th_y->GetSumw2()->At(bi);
		
		double si_sq = Syy/N - Sy*Sy/N/N;
		double si = (si_sq >= 0.) ? sqrt(si_sq) : 0.;

		int idx = th_y_sigmaLR_vs_th_y->GetN();
		th_y_sigmaLR_vs_th_y->SetPoint(idx, c, si);
	}
	
	for (int bi = 1; bi <= th_x_diffLR_vs_th_x->GetNbinsX(); ++bi) {
		double c = th_x_diffLR_vs_th_x->GetBinCenter(bi);

		double N = th_x_diffLR_vs_th_x->GetBinEntries(bi);
		double Sx = th_x_diffLR_vs_th_x->GetBinContent(bi) * N;
		double Sxx = th_x_diffLR_vs_th_x->GetSumw2()->At(bi);
		
		double si_sq = Sxx/N - Sx*Sx/N/N;
		double si = (si_sq >= 0.) ? sqrt(si_sq) : 0.;

		int idx = th_x_sigmaLR_vs_th_x->GetN();
		th_x_sigmaLR_vs_th_x->SetPoint(idx, c, si);
	}

	// normalize histograms
	h_th_before->Scale(1., "width");
	h_th_after->Scale(1., "width");
	h_t_ub_before->Scale(1., "width");
	h_t_ub_after->Scale(1., "width");
	h_t_eb_before->Scale(1., "width");
	h_t_eb_after->Scale(1., "width");

	// fit histograms
	double th_y_low_bound = (diagonal == d45b) ? (th_y_lcut_L+th_y_lcut_R)/2. + 0.5E-6 : -((th_y_hcut_L+th_y_hcut_R)/2. - 3E-6);
	double th_y_high_bound = (diagonal == d45b) ? (th_y_hcut_L+th_y_hcut_R)/2. - 3E-6 : -((th_y_lcut_L+th_y_lcut_R)/2. + 0.5E-6);
	printf("* th_y fit bounds: from %E to %E\n", th_y_low_bound, th_y_high_bound);

	printf("* fitting th_y_L_diffNF_vs_th_y_L\n");
	th_y_L_diffNF_vs_th_y_L->Fit("pol1", "", "", th_y_low_bound, th_y_high_bound);
	printf("* fitting th_y_R_diffNF_vs_th_y_R\n");
	th_y_R_diffNF_vs_th_y_R->Fit("pol1", "", "", th_y_low_bound, th_y_high_bound);

	printf("* fitting th_y_diffLR_vs_th_y\n");
	th_y_diffLR_vs_th_y->Fit("pol1", "", "", ((diagonal == d45b) ? 30E-6 : -100E-6), ((diagonal == d45b) ? 100E-6 : -30E-6));
	printf("* fitting th_x_diffLR_vs_th_x\n");
	th_x_diffLR_vs_th_x->Fit("pol1", "", "", -120E-6, +120E-6);

	printf("* fitting nu_L_F\n");
	p_nu_L_F_vs_th_x_L->Fit("pol1", "", "", -150E-6, +150E-6);
	printf("* fitting nu_L_N\n");
	p_nu_L_N_vs_th_x_L->Fit("pol1", "", "", -150E-6, +150E-6);
	printf("* fitting nu_R_F\n");
	p_nu_R_F_vs_th_x_R->Fit("pol1", "", "", -150E-6, +150E-6);
	printf("* fitting nu_R_N\n");
	p_nu_R_N_vs_th_x_R->Fit("pol1", "", "", -150E-6, +150E-6);

	printf("* fitting p_vtx_x_L_vs_th_x_L\n");
	p_vtx_x_L_vs_th_x_L->Fit("pol1", "", "", -120E-6, +120E-6);
	printf("* fitting p_vtx_x_L_vs_th_x\n");
	p_vtx_x_L_vs_th_x->Fit("pol1", "", "", -120E-6, +120E-6);
	printf("* fitting p_vtx_x_R_vs_th_x_R\n");
	p_vtx_x_R_vs_th_x_R->Fit("pol1", "", "", -120E-6, +120E-6);
	printf("* fitting p_vtx_x_R_vs_th_x\n");
	p_vtx_x_R_vs_th_x->Fit("pol1", "", "", -120E-6, +120E-6);
	
	printf("* fitting p_vtx_x_diffLR_vs_th_x\n");
	p_vtx_x_diffLR_vs_th_x->Fit("pol1", "", "", -120E-6, +120E-6);

	printf("* fitting p_vtx_x_L_vs_th_y_L\n");
	p_vtx_x_L_vs_th_y_L->Fit("pol1", "", "", th_y_low_bound, th_y_high_bound);
	printf("* fitting p_vtx_x_R_vs_th_y_R\n");
	p_vtx_x_R_vs_th_y_R->Fit("pol1", "", "", th_y_low_bound, th_y_high_bound);
	
	printf("* fitting p_th_x_L_vs_th_y_L_noYCorr\n");
	p_th_x_L_vs_th_y_L_noYCorr->Fit("pol1", "", "", th_y_low_bound, th_y_high_bound);
	printf("* fitting p_th_x_R_vs_th_y_R_noYCorr\n");
	p_th_x_R_vs_th_y_R_noYCorr->Fit("pol1", "", "", th_y_low_bound, th_y_high_bound);

	printf("* fitting p_th_x_L_vs_th_y_L\n");
	p_th_x_L_vs_th_y_L->Fit("pol1", "", "", th_y_low_bound, th_y_high_bound);
	printf("* fitting p_th_x_R_vs_th_y_R\n");
	p_th_x_R_vs_th_y_R->Fit("pol1", "", "", th_y_low_bound, th_y_high_bound);

	printf("* fitting h_th_x_R\n");
	h_th_x_R->Fit("gaus", "", "", -50E-6, +50E-6);
	printf("* fitting h_th_x_L\n");
	h_th_x_L->Fit("gaus", "", "", -50E-6, +50E-6);
	
	// double-gauss fit for background histograms
	TF1 *dg = new TF1("dg", "[0]*exp(-(x-[1])*(x-[1])/2/[2]/[2]) + [3]*exp(-(x-[4])*(x-[4])/2/[5]/[5])");
	for (map<unsigned int, TH1D *>::iterator it = hb_cq.begin(); it != hb_cq.end(); ++it) {
		printf("* Fitting background projection %u\n", it->first);
		dg->SetParameters(5E3, 0., 1., 1E2, 0., 3.);
		it->second->Fit(dg);

		double A = dg->GetParameter(0), mu = dg->GetParameter(1), si = dg->GetParameter(2);
		double signal_int = A*si*sqrt(2.*M_PI)/2. * ( TMath::Erf((+3. - mu)/sqrt(2.)/si) - TMath::Erf((-3. - mu)/sqrt(2.)/si) );
		
		A = dg->GetParameter(3); mu = dg->GetParameter(4); si = dg->GetParameter(5);
		double background_int = A*si*sqrt(2.*M_PI)/2. * ( TMath::Erf((+3. - mu)/sqrt(2.)/si) - TMath::Erf((-3. - mu)/sqrt(2.)/si) );
		printf("\tsignal int = %E\n", signal_int);
		printf("\tbackground int = %E\n", background_int);
		printf("\tsignal + background int = %E\n", signal_int + background_int);
		printf("\tbackground/signal = %E\n", background_int / signal_int);
	}
	
	// set colors
	h_th_after->SetLineColor(2);
	h_t_ub_after->SetLineColor(2);
	h_t_eb_before->SetLineColor(8);
	h_t_eb_after->SetLineColor(4);
	
	h_th_x_before->SetLineColor(4);
	h_th_x_after->SetLineColor(4);
	h_th_y_before->SetLineColor(2);
	h_th_y_after->SetLineColor(2);
	
	// normalization
	hb_th_x_L->Scale(1., "width");
	hb_th_x_R->Scale(1., "width");
	hb_th_y_L->Scale(1., "width");
	hb_th_y_R->Scale(1., "width");

	th_y_diffLR_safe->Fit("gaus");
	th_x_diffLR_safe->Fit("gaus");

	// save histograms
	TCanvas *c;
	
	gDirectory = outF->mkdir("metadata");
	h_timestamp_dgn->Write();
	h_timestamp_B0->SetLineColor(4);
	h_timestamp_B0->Write();
	h_timestamp_sel->SetLineColor(2);
	h_timestamp_sel->Write();
	g_timestamp_vs_ev_idx_sel->Write();

	c = new TCanvas("rate cmp");
	h_timestamp_dgn->Draw();
	h_timestamp_B0->Draw("sames");
	h_timestamp_sel->Draw("sames");
	c->Write();

	g_run_vs_timestamp->Write();
	g_ev_num_vs_timestamp->Write();
	g_tr_num_vs_timestamp->Write();
	
	gDirectory = outF->mkdir("hit distributions");
	h_y_L_N_vs_x_L_N_raw->Write();
	h_y_L_F_vs_x_L_F_raw->Write();
	h_y_R_N_vs_x_R_N_raw->Write();
	h_y_R_F_vs_x_R_F_raw->Write();
	
	h_y_L_N_vs_x_L_N_sel->Write();
	h_y_L_F_vs_x_L_F_sel->Write();
	h_y_R_N_vs_x_R_N_sel->Write();
	h_y_R_F_vs_x_R_F_sel->Write();

	g_y_L_N_vs_x_L_N_sel->Write();
	g_y_L_F_vs_x_L_F_sel->Write();
	g_y_R_N_vs_x_R_N_sel->Write();
	g_y_R_F_vs_x_R_F_sel->Write();

	gDirectory = outF->mkdir("elastic cuts");
	for (map<unsigned int, double>::iterator it = cca.begin(); it != cca.end(); ++it) {
		unsigned int i = it->first;

		printf("* fitting cut distribution %u\n", i);
		h_cq[i]->Fit("gaus");

		h_cq[i]->Write();
		h2_cq[i]->Write();
		h2_cq_full[i]->Write();
		//g_cq[i]->Write();

		h2_cq_full[i]->Draw();

		char buf[100];
		sprintf(buf, "plot_before_cq%u", i);
		c = new TCanvas(buf);
		c->SetLogz(1);
		h2_cq_full[i]->Draw("colz");
		double lim = (i < 5 || i > 6) ? 3E-4 : 30.;
		double qa[2] = {-lim, +lim};
		double qbp[2]= {(+n_si*csi[i] - cca[i]*qa[0] - ccc[i])/ccb[i], (+n_si*csi[i] - cca[i]*qa[1] - ccc[i])/ccb[i]};
		double qbm[2]= {(-n_si*csi[i] - cca[i]*qa[0] - ccc[i])/ccb[i], (-n_si*csi[i] - cca[i]*qa[1] - ccc[i])/ccb[i]};
		TGraph *gP = new TGraph(2, qa, qbp); gP->Draw("l");
		TGraph *gM = new TGraph(2, qa, qbm); gM->Draw("l");
		c->Write();
		
		sprintf(buf, "plot_after_cq%u", i);
		c = new TCanvas(buf);
		c->SetLogz(1);
		h2_cq[i]->Draw("colz");
		gP = new TGraph(2, qa, qbp); gP->Draw("l");
		gM = new TGraph(2, qa, qbm); gM->Draw("l");
		gP->Draw("l");
		gM->Draw("l");
		c->Write();
	}

	gDirectory = outF->mkdir("selected - hits");
	p_x_vs_y_L_F->Write();
	p_x_vs_y_L_N->Write();
	p_x_vs_y_R_N->Write();
	p_x_vs_y_R_F->Write();
	
	gDirectory = outF->mkdir("selected - angles");
	th_y_L_diffNF->Write();
	th_y_R_diffNF->Write();
	th_y_L_diffNF_vs_th_y_L->Write();
	th_y_R_diffNF_vs_th_y_R->Write();

	th_y_diffLR->Write();
	th_x_diffLR->Write();
	th_y_diffLR_vs_th_y->Write();
	th_x_diffLR_vs_th_x->Write();
	th_y_sigmaLR_vs_th_y->Write();
	th_x_sigmaLR_vs_th_x->Write();
	
	th_y_diffLR_safe->Write();
	th_x_diffLR_safe->Write();

	p_th_x_L_vs_th_y_L_noYCorr->Write();
	p_th_x_R_vs_th_y_R_noYCorr->Write();
	p_th_x_L_vs_th_y_L->Write();
	p_th_x_R_vs_th_y_R->Write();

	h_th_y_L_vs_th_x_L->Write();
	h_th_y_R_vs_th_x_R->Write();
	h_th_y_vs_th_x->Write();

	g_th_y_L_vs_th_x_L->Write();
	g_th_y_R_vs_th_x_R->Write();
	g_th_y_vs_th_x->Write();
	
	h_th_y_L_vs_th_y_R->Write();
	g_th_y_L_vs_th_y_R->Write();
	
	h_th_x_raw->Write();
	h_th_y_raw->Write();
	h_th_y_raw_flipped->Write();
	
	h_th_x_L->Write();
	h_th_x_R->Write();
	
	gDirectory = outF->mkdir("selected - vertex");
	p_nu_L_F_vs_th_x_L->Write();
	p_nu_L_N_vs_th_x_L->Write();
	p_nu_R_F_vs_th_x_R->Write();
	p_nu_R_N_vs_th_x_R->Write();
	
	p_vtx_x_L_vs_th_x_L->Write();
	p_vtx_x_L_vs_th_x->Write();
	p_vtx_x_R_vs_th_x_R->Write();
	p_vtx_x_R_vs_th_x->Write();
	
	p_vtx_x_L_vs_th_y_L->Write();
	p_vtx_x_R_vs_th_y_R->Write();

	h_vtx_x_L_vs_th_x_L->Write();
	h_vtx_x_R_vs_th_x_R->Write();

	h_vtx_x_diffLR->Write();
	h_vtx_x_L_vs_vtx_x_R->Write();
	p_vtx_x_diffLR_vs_th_y->Write();
	p_vtx_x_diffLR_vs_th_x->Write();
	h_vtx_x_diffLR_vs_th_y->Write();
	h_vtx_x_diffLR_vs_th_x->Write();
	
	gDirectory = outF->mkdir("acceptance correction");
	h_th_before->Write();
	h_th_after->Write();
	p_th_div_corr->Write();
	p_th_phi_corr->Write();
	p_th_full_corr->Write();
		
	h_t_ub_before->Write();
	h_t_ub_after->Write();
	p_t_ub_div_corr->Write();
	p_t_ub_phi_corr->Write();
	p_t_ub_full_corr->Write();
	
	h_t_eb_before->Write();
	h_t_eb_after->Write();
	p_t_eb_div_corr->Write();
	p_t_eb_phi_corr->Write();
	p_t_eb_full_corr->Write();
		
	h_th_x_before->Write();
	h_th_x_after->Write();
	h_th_y_before->Write();
	h_th_y_before_flipped->Write();
	h_th_y_after->Write();
	h_th_y_after_flipped->Write();
	
	h_th_y_vs_th_x_before->Write();
	h_th_y_vs_th_x_after->Write();
	h_th_vs_phi_after->Write();

	g_weight_vs_th_y->Write();
	
	c = new TCanvas("th_y, th_x cmp");
	c->SetLogy(1);
	h_th_x_after->Draw();
	h_th_y_after->Draw("same");
	c->Write();
	
	c = new TCanvas("th cmp");
	c->SetLogy(1);
	h_th_after->Draw();
	h_th_before->Draw("same");
	c->Write();
	
	c = new TCanvas("t cmp");
	c->SetLogy(1);
	//h_t_ub_after->Draw();
	//h_t_ub_before->Draw("same");
	h_t_eb_after->Draw("");
	h_t_eb_before->Draw("same");
	c->Write();
	
	gDirectory = outF->mkdir("background");
	for (map<unsigned int, TH1D *>::iterator it = hb_cq.begin(); it != hb_cq.end(); ++it)
		it->second->Write();
	
	hb_th_y_L->SetLineColor(2);
	hb_th_y_R->SetLineColor(4);
	
	hb_th_y_L->Write();
	hb_th_y_R->Write();

	c = new TCanvas("th_y LR cmp");
	c->SetLogy(1);
	hb_th_y_L->Draw("");
	hb_th_y_R->Draw("same");
	c->Write();
	
	hb_th_x_L->SetLineColor(2);
	hb_th_x_R->SetLineColor(4);
	
	hb_th_x_L->Write();
	hb_th_x_R->Write();
	
	c = new TCanvas("th_x LR cmp");
	c->SetLogy(1);
	hb_th_x_L->Draw("");
	hb_th_x_R->Draw("same");
	c->Write();

	hb_th_y_L_vs_th_x_L->Write();
	hb_th_y_R_vs_th_x_R->Write();
	
	hb_th_y_diffLR->Write();
	hb_th_x_diffLR->Write();
	
	hb_th_y_diffLR_vs_th_y->Write();
	hb_th_x_diffLR_vs_th_y->Write();
	
	hb_th_y_L_vs_th_y_R->Write();
	hb_th_x_L_vs_th_x_R->Write();

	// print counters
	printf("* event numbers:\n");
	printf("\tfull: %lu\n", n_ev_full);
	for (map<unsigned int, unsigned long>::iterator it = n_ev_cut.begin(); it != n_ev_cut.end(); ++it)
		printf("\tcut %u: %lu\n", it->first, it->second);
	printf("\tcuts 1 & 2: %lu\n", n_ev_cuts12);
	printf("\tcuts 3 to 6: %lu\n", n_ev_cuts3t6);
	printf("\tcut 7: %lu\n", n_ev_cut7);
	printf("\tall cuts: %lu\n", n_ev_all_cuts);

	return 0;
}
