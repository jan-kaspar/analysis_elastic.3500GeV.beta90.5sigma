import root;
import pad_layout;

bool separate = true;
void CShipout(string prefix)
{
	if (separate)
		GShipout(prefix);
	else
		NewRow();
}

string basedir = "../";
string dataSets[] = { "NN_2011_10_20_1", "NN_2011_10_20_2", "NN_2011_10_20_3" };
real t_min[] = {7.3e-3, 5.7e-3, 4.6e-3};

real defV[] = {-3, -4, -5};

TH2_palette = Gradient(blue, heavygreen, yellow, red, black);

string cqa[] = {"\th_x^R", "\th_y^R", "x^{L,N}", "x^{R,N}", "\th_y^R", "\th_y^L"};
string cqb[] = {"\th_x^L", "\th_y^L", "x^{L,F} - x^{L,N}", "x^{R,F} - x^{R,N}", "y^{R,F} - y^{R,N}", "y^{L,F} - y^{L,N}"};


/*
for (int i = 1; i <= 6; ++i) {
	NewPad("$"+cqa[i-1]+"$", "$"+cqb[i-1]+"$");
	draw(rGetObj(basedir+dataSets[0]+"/distributions_45b_56t.root", "elastic cuts/h2_cq"+format("%i", i)), "p");
	if (i == 3)
		NewRow();
}

CShipout("el_cuts");

NewPad("$x\ung{mm}$", "$y\ung{mm}$");
scale(Linear, Linear, Log);
draw(rGetObj(basedir+dataSets[0]+"/distributions_45t_56b.root", "hit distributions/h_y_L_F_vs_x_L_F_raw"), "p,bar");
limits((-10, 0), (10, 40));

CShipout("hit_dist");
*/

for (int dsi : dataSets.keys) {
	NewPad("$\th_{x, y}\ung{\mu rad}$", "mean of $\De^{R-L} \th_{x, y}\ung{\mu rad}$", yTicks=RightTicks(Step=1,step=0.5));
	draw(scale(1e6, 1e6), rGetObj(basedir+dataSets[dsi]+"/distributions_45b_56t.root", "selected - angles/th_x_diffLR_vs_th_x"), "ec,d0", blue, "$x$");
	draw(scale(1e6, 1e6), rGetObj(basedir+dataSets[dsi]+"/distributions_45b_56t.root", "selected - angles/th_x_diffLR_vs_th_x|pol1"), heavygreen+1pt);
	draw(scale(1e6, 1e6), rGetObj(basedir+dataSets[dsi]+"/distributions_45b_56t.root", "selected - angles/th_y_diffLR_vs_th_y"), "ec,d0", red, "$y$");
	draw(scale(1e6, 1e6), rGetObj(basedir+dataSets[dsi]+"/distributions_45b_56t.root", "selected - angles/th_y_diffLR_vs_th_y|pol1"), heavygreen+1pt);
	limits((-150, -3), (+150, +3), Crop);
	xaxis(YEquals(0, false), dashed);
	AttachLegend(format("dataset %i, 45 bottom -- 56 top", dsi+1));
}

CShipout("th_xy_diffLR_vs_th_xy_45b");

for (int dsi : dataSets.keys) {
	NewPad("$\th_{x, y}\ung{\mu rad}$", "mean of $\De^{R-L} \th_{x, y}\ung{\mu rad}$", yTicks=RightTicks(Step=1,step=0.5));
	draw(scale(1e6, 1e6), rGetObj(basedir+dataSets[dsi]+"/distributions_45t_56b.root", "selected - angles/th_x_diffLR_vs_th_x"), "ec,d0", blue, "$x$");
	draw(scale(1e6, 1e6), rGetObj(basedir+dataSets[dsi]+"/distributions_45t_56b.root", "selected - angles/th_x_diffLR_vs_th_x|pol1"), heavygreen+1pt);
	draw(scale(1e6, 1e6), rGetObj(basedir+dataSets[dsi]+"/distributions_45t_56b.root", "selected - angles/th_y_diffLR_vs_th_y"), "ec,d0", red, "$y$");
	draw(scale(1e6, 1e6), rGetObj(basedir+dataSets[dsi]+"/distributions_45t_56b.root", "selected - angles/th_y_diffLR_vs_th_y|pol1"), heavygreen+1pt);
	limits((-150, -3), (+150, +3), Crop);
	xaxis(YEquals(0, false), dashed);
	AttachLegend(format("dataset %i, 45 top -- 56 bottom", dsi+1));
}

CShipout("th_xy_diffLR_vs_th_xy_45t");

for (int dsi : dataSets.keys) {
	NewPad("$\th_{y}\ung{\mu rad}$", "mean of $\th_{x}\ung{\mu rad}$", yTicks=RightTicks(Step=1,step=0.5));
	draw(scale(1e6, 1e6), rGetObj(basedir+dataSets[dsi]+"/distributions_45b_56t.root", "selected - angles/p_th_x_L_vs_th_y_L"), "ec,d0", blue, "45 B -- 56 T");
	draw(scale(1e6, 1e6), rGetObj(basedir+dataSets[dsi]+"/distributions_45t_56b.root", "selected - angles/p_th_x_L_vs_th_y_L"), "ec,d0", red, "45 T -- 56 B");
	limits((-150, -5), (+150, +5), Crop);
	xaxis(YEquals(0, false), dashed);
	AttachLegend(format("dataset %i, left", dsi+1));
}

CShipout("th_x_vs_th_y_45t_left");

for (int dsi : dataSets.keys) {
	NewPad("$\th_{y}\ung{\mu rad}$", "mean of $\th_{x}\ung{\mu rad}$", yTicks=RightTicks(Step=1,step=0.5));
	draw(scale(1e6, 1e6), rGetObj(basedir+dataSets[dsi]+"/distributions_45b_56t.root", "selected - angles/p_th_x_R_vs_th_y_R"), "ec,d0", blue, "45 B -- 56 T");
	draw(scale(1e6, 1e6), rGetObj(basedir+dataSets[dsi]+"/distributions_45t_56b.root", "selected - angles/p_th_x_R_vs_th_y_R"), "ec,d0", red, "45 T -- 56 B");
	limits((-150, -5), (+150, +5), Crop);
	xaxis(YEquals(0, false), dashed);
	AttachLegend(format("dataset %i, right", dsi+1));
}

CShipout("th_x_vs_th_y_45t_right");

for (int dsi : dataSets.keys) {
	NewPad("$\th_{x, y}\ung{\mu rad}$", "$\De^{R-L} \th_{x, y}\ung{\mu rad}$");
	draw(scale(1e6, 1e6), rGetObj(basedir+dataSets[dsi]+"/distributions_45b_56t.root", "selected - angles/th_x_sigmaLR_vs_th_x"), "p", blue, mCi+1pt+blue, "$x$");
	draw(scale(1e6, 1e6), rGetObj(basedir+dataSets[dsi]+"/distributions_45b_56t.root", "selected - angles/th_y_sigmaLR_vs_th_y"), "p", red, mCi+1pt+red, "$y$");
	limits((-150, 0), (+150, +10), Crop);
	AttachLegend(format("dataset %i", dsi+1), SW, SW);
}

CShipout("th_xy_sigmaLR_vs_th_xy");

for (int dsi : dataSets.keys) {
	NewPad("$|t_y|\ung{GeV^2}$", "divergence acceptance correction");
	draw(rGetObj(basedir+dataSets[dsi]+"/distributions_45b_56t.root", "acceptance correction/p_t_eb_div_corr"), "ec,d0", blue);
	xlimits(0, 0.15, Crop);
	AttachLegend(format("dataset %i", dsi+1));
}

CShipout("acc_corr_div");

for (int dsi : dataSets.keys) {
	NewPad("$|t|\ung{GeV^2}$", "$\ph$ acceptance correction");
	scale(Linear, Log);
	draw(rGetObj(basedir+dataSets[dsi]+"/distributions_45b_56t.root", "acceptance correction/p_t_eb_phi_corr"), "ec,d0", red);
	xlimits(0, 0.5, Crop);
	yaxis(XEquals(t_min[dsi], false), dashed);
	AttachLegend(format("dataset %i", dsi+1));
}

CShipout("acc_corr_phi");

for (int dsi : dataSets.keys) {
	NewPad("$|t|\ung{GeV^2}$", "full acceptance corrections");
	scale(Log, Log);
	draw(rGetObj(basedir+dataSets[dsi]+"/distributions_45b_56t.root", "acceptance correction/p_t_eb_full_corr"), "ec,d0", heavygreen);
	xlimits(1e-3, 1e0, Crop);
	yaxis(XEquals(t_min[dsi], false), dashed);
	AttachLegend(format("dataset %i", dsi+1));
}

CShipout("acc_corr_full");

NewPad("$\th_x\ung{\mu rad}$", "$\th_y\ung{\mu rad}$");
scale(Linear, Linear, Log);
draw(scale(1e6, 1e6), rGetObj(basedir+dataSets[0]+"/distributions_45t_56b.root", "acceptance correction/h_th_y_vs_th_x_after"), "p,bar");
limits((-150, -150), (150, 150), Crop);

NewPad("$\ph\ung{rad}$", "$\th\ung{\mu rad}$");
scale(Linear, Linear, Log);
draw(scale(1, 1e6), rGetObj(basedir+dataSets[0]+"/distributions_45b_56t.root", "acceptance correction/h_th_vs_phi_after"), "p,bar");
limits((0, 0), (3.15, 150), Crop);

NewPad("$\ph\ung{rad}$", "$\th\ung{\mu rad}$");
scale(Linear, Linear, Log);
draw(scale(1, 1e6), rGetObj(basedir+dataSets[0]+"/distributions_45t_56b.root", "acceptance correction/h_th_vs_phi_after"), "p,bar");
limits((-3.15, 0), (0, 150), Crop);

CShipout("acc_corrected_th_x_vs_th_y");

for (int dsi : dataSets.keys) {
	NewPad("$\th_y\ung{\mu rad}$", "");
	scale(Linear, Log);
	draw(xscale(1e6), rGetObj(basedir+dataSets[dsi]+"/distributions_45b_56t.root", "acceptance correction/h_th_y_after"), blue);
	draw(xscale(1e6), rGetObj(basedir+dataSets[dsi]+"/distributions_45t_56b.root", "acceptance correction/h_th_y_after_flipped"), red);
	limits((0, 1e2), (150, 5e4), Crop);
	AttachLegend(format("dataset %i", dsi+1));
}

CShipout("acc_corrected_th_y_cmp");

for (int dsi : dataSets.keys) {
	NewPad("$\th_x\ung{\mu rad}$", "");
	scale(Linear, Log);
	draw(xscale(1e6), rGetObj(basedir+dataSets[dsi]+"/distributions_45b_56t.root", "acceptance correction/h_th_x_after"), blue);
	draw(xscale(1e6), rGetObj(basedir+dataSets[dsi]+"/distributions_45t_56b.root", "acceptance correction/h_th_x_after"), red);
	//limits((0, 1e-3), (1.5, 1e3), Crop);
	AttachLegend(format("dataset %i", dsi+1));
}

CShipout("acc_corrected_th_x_cmp");

for (int dsi : dataSets.keys) {
	NewPad("$|t|\ung{GeV^2}$", "$\d N/\d t$ before unsmearing");
	scale(Linear, Log);
	draw(rGetObj(basedir+dataSets[dsi]+"/distributions_45b_56t.root", "acceptance correction/h_t_eb_after"), "vl,ec", blue);
	draw(rGetObj(basedir+dataSets[dsi]+"/distributions_45t_56b.root", "acceptance correction/h_t_eb_after"), "vl,ec", red);
	
	draw(rGetObj(basedir+dataSets[dsi]+"/distributions_45b_56t.root", "acceptance correction/h_t_eb_before"), "vl,ec", black);
	draw(rGetObj(basedir+dataSets[dsi]+"/distributions_45t_56b.root", "acceptance correction/h_t_eb_before"), "vl,ec", heavygreen);

	limits((0, 1e1), (0.6, 1e8), Crop);
	AttachLegend(format("dataset %i", dsi+1));
}

CShipout("acc_corrected_t_cmp");

for (int dsi : dataSets.keys) {
	write(dsi);
	NewPad("$\De^{R-L} \th_y^*\ung{\mu rad}$", "");
	//scale(Linear, Log);
	rObject fit = rGetObj(basedir+dataSets[dsi]+"/distributions_45b_56t.root", "selected - angles/th_y_diffLR_safe|gaus");
	real rms = fit.rExec("GetParameter", 2);
	draw(xscale(1e6), rGetObj(basedir+dataSets[dsi]+"/distributions_45b_56t.root", "selected - angles/th_y_diffLR_safe"), blue, "45b-56t, " + format("RMS = $%.1E$", rms));
	
	rObject fit = rGetObj(basedir+dataSets[dsi]+"/distributions_45t_56b.root", "selected - angles/th_y_diffLR_safe|gaus");
	real rms = fit.rExec("GetParameter", 2);
	draw(xscale(1e6), rGetObj(basedir+dataSets[dsi]+"/distributions_45t_56b.root", "selected - angles/th_y_diffLR_safe"), red, "45t-56b, " + format("RMS = $%.1E$", rms));

	//limits((0, 1e-3), (1.5, 1e3), Crop);
	AttachLegend(format("dataset %i", dsi+1));
}

CShipout("si_diffLR_th_y");

for (int dsi : dataSets.keys) {
	NewPad("$\De^{R-L} \th_x^*\ung{\mu rad}$", "");
	//scale(Linear, Log);
	
	rObject fit = rGetObj(basedir+dataSets[dsi]+"/distributions_45b_56t.root", "selected - angles/th_x_diffLR_safe|gaus");
	real rms = fit.rExec("GetParameter", 2);
	draw(xscale(1e6), rGetObj(basedir+dataSets[dsi]+"/distributions_45b_56t.root", "selected - angles/th_x_diffLR_safe"), blue, "45b-56t, " + format("RMS = $%.1E$", rms));
	
	rObject fit = rGetObj(basedir+dataSets[dsi]+"/distributions_45t_56b.root", "selected - angles/th_x_diffLR_safe|gaus");
	real rms = fit.rExec("GetParameter", 2);
	draw(xscale(1e6), rGetObj(basedir+dataSets[dsi]+"/distributions_45t_56b.root", "selected - angles/th_x_diffLR_safe"), red, "45t-56b, " + format("RMS = $%.1E$", rms));
	//limits((0, 1e-3), (1.5, 1e3), Crop);
	AttachLegend(format("dataset %i", dsi+1));
}

CShipout("si_diffLR_th_x");

for (int dsi : dataSets.keys) {
	NewPad("$|t|\ung{GeV^2}$", "smearing correction$\ung{\%}$");
	draw(yscale(100)*shift(0, -1), rGetObj(basedir+dataSets[dsi]+"/unfolding_45b_56t.root", "cf,ub/corr_final"), "ec", blue);
	draw(yscale(100)*shift(0, -1), rGetObj(basedir+dataSets[dsi]+"/unfolding_45t_56b.root", "cf,ub/corr_final"), "ec", red);
	limits((0, -10), (1, +2), Crop);
	AttachLegend(format("dataset %i", dsi+1));
}

CShipout("smearing_corr");

for (int dsi : dataSets.keys) {
	NewPad("$|t|\ung{GeV^2}$", "smearing correction$\ung{\%}$");
	draw(yscale(100)*shift(0, -1), rGetObj(basedir+dataSets[dsi]+"/unfolding_45b_56t.root", "cf,eb/corr_final"), "ec", blue);
	draw(yscale(100)*shift(0, -1), rGetObj(basedir+dataSets[dsi]+"/unfolding_45t_56b.root", "cf,eb/corr_final"), "ec", red);
	limits((0, -3), (0.2, +2), Crop);
	AttachLegend(format("dataset %i", dsi+1));
}

CShipout("smearing_corr_detail");

//NewPad("$|t|\ung{GeV^2}$", "smearing correction");
//draw(rGetObj(basedir+dataSets[0]+"/unfolding_45b_56t.root", "cf,eb/corr_final"), "ec", blue);
//robj.vExec("Fit", "expo");
//draw(rGetObj(basedir+dataSets[0]+"/unfolding_45b_56t.root", "cf,eb/corr_final|expo"), "ec", heavygreen, "exp slope: " + format("%.2f", robj.rExec("GetParameter", 1)));
//limits((0, 0.8), (0.4, 1.1), Crop);
//AttachLegend();
//CShipout("smear_corr_fit");

for (int dsi : dataSets.keys) {
	NewPad("$|t|\ung{GeV^2}$", "$\d N/\d t$ after unsmearing");
	scale(Linear, Log);
	draw(rGetObj(basedir+dataSets[dsi]+"/unfolding_45b_56t.root", "cf,eb/output"), "ec", blue);
	draw(rGetObj(basedir+dataSets[dsi]+"/unfolding_45t_56b.root", "cf,eb/output"), "ec", red);
	//draw(rGetObj("/home/jkaspar/publications/2012/elastic scattering/publication2.root", "c1#1"), "l,ec", heavygreen);
	limits((0, 1e2), (0.4, 1e8), Crop);
	AttachLegend(format("dataset %i", dsi+1));
}

CShipout("dNdt_unsmeared");

//NewPad("timestamp $\ung{s}$", "elastic rate $\ung{Hz}$");
//scale(Linear, Log);
//draw(rGetObj(dataSets[1]+"/compare_diagonals.root", "timestamp_sel|45t_56b"), blue+0.5pt);
//draw(rGetObj(dataSets[1]+"/compare_diagonals.root", "timestamp_sel|45b_56t"), red+0.25pt);
//limits((71500, 0.1), (72600, 100), Crop);
//xaxis(YEquals(1, false), heavygreen+1pt);
//CShipout("timestamp_dist");

for (int dsi : dataSets.keys) {
	NewPad("$|t|\ung{GeV^2}$", "$\d\si/\d t \ung{mb/GeV^2}$");
	scale(Linear, Log);
	draw(rGetObj(basedir+dataSets[dsi]+"/normalize_45b_56t.root", "cross section/output"), "vl,ec", blue, "45b-56t");
	draw(rGetObj(basedir+dataSets[dsi]+"/normalize_45t_56b.root", "cross section/output"), "vl,ec", red, "45t-56b");

	draw(rGetObj("/home/jkaspar/publications/2012/elastic scattering/publication1.root", "tc#1"), "vl,ec", black, "EPL 95");
	//draw(rGetObj("/home/jkaspar/publications/2012/elastic scattering/publication2.root", "c1#1"), "l,ec", heavygreen, "EPL 96");
	limits((0, 1e-3), (0.8, 1e3), Crop);
	AttachLegend(format("dataset %i", dsi+1));
}

CShipout("dsdt_diag_cmp");

for (int dsi : dataSets.keys) {
	NewPad("$|t|\ung{GeV^2}$", "$\d\si/\d t$: ratio between diagonals");
	scale(Linear, Linear);
	rObject ratio = rGetObj("../comparisons/diagonal_cmp_"+dataSets[dsi]+".root", "output");
	draw(ratio, blue);
	limits((0, 0.8), (0.4, 1.2), Crop);
	xaxis(YEquals(1, false), dashed);
	AttachLegend(format("dataset %i", dsi+1));
}

CShipout("dsdt_diag_ratio");

NewPad("$|t|\ung{GeV^2}$", "$\d\si/\d t$: DS2 / DS1");
draw(rGetObj("../comparisons/dataset_cmp.root", "ds2_over_ds1"), "vl,ec", blue);
limits((0, 0.8), (0.4, 1.2), Crop);
	xaxis(YEquals(1, false), dashed);

NewPad("$|t|\ung{GeV^2}$", "$\d\si/\d t$: DS3 / DS1");
draw(rGetObj("../comparisons/dataset_cmp.root", "ds3_over_ds1"), "vl,ec", blue);
limits((0, 0.8), (0.4, 1.2), Crop);
	xaxis(YEquals(1, false), dashed);

CShipout("dsdt_dataset_ratio");

for (int dsi : dataSets.keys) {
	NewPad("$|t|\ung{GeV^2}$", "$\d\si/\d t \ung{mb/GeV^2}$");
	scale(Linear, Log);
	draw(rGetObj("/home/jkaspar/publications/2012/elastic scattering/publication2.root", "c1#1"), "l,ec", heavygreen, "EPL 96");
	draw(rGetObj(basedir+dataSets[dsi]+"/normalize_merged.root", "cross section/output"), "vl,ec", magenta, "diag. merged");
	draw(rGetObj("/home/jkaspar/publications/2012/elastic scattering/publication1.root", "tc#1"), "vl,ec", black, "EPL 95");
	limits((0, 1e-3), (0.8, 1e3), Crop);
	AttachLegend(format("dataset %i", dsi+1));
}

CShipout("dsdt_merged_cmp");
