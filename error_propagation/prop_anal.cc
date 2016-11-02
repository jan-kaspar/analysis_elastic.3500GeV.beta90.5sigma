void prop_anal()
{
	double K = 19.572;
	double rho = 0.14, rho_ep = 0.01, rho_em = 0.08;

	double B_j = 20.1, B_e_j = 0.4;

	double I_j = 8.291486E5, I_e_j = 0.01 * I_j;
	double N_elo_j = 25033.6, N_elo_e_j = 0.01 * N_elo_j;

	double t0 = 0., t1 = 0.025, t2 = 0.3;

	double R = (exp(-B_j * t0) - exp(-B_j * t1)) / (exp(-B_j * t1) - exp(-B_j * t2));
	double d0 = t0-t1, d2 = t2-t1;
	double R_e = sqrt(pow(
					d0*exp(-B_j*d0) / (1. - exp(-B_j*d2)) + (exp(-B_j*d0)) / pow(1. - exp(-B_j*d2), 2.)	* d2 * exp(-B_j*d2)		
				, 2.) * pow(B_e_j, 2.)
			);

	printf("R = %E +- %E\n", R, R_e);

	double N_ele_j = N_elo_j * R;
	double N_ele_e_j = 0.;
	
	printf("N_ele_j = %E +- %E\n", N_ele_j, N_ele_e_j);

	double N_el_j = N_elo_j * (1. + R);
	double N_el_e_j = 0.;
	
	printf("N_el_j = %E +- %E\n", N_el_j, N_el_e_j);

	double I_o = 506.4, I_e_o = 10.1;
	double N_el_o = 25.43, N_el_e_o = 0.51;
	double N_inel_o = 73.7, N_inel_e_o = 1.70;

	double si_tot = K / (1. + rho*rho) * I_o / (N_el_o + N_inel_o);
	double si_tot_e = 0;

	printf("si_tot = %E +- %E\n", si_tot, si_tot_e);

	double L1_j = K / (1. + rho*rho) * I_j / si_tot / si_tot;
	double L1_ep_j = L1_j * sqrt(
				pow(2.*rho/(1. + rho*rho), 2.) * rho_ep*rho_ep
				+ I_e_j*I_e_j / I_j/I_j
				+ 4. * pow(1./(N_el_o + N_inel_o) - 1./N_el_o, 2.) * N_el_e_o*N_el_e_o
				+ 4. * pow(1./(N_el_o + N_inel_o), 2.) * N_inel_e_o*N_inel_e_o
			);
	
	double L1_em_j = L1_j * sqrt(
				pow(2.*rho/(1. + rho*rho), 2.) * rho_em*rho_em
				+ I_e_j*I_e_j / I_j/I_j
				+ 4. * pow(1./(N_el_o + N_inel_o) - 1./N_el_o, 2.) * N_el_e_o*N_el_e_o
				+ 4. * pow(1./(N_el_o + N_inel_o), 2.) * N_inel_e_o*N_inel_e_o
			);

	printf("L1_j = %E + %E - %E\n", L1_j, L1_ep_j, L1_em_j);

	double si_el_o = si_tot * N_el_o / (N_el_o + N_inel_o);
	double si_el_ep_o = si_el_o * sqrt(
				pow(2.*rho/(1. + rho*rho), 2.) * rho_em*rho_em
				+ 4. * pow(1./N_el_o - 1./(N_el_o + N_inel_o), 2.) * pow(N_el_e_o, 2.)
				+ 4. * pow(1./(N_el_o + N_inel_o), 2.) * pow(N_inel_e_o, 2.)
			);
	double si_el_em_o = si_el_o * sqrt(
				pow(2.*rho/(1. + rho*rho), 2.) * rho_ep*rho_ep
				+ 4. * pow(1./N_el_o - 1./(N_el_o + N_inel_o), 2.) * pow(N_el_e_o, 2.)
				+ 4. * pow(1./(N_el_o + N_inel_o), 2.) * pow(N_inel_e_o, 2.)
			);

	printf("si_el_o = %E + %E - %E\n", si_el_o, si_el_ep_o, si_el_em_o);
	
	double si_inel_o = si_tot * N_inel_o / (N_el_o + N_inel_o);
	double si_inel_ep_o = si_inel_o * sqrt(
				+ pow(2.*rho/(1. + rho*rho), 2.) * pow(rho_em, 2.)
				+ pow(1./N_el_o - 2./(N_el_o + N_inel_o), 2.) * pow(N_el_e_o, 2.)
				+ pow(1./N_inel_o - 2./(N_el_o + N_inel_o), 2.) * pow(N_inel_e_o, 2.)
			);
	
	double si_inel_em_o = si_inel_o * sqrt(
				+ pow(2.*rho/(1. + rho*rho), 2.) * pow(rho_ep, 2.)
				+ pow(1./N_el_o - 2./(N_el_o + N_inel_o), 2.) * pow(N_el_e_o, 2.)
				+ pow(1./N_inel_o - 2./(N_el_o + N_inel_o), 2.) * pow(N_inel_e_o, 2.)
			);

	printf("si_inel_o = %E + %E - %E\n", si_inel_o, si_inel_ep_o, si_inel_em_o);
	
	double L2_j = N_el_j / si_el_o;
	double L2_ep_j = L2_j * sqrt(
				pow(R_e / (1. + R), 2.)
				+ pow(N_elo_e_j / N_elo_j, 2.)
				+ pow(si_el_em_o / si_el_o, 2.)
			);
	double L2_em_j = L2_j * sqrt(
				pow(R_e / (1. + R), 2.)
				+ pow(N_elo_e_j / N_elo_j, 2.)
				+ pow(si_el_ep_o / si_el_o, 2.)
			);

	printf("L2_j = %E + %E - %E\n", L2_j, L2_ep_j, L2_em_j);
}
