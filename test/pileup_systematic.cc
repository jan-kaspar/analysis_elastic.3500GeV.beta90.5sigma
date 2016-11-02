#include "TGraph.h"
#include "TGraphErrors.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TChain.h"
#include "TH1D.h"

#include <string>
#include <map>
#include <vector>

using namespace std;

//string prefix = "rfio:///castor/cern.ch/user/j/jkaspar/RP_reco/ntuples/";
//string prefix = "/mnt/lhcanalysis/2012/validation/";
string prefix = "/afs/cern.ch/work/j/jkaspar/ntuples/UN_2011_10_20/";

string start_cut = "(trigger_data.input_status_bits & 512) > 0 && trigger_data.bunch_num == 0";	// T:BX && B=0

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

void AnalyzeDiagonal(const vector<string> &input_files, const string &rp1, const string &rp2, const string &rp3, const string &rp4, TGraphErrors *g)
{
	printf("\n >> AnalyzeDiagonal\n");

	double Sw1 = 0., Scw1 = 0., Swwss1 = 0.;
	double Sw2 = 0., Scw2 = 0.;

	for (unsigned int fi = 0; fi < input_files.size(); fi++) {
		printf("* file %u: %s\n", fi, input_files[fi].c_str());
		TChain *ch = new TChain("TotemNtuple");
		ch->Add(input_files[fi].c_str());

		double N, tm, tv;
		GetTimeData(ch, start_cut, N, tm, tv);

		string cut = start_cut + " && ("
			"((nonpar_patterns_rp_"+rp1+".@u.size() > 0 || nonpar_patterns_rp_"+rp1+".@v.size() > 0) && (nonpar_patterns_rp_"+rp2+".@u.size() > 0 || nonpar_patterns_rp_"+rp2+".@v.size() > 0))"
			"|| ((nonpar_patterns_rp_"+rp3+".@u.size() > 0 || nonpar_patterns_rp_"+rp3+".@v.size() > 0) && (nonpar_patterns_rp_"+rp4+".@u.size() > 0 || nonpar_patterns_rp_"+rp4+".@v.size() > 0))"
		")";
		double Nt = GetN(ch, cut);		
		double prob = Nt / N;
		double prob_e = sqrt(Nt) / N;

		int idx = g->GetN();
		g->SetPoint(idx, tm, prob);
		g->SetPointError(idx, tv/sqrt(N), prob_e);

		printf("\tN = %.1E\n", N);
		printf("\ttime = %.3E +- %.2E, prob = (%.2f +- %.2f) %%\n", tm, tv/sqrt(N), prob*100., prob_e*100.);

		double c = 1./(1. - prob), c_e = c*c * prob_e;

		Sw1 += N;
		Scw1 += N * c;
		Swwss1 += N*N * c_e*c_e;
		
		Sw2 += 1./c_e/c_e;
		Scw2 += c/c_e/c_e;

		delete ch;
	}

	double cm1 = Scw1 / Sw1, cm_e1 = sqrt(Swwss1) / Sw1;
	printf("cm1: %.3f +- %.4f, mean prob 1: (%.2f +- %.2f) %%\n", cm1, cm_e1, 100. - 100./cm1, cm_e1/cm1/cm1 * 100.);

	double cm2 = Scw2 / Sw2, cm_e2 = 1./sqrt(Sw2);
	printf("cm2: %.3f +- %.4f, mean prob 2: (%.2f +- %.2f) %%\n", cm2, cm_e2, 100. - 100./cm2, cm_e2/cm2/cm2 * 100.);
}

//----------------------------------------------------------------------------------------------------

void pileup_systematic()
{
	map< unsigned int, vector<string> > files;

	// DS 1
	files[1].push_back(prefix + "6884_ntuple.root");
	files[1].push_back(prefix + "6885_ntuple.root");
	files[1].push_back(prefix + "6887_ntuple.root");
	files[1].push_back(prefix + "6895_ntuple.root");
	files[1].push_back(prefix + "6896_ntuple.root");	// short
	files[1].push_back(prefix + "6897_ntuple.root");
	files[1].push_back(prefix + "6899_ntuple.root");
	files[1].push_back(prefix + "6900_ntuple.root");
	files[1].push_back(prefix + "6903_ntuple.root");
	files[1].push_back(prefix + "6904_ntuple.root");
	files[1].push_back(prefix + "6905_ntuple.root");
	files[1].push_back(prefix + "6908_ntuple.root");	// short
	files[1].push_back(prefix + "6909_ntuple.root");
	files[1].push_back(prefix + "6910_ntuple.root");
	files[1].push_back(prefix + "6911_ntuple.root");
	files[1].push_back(prefix + "6912_ntuple.root");
	files[1].push_back(prefix + "6913_ntuple.root");
	files[1].push_back(prefix + "6914_ntuple.root");
	files[1].push_back(prefix + "6915_ntuple.root");
	//files[1].push_back(prefix + "6916_ntuple.root");	// short
	files[1].push_back(prefix + "6917_ntuple.root");
	files[1].push_back(prefix + "6918_ntuple.root");
	files[1].push_back(prefix + "6919_ntuple.root");
	files[1].push_back(prefix + "6920_ntuple.root");
	files[1].push_back(prefix + "6921_ntuple.root");
	files[1].push_back(prefix + "6922_ntuple.root");
	files[1].push_back(prefix + "6923_ntuple.root");
	files[1].push_back(prefix + "6924_ntuple.root");
	files[1].push_back(prefix + "6925_ntuple.root");
	files[1].push_back(prefix + "6926_ntuple.root");
	files[1].push_back(prefix + "6927_ntuple.root");
	files[1].push_back(prefix + "6929_ntuple.root");

	// DS 2
	files[2].push_back(prefix + "6936_ntuple.root");
	files[2].push_back(prefix + "6938_ntuple.root");
	files[2].push_back(prefix + "6941_ntuple.root");
	files[2].push_back(prefix + "6942_ntuple.root");

	// DS 3
	files[3].push_back(prefix + "6944_ntuple.root");
	files[3].push_back(prefix + "6945_ntuple.root");
	files[3].push_back(prefix + "6947_ntuple.root");

	TFile *outF = new TFile("pileup_systematic.root", "recreate");

	for (map< unsigned int, vector<string> >::iterator dsi = files.begin(); dsi != files.end(); ++dsi) {
		printf("\n\n-------------------- DS%u --------------------\n", dsi->first);

		char buf[20];
		sprintf(buf, "DS%u", dsi->first);
		gDirectory = outF->mkdir(buf);

		TGraphErrors *g45b = new TGraphErrors(); g45b->SetName("g45b");
		AnalyzeDiagonal(dsi->second, "25", "21", "120", "124", g45b);
		g45b->Write();

		TGraphErrors *g45t = new TGraphErrors(); g45t->SetName("g45t");
		AnalyzeDiagonal(dsi->second, "24", "20", "121", "125", g45t);
		g45t->Write();
	}

	delete outF;
}
