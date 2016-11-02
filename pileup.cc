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

string start_cut = "(trigger_data.input_status_bits & 512) > 0 && trigger_data.bunch_num == 0";	// T:BX && B=0
//string start_cut = "(trigger_data.input_status_bits & 1) > 0 && trigger_data.bunch_num == 0";		// T:RPV && B=0
//string start_cut = "(trigger_data.input_status_bits & 512) > 0 && (trigger_data.input_status_bits & 1) > 0 && trigger_data.bunch_num == 0"; // T:(RPV && BX) && B=0

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

void AnalyzePot(TChain *ch, const string &rp, double N)
{
	printf(">> RP %s\n", rp.c_str());
	printf("\tN = %E\n", N);
	
	string cut;
	double Nt;

	cut = start_cut + " && digi_rp_"+rp+".numberOfPlanesOn < 1";
	Nt = GetN(ch, cut); printf("\t#pl == 0                : %.3E, %5.2f %%\n", Nt, Nt / N * 100.);
	
	cut = start_cut + " && (track_rp_"+rp+".u_sect_no > 0 || track_rp_"+rp+".v_sect_no > 0)";
	Nt = GetN(ch, cut); printf("\ttrig U||V               : %.3E, %5.2f %%\n", Nt, Nt / N * 100.);
	
	cut = start_cut + " && digi_rp_"+rp+".numberOfPlanesOn > 5";
	Nt = GetN(ch, cut); printf("\t#pl > 5                 : %.3E, %5.2f %%\n", Nt, Nt / N * 100.);
	
	cut = start_cut + " && @multi_track_rp_"+rp+".size() > 0";
	Nt = GetN(ch, cut); printf("\tmulti track             : %.3E, %5.2f %%\n", Nt, Nt / N * 100.);
	
	cut = start_cut + " && @multi_track_rp_"+rp+".size() > 0 && !track_rp_"+rp+".valid";
	Nt = GetN(ch, cut); printf("\tmu.track & !si.track    : %.3E, %5.2f %%\n", Nt, Nt / N * 100.);
	
	cut = start_cut + " && track_rp_"+rp+".valid";
	Nt = GetN(ch, cut); printf("\tsingle track            : %.3E, %5.2f %%\n", Nt, Nt / N * 100.);
	
	cut = start_cut + " && (track_rp_"+rp+".valid || @multi_track_rp_"+rp+".size() > 0)";
	Nt = GetN(ch, cut); printf("\tany (mu||si) track      : %.3E, %5.2f %%\n", Nt, Nt / N * 100.);
	
	if (updated_ntuples) {
		cut = start_cut + " && (par_patterns_rp_"+rp+".@u.size() > 0 || par_patterns_rp_"+rp+".@v.size() > 0)";
		Nt = GetN(ch, cut); printf("\t#ParCand>0 (U||V)       : %.3E, %5.2f %%\n", Nt, Nt / N * 100.);
	
		cut = start_cut + " && (nonpar_patterns_rp_"+rp+".@u.size() > 0 || nonpar_patterns_rp_"+rp+".@v.size() > 0)";
		Nt = GetN(ch, cut); printf("\t#NonParCand>0 (U||V)    : %.3E, %5.2f %%\n", Nt, Nt / N * 100.);
	}
}

//----------------------------------------------------------------------------------------------------

void AnalyzePair(TChain *ch, const string &rp1, const string &rp2, double N)
{
	AnalyzePot(ch, rp1, N);
	AnalyzePot(ch, rp2, N);
	
	printf(">> pair %s, %s\n", rp1.c_str(), rp2.c_str());
	
	string cut;
	double Nt;
	
	cut = start_cut + " && digi_rp_"+rp1+".numberOfPlanesOn < 1 && digi_rp_"+rp2+".numberOfPlanesOn < 1";
	Nt = GetN(ch, cut); printf("\t#pl == 0  && #pl == 0   : %.3E, %5.2f %%\n", Nt, Nt / N * 100.);
	
	cut = start_cut + " && (digi_rp_"+rp1+".numberOfPlanesOn < 1 || digi_rp_"+rp2+".numberOfPlanesOn < 1)";
	Nt = GetN(ch, cut); printf("\t#pl == 0  || #pl == 0   : %.3E, %5.2f %%\n", Nt, Nt / N * 100.);

	cut = start_cut + " && digi_rp_"+rp1+".numberOfPlanesOn > 5 && digi_rp_"+rp2+".numberOfPlanesOn > 5";
	Nt = GetN(ch, cut); printf("\t#pl > 5   && #pl > 5    : %.3E, %5.2f %%\n", Nt, Nt / N * 100.);

	cut = start_cut + " && (digi_rp_"+rp1+".numberOfPlanesOn > 5 || digi_rp_"+rp2+".numberOfPlanesOn > 5)";
	Nt = GetN(ch, cut); printf("\t#pl > 5   || #pl > 5    : %.3E, %5.2f %%\n", Nt, Nt / N * 100.);
	
	cut = start_cut + " && ((@multi_track_rp_"+rp1+".size() > 0 || track_rp_"+rp1+".valid) && (@multi_track_rp_"+rp2+".size() > 0 || track_rp_"+rp2+".valid))";
	Nt = GetN(ch, cut); printf("\tany track && any track  : %.3E, %5.2f %%\n", Nt, Nt / N * 100.);
	
	cut = start_cut + " && ((@multi_track_rp_"+rp1+".size() > 0 || track_rp_"+rp1+".valid) || (@multi_track_rp_"+rp2+".size() > 0 || track_rp_"+rp2+".valid))";
	Nt = GetN(ch, cut); printf("\tany track || any track  : %.3E, %5.2f %%\n", Nt, Nt / N * 100.);

	if (updated_ntuples) {
		cut = start_cut + " && (  (par_patterns_rp_"+rp1+".@u.size() > 0 || par_patterns_rp_"+rp1+".@v.size() > 0) && (par_patterns_rp_"+rp2+".@u.size() > 0 || par_patterns_rp_"+rp2+".@v.size() > 0)  )";
		Nt = GetN(ch, cut); printf("\tParCand>0 && ParCand>0  : %.3E, %5.2f %%\n", Nt, Nt / N * 100.);
		
		cut = start_cut + " && (  (par_patterns_rp_"+rp1+".@u.size() > 0 || par_patterns_rp_"+rp1+".@v.size() > 0) || (par_patterns_rp_"+rp2+".@u.size() > 0 || par_patterns_rp_"+rp2+".@v.size() > 0)  )";
		Nt = GetN(ch, cut); printf("\tParCand>0 || ParCand>0  : %.3E, %5.2f %%\n", Nt, Nt / N * 100.);
		
		cut = start_cut + " && (  (nonpar_patterns_rp_"+rp1+".@u.size() > 0 || nonpar_patterns_rp_"+rp1+".@v.size() > 0) && (nonpar_patterns_rp_"+rp2+".@u.size() > 0 || nonpar_patterns_rp_"+rp2+".@v.size() > 0)  )";
		Nt = GetN(ch, cut); printf("\tNoPCand>0 && NoPCand>0  : %.3E, %5.2f %%\n", Nt, Nt / N * 100.);
		
		cut = start_cut + " && (  (nonpar_patterns_rp_"+rp1+".@u.size() > 0 || nonpar_patterns_rp_"+rp1+".@v.size() > 0) || (nonpar_patterns_rp_"+rp2+".@u.size() > 0 || nonpar_patterns_rp_"+rp2+".@v.size() > 0)  )";
		Nt = GetN(ch, cut); printf("\tNoPCand>0 || NoPCand>0  : %.3E, %5.2f %%\n", Nt, Nt / N * 100.);
	}
}

//----------------------------------------------------------------------------------------------------

void AnalyzeDiagonal(TChain *ch, const string &rp1, const string &rp2, const string &rp3, const string &rp4)
{
	printf("\n***** diagonal %s, %s -- %s, %s *****\n", rp1.c_str(), rp2.c_str(), rp3.c_str(), rp4.c_str());
	printf("\n");
	printf("*chain entries: %.5E\n", double(ch->GetEntries()));

	string cut;

	cut = start_cut;
	double N = GetN(ch, cut);
	printf("* full statistics (with start cut): %.5E\n", N);
	printf("\n");

	AnalyzePair(ch, rp2, rp1, N);
	AnalyzePair(ch, rp3, rp4, N);

	double Nt;
	
	printf(">> diagonal\n");
	
	cut = start_cut + " && ((digi_rp_"+rp1+".numberOfPlanesOn > 5 && digi_rp_"+rp2+".numberOfPlanesOn > 5) && (digi_rp_"+rp3+".numberOfPlanesOn > 5 && digi_rp_"+rp4+".numberOfPlanesOn > 5))";
	Nt = GetN(ch, cut); printf("\t#pl > 5  && #pl > 5;    L && R : %.3E, %5.2f %%\n", Nt, Nt / N * 100.);
	
	cut = start_cut + " && ((digi_rp_"+rp1+".numberOfPlanesOn > 5 && digi_rp_"+rp2+".numberOfPlanesOn > 5) || (digi_rp_"+rp3+".numberOfPlanesOn > 5 && digi_rp_"+rp4+".numberOfPlanesOn > 5))";
	Nt = GetN(ch, cut); printf("\t#pl > 5  && #pl > 5;    L || R : %.3E, %5.2f %%\n", Nt, Nt / N * 100.);

	cut = start_cut + " && (  ((@multi_track_rp_"+rp1+".size() > 0 || track_rp_"+rp1+".valid) && (@multi_track_rp_"+rp2+".size() > 0 || track_rp_"+rp2+".valid)) && ((@multi_track_rp_"+rp3+".size() > 0 || track_rp_"+rp3+".valid) && (@multi_track_rp_"+rp4+".size() > 0 || track_rp_"+rp4+".valid))  )";
	Nt = GetN(ch, cut); printf("\tany track && any track; L && R : %.3E, %5.2f %%\n", Nt, Nt / N * 100.);

	cut = start_cut + " && (  ((@multi_track_rp_"+rp1+".size() > 0 || track_rp_"+rp1+".valid) && (@multi_track_rp_"+rp2+".size() > 0 || track_rp_"+rp2+".valid)) || ((@multi_track_rp_"+rp3+".size() > 0 || track_rp_"+rp3+".valid) && (@multi_track_rp_"+rp4+".size() > 0 || track_rp_"+rp4+".valid))  )";
	Nt = GetN(ch, cut); printf("\tany track && any track; L || R : %.3E, %5.2f %%\n", Nt, Nt / N * 100.);
	
	
	if (updated_ntuples) {
		cut = start_cut + " && ("
			"((par_patterns_rp_"+rp1+".@u.size() > 0 || par_patterns_rp_"+rp1+".@v.size() > 0) && (par_patterns_rp_"+rp2+".@u.size() > 0 || par_patterns_rp_"+rp2+".@v.size() > 0))"
			"&& ((par_patterns_rp_"+rp3+".@u.size() > 0 || par_patterns_rp_"+rp3+".@v.size() > 0) && (par_patterns_rp_"+rp4+".@u.size() > 0 || par_patterns_rp_"+rp4+".@v.size() > 0))"
		")";
		Nt = GetN(ch, cut); printf("\tParCand>0 && ParCand>0; L && R : %.3E, %5.2f %%\n", Nt, Nt / N * 100.);
		
		cut = start_cut + " && ("
			"((par_patterns_rp_"+rp1+".@u.size() > 0 || par_patterns_rp_"+rp1+".@v.size() > 0) && (par_patterns_rp_"+rp2+".@u.size() > 0 || par_patterns_rp_"+rp2+".@v.size() > 0))"
			"|| ((par_patterns_rp_"+rp3+".@u.size() > 0 || par_patterns_rp_"+rp3+".@v.size() > 0) && (par_patterns_rp_"+rp4+".@u.size() > 0 || par_patterns_rp_"+rp4+".@v.size() > 0))"
		")";
		Nt = GetN(ch, cut); printf("\tParCand>0 && ParCand>0; L || R : %.3E, %5.2f %%\n", Nt, Nt / N * 100.);
		
		cut = start_cut + " && ("
			"((nonpar_patterns_rp_"+rp1+".@u.size() > 0 || nonpar_patterns_rp_"+rp1+".@v.size() > 0) && (nonpar_patterns_rp_"+rp2+".@u.size() > 0 || nonpar_patterns_rp_"+rp2+".@v.size() > 0))"
			"&& ((nonpar_patterns_rp_"+rp3+".@u.size() > 0 || nonpar_patterns_rp_"+rp3+".@v.size() > 0) && (nonpar_patterns_rp_"+rp4+".@u.size() > 0 || nonpar_patterns_rp_"+rp4+".@v.size() > 0))"
		")";
		Nt = GetN(ch, cut); printf("\tNoPCand>0 && NoPCand>0; L && R : %.3E, %5.2f %%\n", Nt, Nt / N * 100.);
		
		cut = start_cut + " && ("
			"((nonpar_patterns_rp_"+rp1+".@u.size() > 0 || nonpar_patterns_rp_"+rp1+".@v.size() > 0) && (nonpar_patterns_rp_"+rp2+".@u.size() > 0 || nonpar_patterns_rp_"+rp2+".@v.size() > 0))"
			"|| ((nonpar_patterns_rp_"+rp3+".@u.size() > 0 || nonpar_patterns_rp_"+rp3+".@v.size() > 0) && (nonpar_patterns_rp_"+rp4+".@u.size() > 0 || nonpar_patterns_rp_"+rp4+".@v.size() > 0))"
		")";
		Nt = GetN(ch, cut); printf("\tNoPCand>0 && NoPCand>0; L || R : %.3E, %5.2f %%\n", Nt, Nt / N * 100.);
	}
	
	printf("\t--------------------\n");
	
	cut = start_cut + " && track_rp_"+rp1+".valid && track_rp_"+rp2+".valid && track_rp_"+rp3+".valid && track_rp_"+rp4+".valid";
	N = Nt = GetN(ch, cut);
	printf("\t4 out of 4              : %.3E, %5.2f %%\n", Nt, Nt / N * 100.);
	
	cut = start_cut +
		" && ( (track_rp_"+rp1+".valid && track_rp_"+rp2+".valid && !track_rp_"+rp3+".valid && !track_rp_"+rp4+".valid && digi_rp_"+rp3+".numberOfPlanesOn > 5 && digi_rp_"+rp4+".numberOfPlanesOn > 5)"
		+ "|| (track_rp_"+rp3+".valid && track_rp_"+rp4+".valid && !track_rp_"+rp1+".valid && !track_rp_"+rp2+".valid && digi_rp_"+rp1+".numberOfPlanesOn > 5 && digi_rp_"+rp2+".numberOfPlanesOn > 5) )";
	Nt = GetN(ch, cut);
	printf("\t2 out of 4: >5pl && !ST : %.3E, %5.2f %%\n", Nt, Nt / N * 100.);
	
	cut = start_cut +
		" && ( (track_rp_"+rp1+".valid && track_rp_"+rp2+".valid && !track_rp_"+rp3+".valid && !track_rp_"+rp4+".valid && @multi_track_rp_"+rp3+".size() > 0 && @multi_track_rp_"+rp4+".size() > 0)"
		+ "|| (track_rp_"+rp3+".valid && track_rp_"+rp4+".valid && !track_rp_"+rp1+".valid && !track_rp_"+rp2+".valid && @multi_track_rp_"+rp1+".size() > 0 && @multi_track_rp_"+rp2+".size() > 0) )";
	Nt = GetN(ch, cut);
	printf("\t2 out of 4: MT && !ST   : %.3E, %5.2f %%\n", Nt, Nt / N * 100.);
}

//----------------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{
	printf("* start cut:\n\t%s\n", start_cut.c_str());

	if (argc != 2)
		return 1;

	Init(argv[1]);

	InitInputFiles();
	TChain *ch = new TChain("TotemNtuple");
	for (unsigned int i = 0; i < input_files.size(); i++) {
		ch->Add(input_files[i].c_str());
	}

	// ouput file
	TFile *outF = new TFile("pileup.root", "recreate");

	// run anlysis
	if (diagonal == d45b)
		AnalyzeDiagonal(ch, "25", "21", "120", "124");
	if (diagonal == d45t)
		AnalyzeDiagonal(ch, "24", "20", "121", "125");

	delete outF;
	return 0;
}
