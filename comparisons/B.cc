void B_fits()
{
	TFile *f = new TFile("dataset_cmp.root");
	TH1D *h_avg = (TH1D *) f->Get("h_avg");

	TF1 *ff = new TF1("ff", "[0] * exp(-[1]*x)");

	double ep = 1E-4;

	printf("---------- global fit ----------\n");
	ff->SetParameters(1., 0.); h_avg->Fit(ff, "", "", 0.000-ep, 0.200+ep);
	
	printf("---------- fit 1 ----------\n");
	ff->SetParameters(1., 0.); h_avg->Fit(ff, "", "", 0.005-ep, 0.100+ep);
	
	printf("---------- fit 2 ----------\n");
	ff->SetParameters(1., 0.); h_avg->Fit(ff, "", "", 0.005-ep, 0.200+ep);
	
	printf("---------- fit 3 ----------\n");
	ff->SetParameters(1., 0.); h_avg->Fit(ff, "", "", 0.020-ep, 0.100+ep);
	
	printf("---------- fit 4 ----------\n");
	ff->SetParameters(1., 0.); h_avg->Fit(ff, "", "", 0.020-ep, 0.200+ep);
}
