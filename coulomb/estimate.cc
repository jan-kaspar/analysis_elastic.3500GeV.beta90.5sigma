#include "TFile.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TF1.h"

#include <vector>
#include <string>

void EstimateOne(TGraph *g_C, TGraphErrors *g_d)
{
	TF1 *ff = new TF1("ff", "[0] * exp(-[1]*x)");

	g_d->Fit(ff, "Q", "", 0., 0.2);
	double I0 = ff->GetParameter(0);
	double B0 = ff->GetParameter(1);

	g_d->Write("no corr");

	TGraph *g_mf = new TGraph();
	g_mf->SetName("g_mf");
	
	double *X = g_d->GetX();
	double *Y = g_d->GetY();
	for (unsigned int i = 0; i < g_d->GetN(); i++) {
		double C = g_C->Eval(X[i]);
		double mf = 1./(C + 1.);

		Y[i] *= mf;

		int idx = g_mf->GetN();
		g_mf->SetPoint(idx, X[i], mf);
	}

	g_mf->Write();
	
	g_d->Fit(ff, "Q", "", 0., 0.2);
	double I = ff->GetParameter(0);
	double B = ff->GetParameter(1);
	
	g_d->Write("corr");

	printf("\tintercept : no corr = %6.2f, with corr = %6.2f, diff = %6.2f, rel. diff = %6.2f \%\n", I0, I, I - I0, (I - I0) / I0 * 100.);
	printf("\tslope     : no corr = %6.2f, with corr = %6.2f, diff = %6.2f, rel. diff = %6.2f \%\n", B0, B, B - B0, (B - B0) / B0 * 100.);
}

//----------------------------------------------------------------------------------------------------

void estimate()
{
	vector<string> tags;
	tags.push_back("islam_bfkl");
	tags.push_back("islam_cgc");
	tags.push_back("ppp2");
	tags.push_back("ppp3");
	tags.push_back("bsw");
	tags.push_back("bh");
	tags.push_back("jenkovszky");

	TFile *cF = new TFile("3500GeV_0_20_4E3.details.root");
	TFile *dataF = new TFile("tab_this_pub.root");

	TFile *outF = new TFile("estimate.root", "recreate");

	for (unsigned int i = 0; i < tags.size(); i++) {
		printf("* %s\n", tags[i].c_str());

		TGraph *g_C = (TGraph *) cF->Get(("C/" + tags[i]).c_str());
		TGraphErrors *g_data = (TGraphErrors *) dataF->Get("g_stat_err");

		gDirectory = outF->mkdir(tags[i].c_str());
		EstimateOne(g_C, g_data);
	}
}
