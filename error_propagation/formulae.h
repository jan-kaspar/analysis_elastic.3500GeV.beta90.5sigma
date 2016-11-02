#include "common.h"


double K = 19.572;
double t0 = 0., t1 = 0.025, t2 = 0.3;

//----------------------------------------------------------------------------------------------------

double I_o_m2(const VarSet &v)
{
	return v[iI_o] / v[iL_o];
}	

//----------------------------------------------------------------------------------------------------

double si_el_o_m2(const VarSet &v)
{
	return v[iN_el_o] / v[iL_o];
}	

//----------------------------------------------------------------------------------------------------

double si_tot_o_m2(const VarSet &v)
{
	return sqrt(K / (1. + v[irho]*v[irho]) * v[iI_o] / v[iL_o]);
}	

//----------------------------------------------------------------------------------------------------

double si_inel_o_m2(const VarSet &v)
{
	return si_tot_o_m2(v) - si_el_o_m2(v);
}	

//----------------------------------------------------------------------------------------------------

double si_el_to_tot_m2(const VarSet &v)
{
	return v[iN_el_o] / si_tot_o_m2(v);
}	

//----------------------------------------------------------------------------------------------------

double si_tot_o_m3(const VarSet &v)
{
	return (v[iN_el_o] + v[iN_inel_o]) / v[iL_o];
}	

//----------------------------------------------------------------------------------------------------

double si_inel_o_m3(const VarSet &v)
{
	return v[iN_inel_o] / v[iL_o];
}	

//----------------------------------------------------------------------------------------------------

double si_el_o_m3(const VarSet &v)
{
	return v[iN_el_o] / v[iL_o];
}	

//----------------------------------------------------------------------------------------------------

double si_el_to_inel_m3(const VarSet &v)
{
	return v[iN_el_o] / v[iN_inel_o];
}	

//----------------------------------------------------------------------------------------------------

double si_el_to_tot_m3(const VarSet &v)
{
	return v[iN_el_o] / (v[iN_el_o] + v[iN_inel_o]);
}	

//----------------------------------------------------------------------------------------------------

double si_tot_o_m4(const VarSet &v)
{
	return K / (1. + v[irho]*v[irho]) * v[iI_o] / (v[iN_el_o] + v[iN_inel_o]);
}	

//----------------------------------------------------------------------------------------------------

double si_el_o(const VarSet &v)
{
	double si_tot = K / (1. + v[irho]*v[irho]) * v[iI_o] / (v[iN_el_o] + v[iN_inel_o]);
	return si_tot * v[iN_el_o] / (v[iN_el_o] + v[iN_inel_o]);
}

//----------------------------------------------------------------------------------------------------

double si_inel_o(const VarSet &v)
{
	double si_tot = K / (1. + v[irho]*v[irho]) * v[iI_o] / (v[iN_el_o] + v[iN_inel_o]);
	return si_tot * v[iN_inel_o] / (v[iN_el_o] + v[iN_inel_o]);
}

//----------------------------------------------------------------------------------------------------

double si_lmd_o(const VarSet &v)
{
	return si_tot_o_m2(v) - si_el_o_m2(v) - v[iN_inel_hm_o] / v[iL_o];
}

//----------------------------------------------------------------------------------------------------

double L_o(const VarSet &v)
{
	return (1. + v[irho]*v[irho]) / K * pow(v[iN_el_o] + v[iN_inel_o], 2.) / v[iI_o];
}

//----------------------------------------------------------------------------------------------------

double R_j(const VarSet &v)
{
	return (exp(-v[iB_j] * t0) - exp(-v[iB_j] * t1)) / (exp(-v[iB_j] * t1) - exp(-v[iB_j] * t2));
}

//----------------------------------------------------------------------------------------------------

double L1_j(const VarSet &v)
{
	double si_tot = K / (1. + v[irho]*v[irho]) * v[iI_o] / (v[iN_el_o] + v[iN_inel_o]);
	return K / (1. + v[irho]*v[irho]) * v[iI_j] / si_tot / si_tot;
}

//----------------------------------------------------------------------------------------------------

double L2_j(const VarSet &v)
{
	double R = (exp(-v[iB_j] * t0) - exp(-v[iB_j] * t1)) / (exp(-v[iB_j] * t1) - exp(-v[iB_j] * t2));
	double si_tot = K / (1. + v[irho]*v[irho]) * v[iI_o] / (v[iN_el_o] + v[iN_inel_o]);
	double si_el_o = si_tot * v[iN_el_o] / (v[iN_el_o] + v[iN_inel_o]);
	double N_el_j = v[iN_elo_j] * (1. + R);
	return N_el_j / si_el_o;
}

//----------------------------------------------------------------------------------------------------

double si_tot_o_avg(const VarSet &v)
{
	return (si_tot_o_m2(v) + si_tot_o_m3(v) + si_tot_o_m4(v)) / 3.;
}	

//----------------------------------------------------------------------------------------------------

double rho_sq(const VarSet &v)
{
	double rho_sq = K * v[iL_o] * v[iI_o] / pow(v[iN_el_o] + v[iN_inel_o], 2.) - 1.;
	return rho_sq;
}

//----------------------------------------------------------------------------------------------------

double rho(const VarSet &v)
{
	double rho_sq = K * v[iL_o] * v[iI_o] / pow(v[iN_el_o] + v[iN_inel_o], 2.) - 1.;
	return sqrt(rho_sq);
}

//----------------------------------------------------------------------------------------------------

void EvalAll(const VarSet &v, const VarMat &ep, const VarMat &em)
{
	// evaluations
	//Eval("I_Oct (m2)", I_o_m2, v, ep, em);
	//Eval("si_el_Oct (m2)", si_el_o_m2, v, ep, em);
	//Eval("si_tot_Oct (m2)", si_tot_o_m2, v, ep, em);
	//Eval("si_inel_Oct (m2)", si_inel_o_m2, v, ep, em);
	//Eval("si_el_to_tot_Oct (m2)", si_el_to_tot_m2, v, ep, em);

	//Eval("si_tot_Oct (m3)", si_tot_o_m3, v, ep, em);
	//Eval("si_inel_Oct (m3)", si_inel_o_m3, v, ep, em);
	//Eval("si_el_Oct (m3)", si_el_o_m3, v, ep, em);

	//Eval("si_el_Oct : si_inel_Oct (m3)", si_el_to_inel_m3, v, ep, em);
	//Eval("si_el_Oct : si_tot_Oct (m3)", si_el_to_tot_m3, v, ep, em);

	//Eval("si_tot_Oct (m4)", si_tot_o_m4, v, ep, em);
	//Eval("si_inel_Oct (m4)", si_inel_o, v, ep, em);
	Eval("si_el_Oct (m4)", si_el_o, v, ep, em);

/*
	Eval("L_Oct", L_o, v, ep, em);
	//Eval("R_Jun", R_j, v, ep, em);
	Eval("L_Jun (from intercept)", L1_j, v, ep, em);
	Eval("L_Jun (from si_el)", L2_j, v, ep, em);
	
	//Eval("si_tot_Oct avg", si_tot_o_avg, v, ep, em);
	
	Eval("rho^2", rho_sq, v, ep, em);
	//Eval("rho", rho, v, ep, em);
*/
	
	//Eval("si_lmd_o", si_lmd_o, v, ep, em);
}
