using namespace std;

#include "parameters.h"
#include "../common.h"
#include "../Profile.h"
#include "../unfolding_CF.h"
#include "../unfolding_GR.h"

#include "TFile.h"
#include "TH1D.h"
#include "TProfile.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TMath.h"
#include "TRandom2.h"


//----------------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{
	if (argc != 2)
		return 1;

	// init diagonal settings
	Init(argv[1]);

	// get input data
	string inFn = string("distributions_") + argv[1] + ".root";	
	TFile *inF = new TFile(inFn.c_str());
	
	TH1D *h_t_ub = (TH1D *) inF->Get("acceptance correction/h_t_ub_after");
	TH1D *h_t_eb = (TH1D *) inF->Get("acceptance correction/h_t_eb_after");
	
	// get EPL 95 data as hight-|t| guide
	TFile *oldF = new TFile("../publication1.root");
	TH1D *h_t_old = (TH1D *) oldF->Get("h_dsdt");
	h_t_old->Sumw2();
	double sf = bckg_correction * full_eff_corr / L_int_eff;
	h_t_old->Scale(1. / sf);

	// prepare output
	string outFn = string("unfolding_") + argv[1] + ".root";	

	/*
	string outFn = string("unfolding_plus_") + argv[1] + ".root";	
	si_th_y_os += si_th_y_os_unc;
	si_th_x_os += si_th_x_os_unc;
	
	string outFn = string("unfolding_minus_") + argv[1] + ".root";	
	si_th_y_os -= si_th_y_os_unc;
	si_th_x_os -= si_th_x_os_unc;
	*/

	TFile *outF = new TFile(outFn.c_str(), "recreate");

	// set ranges
	CF::t_middle = 0.4;
	t_min = t_min_fit; t_max = 1.0;
	t_min_full = 0.0; t_max_full = 1.1;

	// run CF unfolding
	gDirectory = outF->mkdir("cf,ub");
	CF::DoUnfolding(h_t_ub, h_t_old);
	
	gDirectory = outF->mkdir("cf,eb");
	CF::DoUnfolding(h_t_eb, h_t_old);
	
	/*
	// get reference unfolding matrix
	TFile *rF = new TFile("mc_ref.root");
	const TMatrixD &ub_M = GR::GetMatrix((TH2D *) rF->Get("uni/h_res_mat2"));
	const TMatrixD &eb_M = GR::GetMatrix((TH2D *) rF->Get("exp/h_res_mat2"));

	// run GR unfolding
	alpha_gr = 1E-2;
	gDirectory = outF->mkdir("gr,ub");
	GR::DoUnfolding(h_t_ub, ub_M);
	
	alpha_gr = 1E-3;
	gDirectory = outF->mkdir("gr,eb");
	GR::DoUnfolding(h_t_eb, eb_M);
	*/
}
