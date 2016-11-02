#include "input_files.h"
#include "parameters.h"
#include "../common.h"

#include "TFile.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TChain.h"
#include "TH1D.h"

#include "TotemAnalysis/TotemNtuplizer/interface/TriggerDataFormats.h"
#include "TotemAnalysis/TotemNtuplizer/interface/RawDataFormats.h"
#include "TotemAnalysis/TotemNtuplizer/interface/RPRootTrackInfo.h"

using namespace std;

int main(int argc, char **argv)
{
	//printf("!! I'm protected !!\n");
	//return 1;

	if (argc != 2)
		return 1;

	Init(argv[1]);

	InitInputFiles();
	TChain *ch = new TChain("TotemNtuple");
	for (unsigned int i = 0; i < input_files.size(); i++) {
		ch->Add(input_files[i].c_str());
	}

	// set intput branches
	TriggerData *triggerData = new TriggerData();
	ch->SetBranchAddress("trigger_data.", &triggerData);

	EventMetaData *metaData = new EventMetaData();
	ch->SetBranchAddress("event_info.", &metaData);

	RPRootDumpTrackInfo *rp_L_F = new RPRootDumpTrackInfo();
	RPRootDumpTrackInfo *rp_L_N = new RPRootDumpTrackInfo();
	RPRootDumpTrackInfo *rp_R_N = new RPRootDumpTrackInfo();
	RPRootDumpTrackInfo *rp_R_F = new RPRootDumpTrackInfo();

	if (strcmp(argv[1], "45b_56t") == 0) {
		ch->SetBranchAddress("track_rp_25.", &rp_L_F);
		ch->SetBranchAddress("track_rp_21.", &rp_L_N);
		ch->SetBranchAddress("track_rp_120.", &rp_R_N);
		ch->SetBranchAddress("track_rp_124.", &rp_R_F);
	} else {
		ch->SetBranchAddress("track_rp_24.", &rp_L_F);
		ch->SetBranchAddress("track_rp_20.", &rp_L_N);
		ch->SetBranchAddress("track_rp_121.", &rp_R_N);
		ch->SetBranchAddress("track_rp_125.", &rp_R_F);
	}

	// ouput file
	TFile *outF = new TFile((string("distill_") + argv[1] + ".root").c_str(), "recreate");

	// set up output tree
	EventRed ev;
	TTree *outT = new TTree("distilled", "bla");

	outT->Branch("v_L_F", &ev.v_L_F); outT->Branch("x_L_F", &ev.x_L_F); outT->Branch("y_L_F", &ev.y_L_F);
	outT->Branch("v_L_N", &ev.v_L_N); outT->Branch("x_L_N", &ev.x_L_N); outT->Branch("y_L_N", &ev.y_L_N);
	outT->Branch("v_R_N", &ev.v_R_N); outT->Branch("x_R_N", &ev.x_R_N); outT->Branch("y_R_N", &ev.y_R_N);
	outT->Branch("v_R_F", &ev.v_R_F); outT->Branch("x_R_F", &ev.x_R_F); outT->Branch("y_R_F", &ev.y_R_F);

	outT->Branch("timestamp", &ev.timestamp);
	outT->Branch("run_num", &ev.run_num);
	outT->Branch("bunch_num", &ev.bunch_num);
	outT->Branch("event_num", &ev.event_num);
	outT->Branch("trigger_num", &ev.trigger_num);
	outT->Branch("trigger_bits", &ev.trigger_bits);

	// loop over the chain entries
	for (unsigned int i = 0; i < ch->GetEntries(); i++) {
		ch->GetEvent(i);

		ev.v_L_F = rp_L_F->valid; ev.x_L_F = rp_L_F->x; ev.y_L_F = rp_L_F->y;
		ev.v_L_N = rp_L_N->valid; ev.x_L_N = rp_L_N->x; ev.y_L_N = rp_L_N->y;
		ev.v_R_N = rp_R_N->valid; ev.x_R_N = rp_R_N->x; ev.y_R_N = rp_R_N->y;
		ev.v_R_F = rp_R_F->valid; ev.x_R_F = rp_R_F->x; ev.y_R_F = rp_R_F->y;

		bool save = (ev.v_L_F || ev.v_L_N) && (ev.v_R_N || ev.v_R_F);
		if (!save)
			continue;

		ev.timestamp = metaData->timestamp - timestamp0;
		ev.run_num = metaData->run_no;
		ev.bunch_num = triggerData->bunch_num;
		ev.event_num = triggerData->event_num;
		ev.trigger_num = triggerData->trigger_num;
		ev.trigger_bits = triggerData->input_status_bits;

		outT->Fill();
	}

	// save output tree
	outT->Write();

	/*
	// raw data rate
	TCanvas *c = new TCanvas();
	TH1D *h_rate = new TH1D("h_rate", ";timestamp;rate   (Hz)", 40001, 40000-0.5, 80000+0.5);
	ch->Draw("event_info.timestamp - 1319061600 >> h_rate");
	h_rate->Write();
	*/

	delete outF;
	return 0;
}
