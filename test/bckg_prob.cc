#include "TChain.h"
#include "TCanvas.h"
#include "TH1D.h"

#include <string>


using namespace std;

//----------------------------------------------------------------------------------------------------

TH1D *h_test = new TH1D("h_test", "", 100, 0, 0);

//string base_cut = "(trigger_data.input_status_bits & 512) > 0 && trigger_data.bunch_num == 0";	// T:BX && B=0
//string base_cut = "(trigger_data.input_status_bits & 1) > 0 && trigger_data.bunch_num == 0";		// T:RPV && B=0

// T:(RPV && BX) && B=0
string base_cut = "(trigger_data.input_status_bits & 512) > 0 && (trigger_data.input_status_bits & 1) > 0 && trigger_data.bunch_num == 0";

//----------------------------------------------------------------------------------------------------

void AnalyzePot(TChain *ch, const string &rp, double N)
{
	printf(">> RP %s\n", rp.c_str());
	printf("\tN = %E\n", N);
	
	string cut;

	cut = base_cut + " && digi_rp_"+rp+".numberOfPlanesOn < 1";
	ch->Draw("event_info.timestamp >> h_test", cut.c_str(), "goff");
	printf("\t#pl == 0             : %.3E, %5.2f %%\n", h_test->GetEntries(), h_test->GetEntries() / N * 100.);
	
	cut = base_cut + " && (track_rp_"+rp+".u_sect_no > 0 || track_rp_"+rp+".v_sect_no > 0)";
	ch->Draw("event_info.timestamp >> h_test", cut.c_str(), "goff");
	printf("\ttrig U||V            : %.3E, %5.2f %%\n", h_test->GetEntries(), h_test->GetEntries() / N * 100.);
	
	cut = base_cut + " && digi_rp_"+rp+".numberOfPlanesOn > 5";
	ch->Draw("event_info.timestamp >> h_test", cut.c_str(), "goff");
	printf("\t#pl > 5              : %.3E, %5.2f %%\n", h_test->GetEntries(), h_test->GetEntries() / N * 100.);
	
	cut = base_cut + " && @multi_track_rp_"+rp+".size() > 0";
	ch->Draw("event_info.timestamp >> h_test", cut.c_str(), "goff");
	printf("\tmulti track          : %.3E, %5.2f %%\n", h_test->GetEntries(), h_test->GetEntries() / N * 100.);
	
	cut = base_cut + " && @multi_track_rp_"+rp+".size() > 0 && !track_rp_"+rp+".valid";
	ch->Draw("event_info.timestamp >> h_test", cut.c_str(), "goff");
	printf("\tmu.track & !si.track : %.3E, %5.2f %%\n", h_test->GetEntries(), h_test->GetEntries() / N * 100.);
	
	cut = base_cut + " && track_rp_"+rp+".valid";
	ch->Draw("event_info.timestamp >> h_test", cut.c_str(), "goff");
	printf("\tsingle track         : %.3E, %5.2f %%\n", h_test->GetEntries(), h_test->GetEntries() / N * 100.);
	
	cut = base_cut + " && (track_rp_"+rp+".valid || @multi_track_rp_"+rp+".size() > 0)";
	ch->Draw("event_info.timestamp >> h_test", cut.c_str(), "goff");
	printf("\tany (mu||si) track   : %.3E, %5.2f %%\n", h_test->GetEntries(), h_test->GetEntries() / N * 100.);
}

//----------------------------------------------------------------------------------------------------

void AnalyzePair(TChain *ch, const string &rp1, const string &rp2, double N)
{
	AnalyzePot(ch, rp1, N);
	AnalyzePot(ch, rp2, N);
	
	printf(">> pair %s, %s\n", rp1.c_str(), rp2.c_str());
	
	string cut;
	
	cut = base_cut + " && digi_rp_"+rp1+".numberOfPlanesOn < 1 && digi_rp_"+rp2+".numberOfPlanesOn < 1";
	ch->Draw("event_info.timestamp >> h_test", cut.c_str(), "goff");
	printf("\t#pl == 0  && #pl == 0   : %.3E, %5.2f %%\n", h_test->GetEntries(), h_test->GetEntries() / N * 100.);
	
	cut = base_cut + " && (digi_rp_"+rp1+".numberOfPlanesOn < 1 || digi_rp_"+rp2+".numberOfPlanesOn < 1)";
	ch->Draw("event_info.timestamp >> h_test", cut.c_str(), "goff");
	printf("\t#pl == 0  || #pl == 0   : %.3E, %5.2f %%\n", h_test->GetEntries(), h_test->GetEntries() / N * 100.);

	cut = base_cut + " && digi_rp_"+rp1+".numberOfPlanesOn > 5 && digi_rp_"+rp2+".numberOfPlanesOn > 5";
	ch->Draw("event_info.timestamp >> h_test", cut.c_str(), "goff");
	printf("\t#pl > 5   && #pl > 5    : %.3E, %5.2f %%\n", h_test->GetEntries(), h_test->GetEntries() / N * 100.);

	cut = base_cut + " && (digi_rp_"+rp1+".numberOfPlanesOn > 5 || digi_rp_"+rp2+".numberOfPlanesOn > 5)";
	ch->Draw("event_info.timestamp >> h_test", cut.c_str(), "goff");
	printf("\t#pl > 5   || #pl > 5    : %.3E, %5.2f %%\n", h_test->GetEntries(), h_test->GetEntries() / N * 100.);
	
	cut = base_cut + " && ((@multi_track_rp_"+rp1+".size() > 0 || track_rp_"+rp1+".valid) && (@multi_track_rp_"+rp2+".size() > 0 || track_rp_"+rp2+".valid))";
	ch->Draw("event_info.timestamp >> h_test", cut.c_str(), "goff");
	printf("\tany track && any track  : %.3E, %5.2f %%\n", h_test->GetEntries(), h_test->GetEntries() / N * 100.);
	
	cut = base_cut + " && ((@multi_track_rp_"+rp1+".size() > 0 || track_rp_"+rp1+".valid) || (@multi_track_rp_"+rp2+".size() > 0 || track_rp_"+rp2+".valid))";
	ch->Draw("event_info.timestamp >> h_test", cut.c_str(), "goff");
	printf("\tany track || any track  : %.3E, %5.2f %%\n", h_test->GetEntries(), h_test->GetEntries() / N * 100.);
}

//----------------------------------------------------------------------------------------------------

void AnalyzeDiagonal(TChain *ch, const string &rp1, const string &rp2, const string &rp3, const string &rp4)
{
	printf("\n***** diagonal %s, %s -- %s, %s *****\n", rp1.c_str(), rp2.c_str(), rp3.c_str(), rp4.c_str());

	string cut;

	cut = base_cut;
	ch->Draw("event_info.timestamp >> h_test", cut.c_str(), "goff");
	double N = h_test->GetEntries();
	printf("\tfull statistics: %.5E\n", N);

	/*
	AnalyzePair(ch, rp2, rp1, N);
	AnalyzePair(ch, rp3, rp4, N);
	
	printf(">> diagonal\n");
	
	cut = base_cut + " && ((digi_rp_"+rp1+".numberOfPlanesOn > 5 && digi_rp_"+rp2+".numberOfPlanesOn > 5) && (digi_rp_"+rp3+".numberOfPlanesOn > 5 && digi_rp_"+rp4+".numberOfPlanesOn > 5))";
	ch->Draw("event_info.timestamp >> h_test", cut.c_str(), "goff");
	printf("\t#pl > 5  && #pl > 5;    L && R : %.3E, %5.2f %%\n", h_test->GetEntries(), h_test->GetEntries() / N * 100.);
	
	cut = base_cut + " && ((digi_rp_"+rp1+".numberOfPlanesOn > 5 && digi_rp_"+rp2+".numberOfPlanesOn > 5) || (digi_rp_"+rp3+".numberOfPlanesOn > 5 && digi_rp_"+rp4+".numberOfPlanesOn > 5))";
	ch->Draw("event_info.timestamp >> h_test", cut.c_str(), "goff");
	printf("\t#pl > 5  && #pl > 5;    L || R : %.3E, %5.2f %%\n", h_test->GetEntries(), h_test->GetEntries() / N * 100.);

	cut = base_cut + " && (  ((@multi_track_rp_"+rp1+".size() > 0 || track_rp_"+rp1+".valid) && (@multi_track_rp_"+rp2+".size() > 0 || track_rp_"+rp2+".valid)) && ((@multi_track_rp_"+rp3+".size() > 0 || track_rp_"+rp3+".valid) && (@multi_track_rp_"+rp4+".size() > 0 || track_rp_"+rp4+".valid))  )";
	ch->Draw("event_info.timestamp >> h_test", cut.c_str(), "goff");
	printf("\tany track && any track; L && R : %.3E, %5.2f %%\n", h_test->GetEntries(), h_test->GetEntries() / N * 100.);

	cut = base_cut + " && (  ((@multi_track_rp_"+rp1+".size() > 0 || track_rp_"+rp1+".valid) && (@multi_track_rp_"+rp2+".size() > 0 || track_rp_"+rp2+".valid)) || ((@multi_track_rp_"+rp3+".size() > 0 || track_rp_"+rp3+".valid) && (@multi_track_rp_"+rp4+".size() > 0 || track_rp_"+rp4+".valid))  )";
	ch->Draw("event_info.timestamp >> h_test", cut.c_str(), "goff");
	printf("\tany track && any track; L || R : %.3E, %5.2f %%\n", h_test->GetEntries(), h_test->GetEntries() / N * 100.);
	*/
	
	cut = base_cut + " && track_rp_"+rp1+".valid && track_rp_"+rp2+".valid && track_rp_"+rp3+".valid && track_rp_"+rp4+".valid";
		
	ch->Draw("event_info.timestamp >> h_test", cut.c_str(), "goff");
	N = h_test->GetEntries();
	printf("\t4 out of 4             : %.3E, %5.2f %%\n", h_test->GetEntries(), h_test->GetEntries() / N * 100.);
	
	cut = base_cut +
		" && ( (track_rp_"+rp1+".valid && track_rp_"+rp2+".valid && !track_rp_"+rp3+".valid && !track_rp_"+rp4+".valid && digi_rp_"+rp3+".numberOfPlanesOn > 5 && digi_rp_"+rp4+".numberOfPlanesOn > 5)"
		+ "|| (track_rp_"+rp3+".valid && track_rp_"+rp4+".valid && !track_rp_"+rp1+".valid && !track_rp_"+rp2+".valid && digi_rp_"+rp1+".numberOfPlanesOn > 5 && digi_rp_"+rp2+".numberOfPlanesOn > 5) )";
		
	ch->Draw("event_info.timestamp >> h_test", cut.c_str(), "goff");
	printf("\t2 out of 4: >5pl && !ST : %.3E, %5.2f %%\n", h_test->GetEntries(), h_test->GetEntries() / N * 100.);
	
	cut = base_cut +
		" && ( (track_rp_"+rp1+".valid && track_rp_"+rp2+".valid && !track_rp_"+rp3+".valid && !track_rp_"+rp4+".valid && @multi_track_rp_"+rp3+".size() > 0 && @multi_track_rp_"+rp4+".size() > 0)"
		+ "|| (track_rp_"+rp3+".valid && track_rp_"+rp4+".valid && !track_rp_"+rp1+".valid && !track_rp_"+rp2+".valid && @multi_track_rp_"+rp1+".size() > 0 && @multi_track_rp_"+rp2+".size() > 0) )";
		
	ch->Draw("event_info.timestamp >> h_test", cut.c_str(), "goff");
	printf("\t2 out of 4: MT && !ST  : %.3E, %5.2f %%\n", h_test->GetEntries(), h_test->GetEntries() / N * 100.);
}


//----------------------------------------------------------------------------------------------------

void bckg_prob()
{
	TChain *ch = new TChain("TotemNtuple");
	
	string prefix = "/mnt/lhcanalysis/2012/validation/";

	// data set 1
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

	// data set 2
	/*
	ch->Add((prefix + "val6936/val6936_totem_ntuple.root").c_str());
	ch->Add((prefix + "val6938/val6938_totem_ntuple.root").c_str());
	ch->Add((prefix + "val6941/val6941_totem_ntuple.root").c_str());
	ch->Add((prefix + "val6942/val6942_totem_ntuple.root").c_str());
	*/
	
	// data set 3
	//ch->Add((prefix + "val6944/val6944_totem_ntuple.root").c_str());
	/*
	ch->Add((prefix + "val6945/val6945_totem_ntuple.root").c_str());
	ch->Add((prefix + "val6947/val6947_totem_ntuple.root").c_str());
	*/

	AnalyzeDiagonal(ch, "25", "21", "120", "124");
	AnalyzeDiagonal(ch, "24", "20", "121", "125");
}

int main()
{
	bckg_prob();
	return 0;
}
