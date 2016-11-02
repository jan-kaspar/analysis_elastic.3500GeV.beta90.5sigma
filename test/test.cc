#include "TChain.h"
#include "TCanvas.h"
#include "TH1D.h"

#include <string>

//#include "RPRootTrackInfo.h"
//. gSystem->AddLinkedLibs("-L/afs/cern.ch/exp/totem/scratch/jkaspar/software/offline/424/lib/slc5_amd64_gcc434/ -lTotemAnalysisTotemNtuplizer")

using namespace std;


//----------------------------------------------------------------------------------------------------

void test()
{
	TChain *ch = new TChain("TotemNtuple");
	
	string prefix = "/mnt/lhcanalysis/2012/validation/";

	// data set 1
	ch->Add((prefix + "val6884/val6884_totem_ntuple.root").c_str());
	/*
	ch->Add((prefix + "val6884/val6884_totem_ntuple.root").c_str());
	ch->Add((prefix + "val6885/val6885_totem_ntuple.root").c_str());
	ch->Add((prefix + "val6887/val6887_totem_ntuple.root").c_str());
	ch->Add((prefix + "val6895/val6895_totem_ntuple.root").c_str());
	ch->Add((prefix + "val6896/val6896_totem_ntuple.root").c_str());
	ch->Add((prefix + "val6897/val6897_totem_ntuple.root").c_str());
	ch->Add((prefix + "val6899/val6899_totem_ntuple.root").c_str());
	ch->Add((prefix + "val6900/val6900_totem_ntuple.root").c_str());
	ch->Add((prefix + "val6903/val6903_totem_ntuple.root").c_str());
	ch->Add((prefix + "val6904/val6904_totem_ntuple.root").c_str());
	ch->Add((prefix + "val6905/val6905_totem_ntuple.root").c_str());
	ch->Add((prefix + "val6908/val6908_totem_ntuple.root").c_str());
	ch->Add((prefix + "val6909/val6909_totem_ntuple.root").c_str());
	ch->Add((prefix + "val6910/val6910_totem_ntuple.root").c_str());
	ch->Add((prefix + "val6911/val6911_totem_ntuple.root").c_str());
	ch->Add((prefix + "val6912/val6912_totem_ntuple.root").c_str());
	ch->Add((prefix + "val6913/val6913_totem_ntuple.root").c_str());
	ch->Add((prefix + "val6914/val6914_totem_ntuple.root").c_str());
	ch->Add((prefix + "val6915/val6915_totem_ntuple.root").c_str());
	ch->Add((prefix + "val6916/val6916_totem_ntuple.root").c_str());
	ch->Add((prefix + "val6917/val6917_totem_ntuple.root").c_str());
	ch->Add((prefix + "val6918/val6918_totem_ntuple.root").c_str());
	ch->Add((prefix + "val6919/val6919_totem_ntuple.root").c_str());
	ch->Add((prefix + "val6920/val6920_totem_ntuple.root").c_str());
	ch->Add((prefix + "val6921/val6921_totem_ntuple.root").c_str());
	ch->Add((prefix + "val6922/val6922_totem_ntuple.root").c_str());
	ch->Add((prefix + "val6923/val6923_totem_ntuple.root").c_str());
	ch->Add((prefix + "val6924/val6924_totem_ntuple.root").c_str());
	ch->Add((prefix + "val6925/val6925_totem_ntuple.root").c_str());
	ch->Add((prefix + "val6926/val6926_totem_ntuple.root").c_str());
	ch->Add((prefix + "val6927/val6927_totem_ntuple.root").c_str());
	ch->Add((prefix + "val6929/val6929_totem_ntuple.root").c_str());
	*/

	// data set 2
	/*
	ch->Add((prefix + "val6936/val6936_totem_ntuple.root").c_str());
	ch->Add((prefix + "val6938/val6938_totem_ntuple.root").c_str());
	ch->Add((prefix + "val6941/val6941_totem_ntuple.root").c_str());
	ch->Add((prefix + "val6942/val6942_totem_ntuple.root").c_str());
	*/
	
	// data set 3
	/*
	ch->Add((prefix + "val6944/val6944_totem_ntuple.root").c_str());
	ch->Add((prefix + "val6945/val6945_totem_ntuple.root").c_str());
	ch->Add((prefix + "val6947/val6947_totem_ntuple.root").c_str());
	*/
	
	//ch->Draw("track_rp_120.y : track_rp_120.x >> h_120(100, -10., 10., 400, -40., 40.)", "", "colz");

	/*
	TCanvas *c = new TCanvas();
	c->ToggleEventStatus();
	c->Divide(4, 2);
	c->cd(1); gPad->SetLogz(1); ch->Draw("track_rp_25.y : track_rp_25.x >> h_25(300,0,0,300,0,0)", "digi_rp_25.numberOfPlanesOn > 5", "colz");
	c->cd(2); gPad->SetLogz(1); ch->Draw("track_rp_21.y : track_rp_21.x >> h_21(300,0,0,300,0,0)", "digi_rp_21.numberOfPlanesOn > 5", "colz");
	c->cd(3); gPad->SetLogz(1); ch->Draw("track_rp_120.y : track_rp_120.x >> h_120(300,0,0,300,0,0)", "digi_rp_120.numberOfPlanesOn > 5", "colz");
	c->cd(4); gPad->SetLogz(1); ch->Draw("track_rp_124.y : track_rp_124.x >> h_124(300,0,0,300,0,0)", "digi_rp_124.numberOfPlanesOn > 5", "colz");
	
	c->cd(5); gPad->SetLogz(1); ch->Draw("track_rp_24.y : track_rp_24.x >> h_24(300,0,0,300,0,0)", "digi_rp_24.numberOfPlanesOn > 5", "colz");
	c->cd(6); gPad->SetLogz(1); ch->Draw("track_rp_20.y : track_rp_20.x >> h_20(300,0,0,300,0,0)", "digi_rp_20.numberOfPlanesOn > 5", "colz");
	c->cd(7); gPad->SetLogz(1); ch->Draw("track_rp_121.y : track_rp_121.x >> h_121(300,0,0,300,0,0)", "digi_rp_121.numberOfPlanesOn > 5", "colz");
	c->cd(8); gPad->SetLogz(1); ch->Draw("track_rp_125.y : track_rp_125.x >> h_125(300,0,0,300,0,0)", "digi_rp_125.numberOfPlanesOn > 5", "colz");
	*/
	
	/*
	TCanvas *c = new TCanvas();
	c->Divide(4, 2);
	c->cd(1); gPad->SetLogz(1); ch->Draw("track_rp_25.y : track_rp_25.x >> h_25(300,0,0,300,0,0)", "track_rp_25.valid", "colz");
	c->cd(2); gPad->SetLogz(1); ch->Draw("track_rp_21.y : track_rp_21.x >> h_21(300,0,0,300,0,0)", "track_rp_21.valid", "colz");
	c->cd(3); gPad->SetLogz(1); ch->Draw("track_rp_120.y : track_rp_120.x >> h_120(300,0,0,300,0,0)", "track_rp_120.valid", "colz");
	c->cd(4); gPad->SetLogz(1); ch->Draw("track_rp_124.y : track_rp_124.x >> h_124(300,0,0,300,0,0)", "track_rp_124.valid", "colz");
	
	c->cd(5); gPad->SetLogz(1); ch->Draw("track_rp_24.y : track_rp_24.x >> h_24(300,0,0,300,0,0)", "track_rp_24.valid", "colz");
	c->cd(6); gPad->SetLogz(1); ch->Draw("track_rp_20.y : track_rp_20.x >> h_20(300,0,0,300,0,0)", "track_rp_20.valid", "colz");
	c->cd(7); gPad->SetLogz(1); ch->Draw("track_rp_121.y : track_rp_121.x >> h_121(300,0,0,300,0,0)", "track_rp_121.valid", "colz");
	c->cd(8); gPad->SetLogz(1); ch->Draw("track_rp_125.y : track_rp_125.x >> h_125(300,0,0,300,0,0)", "track_rp_125.valid", "colz");
	*/
	
	/*
	TCanvas *c = new TCanvas();
	c->Divide(4, 2);
	c->cd(1); gPad->SetLogz(1); ch->Draw("track_rp_25.y : track_rp_25.x >> h_25(300,0,0,300,0,0)", "abs(track_rp_25.y) > 3 && abs(track_rp_25.y) <8 && track_rp_25.valid", "colz");
	c->cd(2); gPad->SetLogz(1); ch->Draw("track_rp_21.y : track_rp_21.x >> h_21(300,0,0,300,0,0)", "abs(track_rp_21.y) > 3 && abs(track_rp_21.y) <8 && track_rp_21.valid", "colz");
	c->cd(3); gPad->SetLogz(1); ch->Draw("track_rp_120.y : track_rp_120.x >> h_120(300,0,0,300,0,0)", "abs(track_rp_120.y) > 3 && abs(track_rp_120.y) <8 && track_rp_120.valid", "colz");
	c->cd(4); gPad->SetLogz(1); ch->Draw("track_rp_124.y : track_rp_124.x >> h_124(300,0,0,300,0,0)", "abs(track_rp_124.y) > 3 && abs(track_rp_124.y) <8 && track_rp_124.valid", "colz");
	
	c->cd(5); gPad->SetLogz(1); ch->Draw("track_rp_24.y : track_rp_24.x >> h_24(300,0,0,300,0,0)", "abs(track_rp_24.y) > 3 && abs(track_rp_24.y) <8 && track_rp_24.valid", "colz");
	c->cd(6); gPad->SetLogz(1); ch->Draw("track_rp_20.y : track_rp_20.x >> h_20(300,0,0,300,0,0)", "abs(track_rp_20.y) > 3 && abs(track_rp_20.y) <8 && track_rp_20.valid", "colz");
	c->cd(7); gPad->SetLogz(1); ch->Draw("track_rp_121.y : track_rp_121.x >> h_121(300,0,0,300,0,0)", "abs(track_rp_121.y) > 3 && abs(track_rp_121.y) <8 && track_rp_121.valid", "colz");
	c->cd(8); gPad->SetLogz(1); ch->Draw("track_rp_125.y : track_rp_125.x >> h_125(300,0,0,300,0,0)", "abs(track_rp_125.y) > 3 && abs(track_rp_125.y) <8 && track_rp_125.valid", "colz");
	*/
	
	/*
	TCanvas *c = new TCanvas();
	c->Divide(4, 2);
	c->cd(1); gPad->SetLogz(1); ch->Draw("track_rp_25.y : track_rp_25.x >> h_25(300,0,0,300,0,0)", "abs(track_rp_25.y) > 3 && abs(track_rp_25.y) <8 &&  track_rp_25.valid && track_rp_21.valid && track_rp_120.valid && track_rp_124.valid", "colz");
	c->cd(2); gPad->SetLogz(1); ch->Draw("track_rp_21.y : track_rp_21.x >> h_21(300,0,0,300,0,0)", "abs(track_rp_21.y) > 3 && abs(track_rp_21.y) <8 && track_rp_25.valid && track_rp_21.valid && track_rp_120.valid && track_rp_124.valid", "colz");
	c->cd(3); gPad->SetLogz(1); ch->Draw("track_rp_120.y : track_rp_120.x >> h_120(300,0,0,300,0,0)", "abs(track_rp_120.y) > 3 && abs(track_rp_120.y) <8 && track_rp_25.valid && track_rp_21.valid && track_rp_120.valid && track_rp_124.valid", "colz");
	c->cd(4); gPad->SetLogz(1); ch->Draw("track_rp_124.y : track_rp_124.x >> h_124(300,0,0,300,0,0)", "abs(track_rp_124.y) > 3 && abs(track_rp_124.y) <8 && track_rp_25.valid && track_rp_21.valid && track_rp_120.valid && track_rp_124.valid", "colz");
	
	c->cd(5); gPad->SetLogz(1); ch->Draw("track_rp_24.y : track_rp_24.x >> h_24(300,0,0,300,0,0)", "abs(track_rp_24.y) > 3 && abs(track_rp_24.y) <8 && track_rp_24.valid && track_rp_20.valid && track_rp_121.valid && track_rp_125.valid", "colz");
	c->cd(6); gPad->SetLogz(1); ch->Draw("track_rp_20.y : track_rp_20.x >> h_20(300,0,0,300,0,0)", "abs(track_rp_20.y) > 3 && abs(track_rp_20.y) <8 && track_rp_24.valid && track_rp_20.valid && track_rp_121.valid && track_rp_125.valid", "colz");
	c->cd(7); gPad->SetLogz(1); ch->Draw("track_rp_121.y : track_rp_121.x >> h_121(300,0,0,300,0,0)", "abs(track_rp_121.y) > 3 && abs(track_rp_121.y) <8 && track_rp_24.valid && track_rp_20.valid && track_rp_121.valid && track_rp_125.valid", "colz");
	c->cd(8); gPad->SetLogz(1); ch->Draw("track_rp_125.y : track_rp_125.x >> h_125(300,0,0,300,0,0)", "abs(track_rp_125.y) > 3 && abs(track_rp_125.y) <8 && track_rp_24.valid && track_rp_20.valid && track_rp_121.valid && track_rp_125.valid", "colz");
	*/

	/*
	char meanVClusters[] = "(digi_rp_120.numberOfClusters[0] + digi_rp_120.numberOfClusters[2] + digi_rp_120.numberOfClusters[4] + digi_rp_120.numberOfClusters[6] + digi_rp_120.numberOfClusters[8])/5";
	char meanUClusters[] = "(digi_rp_120.numberOfClusters[1] + digi_rp_120.numberOfClusters[3] + digi_rp_120.numberOfClusters[5] + digi_rp_120.numberOfClusters[7] + digi_rp_120.numberOfClusters[9])/5";


	ch->Draw((string(meanVClusters) + ">> h_sig(51, -0.1, 10.1)").c_str(), "digi_rp_120.numberOfPlanesOn > 5");
	//ch->Draw("digi_rp_120.numberOfClusters[3]", "digi_rp_120.numberOfPlanesOn > 7", "sames");
	ch->Draw((string(meanVClusters) + ">> h_valid(51, -0.1, 10.1)").c_str(), "track_rp_120.valid", "sames");
	((TH1D *) gPad->GetPrimitive("h_valid"))->SetLineColor(2);
	
	new TCanvas();
	//ch->Draw("multi_track_rp_120.size()", "digi_rp_120.numberOfPlanesOn > 5");
	ch->Draw("digi_rp_121.numberOfClusters[2]", "digi_rp_121.numberOfPlanesOn > 7");
	ch->Draw("digi_rp_121.numberOfClusters[3]", "digi_rp_121.numberOfPlanesOn > 7", "sames");
	*/

	/*
	new TCanvas();
	ch->Draw("trigger_data.input_status_bits : event_info.timestamp - 1319061600");
	
	new TCanvas();
	ch->Draw("trigger_data.input_status_bits >> bits(601, -0.5, 600.5)");
	*/
	
	//ch->Draw("event_info.timestamp - 1319061600 >> rate_all(40001, 40000-0.5, 80000+0.5)", "", "");

	//new TCanvas();
	//ch->Draw("event_info.run_no : event_info.timestamp - 1319061600", "", "");

	new TCanvas();
	ch->Draw("event_info.timestamp - 1319061600", "trigger_data.bunch_num == 0", "");
	//gPad->GetPrimitive("Graph")->SaveAs("6944.0000.root");
	
	/*
	ch->Draw("event_info.timestamp - 1319061600 >> rate_all(40001, 40000-0.5, 80000+0.5)", "(track_rp_120.u_sect_no > 0 | track_rp_120.v_sect_no > 0)", "");
	ch->Draw("event_info.timestamp - 1319061600 >> rate_BX(40001, 40000-0.5, 80000+0.5)", "(track_rp_120.u_sect_no > 0 | track_rp_120.v_sect_no > 0) && (trigger_data.input_status_bits & 512) > 0", "sames");
	ch->Draw("event_info.timestamp - 1319061600 >> rate_RP(40001, 40000-0.5, 80000+0.5)", "(track_rp_120.u_sect_no > 0 | track_rp_120.v_sect_no > 0) && (trigger_data.input_status_bits & 3) > 0", "sames");
	*/
	
	/*
	new TCanvas();
	//ch->Draw("event_info.timestamp - 1319061600 >> rate_all(40001, 40000-0.5, 80000+0.5)", "", "");
	//((TH1D *) gPad->GetPrimitive("rate_all"))->SetLineColor(1);
	
	ch->Draw("event_info.timestamp - 1319061600 >> rate_BX(40001, 40000-0.5, 80000+0.5)", "(trigger_data.input_status_bits & 512) > 0", "");
	((TH1D *) gPad->GetPrimitive("rate_BX"))->SetLineColor(1);

	ch->Draw("event_info.timestamp - 1319061600 >> rate_RP(40001, 40000-0.5, 80000+0.5)", "(trigger_data.input_status_bits == 1 || trigger_data.input_status_bits == 2 || trigger_data.input_status_bits == 3)", "sames");
	((TH1D *) gPad->GetPrimitive("rate_RP"))->SetLineColor(2);
	
	ch->Draw("event_info.timestamp - 1319061600 >> rate_RP_2(40001, 40000-0.5, 80000+0.5)", "(trigger_data.input_status_bits == 1 || trigger_data.input_status_bits == 2 || trigger_data.input_status_bits == 3) && digi_rp_124.numberOfPlanesOn > 5", "sames");
	((TH1D *) gPad->GetPrimitive("rate_RP_2"))->SetLineColor(4);


	//ch->Draw("event_info.timestamp - 1319061600 >> rate_RP_BX(40001, 40000-0.5, 80000+0.5)", "trigger_data.input_status_bits == 513 || trigger_data.input_status_bits == 514 || trigger_data.input_status_bits == 515", "sames");
	//((TH1D *) gPad->GetPrimitive("rate_RP_BX"))->SetLineColor(8);
	
	ch->Draw("event_info.timestamp - 1319061600 >> rate_BX_2(40001, 40000-0.5, 80000+0.5)", "trigger_data.input_status_bits == 512 && digi_rp_124.numberOfPlanesOn > 5", "sames");
	((TH1D *) gPad->GetPrimitive("rate_BX_2"))->SetLineColor(8);
	*/

	/*
	new TCanvas();
	ch->Draw("event_info.timestamp - 1319061600 >> rate_BX(40001, 40000-0.5, 80000+0.5)", "trigger_data.bunch_num == 0 && (trigger_data.input_status_bits & 512) > 0", "");
	((TH1D *) gPad->GetPrimitive("rate_BX"))->SetLineColor(1);
	
	ch->Draw("event_info.timestamp - 1319061600 >> rate_45b(40001, 40000-0.5, 80000+0.5)", "trigger_data.bunch_num == 0 && (trigger_data.input_status_bits >= 512) && (track_rp_25.u_sect_no > 0 || track_rp_25.v_sect_no > 0 || track_rp_21.u_sect_no > 0 || track_rp_21.v_sect_no > 0)", "sames");
	((TH1D *) gPad->GetPrimitive("rate_45b"))->SetLineColor(2);
	
	ch->Draw("event_info.timestamp - 1319061600 >> rate_56t(40001, 40000-0.5, 80000+0.5)", "trigger_data.bunch_num == 0 && (trigger_data.input_status_bits >= 512) && (track_rp_120.u_sect_no > 0 || track_rp_120.v_sect_no > 0 || track_rp_124.u_sect_no > 0 || track_rp_124.v_sect_no > 0)", "sames");
	((TH1D *) gPad->GetPrimitive("rate_56t"))->SetLineColor(4);
	
	ch->Draw("event_info.timestamp - 1319061600 >> rate_45b_and_56t(40001, 40000-0.5, 80000+0.5)", "trigger_data.bunch_num == 0 && (trigger_data.input_status_bits >= 512) && (track_rp_25.u_sect_no > 0 || track_rp_25.v_sect_no > 0 || track_rp_21.u_sect_no > 0 || track_rp_21.v_sect_no > 0) && (track_rp_120.u_sect_no > 0 || track_rp_120.v_sect_no > 0 || track_rp_124.u_sect_no > 0 || track_rp_124.v_sect_no > 0)", "sames");
	((TH1D *) gPad->GetPrimitive("rate_45b_and_56t"))->SetLineColor(6);
	
	ch->Draw("event_info.timestamp - 1319061600 >> rate_BXRP(40001, 40000-0.5, 80000+0.5)", "trigger_data.bunch_num == 0 && (trigger_data.input_status_bits == 513)", "sames");
	((TH1D *) gPad->GetPrimitive("rate_BXRP"))->SetLineColor(8);
	
	
	new TCanvas();
	ch->Draw("event_info.timestamp - 1319061600 >> rate_BX2(40001, 40000-0.5, 80000+0.5)", "trigger_data.bunch_num == 0 && (trigger_data.input_status_bits & 512) > 0", "");
	((TH1D *) gPad->GetPrimitive("rate_BX2"))->SetLineColor(1);
	
	ch->Draw("event_info.timestamp - 1319061600 >> rate_45t(40001, 40000-0.5, 80000+0.5)", "(trigger_data.input_status_bits >= 512) && (track_rp_24.u_sect_no > 0 || track_rp_24.v_sect_no > 0 || track_rp_20.u_sect_no > 0 || track_rp_20.v_sect_no > 0)", "sames");
	((TH1D *) gPad->GetPrimitive("rate_45t"))->SetLineColor(2);
	
	ch->Draw("event_info.timestamp - 1319061600 >> rate_56b(40001, 40000-0.5, 80000+0.5)", "(trigger_data.input_status_bits >= 512) && (track_rp_121.u_sect_no > 0 || track_rp_121.v_sect_no > 0 || track_rp_125.u_sect_no > 0 || track_rp_125.v_sect_no > 0)", "sames");
	((TH1D *) gPad->GetPrimitive("rate_56b"))->SetLineColor(4);
	
	ch->Draw("event_info.timestamp - 1319061600 >> rate_45t_and_56b(40001, 40000-0.5, 80000+0.5)", "(trigger_data.input_status_bits >= 512) && (track_rp_24.u_sect_no > 0 || track_rp_24.v_sect_no > 0 || track_rp_20.u_sect_no > 0 || track_rp_20.v_sect_no > 0) && (track_rp_121.u_sect_no > 0 || track_rp_121.v_sect_no > 0 || track_rp_125.u_sect_no > 0 || track_rp_125.v_sect_no > 0)", "sames");
	((TH1D *) gPad->GetPrimitive("rate_45t_and_56b"))->SetLineColor(6);
	*/
	

	/*
	new TCanvas();
	ch->Draw("event_info.timestamp - 1319061600 >> rate_BX(40001, 40000-0.5, 80000+0.5)", "(trigger_data.input_status_bits == 512 || trigger_data.input_status_bits == 513 || trigger_data.input_status_bits == 514 || trigger_data.input_status_bits == 515)", "");
	((TH1D *) gPad->GetPrimitive("rate_BX"))->SetLineColor(1);
	
	ch->Draw("event_info.timestamp - 1319061600 >> rate_25(40001, 40000-0.5, 80000+0.5)", "(trigger_data.input_status_bits == 512 || trigger_data.input_status_bits == 513) && digi_rp_25.numberOfPlanesOn > 5", "sames");
	((TH1D *) gPad->GetPrimitive("rate_25"))->SetLineColor(2);
	
	ch->Draw("event_info.timestamp - 1319061600 >> rate_21(40001, 40000-0.5, 80000+0.5)", "(trigger_data.input_status_bits == 512 || trigger_data.input_status_bits == 513) && digi_rp_21.numberOfPlanesOn > 5", "sames");
	((TH1D *) gPad->GetPrimitive("rate_21"))->SetLineColor(4);
	
	ch->Draw("event_info.timestamp - 1319061600 >> rate_120(40001, 40000-0.5, 80000+0.5)", "(trigger_data.input_status_bits == 512 || trigger_data.input_status_bits == 513) && digi_rp_120.numberOfPlanesOn > 5", "sames");
	((TH1D *) gPad->GetPrimitive("rate_120"))->SetLineColor(6);
	
	ch->Draw("event_info.timestamp - 1319061600 >> rate_124(40001, 40000-0.5, 80000+0.5)", "(trigger_data.input_status_bits == 512 || trigger_data.input_status_bits == 513) && digi_rp_124.numberOfPlanesOn > 5", "sames");
	((TH1D *) gPad->GetPrimitive("rate_124"))->SetLineColor(8);
	*/



	/*
		new TCanvas();
	ch->Draw("event_info.timestamp - 1319061600 >> rate_BX2(40001, 40000-0.5, 80000+0.5)", "trigger_data.input_status_bits == 512", "");
	((TH1D *) gPad->GetPrimitive("rate_BX2"))->SetLineColor(1);
	
	ch->Draw("event_info.timestamp - 1319061600 >> rate_24(40001, 40000-0.5, 80000+0.5)", "trigger_data.input_status_bits == 512 && digi_rp_24.numberOfPlanesOn > 5", "sames");
	((TH1D *) gPad->GetPrimitive("rate_24"))->SetLineColor(2);
	
	ch->Draw("event_info.timestamp - 1319061600 >> rate_20(40001, 40000-0.5, 80000+0.5)", "trigger_data.input_status_bits == 512 && digi_rp_20.numberOfPlanesOn > 5", "sames");
	((TH1D *) gPad->GetPrimitive("rate_20"))->SetLineColor(4);
	
	ch->Draw("event_info.timestamp - 1319061600 >> rate_121(40001, 40000-0.5, 80000+0.5)", "trigger_data.input_status_bits == 512 && digi_rp_121.numberOfPlanesOn > 5", "sames");
	((TH1D *) gPad->GetPrimitive("rate_121"))->SetLineColor(6);
	
	ch->Draw("event_info.timestamp - 1319061600 >> rate_125(40001, 40000-0.5, 80000+0.5)", "trigger_data.input_status_bits == 512 && digi_rp_125.numberOfPlanesOn > 5", "sames");
	((TH1D *) gPad->GetPrimitive("rate_125"))->SetLineColor(8);
	*/
	
	/*
	TCanvas *c = new TCanvas();
	c->Divide(4, 2);

	c->cd(1);
	ch->Draw("event_info.timestamp - 1319061600 >> rate_25(4001, 40000-0.5, 80000+0.5)", "track_rp_25.u_sect_no > 0 && track_rp_25.v_sect_no > 0", "");
	((TH1D *) gPad->GetPrimitive("rate_25"))->SetLineColor(1);
	c->cd(2);
	ch->Draw("event_info.timestamp - 1319061600 >> rate_21(4001, 40000-0.5, 80000+0.5)", "track_rp_21.u_sect_no > 0 && track_rp_21.v_sect_no > 0", "");
	((TH1D *) gPad->GetPrimitive("rate_21"))->SetLineColor(2);
	c->cd(3);
	ch->Draw("event_info.timestamp - 1319061600 >> rate_120(4001, 40000-0.5, 80000+0.5)", "track_rp_120.u_sect_no > 0 && track_rp_120.v_sect_no > 0", "");
	((TH1D *) gPad->GetPrimitive("rate_120"))->SetLineColor(4);
	c->cd(4);
	ch->Draw("event_info.timestamp - 1319061600 >> rate_124(4001, 40000-0.5, 80000+0.5)", "track_rp_124.u_sect_no > 0 && track_rp_124.v_sect_no > 0", "");
	((TH1D *) gPad->GetPrimitive("rate_124"))->SetLineColor(8);
	
	c->cd(5);
	ch->Draw("event_info.timestamp - 1319061600 >> rate_24(4001, 40000-0.5, 80000+0.5)", "track_rp_24.u_sect_no > 0 && track_rp_24.v_sect_no > 0", "");
	((TH1D *) gPad->GetPrimitive("rate_24"))->SetLineColor(1);
	c->cd(6);
	ch->Draw("event_info.timestamp - 1319061600 >> rate_20(4001, 40000-0.5, 80000+0.5)", "track_rp_20.u_sect_no > 0 && track_rp_20.v_sect_no > 0", "");
	((TH1D *) gPad->GetPrimitive("rate_20"))->SetLineColor(2);
	c->cd(7);
	ch->Draw("event_info.timestamp - 1319061600 >> rate_121(4001, 40000-0.5, 80000+0.5)", "track_rp_121.u_sect_no > 0 && track_rp_121.v_sect_no > 0", "");
	((TH1D *) gPad->GetPrimitive("rate_121"))->SetLineColor(4);
	c->cd(8);
	ch->Draw("event_info.timestamp - 1319061600 >> rate_125(4001, 40000-0.5, 80000+0.5)", "track_rp_125.u_sect_no > 0 && track_rp_125.v_sect_no > 0", "");
	((TH1D *) gPad->GetPrimitive("rate_125"))->SetLineColor(8);
	*/

	/*
	TCanvas *c = new TCanvas();
	c->ToggleEventStatus();
	ch->Draw("track_rp_120.y >> h1234", "track_rp_25.valid && track_rp_21.valid && track_rp_120.valid && track_rp_124.valid");
	TH1D *h1234 = (TH1D *) gPad->GetPrimitive("h1234"); h1234->SetLineColor(1);
	ch->Draw("track_rp_120.y >> h123", "track_rp_25.valid && track_rp_21.valid && track_rp_120.valid", "sames");
	TH1D *h123 = (TH1D *) gPad->GetPrimitive("h123"); h123->SetLineColor(2);

	c = new TCanvas();
	TH1D *h_ratio = new TH1D(*h1234);
	h_ratio->Divide(h123);
	h_ratio->Draw();
	
	c = new TCanvas();
	c->ToggleEventStatus();
	ch->Draw("track_rp_124.y >> h1234", "track_rp_25.valid && track_rp_21.valid && track_rp_120.valid && track_rp_124.valid");
	h1234 = (TH1D *) gPad->GetPrimitive("h1234"); h1234->SetLineColor(1);
	ch->Draw("track_rp_124.y >> h124", "track_rp_25.valid && track_rp_21.valid && track_rp_124.valid", "sames");
	TH1D *h124 = (TH1D *) gPad->GetPrimitive("h124"); h124->SetLineColor(2);

	c = new TCanvas();
	h_ratio = new TH1D(*h1234);
	h_ratio->Divide(h124);
	h_ratio->Draw();
	*/
	
	/*
	TCanvas *c;
	TH1D *h_num, *h_den;
   
	c = new TCanvas(); c->ToggleEventStatus();
	ch->Draw("track_rp_124.y >> 45b_tr_56t_tr(100, 0, 40)", "track_rp_25.valid && track_rp_21.valid && track_rp_120.valid && track_rp_124.valid");
	((TH1D *) gPad->GetPrimitive("45b_tr_56t_tr"))->SetLineColor(1);

	//ch->Draw("track_rp_124.y >> 45b_tr_56t_empty(100, 0, 40)", "track_rp_25.valid && track_rp_21.valid && digi_rp_120.numberOfPlanesOn == 0 && digi_rp_124.numberOfPlanesOn == 0", "sames");
	//((TH1D *) gPad->GetPrimitive("45b_tr_56t_empty"))->SetLineColor(2);
	
	//ch->Draw("track_rp_124.y >> 45b_tr_56t_SigNoTrig(100, 0, 40)", "track_rp_25.valid && track_rp_21.valid && digi_rp_120.numberOfPlanesOn > 0 && digi_rp_124.numberOfPlanesOn > 0 && track_rp_120.u_sect_no == 0 && track_rp_120.v_sect_no == 0 && track_rp_124.u_sect_no == 0 && track_rp_124.v_sect_no == 0", "sames");
	//((TH1D *) gPad->GetPrimitive("45b_tr_56t_SigNoTrig"))->SetLineColor(4);

	ch->Draw("track_rp_124.y >> 45b_tr_56t_MinTrackSig(100, 0, 40)", "track_rp_25.valid && track_rp_21.valid && digi_rp_124.numberOfPlanesOn > 5 && digi_rp_124.numberOfPlanesOn > 5", "sames");
	((TH1D *) gPad->GetPrimitive("45b_tr_56t_MinTrackSig"))->SetLineColor(8);
	
	ch->Draw("track_rp_124.y >> 45b_tr_56t_MultiTrack(100, 0, 40)", "track_rp_25.valid && track_rp_21.valid && @multi_track_rp_120.size() > 0 && @multi_track_rp_124.size() > 0", "sames");
	((TH1D *) gPad->GetPrimitive("45b_tr_56t_MultiTrack"))->SetLineColor(2);
	
	ch->Draw("track_rp_124.y >> 45b_tr_56t_MultiTrack2(100, 0, 40)", "track_rp_25.valid && track_rp_21.valid && (@multi_track_rp_120.size() > 0 || @multi_track_rp_124.size() > 0)", "sames");
	((TH1D *) gPad->GetPrimitive("45b_tr_56t_MultiTrack2"))->SetLineColor(4);
	*/

	//h_num = (TH1D *) gPad->GetPrimitive("45b_tr_56t_tr");
	//h_den = (TH1D *) gPad->GetPrimitive("45b_tr_56t_AtLeastOneTrack");
	//printf("45b_tr efficiency = %.3f\n", h_num->GetEntries() / h_den->GetEntries());


	/*
	c = new TCanvas(); c->ToggleEventStatus();
	ch->Draw("-track_rp_25.y >> 56t_tr_45b_tr(100, 0, 40)", "track_rp_124.valid && track_rp_120.valid && track_rp_21.valid && track_rp_25.valid");
	((TH1D *) gPad->GetPrimitive("56t_tr_45b_tr"))->SetLineColor(1);

	ch->Draw("-track_rp_25.y >> 56t_tr_45b_empty(100, 0, 40)", "track_rp_124.valid && track_rp_120.valid && digi_rp_21.numberOfPlanesOn == 0 && digi_rp_25.numberOfPlanesOn == 0", "sames");
	((TH1D *) gPad->GetPrimitive("56t_tr_45b_empty"))->SetLineColor(2);
	
	ch->Draw("-track_rp_25.y >> 56t_tr_45b_SigNoTrig(100, 0, 40)", "track_rp_124.valid && track_rp_120.valid && digi_rp_21.numberOfPlanesOn > 0 && digi_rp_25.numberOfPlanesOn > 0 && track_rp_21.u_sect_no == 0 && track_rp_21.v_sect_no == 0 && track_rp_25.u_sect_no == 0 && track_rp_25.v_sect_no == 0", "sames");
	((TH1D *) gPad->GetPrimitive("56t_tr_45b_SigNoTrig"))->SetLineColor(4);

	ch->Draw("-track_rp_25.y >> 56t_tr_45b_AtLeastOneTrack(100, 0, 40)", "track_rp_124.valid && track_rp_120.valid && digi_rp_25.numberOfPlanesOn > 5 && digi_rp_25.numberOfPlanesOn > 5", "sames");
	((TH1D *) gPad->GetPrimitive("56t_tr_45b_AtLeastOneTrack"))->SetLineColor(8);
	
	h_num = (TH1D *) gPad->GetPrimitive("56t_tr_45b_tr");
	h_den = (TH1D *) gPad->GetPrimitive("56t_tr_45b_AtLeastOneTrack");
	printf("56t_tr efficiency = %.3f\n", h_num->GetEntries() / h_den->GetEntries());
	
	
	
	
	c = new TCanvas(); c->ToggleEventStatus();
	ch->Draw("-track_rp_125.y >> 45t_tr_56b_tr(100, 0, 40)", "track_rp_24.valid && track_rp_20.valid && track_rp_121.valid && track_rp_125.valid");
	((TH1D *) gPad->GetPrimitive("45t_tr_56b_tr"))->SetLineColor(1);

	ch->Draw("-track_rp_125.y >> 45t_tr_56b_empty(100, 0, 40)", "track_rp_24.valid && track_rp_20.valid && digi_rp_121.numberOfPlanesOn == 0 && digi_rp_125.numberOfPlanesOn == 0", "sames");
	((TH1D *) gPad->GetPrimitive("45t_tr_56b_empty"))->SetLineColor(2);
	
	ch->Draw("-track_rp_125.y >> 45t_tr_56b_SigNoTrig(100, 0, 40)", "track_rp_24.valid && track_rp_20.valid && digi_rp_121.numberOfPlanesOn > 0 && digi_rp_125.numberOfPlanesOn > 0 && track_rp_121.u_sect_no == 0 && track_rp_121.v_sect_no == 0 && track_rp_125.u_sect_no == 0 && track_rp_125.v_sect_no == 0", "sames");
	((TH1D *) gPad->GetPrimitive("45t_tr_56b_SigNoTrig"))->SetLineColor(4);

	ch->Draw("-track_rp_125.y >> 45t_tr_56b_AtLeastOneTrack(100, 0, 40)", "track_rp_24.valid && track_rp_20.valid && digi_rp_125.numberOfPlanesOn > 5 && digi_rp_125.numberOfPlanesOn > 5", "sames");
	((TH1D *) gPad->GetPrimitive("45t_tr_56b_AtLeastOneTrack"))->SetLineColor(8);

	h_num = (TH1D *) gPad->GetPrimitive("45t_tr_56b_tr");
	h_den = (TH1D *) gPad->GetPrimitive("45t_tr_56b_AtLeastOneTrack");
	printf("45t_tr efficiency = %.3f\n", h_num->GetEntries() / h_den->GetEntries());


	c = new TCanvas(); c->ToggleEventStatus();
	ch->Draw("-track_rp_24.y >> 56b_tr_45t_tr(100, 0, 40)", "track_rp_125.valid && track_rp_121.valid && track_rp_20.valid && track_rp_24.valid");
	((TH1D *) gPad->GetPrimitive("56b_tr_45t_tr"))->SetLineColor(1);

	ch->Draw("track_rp_24.y >> 56b_tr_45t_empty(100, 0, 40)", "track_rp_125.valid && track_rp_121.valid && digi_rp_20.numberOfPlanesOn == 0 && digi_rp_24.numberOfPlanesOn == 0", "sames");
	((TH1D *) gPad->GetPrimitive("56b_tr_45t_empty"))->SetLineColor(2);
	
	ch->Draw("track_rp_24.y >> 56b_tr_45t_SigNoTrig(100, 0, 40)", "track_rp_125.valid && track_rp_121.valid && digi_rp_20.numberOfPlanesOn > 0 && digi_rp_24.numberOfPlanesOn > 0 && track_rp_20.u_sect_no == 0 && track_rp_20.v_sect_no == 0 && track_rp_24.u_sect_no == 0 && track_rp_24.v_sect_no == 0", "sames");
	((TH1D *) gPad->GetPrimitive("56b_tr_45t_SigNoTrig"))->SetLineColor(4);

	ch->Draw("track_rp_24.y >> 56b_tr_45t_AtLeastOneTrack(100, 0, 40)", "track_rp_125.valid && track_rp_121.valid && digi_rp_24.numberOfPlanesOn > 5 && digi_rp_24.numberOfPlanesOn > 5", "sames");
	((TH1D *) gPad->GetPrimitive("56b_tr_45t_AtLeastOneTrack"))->SetLineColor(8);
	
	h_num = (TH1D *) gPad->GetPrimitive("56b_tr_45t_tr");
	h_den = (TH1D *) gPad->GetPrimitive("56b_tr_45t_AtLeastOneTrack");
	printf("56b_tr efficiency = %.3f\n", h_num->GetEntries() / h_den->GetEntries());
	*/
	
	
	/*
	new TCanvas();
	ch->Draw("event_info.timestamp - 1319061600 >> rate_1(4001, 40000-0.5, 80000+0.5)", "track_rp_25.valid && track_rp_21.valid && !track_rp_120.valid && !track_rp_124.valid", "");
	ch->Draw("event_info.timestamp - 1319061600 >> rate_2(4001, 40000-0.5, 80000+0.5)", "track_rp_25.valid && track_rp_21.valid && digi_rp_120.numberOfPlanesOn > 0 && digi_rp_124.numberOfPlanesOn > 0 && track_rp_120.u_sect_no == 0 && track_rp_120.v_sect_no == 0 && track_rp_124.u_sect_no == 0 && track_rp_124.v_sect_no == 0", "sames");
	((TH1D *) gPad->GetPrimitive("rate_2"))->SetLineColor(2);
	
	ch->Draw("event_info.timestamp - 1319061600 >> rate_3(4001, 40000-0.5, 80000+0.5)", "track_rp_25.valid && track_rp_21.valid && track_rp_120.u_sect_no == 0 && track_rp_120.v_sect_no == 0 && track_rp_124.u_sect_no == 0 && track_rp_124.v_sect_no == 0", "sames");
	((TH1D *) gPad->GetPrimitive("rate_3"))->SetLineColor(4);
	
	ch->Draw("event_info.timestamp - 1319061600 >> rate_4(4001, 40000-0.5, 80000+0.5)", "track_rp_25.valid && track_rp_21.valid && digi_rp_120.numberOfPlanesOn == 0 && digi_rp_124.numberOfPlanesOn == 0 && track_rp_120.u_sect_no == 0 && track_rp_120.v_sect_no == 0 && track_rp_124.u_sect_no == 0 && track_rp_124.v_sect_no == 0", "sames");
	((TH1D *) gPad->GetPrimitive("rate_4"))->SetLineColor(8);
	
	
	
	new TCanvas();
	ch->Draw("event_info.timestamp - 1319061600 >> rate_N1(4001, 40000-0.5, 80000+0.5)", "track_rp_24.valid && track_rp_20.valid && !track_rp_121.valid && !track_rp_125.valid", "");
	ch->Draw("event_info.timestamp - 1319061600 >> rate_N2(4001, 40000-0.5, 80000+0.5)", "track_rp_24.valid && track_rp_20.valid && digi_rp_121.numberOfPlanesOn > 0 && digi_rp_125.numberOfPlanesOn > 0 && track_rp_121.u_sect_no == 0 && track_rp_121.v_sect_no == 0 && track_rp_125.u_sect_no == 0 && track_rp_125.v_sect_no == 0", "sames");
	((TH1D *) gPad->GetPrimitive("rate_N2"))->SetLineColor(2);
	
	ch->Draw("event_info.timestamp - 1319061600 >> rate_N3(4001, 40000-0.5, 80000+0.5)", "track_rp_24.valid && track_rp_20.valid && track_rp_121.u_sect_no == 0 && track_rp_121.v_sect_no == 0 && track_rp_125.u_sect_no == 0 && track_rp_125.v_sect_no == 0", "sames");
	((TH1D *) gPad->GetPrimitive("rate_N3"))->SetLineColor(4);
	
	ch->Draw("event_info.timestamp - 1319061600 >> rate_N4(4001, 40000-0.5, 80000+0.5)", "track_rp_24.valid && track_rp_20.valid && digi_rp_121.numberOfPlanesOn == 0 && digi_rp_125.numberOfPlanesOn == 0 && track_rp_121.u_sect_no == 0 && track_rp_121.v_sect_no == 0 && track_rp_125.u_sect_no == 0 && track_rp_125.v_sect_no == 0", "sames");
	((TH1D *) gPad->GetPrimitive("rate_N4"))->SetLineColor(8);
	*/
}

int main()
{
	test();
	return 0;
}
