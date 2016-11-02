#include <vector>
#include <cmath>
#include <cstdio>

#include "common.h"

using namespace std;

//----------------------------------------------------------------------------------------------------

void EvalDerivative(Func f, const VarSet &c, const VarMat &em_p, const VarMat &em_m)
{
	double v = f(c);

	// calculate derivatives
	vector<double> der;
	for (unsigned int qi = 0; qi < N_q; qi++) {
		//printf("\tquantity %u\n", qi);

		// calculate derivative
		double mu = 1E-4;
		double eps = mu * (sqrt(em_p[qi][qi]) + sqrt(em_m[qi][qi])) / 2.;
		if (eps <= 0.)
			eps = 1E-5;

		//printf("\t\teps = %E\n", eps);

		VarSet pp = c;
		pp[qi] += eps;
		double vp = f(pp);
		double dp = (vp - v) / eps;
		//printf("\t\tder+ = %E\n", dp);
		
		VarSet pm = c;
		pm[qi] -= eps;
		double vm = f(pm);
		double dm = -(vm - v) / eps;
		//printf("\t\tder- = %E\n", dm);
		
		double d = (dp + dm) / 2.;

		if ((d != 0.) && (fabs(dp - dm) / fabs(d) > 0.01))
			printf("\tWARNING: variable %u, dp - dm > 1%%\n", qi);

		der.push_back(d);
	}

	// sum contributions
	double ess_p = 0., ess_m = 0.;

	for (unsigned int i = 0; i < N_q; i++) {
		for (unsigned int j = 0; j < N_q; j++) {
			ess_p += der[i]*der[j] * em_p[i][j];
			ess_m += der[i]*der[j] * em_m[i][j];

			//if (i == j)
			//.	printf("\t\t\t%i, err + = %.3f, err - = %.3f\n", i, sqrt(der[i]*der[j] * em_p[i][j])/v*100. , sqrt(der[i]*der[j] * em_m[i][j])/v*100.);
		}
	}

	double e_p = sqrt(ess_p);
	double e_m = sqrt(ess_m);

	printf("\terr + = %.3E   (%.3f %%)\n", e_p, e_p/v*100.);
	printf("\terr - = %.3E   (%.3f %%)\n", e_m, e_m/v*100.);
}

//----------------------------------------------------------------------------------------------------

void Eval(const string &label, Func f, const VarSet &c, const VarMat &em_p, const VarMat &em_m)
{
	printf("\n* %s\n", label.c_str());
	double v = f(c);
	printf("  value = %.3E\n", v);

	EvalDerivative(f, c, em_p, em_m);
}

//----------------------------------------------------------------------------------------------------

#include "formulae.h"

//----------------------------------------------------------------------------------------------------

void prop_deriv()
{
	VarSet v(N_q);
	v[irho] = 0.141;

	v[iB_j] = 20.1;
	v[iN_elo_j] = 25033.6;
	v[iI_j] = 8.291486E5;
	v[iL_j] = 1.;

	v[iN_el_o] = 25.43;
	v[iI_o] = 506.4;
	v[iN_inel_o] = 73.74;
	v[iN_inel_hm_o] = 70.53;
	v[iL_o] = 1.;

	// plus error matrix
	vector<vector<double> > ep(N_q, vector<double>(N_q, 0.));
	SetError(ep, irho, 0.007);

	SetError(ep, iB_j, 0.4);
	SetError(ep, iI_j, 0.01 * v[iI_j]);
	SetError(ep, iN_elo_j, 0.01 * v[iN_elo_j]);
	SetCorrelationCoefficient(ep, iI_j, iN_elo_j, 1.);
	SetError(ep, iL_j, 0.04);
	
	//SetError(ep, iI_o, 0.02 * v[iI_o]);					// old 2%
	//SetError(ep, iN_el_o, 0.02 * v[iN_el_o]);				// old 2%
	//SetCorrelationCoefficient(ep, iI_o, iN_el_o, 1.);		// old: full correlation
	SetError(ep, iI_o, 0.0204 * v[iI_o]);
	SetError(ep, iN_el_o, 0.0126 * v[iN_el_o]);
	SetCorrelationCoefficient(ep, iI_o, iN_el_o, 0.74);
	SetError(ep, iN_inel_o, 1.70);
	SetError(ep, iN_inel_hm_o, 0.70);
	SetError(ep, iL_o, 0.04);

	// minus error matrix
	VarMat em = ep;
	SetError(em, irho, 0.007);

	EvalAll(v, ep, em);
}

//----------------------------------------------------------------------------------------------------

int main()
{
	prop_deriv();
}
