#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TRandom2.h"
#include "TGraph.h"
#include "TF1.h"

#include <vector>

using namespace std;

vector<double> ts, si_mi, si_ox, si_oy, si_eo, si_ac, si_uc;

void AddDataPoint(double _ts, double _si_mi, double _si_ox, double _si_oy, double _si_ac, double _si_eo, double _si_uc)
{
	ts.push_back(_ts);
	si_mi.push_back(_si_mi * sqrt(2.));
	si_ox.push_back(_si_ox * sqrt(2.));
	si_oy.push_back(_si_oy * sqrt(2.));
	si_eo.push_back(_si_eo);
	si_ac.push_back(_si_ac);
	si_uc.push_back(_si_uc);
}

//----------------------------------------------------------------------------------------------------

void extra_integ()
{
	gRandom->SetSeed(1);

	AddDataPoint(0.007, 0.05, -0.60, -0.20, 1.50, -0.18, -0.03);
	AddDataPoint(0.01, 0.05, -0.60, -0.20, 0.50, -0.18, -0.03);
	AddDataPoint(0.06, 0.13, -0.05, +0.09, 0.08, 0.04, 0.00);
	AddDataPoint(0.10, 0.15, 0.50, 0.27, 0.05, 0.20, 0.02);
	AddDataPoint(0.12, 0.16, 0.70, 0.30, 0.35, 0.28, 0.05);
	AddDataPoint(0.16, 0.17, 2.04, 0.23, 0.18, 0.44, 0.10);
	AddDataPoint(0.20, 0.17, 3.10, 0.20, 0.16, 0.60, 0.16);
	AddDataPoint(0.30, 0.20, 5.80, 0.20, 0.15, 0.99, 0.31);
	AddDataPoint(0.40, 0.24, 8.60, 0.20, 0.15, 1.37, 0.47);

	double t_zero = 0., t_min = 0.005, t_max = 0.4;
	vector<double> le, he;
	le.push_back(t_min); he.push_back(0.008);
	le.push_back(0.008); he.push_back(0.03);
	le.push_back(0.03); he.push_back(0.08);
	le.push_back(0.08); he.push_back(0.13);
	le.push_back(0.13); he.push_back(0.14);
	le.push_back(0.14); he.push_back(0.18);
	le.push_back(0.18); he.push_back(0.25);
	le.push_back(0.25); he.push_back(0.35);
	le.push_back(0.35); he.push_back(0.40);

	char buf[500];

	TFile *outF = new TFile("extra_integ.root", "recreate");

	vector<TH1D *> h_point;

	for (unsigned int i = 0; i < ts.size(); i++) {
		sprintf(buf, "h_%.3f", ts[i]);
		TH1D *h = new TH1D(buf, "", 100, 0., 0.);
		h_point.push_back(h);
	}

	TH1D *h_mi = new TH1D("h_mi", "", 100, 0., 0.);
	TH1D *h_ox = new TH1D("h_ox", "", 100, 0., 0.);
	TH1D *h_oy = new TH1D("h_oy", "", 100, 0., 0.);
	TH1D *h_eo = new TH1D("h_eo", "", 100, 0., 0.);
	TH1D *h_ac = new TH1D("h_ac", "", 100, 0., 0.);
	TH1D *h_uc = new TH1D("h_uc", "", 100, 0., 0.);
	
	TH1D *h_a = new TH1D("h_a", "", 100, 0., 0.);
	TH1D *h_b = new TH1D("h_b", "", 100, 0., 0.);
	
	TH1D *h_b1 = new TH1D("h_b1", "", 100, 0., 0.);
	TH1D *h_b2 = new TH1D("h_b2", "", 100, 0., 0.);
	TH1D *h_b3 = new TH1D("h_b3", "", 100, 0., 0.);
	TH1D *h_b4 = new TH1D("h_b4", "", 100, 0., 0.);
	
	TH1D *h_si_el_obs = new TH1D("h_si_el_obs", "", 100, 0., 0.);
	TH1D *h_si_el_ext = new TH1D("h_si_el_ext", "", 100, 0., 0.);
	TH1D *h_si_el_tot = new TH1D("h_si_el_tot", "", 100, 0., 0.);

	TH2D *h_de_I_vs_de_si_el = new TH2D("h_de_I_vs_de_si_el", ";relative error of #sigma_{el};relative error of d#sigma_{el}/dt", 50, 0., 0., 50, 0., 0.);

	vector<double> errs;
	errs.resize(ts.size());

	TGraph *g_err = new TGraph();
	g_err->SetName("g_err");
	g_err->SetTitle(";|t|   (GeV^2);multiplicative error factor in d#sigma/dt");

	TF1 *ff = new TF1("ff", "[0] * exp([1]*x)");

	double S1=0., Si=0., Ss=0., Sii=0., Sis=0., Sss=0.;

	unsigned long N_ev = 1E5;
	for (unsigned long n_ev = 0; n_ev < N_ev; ++n_ev) {
		// generate random error factors
		double f_mi = gRandom->Gaus();
		double f_ox = gRandom->Gaus();
		double f_oy = gRandom->Gaus();
		double f_eo = gRandom->Gaus();
		double f_ac = gRandom->Gaus();
		double f_uc = gRandom->Gaus();

		h_mi->Fill(f_mi);
        h_ox->Fill(f_ox);
        h_oy->Fill(f_oy);
        h_eo->Fill(f_eo);
        h_ac->Fill(f_ac);
        h_uc->Fill(f_uc);

		//printf("** %E\n", f_ox);

		// generate point errors
		for (unsigned int i = 0; i < ts.size(); i++) {
			errs[i] = (100.
				+ f_mi * si_mi[i]
				+ f_ox * si_ox[i]
				+ f_oy * si_oy[i]
				+ f_eo * si_eo[i]
				+ f_ac * si_ac[i]
				+ f_uc * si_uc[i]) / 100.;
		}

		// fill point histograms
		for (unsigned int i = 0; i < ts.size(); i++) {
			h_point[i]->Fill(errs[i]);
		}

		// do extrapolation
		g_err->Set(0);
		for (unsigned int i = 0; i < ts.size(); i++) {
			if (ts[i] > 0.21)
				break;
			g_err->SetPoint(g_err->GetN(), ts[i], errs[i]);
		}

		ff->SetParameters(1, 0);
		g_err->Fit(ff, "Q");

		if (n_ev < 10)
			g_err->Write();

		double a = ff->GetParameter(0);
		double b = ff->GetParameter(1);
		
		//printf("* %.1E, %.1E\n", a, b);

		h_a->Fill(a);
		h_b->Fill(b);

		// B fits over different ranges
		double ep = 1E-4;

		ff->SetParameters(1, 0); g_err->Fit(ff, "Q", "", 0.005-ep, 0.100+ep); h_b1->Fill(ff->GetParameter(1));
		ff->SetParameters(1, 0); g_err->Fit(ff, "Q", "", 0.005-ep, 0.200+ep); h_b2->Fill(ff->GetParameter(1));
		ff->SetParameters(1, 0); g_err->Fit(ff, "Q", "", 0.020-ep, 0.100+ep); h_b3->Fill(ff->GetParameter(1));
		ff->SetParameters(1, 0); g_err->Fit(ff, "Q", "", 0.020-ep, 0.200+ep); h_b4->Fill(ff->GetParameter(1));

		// calculate observed and extrapolated si_el
		double A = 1., B = 20.;
		double I_obs_true = A / B * (exp(-B*t_min) - exp(-B*t_max));
		double I_ext_true = A / B * (exp(-B*t_zero) - exp(-B*t_min));
		double I_tot_true = I_obs_true + I_ext_true;
		
		if (n_ev < 10)
			printf("\n");
		
		double I_obs_exp = 0.;
		for (unsigned int i = 0; i < ts.size(); i++) {
			double f = A / B * (exp(-B*le[i]) - exp(-B*he[i]));
			I_obs_exp += f * errs[i];

			if (n_ev < 10)
				printf("** %u, %f, %f, %f\n", i, f, errs[i], f*errs[i]);
		}

		A *= a; B += b;
		double I_ext_exp = A / B * (exp(-B*t_zero) - exp(-B*t_min));
		double I_tot_exp = I_obs_exp + I_ext_exp;

		h_si_el_obs->Fill(I_obs_exp/I_obs_true - 1.);
		h_si_el_ext->Fill(I_ext_exp/I_ext_true - 1.);
		h_si_el_tot->Fill(I_tot_exp/I_tot_true - 1.);

		h_de_I_vs_de_si_el->Fill(I_tot_exp/I_tot_true - 1., a - 1.);

		double dr_i = a - 1.;						// relative error of intercept
		double dr_s = I_tot_exp/I_tot_true - 1.;	// relative error of integral

		double ne = gRandom->Gaus() * 0.012;		// normalization error
		dr_i += ne;
		dr_s += ne;

		S1 += 1.;
		Si += dr_i;
		Ss += dr_s;
		Sii += dr_i * dr_i;
		Sis += dr_i * dr_s;
		Sss += dr_s * dr_s;
	}	

	// write
	h_mi->Write();
    h_ox->Write();
    h_oy->Write();
    h_eo->Write();
    h_ac->Write();
    h_uc->Write();

	for (unsigned int i = 0; i < ts.size(); i++)
		h_point[i]->Write();
	
	h_a->Write();
	h_b->Write();
	
	h_b1->Write();
	h_b2->Write();
	h_b3->Write();
	h_b4->Write();
	
	h_si_el_obs->Write();
	h_si_el_ext->Write();
	h_si_el_tot->Write();

	h_de_I_vs_de_si_el->Write();

	double Vi = (Sii - Si*Si/S1) / S1;
	double Vs = (Sss - Ss*Ss/S1) / S1;
	double Cis = (Sis - Ss*Si/S1) / S1;
	double rh_is = Cis / sqrt(Vi * Vs);

	printf("si_intercept = %.2E\n", sqrt(Vi));
	printf("si_integral = %.2E\n", sqrt(Vs));
	printf("integral-intercept correlation = %.2E\n", rh_is);

	delete outF;
}
