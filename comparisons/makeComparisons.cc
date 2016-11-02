#include "TFile.h"
#include "TH1D.h"

void MakeComparisons(const string &ds)
{
	TFile *f1 = new TFile((string("../")+ds+"/normalize_45b_56t.root").c_str());
	TFile *f2 = new TFile((string("../")+ds+"/normalize_45t_56b.root").c_str());
	
	TH1D *h1 = (TH1D *) f1->Get("cross section/output");
	TH1D *h2 = (TH1D *) f2->Get("cross section/output");

	h2->Divide(h1);

	TFile *fOut = new TFile((string("diagonal_cmp_")+ds+".root").c_str(), "recreate");
	h2->Write();
	delete fOut;
}

//----------------------------------------------------------------------------------------------------

TH1D* MakeAverage(TH1D *h1, TH1D *h2, TH1D *h3)
{
	TH1D *ha = new TH1D(*h1);

	for (int i = 1; i <= ha->GetNbinsX(); i++) {
		double v1 = h1->GetBinContent(i), e1 = h1->GetBinError(i), c1 = h1->GetBinCenter(i);
		double v2 = h2->GetBinContent(i), e2 = h2->GetBinError(i), c2 = h2->GetBinCenter(i);
		double v3 = h3->GetBinContent(i), e3 = h3->GetBinError(i), c3 = h3->GetBinCenter(i);

		double w1 = (e1 > 0.) ? 1./e1/e1 : 0.;
		double w2 = (e2 > 0.) ? 1./e2/e2 : 0.;
		double w3 = (e3 > 0.) ? 1./e3/e3 : 0.;

		if (c1 < 0.0073001) w1 = 0.;
		if (c2 < 0.0057001) w2 = 0.;
		if (c3 < 0.0046001) w3 = 0.;

		double sw = w1+w2+w3;

		double v = (sw > 0.) ? (w1*v1 + w2*v2 + w3*v3) / sw : 0.;
		double e = (sw > 0.) ? 1. / sqrt(sw) : 0.;

		ha->SetBinContent(i, v);
		ha->SetBinError(i, e);

		//printf("t=%E, v1=%E, v2=%E, v=%E\n", h_t_45b->GetBinCenter(i), v1, v2, v);
	}	

	return ha;
}

//----------------------------------------------------------------------------------------------------

void makeComparisons()
{
	// ratio between diagonals
	vector<string> datasets;
	datasets.push_back("NN_2011_10_20_1");
	datasets.push_back("NN_2011_10_20_2");
	datasets.push_back("NN_2011_10_20_3");

	for (unsigned int i = 0; i < datasets.size(); i++)
		MakeComparisons(datasets[i]);

	// full average
	TFile *f1 = new TFile("/afs/cern.ch/exp/totem/scratch/jkaspar/software/offline/424/user/elastic_analysis/low_t/NN_2011_10_20_1/normalize_merged.root");
	TFile *f2 = new TFile("/afs/cern.ch/exp/totem/scratch/jkaspar/software/offline/424/user/elastic_analysis/low_t/NN_2011_10_20_2/normalize_merged.root");
	TFile *f3 = new TFile("/afs/cern.ch/exp/totem/scratch/jkaspar/software/offline/424/user/elastic_analysis/low_t/NN_2011_10_20_3/normalize_merged.root");

	TH1D *h1 = (TH1D *) f1->Get("cross section/output");
	TH1D *h2 = (TH1D *) f2->Get("cross section/output");
	TH1D *h3 = (TH1D *) f3->Get("cross section/output");
	
	TFile *f = new TFile("dataset_cmp.root", "recreate");

	TH1D *h_avg = MakeAverage(h1, h2, h3);
	h_avg->SetName("h_avg");
	h_avg->Write();

	// ratio between datasets
	h2->Divide(h1);
	h3->Divide(h1);

	h2->SetName("ds2_over_ds1");
	h2->Write();
	h3->SetName("ds3_over_ds1");
	h3->Write();

	delete f;
}
