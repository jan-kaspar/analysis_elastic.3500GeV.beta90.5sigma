void draw()
{
	TFile *f = new TFile("hb_cq7.root");
	TH1D *h = (TH1D *) f->Get("hb_cq7");

	h->Draw();

	TF1 *f_sig = new TF1("f_sig", "7461.28 * exp(-((x + 0.00128) / 0.88968)^2 / 2)", -5., +5.);
	TF1 *f_bckg = new TF1("f_bckg", "73.38 * exp(-((x + 0.03908) / 2.07787)^2 / 2)", -6., +6.);
	
	TF1 *f_both = new TF1("f_both", "7461.28 * exp(-((x + 0.00128) / 0.88968)^2 / 2) + 73.38 * exp(-((x + 0.03908) / 2.07787)^2 / 2)", -6., +6.);

	f_sig->Draw("same");
	f_bckg->Draw("same");
	
	f_both->SetLineColor(8);
	f_both->Draw("same");
}
