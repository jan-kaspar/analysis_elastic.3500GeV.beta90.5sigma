void unfoldingAnalyze()
{
	TFile *f = new TFile("unfolding_45b_56t.root");
	TFile *fp = new TFile("unfolding_plus_45b_56t.root");
	TFile *fm = new TFile("unfolding_minus_45b_56t.root");

	TFile *fOut = new TFile("unfolding_comparison.root", "recreate");

	TH1D *h = (TH1D *) f->Get("cf,ub/corr_final");
	TH1D *hp = (TH1D *) fp->Get("cf,ub/corr_final");
	TH1D *hm = (TH1D *) fm->Get("cf,ub/corr_final");

	for (int i = 1; i <= h->GetNbinsX(); i++) {
		double v = h->GetBinContent(i), e = h->GetBinError(i);
		double vp = hp->GetBinContent(i), ep = hp->GetBinError(i);
		double vm = hm->GetBinContent(i), em = hm->GetBinError(i);

		hp->SetBinContent(i, vp - v);
		hm->SetBinContent(i, vm - v);

		hp->SetBinError(i, sqrt(e*e + ep*ep));
		hm->SetBinError(i, sqrt(e*e + em*em));
	}
	
	hp->SetName("hp");
	hm->SetName("hm");

	hp->SetLineColor(2);
	hm->SetLineColor(4);

	hp->Draw();
	hm->Draw("same");
	
	hp->Write();
	hm->Write();
}
