void rho_interpolation()
{
	double sqrt_s[] = { 100, 200, 300, 400, 500, 600, 1960, 10000, 12000, 14000 };
	double rho[] = { 0.1058, 0.1275, 0.1352, 0.1391, 0.1413, 0.1416, 0.1450, 0.1382, 0.1371, 0.1361 };
	double stat_e[] = { 0.0012, 0.0014, 0.0016, 0.0017, 0.0017, 0.0018, 0.0018, 0.0016, 0.0015, 0.0015 };
	double syst_ep[] = { 0.0040, 0.0051, 0.0055, 0.0056, 0.0057, 0.0058, 0.0057, 0.0047, 0.0046, 0.0058 };
	double syst_em[] = { 0.0021, 0.0026, 0.0028, 0.0030, 0.0030, 0.0031, 0.0030, 0.0027, 0.0026, 0.0025 };

	TGraph *g_rho = new TGraph(10, sqrt_s, rho); g_rho->SetName("rho");
	TGraph *g_stat_e = new TGraph(10, sqrt_s, stat_e); g_stat_e->SetName("stat_e");
	TGraph *g_syst_ep = new TGraph(10, sqrt_s, syst_ep); g_syst_ep->SetName("syst_ep");
	TGraph *g_syst_em = new TGraph(10, sqrt_s, syst_em); g_syst_em->SetName("syst_em");

	new TCanvas();
	g_rho->Draw("alp");
	
	new TCanvas();
	g_stat_e->Draw("alp");
	g_syst_ep->Draw("lp");
	g_syst_em->Draw("lp");

	double W = 7000.;

	printf("rho = %.4f\n", g_rho->Eval(W));
	printf("stat_e = %.4f\n", g_stat_e->Eval(W));
	printf("syst_ep = %.4f\n", g_syst_ep->Eval(W));
	printf("syst_em = %.4f\n", g_syst_em->Eval(W));
}
