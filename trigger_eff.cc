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


//----------------------------------------------------------------------------------------------------

double GetN(TChain *ch, const string &cut, const string &label)
{
	TCanvas *c = new TCanvas();
	ch->Draw("event_info.timestamp", cut.c_str());
	TH1D *h = (TH1D *) gPad->GetPrimitive("htemp");
	double N = (h == NULL) ? 0. : h->GetEntries();
	delete c;
	
	printf(">> %s\n\t%s\n\t%.3E\n", label.c_str(), cut.c_str(), N);

	return N;
}

//----------------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{
	if (argc != 2)
		return 1;

	Init(argv[1]);

	if (diagonal == d45t)
		return 0;

	InitInputFiles();
	TChain *ch = new TChain("TotemNtuple");
	for (unsigned int i = 0; i < input_files.size(); i++) {
		ch->Add(input_files[i].c_str());
	}


	string base_cut = "(trigger_data.input_status_bits & 512) > 0 && trigger_data.bunch_num == 0";	// T:BX && B=0
	//string start_cut = "(trigger_data.input_status_bits & 1) > 0 && trigger_data.bunch_num == 0";		// T:RPV && B=0
	//string start_cut = "(trigger_data.input_status_bits & 512) > 0 && (trigger_data.input_status_bits & 1) > 0 && trigger_data.bunch_num == 0"; // T:(RPV && BX) && B=0

	string hw_trigger = "(trigger_data.input_status_bits & 3) > 0";
	
	string swt_20 = "(track_rp_20.u_sect_no >= 1 || track_rp_20.v_sect_no >= 1)";
	string swt_21 = "(track_rp_21.u_sect_no >= 1 || track_rp_21.v_sect_no >= 1)";
	string swt_22 = "(track_rp_22.u_sect_no >= 1 || track_rp_22.v_sect_no >= 1)";
	string swt_23 = "(track_rp_23.u_sect_no >= 1 || track_rp_23.v_sect_no >= 1)";
	string swt_24 = "(track_rp_24.u_sect_no >= 1 || track_rp_24.v_sect_no >= 1)";
	string swt_25 = "(track_rp_25.u_sect_no >= 1 || track_rp_25.v_sect_no >= 1)";

	string swt_120 = "(track_rp_120.u_sect_no >= 1 || track_rp_120.v_sect_no >= 1)";
	string swt_121 = "(track_rp_121.u_sect_no >= 1 || track_rp_121.v_sect_no >= 1)";
	string swt_122 = "(track_rp_122.u_sect_no >= 1 || track_rp_122.v_sect_no >= 1)";
	string swt_123 = "(track_rp_123.u_sect_no >= 1 || track_rp_123.v_sect_no >= 1)";
	string swt_124 = "(track_rp_124.u_sect_no >= 1 || track_rp_124.v_sect_no >= 1)";
	string swt_125 = "(track_rp_125.u_sect_no >= 1 || track_rp_125.v_sect_no >= 1)";
	
	string sw_trigger_left = swt_20 + "||" + swt_21 + "||" +  swt_22 + "||" +  swt_23 + "||" +  swt_24 + "||" +  swt_25;
	string sw_trigger_right = swt_120 + "||" + swt_121 + "||" +  swt_122 + "||" +  swt_123 + "||" +  swt_124 + "||" +  swt_125;
	string sw_trigger = "(" + sw_trigger_left + ") && (" + sw_trigger_right + ")";
	
	double Nb = GetN(ch, base_cut, "Nb");
	double Nhw = GetN(ch, "(" + base_cut + ") && (" + hw_trigger + ")", "Nhw");

	double Nsw_20 = GetN(ch, "(" + base_cut + ") && (" + swt_20 + ")", "Nsw_20");
	double Nsw_21 = GetN(ch, "(" + base_cut + ") && (" + swt_21 + ")", "Nsw_21");
	double Nsw_22 = GetN(ch, "(" + base_cut + ") && (" + swt_22 + ")", "Nsw_22");
	double Nsw_23 = GetN(ch, "(" + base_cut + ") && (" + swt_23 + ")", "Nsw_23");
	double Nsw_24 = GetN(ch, "(" + base_cut + ") && (" + swt_24 + ")", "Nsw_24");
	double Nsw_25 = GetN(ch, "(" + base_cut + ") && (" + swt_25 + ")", "Nsw_25");

	double Nsw_120 = GetN(ch, "(" + base_cut + ") && (" + swt_120 + ")", "Nsw_120");
	double Nsw_121 = GetN(ch, "(" + base_cut + ") && (" + swt_121 + ")", "Nsw_121");
	double Nsw_122 = GetN(ch, "(" + base_cut + ") && (" + swt_122 + ")", "Nsw_122");
	double Nsw_123 = GetN(ch, "(" + base_cut + ") && (" + swt_123 + ")", "Nsw_123");
	double Nsw_124 = GetN(ch, "(" + base_cut + ") && (" + swt_124 + ")", "Nsw_124");
	double Nsw_125 = GetN(ch, "(" + base_cut + ") && (" + swt_125 + ")", "Nsw_125");

	double Nswl = GetN(ch, "(" + base_cut + ") && (" + sw_trigger_left + ")", "Nswl");
	double Nswr = GetN(ch, "(" + base_cut + ") && (" + sw_trigger_right + ")", "Nswr");
	double Nsw = GetN(ch, "(" + base_cut + ") && (" + sw_trigger + ")", "Nsw");
	double Nhwsw = GetN(ch, "(" + base_cut + ") && (" + hw_trigger + ") && (" + sw_trigger + ")", "Nhwsw");

	printf("\n>> trigger efficiency: %.3f\n", Nhw/Nsw);

	return 0;
}
