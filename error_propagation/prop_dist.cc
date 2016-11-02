#include <TMatrixD.h>
#include <TVectorD.h>
#include <TDecompChol.h>
#include <TRandom2.h>

#include <TFile.h>
#include <TH1D.h>

#include <vector>
#include <cmath>

#include "common.h"

using namespace std;

double _rho_sq(const VarSet &v)
{
	double K = 19.572;
	double rho_sq = K * v[iL_o] * v[iI_o] / pow(v[iN_el_o] + v[iN_inel_o], 2.) - 1.;
	return rho_sq;
}

//----------------------------------------------------------------------------------------------------

void GetCentralConfidenceInterval(double level, TH1D *h_f, double &f_min, double &f_plu)
{
	double al = (1. - level)/2.;

	double N = h_f->GetEntries();

	// f minus
	double S_min = 0.;
	for (unsigned int i = 1; i < h_f->GetNbinsX(); i++) {
		double v = h_f->GetBinContent(i) / N;
		S_min += v;
		
		//printf("%i => %E\n", i, S_min);

		if (S_min > al) {
			if (i > 1)
				f_min = (h_f->GetBinCenter(i) + h_f->GetBinCenter(i-1)) / 2.;
			else
				f_min = h_f->GetBinCenter(i);
			break;
		}
	}

	// f plus
	double S_plu = 0.;
	for (unsigned int i = h_f->GetNbinsX(); i > 0; i--) {
		double v = h_f->GetBinContent(i) / N;
		S_plu += v;
		if (S_plu > al) {
			if (i < h_f->GetNbinsX())
				f_plu = (h_f->GetBinCenter(i) + h_f->GetBinCenter(i-1)) / 2.;
			else
				f_plu = h_f->GetBinCenter(i);
			break;
		}
	}
}

//----------------------------------------------------------------------------------------------------

void GetLeftConfidenceInterval(double level, TH1D *h_f, double &f)
{
	double N = h_f->GetEntries();

	// f minus
	double S = 0.;
	for (unsigned int i = 1; i < h_f->GetNbinsX(); i++) {
		double v = h_f->GetBinContent(i) / N;
		S += v;
		
		if (S > level) {
			if (i > 1)
				f = (h_f->GetBinCenter(i) + h_f->GetBinCenter(i-1)) / 2.;
			else
				f = h_f->GetBinCenter(i);
			break;
		}
	}

}

//----------------------------------------------------------------------------------------------------

double FRhoInv(double p)
{
	double L = -0.08, C = 0., R = 0.01;
	double h = 2./(R-L);

	double rho;
	if (p <= h/2.*(C-L))
		rho = L + sqrt(2.*p * (C-L)/h);
	else
		rho = R - sqrt((1. - p)*2.*(R-C)/h);

	//printf("p=%.3f, rho=%.3f\n", p, rho);
	return rho;
}

//----------------------------------------------------------------------------------------------------

VarSet cv(N_q);

TMatrixD gen_mat_j(2, 2), gen_mat_o(2, 2);

void InitGenerator()
{
	// central values
	cv[irho] = 0.141;

	cv[iB_j] = 20.1;
	cv[iN_elo_j] = 25033.6;
	cv[iI_j] = 8.291486E5;
	cv[iL_j] = 1.;

	cv[iN_el_o] = 25.43;
	cv[iI_o] = 506.4;
	cv[iN_inel_o] = 73.74;
	cv[iN_inel_hm_o] = 70.53;
	cv[iL_o] = 1.;

	TMatrixD el_err_mat_j(2, 2), el_err_mat_o(2, 2);
	
	// 0: I, 1: N_elo; June
	el_err_mat_j(0, 0) = pow(0.01 * cv[iI_j], 2.);
	el_err_mat_j(1, 1) = pow(0.01 * cv[iN_elo_j], 2.);
	el_err_mat_j(0, 1) = el_err_mat_j(1, 0) = 1. * sqrt(el_err_mat_j(0, 0) * el_err_mat_j(1, 1));

	TDecompChol decomp_j(el_err_mat_j);
	decomp_j.Decompose();
	TMatrixD U_j = decomp_j.GetU();
	TMatrixD V_j(TMatrixD::kTransposed, U_j);
	gen_mat_j = V_j;

	// 0: I, 1: N_el; October
	el_err_mat_o(0, 0) = pow(0.017 * cv[iI_o], 2.);
	el_err_mat_o(1, 1) = pow(0.004 * cv[iN_el_o], 2.);
	el_err_mat_o(0, 1) = el_err_mat_o(1, 0) = 0.71 * sqrt(el_err_mat_o(0, 0) * el_err_mat_o(1, 1));
	
	TDecompChol decomp_o(el_err_mat_o);
	decomp_o.Decompose();
	TMatrixD U_o = decomp_o.GetU();
	TMatrixD V_o(TMatrixD::kTransposed, U_o);
	gen_mat_o = V_o;
}

//----------------------------------------------------------------------------------------------------

void GenerateSet(VarSet &v, unsigned int group)
{
	// central values
	v = cv;

	// generate errors

	if (group == 0 || group == 1) {
		//v[irho] += FRhoInv(gRandom->Rndm());
		v[irho] += gRandom->Gaus() * 0.007;
	}
	
	if (group == 0 || group == 2) {
		TVectorD x(2);
		x(0) = gRandom->Gaus();
		x(1) = gRandom->Gaus();
		TVectorD y(gen_mat_j * x);
		v[iI_j] += y(0);
		v[iN_elo_j] += y(1);
	}
	
	if (group == 0 || group == 3)
		v[iB_j] += gRandom->Gaus() * 0.4;
	
	if (group == 0 || group == 4)
		v[iL_j] += gRandom->Gaus() * 0.04;
	
	if (group == 0 || group == 5 || group == 101) {
		TVectorD x(2);
		x(0) = gRandom->Gaus();
		x(1) = gRandom->Gaus();
		TVectorD y(gen_mat_o * x);
		v[iI_o] += y(0);
		v[iN_el_o] += y(1);
	}
	
	if (group == 0 || group == 6 || group == 101) {
		double nf = gRandom->Gaus() * 0.012;
		v[iN_el_o] += cv[iN_el_o] * nf;
		v[iI_o] += cv[iI_o] * nf;
	}

	if (group == 0 || group == 7)
		v[iN_inel_o] += gRandom->Gaus() * 1.70;
	
	if (group == 0 || group == 8)
		v[iL_o] += gRandom->Gaus() * 0.04;
	
	if (group == 0 || group == 9)
		v[iN_inel_hm_o] += gRandom->Gaus() * 0.70;
}

//----------------------------------------------------------------------------------------------------

string GetGroupName(unsigned int group)
{
	if (group == 1) return "rho";
	if (group == 2) return "el:Jun";
	if (group == 3) return "B:Jun";
	if (group == 4) return "lumi:Jun";
	if (group == 5) return "el t-dep:Oct";
	if (group == 6) return "el norm:Oct";
	if (group == 7) return "inel:Oct";
	if (group == 8) return "lumi:Oct";
	if (group == 9) return "inel HM:Oct";
	if (group == 101) return "el:Oct";

	return "??";
}

//----------------------------------------------------------------------------------------------------

bool statPrinted = false;

void EvalDist(Func f, unsigned int group)
{
	TDirectory *baseDir = gDirectory;

	char buf[100];
	if (group == 0)
		sprintf(buf, "total");
	else
		sprintf(buf, "group %s", GetGroupName(group).c_str());
	gDirectory = gDirectory->mkdir(buf);

	printf("  %s\n", buf);

	vector<TH1D *> h_q;
	for (unsigned int i = 0; i < N_q; i++) {
		sprintf(buf, "%s", GetQuantityName(i).c_str());
		TH1D *h = new TH1D(buf, "", 500, 0., 0.);
		h_q.push_back(h);
	}

	TH1D *h_f = new TH1D("h_f", ";f", 500, 0., 0.);

	double S1 = 0., Sw = 0., Swf = 0., Swff = 0.;

	vector<double> Sv(N_q, 0.);
	vector<vector<double> > Sm(N_q, vector<double>(N_q, 0.));

	unsigned long N_ev = 1000000;
	for (unsigned long n_ev = 0; n_ev < N_ev; n_ev++) {
		// generate random variable set
		VarSet p(N_q);
		GenerateSet(p, group);
		for (unsigned int i = 0; i < N_q; i++)
			h_q[i]->Fill(p[i]);
		
		double vp = f(p);

		/*
		double rho2 = _rho_sq(p);
		if (rho2 < 0.)
			continue;
		double w = 1. / sqrt(rho2);
		*/
		double w = 1.;

		h_f->Fill(vp, w);

		S1 += 1.;
		Sw += w;
		Swf += w*vp;
		Swff += w*vp*vp;

		for (unsigned int i = 0; i < N_q; i++) {
			Sv[i] += p[i];
			for (unsigned int j = 0; j < N_q; j++)
				Sm[i][j] += p[i]*p[j]; 
		}
	}

	for (unsigned int i = 0; i < N_q; i++)
		h_q[i]->Write();

	h_f->Write();

	if (!statPrinted && group == 0) {
		for (unsigned int i = 0; i < N_q; i++) {
			double stddev = sqrt(Sm[i][i]/ S1 - Sv[i]/S1*Sv[i]/S1);
			printf("    stddev[%u] = %.1E (%s)\n", i, stddev, GetQuantityName(i).c_str());
			for (unsigned int j = i+1; j < N_q; j++) {
				if (j != iI_j && j != iN_elo_j && j != iI_o && j != iN_el_o)
					continue;
				double vi = sqrt(Sm[i][i]/S1 - Sv[i]/S1*Sv[i]/S1);
				double vj = sqrt(Sm[j][j]/S1 - Sv[j]/S1*Sv[j]/S1);
				double coef = (Sm[i][j]/S1 - Sv[i]/S1*Sv[j]/S1) / vi / vj;
				printf("    corr. coef[%u, %u] = %.1E\n", i, j, coef);
			}
		}

		statPrinted = true;
	}

	double mean = Swf/Sw;
	double var = Swff/Sw - mean*mean;
	double stddev = sqrt(var);

	printf("      mean      = %.3E\n", mean);
	printf("      std.dev.  = %.3E  (%.2f %%)\n", stddev, stddev/mean*100.);
	//printf("      std.dev.  = %.3E\n", stddev);
	
	printf("      from histogram: mean = %.3E, RMS = %.3E\n", h_f->GetMean(), h_f->GetRMS());

	double f_min = 0., f_plu = 0.;
	GetCentralConfidenceInterval(0.683, h_f, f_min, f_plu);
	printf("      conf.int. from %.3E to %.3E\n", f_min, f_plu);
	printf("      relative conf.int: %+.3E (%+.2f %%), %+.3E (%+.2f %%)\n",
		f_plu - mean, (f_plu - mean) / mean * 100., f_min - mean, (f_min - mean) / mean * 100.);


	double f_left = 0.;
	GetLeftConfidenceInterval(0.95, h_f, f_left);
	printf("      top limit: %.3E\n", f_left);

	/*
	double v4 = sqrt(S44/ S1 - S4/S1*S4/S1);
	double v5 = sqrt(S55/ S1 - S5/S1*S5/S1);
	double c45 = S45/ S1 - S4/S1*S5/S1;
	//printf("      v4 = %E, v5 = %E, r45 = %E\n", v4, v5, c45/v4/v5);
	*/
	
	gDirectory = baseDir;
}
	
//----------------------------------------------------------------------------------------------------

void Eval(const string &label, Func f, const VarSet &c, const VarMat &em_p, const VarMat &em_m)
{
	TDirectory *baseDir = gDirectory;
	gDirectory = gDirectory->mkdir(label.c_str());

	printf("\n* %s\n", label.c_str());
	EvalDist(f, 0);
	//EvalDist(f, 101);
	EvalDist(f, 7);
	EvalDist(f, 8);
	EvalDist(f, 1);
	EvalDist(f, 0);
	
	gDirectory = baseDir;
}

//----------------------------------------------------------------------------------------------------

#include "formulae.h"

//----------------------------------------------------------------------------------------------------

void prop_dist()
{
	TFile *outF = new TFile("prop_dist.root", "recreate");
	
	gRandom->SetSeed(2);
	InitGenerator();

	VarSet v(N_q);
	VarMat ep, em;
	EvalAll(v, ep, em);

	delete outF;
}

//----------------------------------------------------------------------------------------------------

int main()
{
	prop_dist();
}
