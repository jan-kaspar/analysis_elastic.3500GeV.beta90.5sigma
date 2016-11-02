#include "TChain.h"
#include "TCanvas.h"
#include "TH1D.h"

#include <string>

//#include "RPRootTrackInfo.h"

using namespace std;

//----------------------------------------------------------------------------------------------------

void TwoOutOfFour(TChain *ch, const string &fp1, const string &fp2, const string &tp1, const string &tp2, const string &cut0)
{
	string base_cut;
	if (cut0.empty())
		base_cut = "track_rp_"+fp1+".valid && track_rp_"+fp2+".valid";
	else
		base_cut = cut0 + " && track_rp_"+fp1+".valid && track_rp_"+fp2+".valid";

	base_cut += "&& trigger_data.bunch_num == 0";
	base_cut += "&& ((track_rp_"+tp1+".u_sect_no > 0 || track_rp_"+tp1+".v_sect_no > 0) || (track_rp_"+tp2+".u_sect_no > 0 || track_rp_"+tp2+".v_sect_no > 0))";


	TH1D *h_test = new TH1D("h_test", "", 100, 0, 0);
	string cut;
	
	printf("\n* fixed pots: %s, %s | tested pots: %s, %s | base cut: %s\n", fp1.c_str(), fp2.c_str(), tp1.c_str(), tp2.c_str(), base_cut.c_str());
	
	cut = "";
	ch->Draw("event_info.timestamp >> h_test", "trigger_data.bunch_num == 0", "goff");
	printf("\tfull statistics (bunch 0): %.5E\n", h_test->GetEntries());

	cut = base_cut;
	ch->Draw("event_info.timestamp >> h_test", cut.c_str(), "goff");
	printf("\tall: %.2E\n", h_test->GetEntries());
	
	cut = base_cut + " && digi_rp_"+tp1+".numberOfPlanesOn == 0 && digi_rp_"+tp2+".numberOfPlanesOn == 0";
	ch->Draw("event_info.timestamp >> h_test", cut.c_str(), "goff");
	printf("\tempty && empty: %.2E\n", h_test->GetEntries());
	
	cut = base_cut + " && (digi_rp_"+tp1+".numberOfPlanesOn == 0 || digi_rp_"+tp2+".numberOfPlanesOn == 0)";
	ch->Draw("event_info.timestamp >> h_test", cut.c_str(), "goff");
	printf("\tempty || empty: %.2E\n", h_test->GetEntries());
	
	cut = base_cut + " && digi_rp_"+tp1+".numberOfPlanesOn > 5 && digi_rp_"+tp2+".numberOfPlanesOn > 5";
	ch->Draw("event_info.timestamp >> h_test", cut.c_str(), "goff");
	printf("\tmore than 5 pl && more than 5 pl: %.2E\n", h_test->GetEntries());
	
	cut = base_cut + " && track_rp_"+tp1+".valid && track_rp_"+tp2+".valid";
	ch->Draw("event_info.timestamp >> h_test", cut.c_str(), "goff");
	printf("\ttrack && track: %.2E\n", h_test->GetEntries());
	
	cut = base_cut + " && @multi_track_rp_"+tp1+".size() > 0 && @multi_track_rp_"+tp2+".size() > 0";
	ch->Draw("event_info.timestamp >> h_test", cut.c_str(), "goff");
	printf("\tmultitrack && multitrack: %.2E\n", h_test->GetEntries());
}

//----------------------------------------------------------------------------------------------------

void two_out_of_four()
{
	TChain *ch = new TChain("TotemNtuple");
	
	string prefix = "/mnt/lhcanalysis/2012/validation/";

	// data set 1
	/*
	ch->Add((prefix + "val6884/val6884_totem_ntuple.root").c_str());
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

	/*
	// data set 2
	ch->Add((prefix + "val6936/val6936_totem_ntuple.root").c_str());
	ch->Add((prefix + "val6938/val6938_totem_ntuple.root").c_str());
	ch->Add((prefix + "val6941/val6941_totem_ntuple.root").c_str());
	ch->Add((prefix + "val6942/val6942_totem_ntuple.root").c_str());
	*/
	
	// data set 3
	ch->Add((prefix + "val6944/val6944_totem_ntuple.root").c_str());
	ch->Add((prefix + "val6945/val6945_totem_ntuple.root").c_str());
	ch->Add((prefix + "val6947/val6947_totem_ntuple.root").c_str());

	printf("********** diagonal 45 bottom - 56 top **********\n");
	TwoOutOfFour(ch, "25", "21", "120", "124", "(trigger_data.input_status_bits == 1 || trigger_data.input_status_bits == 513)");
	TwoOutOfFour(ch, "120", "124", "25", "21", "(trigger_data.input_status_bits == 1 || trigger_data.input_status_bits == 513)");
	
	printf("********** diagonal 45 top - 56 bottom **********\n");
	TwoOutOfFour(ch, "24", "20", "121", "125", "(trigger_data.input_status_bits == 1 || trigger_data.input_status_bits == 513)");
	TwoOutOfFour(ch, "121", "125", "24", "20", "(trigger_data.input_status_bits == 1 || trigger_data.input_status_bits == 513)");
}

int main()
{
	two_out_of_four();
	return 0;
}
