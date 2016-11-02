#include "TGraph.h"
#include "TGraphErrors.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TChain.h"
#include "TH1D.h"

#include <string>
#include <map>
#include <vector>
#include <cmath>

using namespace std;

string start_cut = "(trigger_data.input_status_bits & 1) > 0 && trigger_data.bunch_num == 0";	// T:RPV && B=0

//----------------------------------------------------------------------------------------------------

void GetTimeData(TChain *ch, string cut, double &N, double &mean, double &RMS)
{
	TCanvas *c = new TCanvas();
	ch->Draw("event_info.timestamp", cut.c_str());
	TH1D *h = (TH1D *) gPad->GetPrimitive("htemp");
	N = (h == NULL) ? 0. : h->GetEntries();
	mean = (h == NULL) ? 0. : h->GetMean();
	RMS = (h == NULL) ? 0. : h->GetRMS();
	delete c;
}

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

void Analyze(const vector<string> &input_files, const string &label,
	const string &ref_F, const string &ref_N, const string &std_N, const string &std_F, const string &veto_N)
{
	printf("\n>> %s\n", label.c_str());

	char buf[500];
	sprintf(buf, "%s", label.c_str());
	gDirectory = gDirectory->mkdir(buf);

	TGraphErrors *g_prob = new TGraphErrors();
	g_prob->SetName("g_prob");

	double Sw=0., Swp=0., Swpp=0.;

	for (unsigned int fi = 0; fi < input_files.size(); fi++) {
		printf("* file %u: %s\n", fi, input_files[fi].c_str());
		TChain *ch = new TChain("TotemNtuple");
		ch->Add(input_files[fi].c_str());

		double N, tm, tv;
		GetTimeData(ch, start_cut, N, tm, tv);
		
		printf("\tN = %.1E\n", N);
		printf("\ttime = %.3E +- %.2E\n", tm, tv/sqrt(N));

		string ref_tr_val = "(track_rp_"+ref_F+".valid && track_rp_"+ref_N+".valid)";
		string std_pl_suff = "(digi_rp_"+std_N+".numberOfPlanesOn >= 8 && digi_rp_"+std_F+".numberOfPlanesOn >= 8)";
		string std_pat_inval = "((nonpar_patterns_rp_"+std_N+".@u.size() != 1 || nonpar_patterns_rp_"+std_N+".@v.size() != 1) && (nonpar_patterns_rp_"+std_F+".@u.size() != 1 || nonpar_patterns_rp_"+std_F+".@v.size() != 1))";
		string std_pu = "((@multi_track_rp_"+std_N+".size() == 4 || @multi_track_rp_"+std_N+".size() == 2) && (@multi_track_rp_"+std_F+".size() == 4 || @multi_track_rp_"+std_F+".size() == 2))";
		string veto_pl_insuff = "(digi_rp_"+veto_N+".numberOfPlanesOn < 8)";
		
		double N_el = GetN(ch, start_cut+"&&"+ref_tr_val+"&&"+std_pl_suff+"&&"+veto_pl_insuff);		
		double N_24 = GetN(ch, start_cut+"&&"+ref_tr_val+"&&"+std_pl_suff+"&&"+veto_pl_insuff+"&&"+std_pat_inval);		
		double N_pu = GetN(ch, start_cut+"&&"+ref_tr_val+"&&"+std_pl_suff+"&&"+veto_pl_insuff+"&&"+std_pu);		
		printf("\t%.0f, %.0f, %.0f\n", N_el, N_24, N_pu);

		double N_prob = N_24 - N_pu;
		double prob = N_prob / N_el, prob_e = sqrt(N_prob) / N_el;

		printf("\tprob = (%.2f +- %.2f) %%\n", prob*100., prob_e*100.);

		// fill histogram
		int idx = g_prob->GetN();
		g_prob->SetPoint(idx, tm, prob);
		g_prob->SetPointError(idx, tv/sqrt(N), prob_e);

		// increase counters
		double w = 1./ prob_e / prob_e;
		Sw += w;
		Swp += w * prob;
		Swpp += w * prob * prob;

		delete ch;
	}

	double prob_mean = Swp / Sw, prob_mean_e =  1. / sqrt(Sw);
	double prob_var = (Swpp - Swp*Swp/Sw) / Sw;
	double prob_stddev = (prob_var > 0.) ? sqrt(prob_var) : 0.;
	printf("prob_mean = (%.3f +- %.3f) %%\n", prob_mean*100., prob_mean_e*100.);
	printf("prob_var = %.3f %%\n", prob_stddev*100.);

	g_prob->Write();
}

//----------------------------------------------------------------------------------------------------

int main()
{
	string prefix = "/afs/cern.ch/work/j/jkaspar/ntuples/UN_2011_10_20/";

	map<unsigned int, vector<string> > file_map;

	// DS 1
	file_map[1].push_back(prefix + "6884_ntuple.root");
	file_map[1].push_back(prefix + "6885_ntuple.root");
	file_map[1].push_back(prefix + "6887_ntuple.root");
	file_map[1].push_back(prefix + "6895_ntuple.root");
	file_map[1].push_back(prefix + "6896_ntuple.root");	// short
	file_map[1].push_back(prefix + "6897_ntuple.root");
	file_map[1].push_back(prefix + "6899_ntuple.root");
	file_map[1].push_back(prefix + "6900_ntuple.root");
	file_map[1].push_back(prefix + "6903_ntuple.root");
	file_map[1].push_back(prefix + "6904_ntuple.root");
	file_map[1].push_back(prefix + "6905_ntuple.root");
	file_map[1].push_back(prefix + "6908_ntuple.root");	// short
	file_map[1].push_back(prefix + "6909_ntuple.root");
	file_map[1].push_back(prefix + "6910_ntuple.root");
	file_map[1].push_back(prefix + "6911_ntuple.root");
	file_map[1].push_back(prefix + "6912_ntuple.root");
	file_map[1].push_back(prefix + "6913_ntuple.root");
	file_map[1].push_back(prefix + "6914_ntuple.root");
	file_map[1].push_back(prefix + "6915_ntuple.root");
	//file_map[1].push_back(prefix + "6916_ntuple.root");	// short
	file_map[1].push_back(prefix + "6917_ntuple.root");
	file_map[1].push_back(prefix + "6918_ntuple.root");
	file_map[1].push_back(prefix + "6919_ntuple.root");
	file_map[1].push_back(prefix + "6920_ntuple.root");
	file_map[1].push_back(prefix + "6921_ntuple.root");
	file_map[1].push_back(prefix + "6922_ntuple.root");
	file_map[1].push_back(prefix + "6923_ntuple.root");
	file_map[1].push_back(prefix + "6924_ntuple.root");
	file_map[1].push_back(prefix + "6925_ntuple.root");
	file_map[1].push_back(prefix + "6926_ntuple.root");
	file_map[1].push_back(prefix + "6927_ntuple.root");
	file_map[1].push_back(prefix + "6929_ntuple.root");

	// DS 2
	file_map[2].push_back(prefix + "6936_ntuple.root");
	file_map[2].push_back(prefix + "6938_ntuple.root");
	file_map[2].push_back(prefix + "6941_ntuple.root");
	file_map[2].push_back(prefix + "6942_ntuple.root");

	// DS 3
	file_map[3].push_back(prefix + "6944_ntuple.root");
	file_map[3].push_back(prefix + "6945_ntuple.root");
	file_map[3].push_back(prefix + "6947_ntuple.root");


	TFile *outF = new TFile("shower_test3.root", "recreate");

	for (map< unsigned int, vector<string> >::iterator dsi = file_map.begin(); dsi != file_map.end(); ++dsi) {
		printf("\n\n-------------------- DS%u --------------------\n", dsi->first);

		char buf[20];
		sprintf(buf, "DS%u", dsi->first);
		gDirectory = outF->mkdir(buf);

		Analyze(dsi->second, "45bot,R", "25", "21", "120", "124", "121");
		Analyze(dsi->second, "45bot,L", "124", "120", "21", "25", "20");
		Analyze(dsi->second, "45top,R", "24", "20", "121", "125", "120");
		Analyze(dsi->second, "45top,L", "125", "121", "20", "24", "21");
	}

	delete outF;
	return 0;
}
