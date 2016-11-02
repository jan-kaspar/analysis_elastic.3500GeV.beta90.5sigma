using namespace std;

#include "parameters.h"
#include "../common.h"
#include "../LumiCalculator.h"

#include "TFile.h"
#include "TH2.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TProfile.h"

#include <cmath>

struct period
{
	double first, last;
	unsigned int ev_first, ev_last;
	unsigned int tr_first, tr_last;
	unsigned int ev_total;
	unsigned int run;

	double daq_eff;
	double L_int;

	period() : first(0.), last(0.), ev_first(0), ev_last(0), tr_first(0), tr_last(0), ev_total(0), run(0), daq_eff(0.), L_int(0.) {}
};

//----------------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{
	if (argc != 2)
		return 1;

	// initializations
	Init(argv[1]);
	PrepareRun();

	// save luminosity graphs
	if (diagonal == d45b) {
		TFile *outF = new TFile("luminosity.root", "recreate");

		g_lumi_0->Write();
		g_lumi_0_corr->Write();
		g_lumi_891->Write();
		g_lumi_1785->Write();
		g_lumi_1900->Write();
		g_lumi_2050->Write();
		g_lumi_2250->Write();
		g_lumi_2450->Write();

		delete outF;
	}

	// get input data
	TFile *distF = new TFile((string("distributions_") + argv[1] + ".root").c_str());
	TGraph *g_timestamp_vs_ev_idx_sel = (TGraph *) distF->Get("metadata/g_timestamp_vs_ev_idx_sel");

	TFile *dataF = new TFile((string("distill_") + argv[1] + ".root").c_str());
	TTree *inT = (TTree *) dataF->Get("distilled");
	EventRed ev;
	
	inT->SetBranchAddress("timestamp", &ev.timestamp);
	inT->SetBranchAddress("run_num", &ev.run_num);
	inT->SetBranchAddress("event_num", &ev.event_num);
	inT->SetBranchAddress("trigger_num", &ev.trigger_num);
	
	// find periods
	vector<period> periods;
	for (int si = 0; si < g_timestamp_vs_ev_idx_sel->GetN(); si++) {

		// use the same event selection as in distributions.root
		unsigned int ev_idx = g_timestamp_vs_ev_idx_sel->GetX()[si];
		inT->GetEntry(ev_idx);

		unsigned int run = ev.run_num / 10000;

		// find an existing and compatible period
		bool found = false;
		for (unsigned int pi = 0; pi < periods.size(); pi++) {
			unsigned int margin = 1;
			if (periods[pi].run == run && ev.timestamp >= periods[pi].first - margin && ev.timestamp <= periods[pi].last + margin) {
				found = true;
				//printf("\tFOUND %u\n", pi);

				periods[pi].first = min(periods[pi].first, double(ev.timestamp));
				periods[pi].last = max(periods[pi].last, double(ev.timestamp));
				
				periods[pi].ev_first = min(periods[pi].ev_first, ev.event_num);
				periods[pi].ev_last = max(periods[pi].ev_last, ev.event_num);
				
				periods[pi].tr_first = min(periods[pi].tr_first, ev.trigger_num);
				periods[pi].tr_last = max(periods[pi].tr_last, ev.trigger_num);

				periods[pi].ev_total++;

				break;
			}
		}

		// add new period
		if (!found) {
			period p;
			p.first = ev.timestamp;
			p.last = ev.timestamp;
			
			p.ev_first = ev.event_num;
			p.ev_last = ev.event_num;
			
			p.tr_first = ev.trigger_num;
			p.tr_last = ev.trigger_num;

			p.ev_total = 1;
			p.run = run;

			periods.push_back(p);
		}
	}

	// prune unreasonable periods
	vector<period> selected_periods;
	for (unsigned int pi = 0; pi < periods.size(); pi++) {
		period &p = periods[pi];
		double duration = p.last - p.first;
		unsigned int events = p.ev_last - p.ev_first;

		bool keep = true;

		if (duration <= 1.) {
			printf("period %u: too little duration %.1f\n", pi, duration);
			keep = false;
		}
		
		if (events < 10) {
			printf("period %u: too little events %u\n", pi, events+1);
			keep = false;
		}
		
		if (keep)	
			selected_periods.push_back(p);
		else
			printf("\t=> removing period %u (from %.1f to %.1f, run %u)\n", pi, p.first, p.last, p.run);
	}

	// print and sum
	printf("\n");
	printf("per| run  | timestamp first, last : diff  | event cnt first, last : diff  | trigger cnt first, last : diff|dgn. evnts| DAQ eff  |L_int (mb^-1)|\n");
	
	double effLumiSum = 0.;
	double durationSum = 0.;
	double effWeightedSum = 0.;
	
	for (unsigned int pi = 0; pi < selected_periods.size(); pi++) {
		period &p = selected_periods[pi];

		double duration = p.last - p.first;
		unsigned int events = p.ev_last - p.ev_first;
		unsigned int triggers = p.tr_last - p.tr_first;

		p.daq_eff = double(events) / triggers;
		p.L_int = CalculateLumi(timestamp0 + p.first, timestamp0 + p.last) * 1E3;	// conversion to mb^-1

		printf("%3u| %u | %8.1f, %8.1f : %8.1f | %8u, %8u : %8u | %8u, %8u : %8u | %8u | %8.3f | %11.3E |\n",
			pi, p.run,
			p.first, p.last, duration,
			p.ev_first, p.ev_last, events,
			p.tr_first, p.tr_last, triggers,
			p.ev_total, p.daq_eff, p.L_int);
		
		effLumiSum += p.L_int * p.daq_eff;
		durationSum += duration;
		effWeightedSum += p.daq_eff * duration;
	}

	printf("\n");
	printf(">> total duration: %.0f s\n", durationSum);
	printf(">> mean DAQ effeciency: %.3f\n", effWeightedSum / durationSum);
	printf(">> effective luminosity sum: %.3E mb^-1\n", effLumiSum);
}
