#include "TChain.h"
#include "TCanvas.h"
#include "TH1D.h"

#include "/afs/cern.ch/exp/totem/scratch/jkaspar/software/offline/424/src/TotemAnalysis/TotemNtuplizer/interface/TriggerDataFormats.h"
#include "/afs/cern.ch/exp/totem/scratch/jkaspar/software/offline/424/src/TotemAnalysis/TotemNtuplizer/interface/RawDataFormats.h"
#include "/afs/cern.ch/exp/totem/scratch/jkaspar/software/offline/424/src/TotemAnalysis/TotemNtuplizer/interface/RPRootTrackInfo.h"

#include <vector>
#include <string>
#include <cmath>

//#include "RPRootTrackInfo.h"
//. gSystem->AddLinkedLibs("-L/afs/cern.ch/exp/totem/scratch/jkaspar/software/offline/424/lib/slc5_amd64_gcc434/ -lTotemAnalysisTotemNtuplizer")

double L_y_N = 237.7E3;			// mm
double L_y_F = 263.2E3;			// mm
double cut2_si = 3.6E-6;		// rad

using namespace std;

//----------------------------------------------------------------------------------------------------

void shower_test2(string file, bool diag45b = false)
{
	printf("\n\n********** %s, %s **********\n", file.c_str(), (diag45b) ? "diagonal 45B - 56T" : "diagonal 45T - 56B");

	TChain *ch = new TChain("TotemNtuple");
	ch->Add(file.c_str());

	// select branches
	ch->SetBranchStatus("*", 0);
	
	ch->SetBranchStatus("trigger_data.*", 1);
	ch->SetBranchStatus("event_info.*", 1);

	unsigned int rp_sel[] = { 20, 21, 24, 25, 120, 121, 124, 125 };
	for (unsigned int i = 0; i < 8; i++) {
		char buf[100];
		sprintf(buf, "digi_rp_%u.*", rp_sel[i]); ch->SetBranchStatus(buf, 1);
		sprintf(buf, "nonpar_patterns_rp_%u.*", rp_sel[i]); ch->SetBranchStatus(buf, 1);
		sprintf(buf, "track_rp_%u.*", rp_sel[i]); ch->SetBranchStatus(buf, 1);
		sprintf(buf, "multi_track_rp_%u.*", rp_sel[i]); ch->SetBranchStatus(buf, 1);
	}

	// set intput branches
	TriggerData *triggerData = new TriggerData();
	ch->SetBranchAddress("trigger_data.", &triggerData);

	EventMetaData *metaData = new EventMetaData();
	ch->SetBranchAddress("event_info.", &metaData);
	
	RPRootDumpDigiInfo *digi_L_F = new RPRootDumpDigiInfo; 
	RPRootDumpDigiInfo *digi_L_N = new RPRootDumpDigiInfo; 
	RPRootDumpDigiInfo *digi_R_N = new RPRootDumpDigiInfo; 
	RPRootDumpDigiInfo *digi_R_F = new RPRootDumpDigiInfo; 
	
	RPRootDumpDigiInfo *digi_od_L_F = new RPRootDumpDigiInfo; 
	RPRootDumpDigiInfo *digi_od_L_N = new RPRootDumpDigiInfo; 
	RPRootDumpDigiInfo *digi_od_R_N = new RPRootDumpDigiInfo; 
	RPRootDumpDigiInfo *digi_od_R_F = new RPRootDumpDigiInfo; 
	
	RPRootDumpTrackInfo *rp_L_F = new RPRootDumpTrackInfo();
	RPRootDumpTrackInfo *rp_L_N = new RPRootDumpTrackInfo();
	RPRootDumpTrackInfo *rp_R_N = new RPRootDumpTrackInfo();
	RPRootDumpTrackInfo *rp_R_F = new RPRootDumpTrackInfo();
	
	vector<RPRootDumpTrackInfo> *mrp_L_F = new vector<RPRootDumpTrackInfo>();
	vector<RPRootDumpTrackInfo> *mrp_L_N = new vector<RPRootDumpTrackInfo>();
	vector<RPRootDumpTrackInfo> *mrp_R_N = new vector<RPRootDumpTrackInfo>();
	vector<RPRootDumpTrackInfo> *mrp_R_F = new vector<RPRootDumpTrackInfo>();

	RPRootDumpPatternInfo *pat_L_F = new RPRootDumpPatternInfo();
	RPRootDumpPatternInfo *pat_L_N = new RPRootDumpPatternInfo();
	RPRootDumpPatternInfo *pat_R_N = new RPRootDumpPatternInfo();
	RPRootDumpPatternInfo *pat_R_F = new RPRootDumpPatternInfo();
	
	RPRootDumpPatternInfo *pat_od_L_F = new RPRootDumpPatternInfo();
	RPRootDumpPatternInfo *pat_od_L_N = new RPRootDumpPatternInfo();
	RPRootDumpPatternInfo *pat_od_R_N = new RPRootDumpPatternInfo();
	RPRootDumpPatternInfo *pat_od_R_F = new RPRootDumpPatternInfo();
	
	if (diag45b) {
		ch->SetBranchAddress("track_rp_25.", &rp_L_F);
		ch->SetBranchAddress("track_rp_21.", &rp_L_N);
		ch->SetBranchAddress("track_rp_120.", &rp_R_N);
		ch->SetBranchAddress("track_rp_124.", &rp_R_F);
		
		ch->SetBranchAddress("multi_track_rp_25", &mrp_L_F);
		ch->SetBranchAddress("multi_track_rp_21", &mrp_L_N);
		ch->SetBranchAddress("multi_track_rp_120", &mrp_R_N);
		ch->SetBranchAddress("multi_track_rp_124", &mrp_R_F);
		
		ch->SetBranchAddress("digi_rp_25.", &digi_L_F);
		ch->SetBranchAddress("digi_rp_21.", &digi_L_N);
		ch->SetBranchAddress("digi_rp_120.", &digi_R_N);
		ch->SetBranchAddress("digi_rp_124.", &digi_R_F);
		
		ch->SetBranchAddress("digi_rp_24.", &digi_od_L_F);
		ch->SetBranchAddress("digi_rp_20.", &digi_od_L_N);
		ch->SetBranchAddress("digi_rp_121.", &digi_od_R_N);
		ch->SetBranchAddress("digi_rp_125.", &digi_od_R_F);
		
		ch->SetBranchAddress("nonpar_patterns_rp_25.", &pat_L_F);
		ch->SetBranchAddress("nonpar_patterns_rp_21.", &pat_L_N);
		ch->SetBranchAddress("nonpar_patterns_rp_120.", &pat_R_N);
		ch->SetBranchAddress("nonpar_patterns_rp_124.", &pat_R_F);
		
		ch->SetBranchAddress("nonpar_patterns_rp_24.", &pat_od_L_F);
		ch->SetBranchAddress("nonpar_patterns_rp_20.", &pat_od_L_N);
		ch->SetBranchAddress("nonpar_patterns_rp_121.", &pat_od_R_N);
		ch->SetBranchAddress("nonpar_patterns_rp_125.", &pat_od_R_F);
	} else {
		ch->SetBranchAddress("track_rp_24.", &rp_L_F);
		ch->SetBranchAddress("track_rp_20.", &rp_L_N);
		ch->SetBranchAddress("track_rp_121.", &rp_R_N);
		ch->SetBranchAddress("track_rp_125.", &rp_R_F);
		
		ch->SetBranchAddress("multi_track_rp_24", &mrp_L_F);
		ch->SetBranchAddress("multi_track_rp_20", &mrp_L_N);
		ch->SetBranchAddress("multi_track_rp_121", &mrp_R_N);
		ch->SetBranchAddress("multi_track_rp_125", &mrp_R_F);
		
		ch->SetBranchAddress("digi_rp_24.", &digi_L_F);
		ch->SetBranchAddress("digi_rp_20.", &digi_L_N);
		ch->SetBranchAddress("digi_rp_121.", &digi_R_N);
		ch->SetBranchAddress("digi_rp_125.", &digi_R_F);
		
		ch->SetBranchAddress("digi_rp_25.", &digi_od_L_F);
		ch->SetBranchAddress("digi_rp_21.", &digi_od_L_N);
		ch->SetBranchAddress("digi_rp_120.", &digi_od_R_N);
		ch->SetBranchAddress("digi_rp_124.", &digi_od_R_F);
		
		ch->SetBranchAddress("nonpar_patterns_rp_24.", &pat_L_F);
		ch->SetBranchAddress("nonpar_patterns_rp_20.", &pat_L_N);
		ch->SetBranchAddress("nonpar_patterns_rp_121.", &pat_R_N);
		ch->SetBranchAddress("nonpar_patterns_rp_125.", &pat_R_F);
		
		ch->SetBranchAddress("nonpar_patterns_rp_25.", &pat_od_L_F);
		ch->SetBranchAddress("nonpar_patterns_rp_21.", &pat_od_L_N);
		ch->SetBranchAddress("nonpar_patterns_rp_120.", &pat_od_R_N);
		ch->SetBranchAddress("nonpar_patterns_rp_124.", &pat_od_R_F);
	}
	
	// reset counters
	unsigned int N = 0, N_base = 0;
	
	unsigned int N_pl_empty_N=0, N_pl_insuff_N=0, N_pl_suff_N=0;
	unsigned int N_pl_empty_F=0, N_pl_insuff_F=0, N_pl_suff_F=0;
	unsigned int N_pl_suff=0, N_pl_suff_odN_insuff=0, N_pl_suff_odN_suff=0;

	unsigned int N_p_empty_N=0, N_p_less_N=0, N_p_valid_N=0, N_p_more_N=0;
	unsigned int N_p_empty_F=0, N_p_less_F=0, N_p_valid_F=0, N_p_more_F=0;
	unsigned int N_pvv=0, N_pvi=0, N_piv=0, N_pii=0;
	unsigned int N_pee=0, N_pmm=0;
	
	unsigned int N_pl_suff_pat_ii=0;

	unsigned int N_vv=0, N_vi=0, N_iv=0, N_ii=0;

	unsigned int N_mtc_N = 0, N_mtc_F = 0, N_mtc = 0;
	unsigned int N_veto_N = 0, N_veto_F = 0, N_veto = 0;
	unsigned int N_mtc_hy = 0, N_mtc_hy_veto = 0;
	unsigned int N_mtc_hy_Nmore_Finvalid = 0, N_mtc_hy_nveto_Nmore_Finvalid = 0;

	unsigned int N_mtc_pat_more = 0, N_mtc_pat_invalid = 0;
	unsigned int N_pl_suff_odN_insuff_pat_invalid=0, N_pl_suff_odN_suff_pat_invalid=0;
	unsigned int N_pl_suff_pileup=0, N_pl_suff_pileup_odN_insuff=0;

	// event loop
	for (unsigned int ev = 0; ev < ch->GetEntries(); ev++) {
		/*
		if (ev >= 100000)
			break;
		*/
		
		ch->GetEvent(ev);

		/*
		if (metaData->run_no != 69290000)
			//continue;
			break;
			*/

		N++;

		// only events triggered by RPV
		if ((triggerData->input_status_bits & 1) == 0)
			continue;
		
		// only bunch-pair number 0
		if (triggerData->bunch_num != 0)
			continue;

		// valid track fits left
		if (!rp_L_F->valid || !rp_L_N->valid)
			continue;

		/*
		// veto on the other diagonal
		if (pat_od_R_N->u.size() > 0 || pat_od_R_N->v.size() > 0 || pat_od_R_F->u.size() > 0 || pat_od_R_F->v.size() > 0)
			continue;
		if (pat_od_L_N->u.size() > 0 || pat_od_L_N->v.size() > 0 || pat_od_L_F->u.size() > 0 || pat_od_L_F->v.size() > 0)
			continue;
		*/

		N_base++;

		// digi analysis
		unsigned int pl_empty = 1;
		unsigned int pl_suff = 8;

		bool pl_empty_N = (digi_R_N->numberOfPlanesOn <= pl_empty);
		bool pl_insuff_N = (digi_R_N->numberOfPlanesOn > pl_empty && digi_R_N->numberOfPlanesOn < pl_suff);
		bool pl_suff_N = (digi_R_N->numberOfPlanesOn >= pl_suff);
		
		bool pl_empty_F = (digi_R_F->numberOfPlanesOn <= pl_empty);
		bool pl_insuff_F = (digi_R_F->numberOfPlanesOn > pl_empty && digi_R_F->numberOfPlanesOn < pl_suff);
		bool pl_suff_F = (digi_R_F->numberOfPlanesOn >= pl_suff);
		
		bool pl_empty_odN = (digi_od_R_N->numberOfPlanesOn <= pl_empty);
		bool pl_insuff_odN = (digi_od_R_N->numberOfPlanesOn > pl_empty && digi_od_R_N->numberOfPlanesOn < pl_suff);
		bool pl_suff_odN = (digi_od_R_N->numberOfPlanesOn >= pl_suff);

		if (pl_empty_N) N_pl_empty_N++;
		if (pl_insuff_N) N_pl_insuff_N++;
		if (pl_suff_N) N_pl_suff_N++;
		
		if (pl_empty_F) N_pl_empty_F++;
		if (pl_insuff_F) N_pl_insuff_F++;
		if (pl_suff_F) N_pl_suff_F++;

		if (pl_suff_N && pl_suff_F) N_pl_suff++;

		if (pl_suff_N && pl_suff_F && (pl_insuff_odN || pl_empty_odN)) N_pl_suff_odN_insuff++;
		if (pl_suff_N && pl_suff_F && pl_suff_odN) N_pl_suff_odN_suff++;

		// pattern reco analysis
		bool p_empty_N = (pat_R_N->u.size() == 0 && pat_R_N->v.size() == 0);
		bool p_less_N = (pat_R_N->u.size() + pat_R_N->v.size() == 1);
		bool p_valid_N = (pat_R_N->u.size() == 1 && pat_R_N->v.size() == 1);
		bool p_more_N = (pat_R_N->u.size() > 1 || pat_R_N->v.size() > 1);
		
		bool p_empty_F = (pat_R_F->u.size() == 0 && pat_R_F->v.size() == 0);
		bool p_less_F = (pat_R_F->u.size() + pat_R_F->v.size() == 1);
		bool p_valid_F = (pat_R_F->u.size() == 1 && pat_R_F->v.size() == 1);
		bool p_more_F = (pat_R_F->u.size() > 1 || pat_R_F->v.size() > 1);

		// pattern reco counters
		if (p_empty_N) N_p_empty_N++;
		if (p_less_N) N_p_less_N++;
		if (p_valid_N) N_p_valid_N++;
		if (p_more_N) N_p_more_N++;
		
		if (p_empty_F) N_p_empty_F++;
		if (p_less_F) N_p_less_F++;
		if (p_valid_F) N_p_valid_F++;
		if (p_more_F) N_p_more_F++;
		
		if (p_empty_N && p_empty_F) N_pee++;
		if (p_valid_N && p_valid_F) N_pvv++;
		if (p_valid_N && !p_valid_F) N_pvi++;
		if (!p_valid_N && p_valid_F) N_piv++;
		if (!p_valid_N && !p_valid_F) N_pii++;
		if (p_more_N && p_more_F) N_pmm++;

		if (pl_suff_N && pl_suff_F && !p_valid_N && !p_valid_F) N_pl_suff_pat_ii++;
		
		if (pl_suff_N && pl_suff_F && (pl_insuff_odN || pl_empty_odN) && !p_valid_N && !p_valid_F) N_pl_suff_odN_insuff_pat_invalid++;
		if (pl_suff_N && pl_suff_F && pl_suff_odN && !p_valid_N && !p_valid_F) N_pl_suff_odN_suff_pat_invalid++;

		// left angle
		double th_y_L = -(rp_L_F->y / L_y_F + rp_L_N->y / L_y_N) / 2.;

		if (rp_R_N->valid && rp_R_F->valid) N_vv++;
		if (rp_R_N->valid && !rp_R_F->valid) N_vi++;
		if (!rp_R_N->valid && rp_R_F->valid) N_iv++;
		if (!rp_R_N->valid && !rp_R_F->valid) N_ii++;
		
		// identify pile-up
		bool pileup = (mrp_R_N->size() == 4 || mrp_R_N->size() == 2) && (mrp_R_F->size() == 4 || mrp_R_F->size() == 2);
		if (pl_suff_N && pl_suff_F && pileup) N_pl_suff_pileup++;
		if (pl_suff_N && pl_suff_F && pileup && (pl_insuff_odN || pl_empty_odN)) N_pl_suff_pileup_odN_insuff++;

		// evaluate multitrack correspondance
		double mtc_n_sigma = 3.;
		bool mtc_N = false;
		for (unsigned int i = 0; i < mrp_R_N->size(); i++) {
			if (fabs(mrp_R_N->at(i).y / L_y_N - th_y_L) < mtc_n_sigma * cut2_si) {
				mtc_N = true;
				break;
			}
		}
		
		bool mtc_F = false;
		for (unsigned int i = 0; i < mrp_R_F->size(); i++) {
			if (fabs(mrp_R_F->at(i).y / L_y_F - th_y_L) < mtc_n_sigma * cut2_si) {
				mtc_F = true;
				break;
			}
		}

		if (mtc_N) N_mtc_N++;
		if (mtc_F) N_mtc_F++;
		if (mtc_N && mtc_F) N_mtc++;

		bool mtc = (mtc_N && mtc_F);

		if (mtc && p_more_N && p_more_F) N_mtc_pat_more++;
		if (mtc && !p_valid_N && !p_valid_F) N_mtc_pat_invalid++;
		
		//if (pl_suff_N && pl_suff_F && !p_valid_N && !p_valid_F)
		//.	printf("%lu, %lu\n", metaData->run_no, metaData->event_no);

		// low y veto
		double y_veto = 10;
		bool veto_N = false;
		for (unsigned int i = 0; i < mrp_R_N->size(); i++) {
			if (fabs(mrp_R_N->at(i).y) < y_veto) {
				veto_N = true;
				break;
			}
		}
		
		bool veto_F = false;
		for (unsigned int i = 0; i < mrp_R_F->size(); i++) {
			if (fabs(mrp_R_F->at(i).y) < y_veto) {
				veto_F = true;
				break;
			}
		}

		if (veto_N) N_veto_N++;
		if (veto_F) N_veto_F++;
		if (veto_N && veto_F) N_veto++;

		bool veto = (veto_N && veto_F);

		// high-y?
		bool hy = (fabs(rp_L_N->y) > y_veto);	

		if (mtc && hy) N_mtc_hy++;	
		if (mtc && hy && veto) N_mtc_hy_veto++;	

		// final calculations

		if (mtc && hy && !veto && p_more_N && !p_valid_F) N_mtc_hy_nveto_Nmore_Finvalid++;	
	}

	printf("N = %u\n", N);
	printf("N_base = %u\n", N_base);
	printf("\n");
	printf("* N: pl-empty %.2f %%, pl-insuff %.2f %%, pl-suff %.2f %%\n", 100.*N_pl_empty_N/N_base, 100.*N_pl_insuff_N/N_base, 100.*N_pl_suff_N/N_base);
	printf("* F: pl-empty %.2f %%, pl-insuff %.2f %%, pl-suff %.2f %%\n", 100.*N_pl_empty_F/N_base, 100.*N_pl_insuff_F/N_base, 100.*N_pl_suff_F/N_base);
	printf("* pl-suff N&F %.2f %%\n", 100.*N_pl_suff/N_base);
	printf("* pl-suff N&F, odN pl-insuff: %u = %.2f %%\n", N_pl_suff_odN_insuff, 100.*N_pl_suff_odN_insuff/N_base);
	printf("* pl-suff N&F, odN pl-suff: %u = %.2f %%\n", N_pl_suff_odN_suff, 100.*N_pl_suff_odN_suff/N_base);
	printf("\n");
	printf("* N: pat-empty %.2f %%, pat-less %.2f %%, pat-valid %.2f %%, pat-more %.2f %%\n", 100.*N_p_empty_N/N_base, 100.*N_p_less_N/N_base, 100.*N_p_valid_N/N_base, 100.*N_p_more_N/N_base);
	printf("* F: pat-empty %.2f %%, pat-less %.2f %%, pat-valid %.2f %%, pat-more %.2f %%\n", 100.*N_p_empty_F/N_base, 100.*N_p_less_F/N_base, 100.*N_p_valid_F/N_base, 100.*N_p_more_F/N_base);
	printf("\n");
	printf("* N: pat-empty, F: pat-empty: %u = %.2f %%\n", N_pee, double(N_pee) / N_base * 100.);
	printf("* N: pat-valid, F: pat-valid: %u = %.2f %%\n", N_pvv, double(N_pvv) / N_base * 100.);
	printf("* N: pat-valid, F: !pat-valid: %u = %.2f %%\n", N_pvi, double(N_pvi) / N_base * 100.);
	printf("* N: !pat-valid, F: pat-valid: %u = %.2f %%\n", N_piv, double(N_piv) / N_base * 100.);
	printf("* N: !pat-valid, F: !pat-valid: %u = %.2f %%\n", N_pii, double(N_pii) / N_base * 100.);
	printf("* N: pat-more, F: pat-more: %u = %.2f\n", N_pmm, double(N_pmm) / N_base * 100.);
	printf("\n");
	printf("* pl-suff && !pat-valid: %u = %.2f\n", N_pl_suff_pat_ii, double(N_pl_suff_pat_ii) / N_base * 100.);
	printf("\n");
	printf("* pl-suff N&F, odN pl-insuff, pat-invalid N&F: %u = %.2f %%\n", N_pl_suff_odN_insuff_pat_invalid, 100.*N_pl_suff_odN_insuff_pat_invalid/N_base);
	printf("* pl-suff N&F, odN pl-suff, pat-invalid N&F: %u = %.2f %%\n", N_pl_suff_odN_suff_pat_invalid, 100.*N_pl_suff_odN_suff_pat_invalid/N_base);
	printf("\n");
	printf("* pl-suff && pile-up: %u = %.2f\n", N_pl_suff_pileup, 100. * N_pl_suff_pileup / N_base);
	printf("* pl-suff && pile-up && odN pl-insuff: %u = %.2f\n", N_pl_suff_pileup_odN_insuff, 100. * N_pl_suff_pileup_odN_insuff / N_base);

	/*
	printf("\n");
	printf("* N: valid, F: valid: %u = %.2f\n", N_vv, double(N_vv) / N_base * 100.);
	printf("* N: valid, F: !valid: %u = %.2f\n", N_vi, double(N_vi) / N_base * 100.);
	printf("* N: !valid, F: valid: %u = %.2f\n", N_iv, double(N_iv) / N_base * 100.);
	printf("* N: !valid, F: !valid: %u = %.2f\n", N_ii, double(N_ii) / N_base * 100.);
	*/
	printf("\n");
	printf("* multi track correspondence N: %u = %.2f %%\n", N_mtc_N, double(N_mtc_N) / N_base * 100.);
	printf("* multi track correspondence F: %u = %.2f %%\n", N_mtc_F, double(N_mtc_F) / N_base * 100.);
	printf("* multi track correspondence N&F: %u = %.2f %%\n", N_mtc, double(N_mtc) / N_base * 100.);
	printf("\n");
	printf("* mtc && N:pat-more && F:pat-more: %u = %.2f %%\n", N_mtc_pat_more, double(N_mtc_pat_more) / N_base * 100.);
	printf("* mtc && N:pat-invalid && F:pat-invalid: %u = %.2f %%\n", N_mtc_pat_invalid, double(N_mtc_pat_invalid) / N_base * 100.);
	printf("\n");
	printf("* veto N: %u = %.2f %%\n", N_veto_N, double(N_veto_N) / N_base * 100.);
	printf("* veto F: %u = %.2f %%\n", N_veto_F, double(N_veto_F) / N_base * 100.);
	printf("* veto N&F: %u = %.2f %%\n", N_veto, double(N_veto) / N_base * 100.);
	printf("\n");
	printf("* mtc && hy: %u = %.2f %%\n", N_mtc_hy, double(N_mtc_hy) / N_base * 100.);
	printf("* mtc && hy && veto: %u = %.2f %%\n", N_mtc_hy_veto, double(N_mtc_hy_veto) / N_base * 100.);
	printf("* mtc && hy && !veto && N:more && F:invalid: %u = %.2f %%\n", N_mtc_hy_nveto_Nmore_Finvalid, double(N_mtc_hy_nveto_Nmore_Finvalid) / N_base * 100.);
}

//----------------------------------------------------------------------------------------------------

int main()
{
	string prefix = "/afs/cern.ch/work/j/jkaspar/ntuples/UN_2011_10_20/";

	vector<string> files;
	// DS1
	//files.push_back("6926_ntuple.root");
	//files.push_back("6927_ntuple.root");
	files.push_back("6929_ntuple.root");

	// DS3
	/*
	files.push_back("6944_ntuple.root");
	files.push_back("6945_ntuple.root");
	*/
	files.push_back("6947_ntuple.root");

	for (unsigned int i = 0; i < files.size(); i++) {
		string file = prefix + files[i];
		shower_test2(file, true);
		shower_test2(file, false);
	}
}
