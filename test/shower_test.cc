#include "TChain.h"
#include "TCanvas.h"
#include "TH1D.h"

#include <string>

//#include "RPRootTrackInfo.h"
//. gSystem->AddLinkedLibs("-L/afs/cern.ch/exp/totem/scratch/jkaspar/software/offline/424/lib/slc5_amd64_gcc434/ -lTotemAnalysisTotemNtuplizer")

using namespace std;

//----------------------------------------------------------------------------------------------------

double GetN(TChain *ch, const string &cut)
{
	TCanvas *c = new TCanvas();
	ch->Draw("event_info.timestamp", cut.c_str());
	TH1D *h = (TH1D *) gPad->GetPrimitive("htemp");
	double N = (h == NULL) ? 0. : h->GetEntries();
	delete c;
	return N;
}

//----------------------------------------------------------------------------------------------------

void shower_test()
{
	TChain *ch = new TChain("TotemNtuple");
	
	string prefix = "/afs/cern.ch/work/j/jkaspar/";
	//ch->Add((prefix + "6884_ntuple.root").c_str());
	//ch->Add((prefix + "6885_ntuple.root").c_str());
	//ch->Add((prefix + "6927_ntuple.root").c_str());
	//ch->Add((prefix + "6929_ntuple.root").c_str());
	
	ch->Add((prefix + "6944_ntuple.root").c_str());
	ch->Add((prefix + "6945_ntuple.root").c_str());
	ch->Add((prefix + "6947_ntuple.root").c_str());
	
	printf("* entries: %i\n", ch->GetEntries());

	// trigger: RPV && bunch 0 && left RPs have both tracks
	string bcut = "(trigger_data.input_status_bits & 1) > 0 && trigger_data.bunch_num == 0 && track_rp_24.valid && track_rp_20.valid";
	double Nb = GetN(ch, bcut); printf("* base cut: %.0f\n", Nb);

	string rcut;
	
	double Nt;

	rcut = "track_rp_121.valid && track_rp_125.valid";
	Nt = GetN(ch, bcut + " && (" + rcut + ")"); printf("* valid && valid: %.0f = %.2f %%\n", Nt, Nt/Nb*100.);
	
	rcut = "track_rp_121.valid && !track_rp_125.valid";
	Nt = GetN(ch, bcut + " && (" + rcut + ")"); printf("* valid && !valid: %.0f = %.2f %%\n", Nt, Nt/Nb*100.);
	
	rcut = "!track_rp_121.valid && track_rp_125.valid";
	Nt = GetN(ch, bcut + " && (" + rcut + ")"); printf("* !valid && valid: %.0f = %.2f %%\n", Nt, Nt/Nb*100.);
	
	rcut = "!track_rp_121.valid && !track_rp_125.valid";
	Nt = GetN(ch, bcut + " && (" + rcut + ")"); printf("* !valid && !valid: %.0f = %.2f %%\n", Nt, Nt/Nb*100.);
	
	rcut = "(nonpar_patterns_rp_121.@u.size() > 0 || nonpar_patterns_rp_121.@v.size() > 0) || (nonpar_patterns_rp_125.@u.size() > 0 || nonpar_patterns_rp_125.@v.size() > 0)";
	Nt = GetN(ch, bcut + " && (" + rcut + ")"); printf("* (u||v) patterns > 0 || (u||v) patterns > 0: %.0f = %.2f %%\n", Nt, Nt/Nb*100.);
	
	rcut = "(nonpar_patterns_rp_121.@u.size() > 0 || nonpar_patterns_rp_121.@v.size() > 0) && (nonpar_patterns_rp_125.@u.size() > 0 || nonpar_patterns_rp_125.@v.size() > 0)";
	Nt = GetN(ch, bcut + " && (" + rcut + ")"); printf("* (u||v) patterns > 0 && (u||v) patterns > 0: %.0f = %.2f %%\n", Nt, Nt/Nb*100.);

	rcut = "(nonpar_patterns_rp_121.@u.size() > 1 || nonpar_patterns_rp_121.@v.size() > 1) && (nonpar_patterns_rp_125.@u.size() > 1 || nonpar_patterns_rp_125.@v.size() > 1)";
	Nt = GetN(ch, bcut + " && (" + rcut + ")"); printf("* (u||v) patterns > 1 && (u||v) patterns > 1 : %.0f = %.2f %%\n", Nt, Nt/Nb*100.);
	
	rcut = "(nonpar_patterns_rp_121.@u.size() > 1 || nonpar_patterns_rp_121.@v.size() > 1) && !track_rp_121.valid && (nonpar_patterns_rp_125.@u.size() > 1 || nonpar_patterns_rp_125.@v.size() > 1) && !track_rp_121.valid";
	Nt = GetN(ch, bcut + " && (" + rcut + ")"); printf("* (u||v) patterns > 1, !valid && (u||v) patterns > 1, !valid : %.0f = %.2f %%\n", Nt, Nt/Nb*100.);
	
	rcut = "(nonpar_patterns_rp_121.@u.size() > 0 && nonpar_patterns_rp_121.@v.size() > 0) && (nonpar_patterns_rp_121.@u.size() > 1 || nonpar_patterns_rp_121.@v.size() > 1)"
		"&& (nonpar_patterns_rp_125.@u.size() > 0 && nonpar_patterns_rp_125.@v.size() > 0) && (nonpar_patterns_rp_125.@u.size() > 1 || nonpar_patterns_rp_125.@v.size() > 1)";
	Nt = GetN(ch, bcut + " && (" + rcut + ")"); printf("* u>0 && v>0 && (u>1 || v>1) ; N && F : %.0f = %.2f %%\n", Nt, Nt/Nb*100.);
	
	rcut = "(nonpar_patterns_rp_121.@u.size() > 0 && nonpar_patterns_rp_121.@v.size() > 0) && (nonpar_patterns_rp_121.@u.size() > 1 || nonpar_patterns_rp_121.@v.size() > 1)"
		"&& !track_rp_125.valid";
	Nt = GetN(ch, bcut + " && (" + rcut + ")"); printf("* N: u>0 && v>0 && (u>1 || v>1); F: !valid : %.0f = %.2f %%\n", Nt, Nt/Nb*100.);

	rcut = "abs(multi_track_rp_121.y / 237.7E3 - track_rp_20.y / -237.7E3) < 3* 3.5E-6 && abs(multi_track_rp_125.y / 263.2E3 - track_rp_24.y / -263.2E3) < 3* 3.5E-6";
	Nt = GetN(ch, bcut + " && (" + rcut + ")"); printf("* multi-track correspondence L-R: %.0f = %.2f %%\n", Nt, Nt/Nb*100.);
	
	rcut = "abs(multi_track_rp_121.y / 237.7E3 - track_rp_20.y / -237.7E3) < 3* 3.5E-6 && abs(multi_track_rp_125.y / 263.2E3 - track_rp_24.y / -263.2E3) < 3* 3.5E-6 && track_rp_20.y > 10";
	Nt = GetN(ch, bcut + " && (" + rcut + ")"); printf("* multi-track correspondence L-R, y > 10: %.0f = %.2f %%\n", Nt, Nt/Nb*100.);
	
	rcut = "abs(multi_track_rp_121.y / 237.7E3 - track_rp_20.y / -237.7E3) < 3* 3.5E-6 && abs(multi_track_rp_125.y / 263.2E3 - track_rp_24.y / -263.2E3) < 3* 3.5E-6 && track_rp_20.y > 10 & multi_track_rp_121.y < -10";
	Nt = GetN(ch, bcut + " && (" + rcut + ")"); printf("* multi-track correspondence L-R, y>10, veto y<10: %.0f = %.2f %%\n", Nt, Nt/Nb*100.);
	
	rcut = "abs(multi_track_rp_121.y / 237.7E3 - track_rp_20.y / -237.7E3) < 3* 3.5E-6 && abs(multi_track_rp_125.y / 263.2E3 - track_rp_24.y / -263.2E3) < 3* 3.5E-6 && !track_rp_121.valid && !track_rp_125.valid";
	Nt = GetN(ch, bcut + " && (" + rcut + ")"); printf("* multi-track correspondence L-R; N !valid && F !valid : %.0f = %.2f %%\n", Nt, Nt/Nb*100.);
	
	rcut = "abs(multi_track_rp_121.y / 237.7E3 - track_rp_20.y / -237.7E3) < 3* 3.5E-6 && abs(multi_track_rp_125.y / 263.2E3 - track_rp_24.y / -263.2E3) < 3* 3.5E-6 && track_rp_20.y > 10 && !track_rp_121.valid && !track_rp_125.valid";
	Nt = GetN(ch, bcut + " && (" + rcut + ")"); printf("* multi-track correspondence L-R, y>10; N !valid && F !valid : %.0f = %.2f %%\n", Nt, Nt/Nb*100.);
}

//----------------------------------------------------------------------------------------------------

int main()
{
	shower_test();
}
