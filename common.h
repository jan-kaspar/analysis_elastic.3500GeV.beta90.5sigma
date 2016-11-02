#include <string>
#include <cmath>

#include "TGraph.h"
#include "TMath.h"

double timestamp0 = 1319061600;

double d_RP = 5372.;	// in mm

double bckg_correction = 0.992;

//----------------------------------------------------------------------------------------------------

struct EventRed {
	unsigned int v_L_F, v_L_N, v_R_F, v_R_N;
	double x_L_F, x_L_N, x_R_F, x_R_N;
	double y_L_F, y_L_N, y_R_F, y_R_N;
	unsigned int timestamp;
	unsigned int run_num, bunch_num, event_num, trigger_num;
	unsigned int trigger_bits;
};


typedef double (*EventFcn)(const EventRed &);

//----------------------------------------------------------------------------------------------------

EventRed GetEvent(int i, TGraph *g_L_N, TGraph *g_L_F, TGraph *g_R_N, TGraph *g_R_F)
{
	EventRed ev;
	g_L_N->GetPoint(i, ev.x_L_N, ev.y_L_N);
	g_L_F->GetPoint(i, ev.x_L_F, ev.y_L_F);
	g_R_N->GetPoint(i, ev.x_R_N, ev.y_R_N);
	g_R_F->GetPoint(i, ev.x_R_F, ev.y_R_F);
	return ev;
}

//----------------------------------------------------------------------------------------------------

enum DiagonalType {dUnknown, d45b, d45t};
DiagonalType diagonal = dUnknown;

void Init(const std::string &dgnStr)
{
	diagonal = dUnknown;

	if (dgnStr.compare("45b_56t") == 0) {
		Init_45b_56t();
		diagonal = d45b;
	}

	if (dgnStr.compare("45t_56b") == 0) {
		Init_45t_56b();
		diagonal = d45t;
	}

	if (diagonal == dUnknown) {
		printf("Init > bad diagonal %s\n", dgnStr.c_str());
	}
}

//----------------------------------------------------------------------------------------------------

double* BuildBinning(bool exponential, unsigned int N_bins, unsigned int N_bins_low, unsigned int N_bins_high)
{
	unsigned int N_bins_full = N_bins + N_bins_low + N_bins_high;
	double *binEdges = new double[N_bins_full+1];

	// between t_min_full and t_min
	double bs = (N_bins_low == 0) ? 0. : (t_min - t_min_full) / N_bins_low;
	for (unsigned int i = 0; i < N_bins_low; i++)
		binEdges[i] = t_min_full + bs * i;

	// between t_min and t_max
	if (exponential) {
		binEdges[N_bins_low + 0] = 0E-3;
		binEdges[N_bins_low + 1] = 1.5E-3;
		binEdges[N_bins_low + 2] = 3E-3;
		binEdges[N_bins_low + 3] = 4.6E-3;
		binEdges[N_bins_low + 4] = 5.7E-3;

		unsigned int N_bins_eff = N_bins - 5;
		double t_min_eff = 7.3E-3;

		double B = 6.;
		for (unsigned int bi = 0; bi < N_bins; bi++)
			binEdges[N_bins_low + bi + 5] = - log( (1. - double(bi) / N_bins_eff) * exp(-B*t_min_eff) + double(bi) * exp(-B*t_max) / N_bins_eff ) / B;
	} else {
		bs = (t_max - t_min) / N_bins;
		for (unsigned int i = 0; i < N_bins; i++)
			binEdges[i+N_bins_low] = t_min + bs * i;
	}

	// between t_max and t_max_full
	bs = (N_bins_high == 0) ? 0. : (t_max_full - t_max) / N_bins_high;
	for (unsigned int i = 0; i <= N_bins_high; i++)
		binEdges[i+N_bins_low+N_bins] = t_max + bs * i;

	// print
	for (unsigned int i = 0; i <= N_bins_full; i++) {
		//printf("\tbi = %u: %.2E\n", i, binEdges[i]);
	}

	return binEdges;
}

//----------------------------------------------------------------------------------------------------

bool CalculateAcceptanceCorrections(double sign, double th_y_L, double th_y_R, double th_y, double th,
		double si_th_y, double &phi_corr, double &div_corr)
{
	if ((sign * th_y_L < th_y_lcut_L) || (sign * th_y_R < th_y_lcut_R))
		return true;
	
	if ((sign * th_y_L > th_y_hcut_L) || (sign * th_y_R > th_y_hcut_R))
		return true;

	double th_y_lcut_eff = (th_y_lcut_L + th_y_lcut_R) / 2.;
	double th_y_hcut_eff = (th_y_hcut_L + th_y_hcut_R) / 2.;
	double th_y_abs = sign * th_y;	// always positive

	double div_corr_l = (th_y_abs > th_y_lcut_eff) ?
		2. / (TMath::Erf((th_y_abs - th_y_lcut_L) / si_th_y) - TMath::Erf((th_y_lcut_R - th_y_abs) / si_th_y)) : 0.;

	double div_corr_h = (th_y_abs < th_y_hcut_eff) ?
		2. / (TMath::Erf((th_y_hcut_L - th_y_abs) / si_th_y) - TMath::Erf((th_y_abs - th_y_hcut_R) / si_th_y)) : 0.;

	div_corr = div_corr_l * div_corr_h;

	// calculate acceptance phi correction
	double th_y_lcut = th_y_lcut_eff + 0.5E-6;
	double th_y_hcut = th_y_hcut_eff - 0.5E-6;

	if (th_y_abs <= th_y_lcut || th_y_abs >= th_y_hcut)
		return true;

	double phiMin = (th > th_y_lcut) ? asin(th_y_lcut / th) : M_PI/2.;
	double phiMax = (th > th_y_hcut) ? asin(th_y_hcut / th) : M_PI/2.;
	double phiAcc = phiMax - phiMin;
	phi_corr = M_PI / phiAcc;

	return false;
}
