#include "TMatrixD.h"
#include "TVectorD.h"
#include "TH2D.h"
#include "Minuit2/FCNBase.h"
#include "TFitterMinuit.h"
#include "TCanvas.h"

namespace GR {

TMatrixD GetMatrix(TH2D *h)
{
	unsigned int dim = 0;
	int fi = 100000000;
	for (int i = 1; i <= h->GetNbinsX(); i++) {
		double t = h->GetXaxis()->GetBinCenter(i);
		if (t > t_min_full && t < t_max_full) {
			dim++;
			fi = min(fi, i);
		}
	}

	printf(">> GR::GetMatrix: fi = %i, dim = %u\n", fi, dim);

	TMatrixD R(dim, dim);
	for (int x_i = 1; x_i <= h->GetNbinsX(); x_i++) {
		for (int y_i = 1; y_i <= h->GetNbinsY(); y_i++) {
			double x = h->GetXaxis()->GetBinCenter(x_i);
			double y = h->GetYaxis()->GetBinCenter(y_i);
			
			if (x > t_min_full && y > t_min_full && x < t_max_full && y < t_max_full) {
				R(x_i-fi, y_i-fi) = h->GetBinContent(x_i, y_i);
			}
		}
	}

	return R;
}

//----------------------------------------------------------------------------------------------------

class S2_FCN : public ROOT::Minuit2::FCNBase
{
	public:
		unsigned int verbosity;
		TVectorD bcp, n;
		TMatrixD V_n, R;
		double al;
		S2_FCN(const TVectorD &_bcp, const TVectorD &_n, const TMatrixD &_V_n, const TMatrixD &_R, double _al)
			: verbosity(0), bcp(_bcp), n(_n), V_n(_V_n), R(_R), al(_al) {}

  		double operator() (const std::vector<double> &mu) const;
  		double Up() const { return 1.; }
  		
		TMatrixD GetCMatrix(std::vector<double> mu);
};

//----------------------------------------------------------------------------------------------------

double S2_FCN::operator() (const std::vector<double> &mu) const
{
	if (verbosity > 1)
		printf("----------------------------------------------------------------------------\n");

	// inversion part
	if (verbosity > 1)
		printf("** inversion part **\n");
	double S2_inv = 0.;
	unsigned int N_inv = 0;
	for (unsigned int i = 0; i < n.GetNrows(); i++) {
		double Rmu = 0.;
		for (unsigned int j = 0; j < mu.size(); j++) {
			Rmu += R(i, j) * mu[j];
		}
		
		double v = (n(i) - Rmu) / sqrt(V_n(i, i));

		if (verbosity > 1)
			printf("%u: n = %E, R*mu = %E, n - R*mu = %E, si = %E, S2 contrib = %E\n",
				i, n(i), Rmu, n(i) - Rmu, sqrt(V_n(i, i)), v*v);

		S2_inv += v*v;
		N_inv++;
	}

	// regularization part
	if (verbosity > 1)
		printf("** regularization part **\n");
	double S2_reg = 0.;
	unsigned int N_reg = 0;
	for (unsigned int k = 1; k < n.GetNrows() - 1; k++) {
		double x1 = bcp[k-1], l1 = log(mu[k-1]), mu1 = mu[k-1], e1sq = V_n(k-1, k-1);
		double x2 = bcp[k-0], l2 = log(mu[k-0]), mu2 = mu[k-0], e2sq = V_n(k-0, k-0);
		double x3 = bcp[k+1], l3 = log(mu[k+1]), mu3 = mu[k+1], e3sq = V_n(k+1, k+1);
		
		if (verbosity > 1)
			printf("%u, %u, %u:\n", k-1, k, k+1);
		
		if (verbosity > 5) {
			printf("\t%u: x = %E, log mu = %E, mu = %E, si^2 = %E\n", k-1, x1, l1, mu1, e1sq);
			printf("\t%u: x = %E, log mu = %E, mu = %E, si^2 = %E\n", k-0, x2, l2, mu2, e2sq);
			printf("\t%u: x = %E, log mu = %E, mu = %E, si^2 = %E\n", k+1, x3, l3, mu3, e3sq);
		}

		double S2_inc = 0.;

		if (mu1 > 0. && mu2 > 0. && mu3 > 0.) {
			double f1 = +1./(x1-x2)/(x1-x3);
			double f2 = +1./(x2-x1)/(x2-x3);
			double f3 = +1./(x3-x1)/(x3-x2);
			
			double g1 = -(x2+x3)/(x1-x2)/(x1-x3);
			double g2 = -(x1+x3)/(x2-x1)/(x2-x3);
			double g3 = -(x1+x2)/(x3-x1)/(x3-x2);
	
			double A = l1*f1 + l2*f2 + l3*f3;
			double B = l1*g1 + l2*g2 + l3*g3;
			double C = l1 - A*x1*x1 - B*x1;
			//double si_A_sq = f1*f1/mu1/mu1*e1sq + f2*f2/mu2/mu2*e2sq + f3*f3/mu3/mu3*e3sq;
			double si_A_sq = 1.;
			
			if (verbosity > 1)
				printf("\tA=%E +- %E, B=%E, C=%E | si_A / A = %E\n", A, sqrt(si_A_sq), B, C, sqrt(si_A_sq)/A);
	
			S2_inc = A*A / si_A_sq;		
		} else {
			S2_inc = 1E20; // large penalty
		}
		
		if (verbosity > 1)
			printf("\tS2 inc = %E\n", S2_inc);

		S2_reg += S2_inc;
		N_reg++;
	}

	double S2 = al * S2_inv/N_inv + S2_reg/N_reg;
	if (verbosity)
		printf("al = %.2E, S2_inv = %.2E, N_inv = %u | S2_reg = %.2E, N_reg = %u | S2_full = al*S2_inv/N_inv + S2_reg/N_reg = %.2E\n",
			al, S2_inv, N_inv, S2_reg, N_reg, S2);
	return S2;
}

//----------------------------------------------------------------------------------------------------

TMatrixD S2_FCN::GetCMatrix(std::vector<double> mu)
{
	double ep = 1E-8;
	unsigned int dim = mu.size();
	TMatrixD A(dim, dim), B(dim, dim);

	for (unsigned int i = 0; i < dim; i++) {
		for (unsigned int j = 0; j < dim; j++) {
			double v1, v2, v3, v4;

			mu[i] += ep; mu[j] += ep; v1 = operator() (mu); mu[i] -= ep; mu[j] -= ep;
			mu[i] += 0.; mu[j] += ep; v2 = operator() (mu); mu[i] -= 0.; mu[j] -= ep;
			mu[i] += ep; mu[j] += 0.; v3 = operator() (mu); mu[i] -= ep; mu[j] -= 0.;
			mu[i] += 0.; mu[j] += 0.; v4 = operator() (mu); mu[i] -= 0.; mu[j] -= 0.;
			A(i, j) = (v1 - v2 - v3 + v4) / ep/ep;
			
			mu[i] += ep; n(j) += ep; v1 = operator() (mu); mu[i] -= ep; n(j) -= ep;
			mu[i] += 0.; n(j) += ep; v2 = operator() (mu); mu[i] -= 0.; n(j) -= ep;
			mu[i] += ep; n(j) += 0.; v3 = operator() (mu); mu[i] -= ep; n(j) -= 0.;
			mu[i] += 0.; n(j) += 0.; v4 = operator() (mu); mu[i] -= 0.; n(j) -= 0.;
			B(i, j) = (v1 - v2 - v3 + v4) / ep/ep;
		}
	}

	TMatrixD Ai(TMatrixD::kInverted, A);
	TMatrixD C(dim, dim);
	C -= Ai * B;

	return C;
}

//----------------------------------------------------------------------------------------------------

void DoMinimization(const TVectorD &bcp, const TVectorD &n, const TMatrixD &V_n, const TMatrixD &R, double al, TVectorD &mu, TMatrixD &V_mu)
{
	printf(">> GR::DoMinimization(al = %.2E)\n", al);

	TFitterMinuit *minuit = new TFitterMinuit();

	S2_FCN fcn(bcp, n, V_n, R, al);
	minuit->SetMinuitFCN(&fcn);

	int dim = n.GetNrows();

	// init settings - from mu input
	vector<double> mu_init;
	for (unsigned int i = 0; i < dim; i++) {
		char buf[100];
		sprintf(buf, "mu_%i", i);
		minuit->SetParameter(i, buf, mu(i), sqrt(V_mu(i, i)), 0., 0.);
		mu_init.push_back(mu(i));
	}

	fcn.verbosity = 10;
	//fcn(mu_init);
	fcn.verbosity = 0;
	
	minuit->SetPrintLevel(3);

	// create Minimizer (default is Migrad)
	minuit->CreateMinimizer();
	int result = minuit->Minimize();
	
	vector<double> mu_final;
	for (unsigned int i = 0; i < dim; i++) {
		mu(i) = minuit->GetParameter(i);
		mu_final.push_back(mu(i));

		V_mu(i, i) = 0.;
	}
	
	fcn.verbosity = 10;
	fcn(mu_final);
	fcn.verbosity = 0;

	//TMatrixD C(fcn.GetCMatrix(mu_vec));
	//TMatrixD CT(TMatrixD::kTransposed, C);
	//V_mu = C * V_n * CT;
}

//----------------------------------------------------------------------------------------------------

void DoUnfolding(TH1D *h, TMatrixD R)
{
	// build input histogram
	TH1D *input = new TH1D(*h);
	input->SetName("input");
	input->SetLineColor(1);
	unsigned int dim = 0;
	int fi = 100000000;		// first index in the full range
	int foi = 100000000;	// lowest index inside the (min, max) range
	int	loi = 0;			// highest index inside the (min, max) range
	for (int i = 1; i <= input->GetNbinsX(); i++) {
		double t = input->GetBinCenter(i);
		if (t < t_min || t > t_max) {
			input->SetBinContent(i, 0.);
			input->SetBinError(i, 0.);
		} else {
			foi = min(foi, i);
			loi = max(loi, i);
		}
		if (t > t_min_full && t < t_max_full) {
			dim++;
			fi = min(fi, i);
		}
	}
	
	printf(">> GR::DoUnfolding > fi = %i, dim = %u, foi=%i, loi=%i\n", fi, dim, foi, loi);

	// build start histogram (extrapolate the input at both ends)
	TH1D *start = new TH1D(*input);
	TF1 *lff, *hff;

	// low |t| fit
	//double fit_bound_low = input->GetBinCenter(foi+1), fit_bound_high = input->GetBinCenter(foi+20);
	double fit_bound_low = input->GetBinCenter(foi+1), fit_bound_high = fit_bound_low + 0.6;
	printf("low |t| extrapolation fit: range from %E to %E\n", fit_bound_low, fit_bound_high);
	lff = new TF1("lff", "[0]*exp([1]*x)"); lff->SetParameters(1E-1, -4.);
	input->Fit(lff, "+", "", fit_bound_low, fit_bound_high);
	input->Fit(lff, "I+", "", fit_bound_low, fit_bound_high);
	
	// high |t| fit
	//fit_bound_low = input->GetBinCenter(loi-6); fit_bound_high = input->GetBinCenter(loi-1);
	fit_bound_high = input->GetBinCenter(loi-1);
	fit_bound_low = fit_bound_high - 1.2;
	printf("low |t| extrapolation fit: range from %E to %E\n", fit_bound_low, fit_bound_high);
	hff = new TF1("hff", "[0]*exp([1]*x)"); hff->SetParameters(1E-1, -4.);
	input->Fit(hff, "+", "", fit_bound_low, fit_bound_high);
	input->Fit(hff, "I+", "", fit_bound_low, fit_bound_high);

	// low |t| extrapolation
	for (int i = fi; i < foi; ++i) {
		start->SetBinContent(i, lff->Eval(start->GetBinCenter(i)));
		start->SetBinError(i, 0.);	// will be regularized later
	}
	
	// high |t| extrapolation
	for (int i = loi+1; i <= start->GetNbinsX(); ++i) {
		start->SetBinContent(i, hff->Eval(start->GetBinCenter(i)));
		start->SetBinError(i, 0.);	// will be regularized later
	}
	
	input->Write();

	start->SetLineColor(4);
	start->SetName("start");
	start->Write();

	// vizualization of the minimization initial state
	TH1D *mu_init = new TH1D(*start);
	mu_init->SetName("mu_init");
	mu_init->SetLineColor(6);

	// build matrices and vectors
	// mu to be filled with interation-start
	TMatrixD V_n(dim, dim), V_mu(dim, dim);
	TVectorD n(dim), mu(dim), bcp(dim);
	for (unsigned int i = 0; i < dim; i++) {
		// bin control points
		double c = start->GetBinCenter(i+1);
		bcp(i) = c;

		// measurement - error regularization needed
		double v = input->GetBinContent(fi + i);
		double e = start->GetBinError(fi + i);

		if (e <= 0.) {
			bool found_low = false;
			double e_low;
			for (signed int j = i-1; j >= 0; --j) {
				e_low = start->GetBinError(fi + j);
				if (e_low > 0.) {
					found_low = true;
					break;
				}
			}
			
			bool found_high = false;
			double e_high;
			for (signed int j = i+1; j < dim; ++j) {
				e_high = start->GetBinError(fi + j);
				if (e_high > 0.) {
					found_high = true;
					break;
				}
			}

			if (found_low && found_high) e = (e_high + e_low) / 2.;
			if (found_low && !found_high) e = e_low;
			if (!found_low && found_high) e = e_high;
			if (!found_low && !found_high)
				throw "this should not happen";
			
			printf("regularazing error of bin %u to %E\n", i, e);
		}

		n(i) = v;
		V_n(i, i) = e;

		// initial minimization state - value and error regularization needed (error is used to determine minimization step size)
		v = start->GetBinContent(fi + i);	// start has the extrapolations
		if (v <= 0.) {
			v = hff->Eval(c);
			printf("regularazing value of bin %u to %E\n", i, v);
		}
		if (e >= v)
			e = 0.9*v;

		mu(i) = v;
		V_mu(i, i) = e;

		mu_init->SetBinContent(fi+i, v);
		mu_init->SetBinError(fi+i, e);
	}

	mu_init->Write();


	// run the unfolding minimization
	//double alpha = 1E-1;
	double alpha = alpha_gr;
	DoMinimization(bcp, n, V_n, R, alpha, mu, V_mu);

	// calculate nu from mu
	TVectorD nu(dim);
	TMatrixD V_nu(dim, dim);
	nu = R * mu;
	TMatrixD RT(TMatrixD::kTransposed, R);
	V_nu = R * V_mu * RT;

	// extract the correction function
	TH1D *output = new TH1D(*input); output->SetName("output");
	TH1D *corr_raw = new TH1D(*input); corr_raw->SetName("corr_raw"); corr_raw->Reset();
	TH1D *corr_reg = new TH1D(*input); corr_reg->SetName("corr_reg"); corr_reg->Reset();

	for (int i = 1; i <= input->GetNbinsX(); i++) {
		int idx = i - fi;
		if (i >= fi && i < fi+dim) {
			output->SetBinContent(i, mu(idx));
			double err_sq = V_mu(idx, idx);
			output->SetBinError(i, (err_sq >= 0.) ? sqrt(err_sq) : 1.);
		} else {
			output->SetBinContent(i, 0.);
			output->SetBinError(i, 0.);
		}

		double n = output->GetBinContent(i), ne = output->GetBinError(i);
		double d = input->GetBinContent(i), de = input->GetBinError(i);

		corr_raw->SetBinContent(i, (d != 0.) ? n/d : 0.);
		corr_raw->SetBinError(i, (d != 0.) ? sqrt(ne*ne/d/d + n*n * de*de /d/d/d/d) : 0.);
		
		if (idx >= 0 && idx < dim) {
			double d2 = nu(idx), de2 = sqrt(V_nu(idx, idx));
			corr_reg->SetBinContent(i, (d2 != 0.) ? n/d2 : 0.);
			corr_reg->SetBinError(i, (d2 != 0.) ? sqrt(ne*ne/d2/d2 + n*n * de2*de2 /d2/d2/d2/d2) : 0.);
		}
	}

	output->SetLineColor(2);
	output->Write();

	corr_raw->SetLineColor(2);
	corr_raw->Write();

	corr_reg->SetLineColor(4);
	corr_reg->Write();

	TCanvas *c = new TCanvas(); c->SetName("dist plot"); c->SetLogy(1);
	input->Draw();
	start->Draw("same");
	mu_init->Draw("same");
	output->Draw("same");
	c->Write();
	
	c = new TCanvas(); c->SetName("corr plot");
	corr_raw->Draw();
	corr_reg->Draw("same");
	c->Write();
}

} // namespace
