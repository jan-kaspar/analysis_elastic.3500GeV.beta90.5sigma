#include "TFile.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TChain.h"
#include "TH1D.h"

#include "/afs/cern.ch/exp/totem/scratch/jkaspar/software/offline/424/src/TotemAnalysis/TotemNtuplizer/interface/TriggerDataFormats.h"
#include "/afs/cern.ch/exp/totem/scratch/jkaspar/software/offline/424/src/TotemAnalysis/TotemNtuplizer/interface/RawDataFormats.h"
#include "/afs/cern.ch/exp/totem/scratch/jkaspar/software/offline/424/src/TotemAnalysis/TotemNtuplizer/interface/RPRootTrackInfo.h"

#include <cmath>

using namespace std;

double L_y_N = 237.7E3;			// mm
double L_y_F = 263.2E3;			// mm
double cut2_si = 3.6E-6;		// rad

void test2()
{
	bool diag45b = true;

	string prefix = "/mnt/lhcanalysis/2012/validation/";
	TChain *ch = new TChain("TotemNtuple");

	//ch->Add((prefix + "val6884/val6884_totem_ntuple.root").c_str());
	//ch->Add((prefix + "val6895/val6895_totem_ntuple.root").c_str());
	//ch->Add((prefix + "val6929/val6929_totem_ntuple.root").c_str());

	//ch->Add((prefix + "val6936/val6936_totem_ntuple.root").c_str());
	//ch->Add((prefix + "val6940/val6940_totem_ntuple.root").c_str());
	//ch->Add((prefix + "val6942/val6942_totem_ntuple.root").c_str());
	
	//ch->Add((prefix + "val6944/val6944_totem_ntuple.root").c_str());
	ch->Add((prefix + "val6947/val6947_totem_ntuple.root").c_str());

	// set intput branches
	TriggerData *triggerData = new TriggerData();
	ch->SetBranchAddress("trigger_data.", &triggerData);

	EventMetaData *metaData = new EventMetaData();
	ch->SetBranchAddress("event_info.", &metaData);

	RPRootDumpTrackInfo *rp_L_F = new RPRootDumpTrackInfo();
	RPRootDumpTrackInfo *rp_L_N = new RPRootDumpTrackInfo();
	RPRootDumpTrackInfo *rp_R_N = new RPRootDumpTrackInfo();
	RPRootDumpTrackInfo *rp_R_F = new RPRootDumpTrackInfo();
	
	vector<RPRootDumpTrackInfo> *mrp_L_F = new vector<RPRootDumpTrackInfo>();
	vector<RPRootDumpTrackInfo> *mrp_L_N = new vector<RPRootDumpTrackInfo>();
	vector<RPRootDumpTrackInfo> *mrp_R_N = new vector<RPRootDumpTrackInfo>();
	vector<RPRootDumpTrackInfo> *mrp_R_F = new vector<RPRootDumpTrackInfo>();

	RPRootDumpDigiInfo *digi_L_F = new RPRootDumpDigiInfo; 
	RPRootDumpDigiInfo *digi_L_N = new RPRootDumpDigiInfo; 
	RPRootDumpDigiInfo *digi_R_N = new RPRootDumpDigiInfo; 
	RPRootDumpDigiInfo *digi_R_F = new RPRootDumpDigiInfo; 

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
	} else {
		ch->SetBranchAddress("track_rp_24.", &rp_L_F);
		ch->SetBranchAddress("track_rp_20.", &rp_L_N);
		ch->SetBranchAddress("track_rp_121.", &rp_R_N);
		ch->SetBranchAddress("track_rp_125.", &rp_R_F);
		
		ch->SetBranchAddress("multi_track_rp_24.", &mrp_L_F);
		ch->SetBranchAddress("multi_track_rp_20.", &mrp_L_N);
		ch->SetBranchAddress("multi_track_rp_121.", &mrp_R_N);
		ch->SetBranchAddress("multi_track_rp_125.", &mrp_R_F);
		
		ch->SetBranchAddress("digi_rp_24.", &digi_L_F);
		ch->SetBranchAddress("digi_rp_20.", &digi_L_N);
		ch->SetBranchAddress("digi_rp_121.", &digi_R_N);
		ch->SetBranchAddress("digi_rp_125.", &digi_R_F);
	}

	unsigned int N_tr = 0, N_mtr_and = 0, N_mtr_or = 0, N_mtr_cand_and = 0, N_mtr_cand_or = 0;
	unsigned int N_minSig = 0;
	for (unsigned int i = 0; i < ch->GetEntries(); i++) {
		if (i >= 50000)
			break;

		ch->GetEvent(i);

		if (triggerData->input_status_bits & 3 == 0)
			continue;

		if (!rp_L_F->valid || !rp_L_N->valid)
			continue;

		double th_y_L = -(rp_L_F->y / L_y_F + rp_L_N->y / L_y_N) / 2.;

		if (rp_R_N->valid && rp_R_F->valid)
			N_tr++;

		if (digi_R_N->numberOfPlanesOn > 5 && digi_R_F->numberOfPlanesOn > 5)
			N_minSig++;

		if (mrp_R_N->size() > 0 && mrp_R_F->size() > 0)
			N_mtr_and++;
		
		if (mrp_R_N->size() > 0 || mrp_R_F->size() > 0)
			N_mtr_or++;

		bool mtr_cand_N = false;
		for (unsigned int i = 0; i < mrp_R_N->size(); i++) {
			double th_y_R = (*mrp_R_N)[i].y / L_y_N;
			if (fabs(th_y_L - th_y_R) < 3.*cut2_si) {
				mtr_cand_N = true;
			}
		}
		
		bool mtr_cand_F = false;
		for (unsigned int i = 0; i < mrp_R_N->size(); i++) {
			double th_y_R = (*mrp_R_F)[i].y / L_y_F;
			if (fabs(th_y_L - th_y_R) < 3.*cut2_si) {
				mtr_cand_F = true;
			}
		}

		if (mtr_cand_N && mtr_cand_F)
			N_mtr_cand_and++;
		
		if (mtr_cand_N || mtr_cand_F)
			N_mtr_cand_or++;

	}

	printf("N_tr = %u\n", N_tr);
	printf("N_minSig = %u\n", N_minSig);
	printf("N_mtr_and = %u\n", N_mtr_and);
	printf("N_mtr_or = %u\n", N_mtr_or);
	printf("N_mtr_cand_and = %u\n", N_mtr_cand_and);
	printf("N_mtr_cand_or = %u\n", N_mtr_cand_or);
	printf("\n");
	printf("N_tr / N_mtr_cand_or = %.3f\n", double(N_tr) / N_mtr_cand_or);
	printf("N_tr / N_minSig = %.3f\n", double(N_tr) / N_minSig);
}

int main()
{
	test2();
	return 0;
}
