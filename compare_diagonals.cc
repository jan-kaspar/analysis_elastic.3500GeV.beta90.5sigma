#include "TFile.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TGraph.h"

#include <vector>
#include <map>
#include <cmath>

using namespace std;

//----------------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{
	if (argc != 2)
		return 1;

	if (strcmp(argv[1], "45t_56b") == 0) 
		return 0;

	vector<string> diagonals;
	diagonals.push_back("45b_56t");
	diagonals.push_back("45t_56b");

	vector<TFile *> files;
	for (unsigned int i = 0; i < diagonals.size(); i++)
		files.push_back(new TFile((string("distributions_" + diagonals[i] + ".root").c_str())));

	map<string, string> list;
	list["th_x"] = "acceptance correction/h_th_x_after";
	list["th_y"] = "acceptance correction/h_th_y_after";
	list["th_y_before"] = "acceptance correction/h_th_y_before";
	list["t"] = "acceptance correction/h_t_eb_after";
	list["timestamp_dgn"] = "metadata/h_timestamp_dgn";
	list["timestamp_sel"] = "metadata/h_timestamp_sel";

	TFile *outF = new TFile("compare_diagonals.root", "recreate");

	int colors[2] = {2, 4};

	for (map<string, string>::iterator it = list.begin(); it != list.end(); ++it) {
		TCanvas *c = new TCanvas(it->first.c_str());
		c->SetLogy(1);
		vector<TH1D *> hists;
		for (unsigned int d = 0; d < diagonals.size(); d++) {
			string obj = it->second;
			if (it->first.compare("th_y") == 0 && d == 1)
				obj += "_flipped";
			if (it->first.compare("th_y_before") == 0 && d == 1)
				obj += "_flipped";
			TH1D *h = (TH1D *) files[d]->Get(obj.c_str());
			h->Sumw2();
			hists.push_back(h);
			h->SetName(diagonals[d].c_str());
			h->SetLineColor(colors[d]);
			h->Draw((d == 0) ? "" : "sames");
		}
		c->Write();

		TH1D *h_ratio = new TH1D(*hists[0]);
		h_ratio->SetName((it->first + ":ratio").c_str());
		h_ratio->Divide(hists[1]);
		h_ratio->SetLineColor(1);
		h_ratio->Write();
	}

	// th_y_L vs. th_y_R from both diagonals
	TH2D *h1 = (TH2D *) files[0]->Get("selected - angles/h_th_y_L_vs_th_y_R");
	TH2D *h2 = (TH2D *) files[1]->Get("selected - angles/h_th_y_L_vs_th_y_R");

	TH2D *h12 = new TH2D(*h1);
	for (unsigned int ix = 1; ix <= h12->GetNbinsX(); ix++) {
		for (unsigned int iy = 1; iy <= h12->GetNbinsX(); iy++) {
			double v1 = h1->GetBinContent(ix, iy);
			double v2 = h2->GetBinContent(ix, iy);
			h12->SetBinContent(ix, iy, max(v1, v2));
		}
	}
	h12->Write();
	
	TGraph *g1 = (TGraph *) files[0]->Get("selected - angles/g_th_y_L_vs_th_y_R");
	TGraph *g2 = (TGraph *) files[1]->Get("selected - angles/g_th_y_L_vs_th_y_R");
	
	TGraph *g12cut = new TGraph(*g1);
	g12cut->Set(0);
	
	for (unsigned int i = 0; i < g1->GetN(); i++) {
		double x, y;
		g1->GetPoint(i, x, y);
		double th = (x+y)/2.;
		if (fabs(th) > 30E-6 && fabs(th) < 105E-6)
			g12cut->SetPoint(g12cut->GetN(), x, y);
	}

	for (unsigned int i = 0; i < g2->GetN(); i++) {
		double x, y;
		g2->GetPoint(i, x, y);
		double th = (x+y)/2.;
		if (fabs(th) > 30E-6 && fabs(th) < 105E-6)
			g12cut->SetPoint(g12cut->GetN(), x, y);
	}
	g12cut->Fit("pol1");
	g12cut->Write();

	delete outF;
	return 0;
}
