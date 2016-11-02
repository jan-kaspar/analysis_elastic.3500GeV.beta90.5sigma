#include "TH1D.h"
#include "TProfile.h"
#include "TFile.h"

TH1D *h_temp;

void WriteRatio(const char *label, TProfile *ht, TProfile *hr)
{
	TH1D *h = new TH1D(*h_temp);
	h->SetName(label);

	for (int i = 1; i <= h->GetNbinsX(); i++) {
		double vt = ht->GetBinContent(i), et = ht->GetBinError(i);
		double vr = hr->GetBinContent(i), er = hr->GetBinError(i);
		//double c = ht->GetBinCenter(i);

		double res = (vr > 0.) ? vt/vr - 1. : 0.;
		double res_e = (vr > 0.) ? sqrt(et*et/vr/vr + vt*vt/vr/vr/vr/vr*er*er) : 0.;

		h->SetBinContent(i, res);
		h->SetBinError(i, res_e);
		
		//printf("\t %u, c: %.3f, t: %.3f +- %.3f, r: %.3f +- %.3f, res: %.3f +- %.3f\n", i, c, vt, et, vr, er, res, res_e);
	}

	h->Write();
}

//----------------------------------------------------------------------------------------------------

void WriteRatioH(const char *label, TH1D *ht, TH1D *hr)
{
	TH1D *h = new TH1D(*ht);
	h->SetName(label);

	for (int i = 1; i <= h->GetNbinsX(); i++) {
		double vt = ht->GetBinContent(i), et = ht->GetBinError(i);
		double vr = hr->GetBinContent(i), er = hr->GetBinError(i);
		//double c = ht->GetBinCenter(i);

		double res = (vr > 0.) ? vt/vr - 1. : 0.;
		double res_e = (vr > 0.) ? sqrt(et*et/vr/vr + vt*vt/vr/vr/vr/vr*er*er) : 0.;

		h->SetBinContent(i, res);
		h->SetBinError(i, res_e);
		
		//printf("\t %u, c: %.3f, t: %.3f +- %.3f, r: %.3f +- %.3f, res: %.3f +- %.3f\n", i, c, vt, et, vr, er, res, res_e);
	}

	h->Write();
}

//----------------------------------------------------------------------------------------------------

void differences()
{
	TFile *f_ns_smz = new TFile("mc_none.root");
	TFile *f_ns_smp = new TFile("mc_none,smp.root");
	TFile *f_ns_smm = new TFile("mc_none,smm.root");

	TFile *f_s_smz = new TFile("mc_optics:ypxp.root");
	TFile *f_s_smp = new TFile("mc_optics:ypxp,smp.root");
	TFile *f_s_smm = new TFile("mc_optics:ypxp,smm.root");

	h_temp = (TH1D *) f_ns_smz->Get("p_t_tr");

	TProfile *ac_ns_smz = (TProfile *) f_ns_smz->Get("acc_corr_vs_t");
	TProfile *ac_ns_smp = (TProfile *) f_ns_smp->Get("acc_corr_vs_t");
	TProfile *ac_ns_smm = (TProfile *) f_ns_smm->Get("acc_corr_vs_t");

	TProfile *ac_s_smz = (TProfile *) f_s_smz->Get("acc_corr_vs_t");
	TProfile *ac_s_smp = (TProfile *) f_s_smp->Get("acc_corr_vs_t");
	TProfile *ac_s_smm = (TProfile *) f_s_smm->Get("acc_corr_vs_t");
	
	TProfile *uc_ns_smz = (TProfile *) f_ns_smz->Get("unsm_corr_ass");
	TProfile *uc_ns_smp = (TProfile *) f_ns_smp->Get("unsm_corr_ass");
	TProfile *uc_ns_smm = (TProfile *) f_ns_smm->Get("unsm_corr_ass");

	TProfile *uc_s_smz = (TProfile *) f_s_smz->Get("unsm_corr_ass");
	TProfile *uc_s_smp = (TProfile *) f_s_smp->Get("unsm_corr_ass");
	TProfile *uc_s_smm = (TProfile *) f_s_smm->Get("unsm_corr_ass");

	TFile *fOut = new TFile("differences.root", "recreate");

	WriteRatio("ac_ns_smz", ac_ns_smz, ac_ns_smz);
	WriteRatio("ac_ns_smp", ac_ns_smp, ac_ns_smz);
	WriteRatio("ac_ns_smm", ac_ns_smm, ac_ns_smz);
	
	WriteRatio("ac_s_smz", ac_s_smz, ac_ns_smz);
	WriteRatio("ac_s_smp", ac_s_smp, ac_ns_smz);
	WriteRatio("ac_s_smm", ac_s_smm, ac_ns_smz);
	
	WriteRatioH("uc_ns_smz", uc_ns_smz, uc_ns_smz);
	WriteRatioH("uc_ns_smp", uc_ns_smp, uc_ns_smz);
	WriteRatioH("uc_ns_smm", uc_ns_smm, uc_ns_smz);
	
	WriteRatioH("uc_s_smz", uc_s_smz, uc_ns_smz);
	WriteRatioH("uc_s_smp", uc_s_smp, uc_ns_smz);
	WriteRatioH("uc_s_smm", uc_s_smm, uc_ns_smz);

	delete fOut;
}
