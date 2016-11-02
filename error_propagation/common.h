#ifndef __common_h__
#define __common_h__

#include <vector>
#include <string>
#include <cmath>

using namespace std;

//----------------------------------------------------------------------------------------------------

enum {irho,   iB_j, iI_j, iN_elo_j, iL_j,   iI_o, iN_el_o, iN_inel_o, iN_inel_hm_o, iL_o};
unsigned int N_q = 10;

//----------------------------------------------------------------------------------------------------

string GetQuantityName(unsigned int idx)
{
	if (idx == 0) return "rho";
	if (idx == 1) return "B_j";
	if (idx == 2) return "I_j";
	if (idx == 3) return "N_elo_j";
	if (idx == 4) return "L_j";
	if (idx == 5) return "I_o";
	if (idx == 6) return "N_el_o";
	if (idx == 7) return "N_inel_o";
	if (idx == 8) return "N_inel_hm_o";
	if (idx == 9) return "L_o";

	return "??";
}

//----------------------------------------------------------------------------------------------------

typedef vector<double> VarSet;
typedef vector< vector<double> > VarMat;

typedef double (*Func)(const VarSet &);

//----------------------------------------------------------------------------------------------------

void SetError(VarMat &m, unsigned int idx, double v)
{
	m[idx][idx] = v*v;
}

//----------------------------------------------------------------------------------------------------

void SetCorrelationCoefficient(VarMat &m, unsigned int idx1, unsigned int idx2, double coef)
{
	double v =  coef * sqrt(m[idx1][idx1] * m[idx2][idx2]);
	m[idx1][idx2] = v;
	m[idx2][idx1] = v;
}

#endif
