#include <string>

bool updated_ntuples = false;

//----------------------------------------------------------------------------------------------------

// alignment parameters
//	a: xy coupling in rad
//	b: x shift in mm
//	c: y shift in mm

double al_a_L_F = -0.0E-3, al_b_L_F = -14.3E-3, al_c_L_F = -20E-3;
double al_a_L_N = -1.7E-3, al_b_L_N = -10.3E-3, al_c_L_N = -86E-3;
double al_a_R_N = +2.0E-3, al_b_R_N = -15.7E-3, al_c_R_N = -22E-3;
double al_a_R_F = -0.8E-3, al_b_R_F = -10.8E-3, al_c_R_F = -49E-3;

//----------------------------------------------------------------------------------------------------

// one-side smearing
double si_th_y_os = 2.47E-6, si_th_y_os_unc = 0.07E-6;
double si_th_x_os = 6.36E-6, si_th_x_os_unc = 0.21E-6;

double p = 3500.;

double th_y_lcut_L = 0E-6, th_y_lcut_R = 0E-6;
double th_y_hcut_L = 0E-6, th_y_hcut_R = 0E-6;

double t_min = 0., t_max = 1.;
double t_min_full = 0., t_max_full = 1.2;
double t_min_fit = 0.0046001;

unsigned int N_bins = 100, N_low = 0, N_high = 50;

double alpha_gr = 1E0;

double full_eff_corr = 0.;

double daq_eff = 0.;
double L_int_eff = 0.;	// mb^-1

//----------------------------------------------------------------------------------------------------

std::string base_cut;
std::string rp_L_N, rp_L_F, rp_R_N, rp_R_F;
	
double v_y_N = 0.02084;			// 1
double v_y_F = 0.0004728;		// 1
double L_y_N = 237.7E3;			// mm
double L_y_F = 263.2E3;			// mm
double dLds_y = 4.743;			// 1
double dvds_y = -0.003791E-3;	// mm^-1

double v_x_N = -2.165;			// 1
double v_x_F = -1.866;			// 1
double L_x_N = 2.876E3;			// mm, nominal only!
double L_x_F = -0.002458E3;		// mm, nominal only!
double dLds_x = -0.5358;		// 1
double dvds_x = 0.05553E-3;		// mm^-1

double v_x_rat = v_x_N / v_x_F;

double th_x_al_R;
double th_x_be_R;
double th_x_al_L;
double th_x_be_L;

double th_y_shift = 0E-6;

double cut1_si, cut1_c;
double cut2_si;
double cut34_si;
double cut5_a, cut5_b, cut5_si;
double cut6_a, cut6_b, cut6_si;
double cut7_al, cut7_c, cut7_si;

//----------------------------------------------------------------------------------------------------

void Init_45b_56t()
{
	rp_L_N = "track_rp_21"; rp_L_F = "track_rp_25";
	rp_R_N = "track_rp_120"; rp_R_F = "track_rp_124";

	//th_x_al_L = -2.6E-6; th_x_be_L = -0.134;
	//th_x_al_R = -3.0E-6; th_x_be_R = +0.294;
	th_x_al_L = +0E-6; th_x_be_L = -0.;		// now included in fine alignment
	th_x_al_R = +0E-6; th_x_be_R = +0.;

	cut1_si = 9.2E-6; cut1_c = +0.8E-6;
	cut2_si = 3.5E-6;

	cut34_si = 0.200;	

	cut5_a = 0.1073; cut5_b = +2.9E-3; cut5_si = 17E-3;
	cut6_a = 0.1073; cut6_b = +6.2E-3; cut6_si = 17E-3;

	cut7_al = -12.4;
	cut7_c = 0E-3;	// now included in fine alignment
	cut7_si = 9E-3;

	th_y_lcut_L = 18.6E-6; th_y_lcut_R = 18.4E-6;
	th_y_hcut_L = 107E-6; th_y_hcut_R = 103E-6;

	full_eff_corr = 1.251;
	
	daq_eff = 0.981;
	L_int_eff = 6.597E+03;
}

//----------------------------------------------------------------------------------------------------

void Init_45t_56b()
{
	rp_L_N = "track_rp_20"; rp_L_F = "track_rp_24";
	rp_R_N = "track_rp_121"; rp_R_F = "track_rp_125";

	//th_x_al_L = -1.4E-6; th_x_be_L = -0.127;
	//th_x_al_R = -5.0E-6; th_x_be_R = +0.188;
	th_x_al_L = +0E-6; th_x_be_L = -0.;		// now included in fine alignment
	th_x_al_R = +0E-6; th_x_be_R = +0.;

	cut1_si = 9.2E-6;	// 8.7E-6 would be more accurate, but using the same value as for the other diagonal, to ensure the same background
	cut1_c = -1.6E-6;
	cut2_si = 3.5E-6;

	cut34_si = 0.200;	

	cut5_a = 0.1073; cut5_b = -5.2E-3; cut5_si = 17E-3;
	cut6_a = 0.1073; cut6_b = +17.3E-3; cut6_si = 17E-3;
	
	cut7_al = -16.6;
	cut7_c = 0E-3;	// now included in fine alignment
	cut7_si = 9E-3;

	th_y_lcut_L = 18.6E-6; th_y_lcut_R = 18.2E-6;
	th_y_hcut_L = 109E-6; th_y_hcut_R = 107E-6;

	full_eff_corr = 1.175;
	
	daq_eff = 0.981;
	L_int_eff = 6.597E+03;
}
