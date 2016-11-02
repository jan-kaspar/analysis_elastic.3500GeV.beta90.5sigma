import root;
import pad_layout;

string f_n = "mc_none.root";
string f_m = "mc_misal.root";
string f_o1 = "mc_optics:ypxp.root";
string f_o2 = "mc_optics:ypxm.root";
string f_mo1 = "mc_misal_optics:ypxp.root";

xSizeDef = 10cm;
ySizeDef = 10cm;

/*
NewPad("$|t|\ung{GeV^2}$", "acceptance correction");
draw(rGetObj("mc_optics:ypxp.root", "acc_corr_vs_t"), "d0,ec,vl", black);
draw(rGetObj("mc_optics:ypxp,smp.root", "acc_corr_vs_t"), "d0,ec,vl", red);
draw(rGetObj("mc_optics:ypxp,smm.root", "acc_corr_vs_t"), "d0,ec,vl", blue);
limits((0, 0), (0.4, +10), Crop);
yaxis(XEquals(0.1225, false), dotted);

NewPad("$|t|\ung{GeV^2}$", "acceptance correction error $\ung{\%}$");
draw(yscale(100), rGetObj("differences.root", "ac_s_smz"), "d0,ec,vl", black);
draw(yscale(100), rGetObj("differences.root", "ac_s_smp"), "d0,ec,vl", red);
draw(yscale(100), rGetObj("differences.root", "ac_s_smm"), "d0,ec,vl", blue);
limits((0, -0.5), (0.4, 0.5), Crop);
xaxis(YEquals(0, false), dashed);
yaxis(XEquals(0.1225, false), dotted);

NewRow();

NewPad("$|t|\ung{GeV^2}$", "smearing correction");
draw(yscale(100)*shift(0, -1), rGetObj("mc_optics:ypxp.root", "unsm_corr_ass"), black);
draw(yscale(100)*shift(0, -1), rGetObj("mc_optics:ypxp,smp.root", "unsm_corr_ass"), red);
draw(yscale(100)*shift(0, -1), rGetObj("mc_optics:ypxp,smm.root", "unsm_corr_ass"), blue);
limits((0, -10), (0.4, +10), Crop);
yaxis(XEquals(0.1225, false), dotted);

NewPad("$|t|\ung{GeV^2}$", "smearing correction error $\ung{\%}$");
draw(yscale(100), rGetObj("differences.root", "uc_s_smz"), "ec,vl", black);
draw(yscale(100), rGetObj("differences.root", "uc_s_smp"), "d0,ec,vl", red);
draw(yscale(100), rGetObj("differences.root", "uc_s_smm"), "d0,ec,vl", blue);
limits((0, -1), (0.4, +1), Crop);
xaxis(YEquals(0, false), dashed);
yaxis(XEquals(0.1225, false), dotted);

NewRow();

NewPad(false);

NewPad("$|t|\ung{GeV^2}$", "$t$-systemtatics -- effect in $\d\si/\d t\ung{\%}$", xTicks=LeftTicks(Step=0.05, step=0.01), yTicks=RightTicks(Step=2, step=1));
draw(yscale(100)*shift(0, -1), rGetObj(f_n, "ratio_uct"), black, "none");
draw(yscale(100)*shift(0, -1), rGetObj(f_m, "ratio_uct"), blue, "misal.");
draw(yscale(100)*shift(0, -1), rGetObj(f_o1, "ratio_uct"), red, "optics ($y$: $+1\si$, $x$: $+1\si$)");
draw(yscale(100)*shift(0, -1), rGetObj(f_mo1, "ratio_uct"), heavygreen, "misal.\ + optics ($y$: $+1\si$, $x$: $+1\si$)");
draw(yscale(100)*shift(0, -1), rGetObj(f_o2, "ratio_uct"), magenta, "optics ($y$: $+1\si$, $x$: $-1\si$)");
limits((0, -10), (0.4, 10), Crop);
yaxis(XEquals(0.2, false), dashed);
yaxis(XEquals(0.1225, false), dotted);
AttachLegend(SW, SW);

NewRow();

NewPad("$|t|\ung{GeV^2}$", "all contributions -- effect in $\d\si/\d t\ung{\%}$", xTicks=LeftTicks(Step=0.05, step=0.01), yTicks=RightTicks(Step=2, step=1));
draw(yscale(100)*shift(0, -1), rGetObj("mc_none.root", "ratio_uca"), black, "0");
draw(yscale(100)*shift(0, -1), rGetObj("mc_none,smp.root", "ratio_uca"), red, "+");
draw(yscale(100)*shift(0, -1), rGetObj("mc_none,smm.root", "ratio_uca"), blue, "-");
limits((0, -10), (0.4, 10), Crop);
yaxis(XEquals(0.2, false), dashed);
yaxis(XEquals(0.1225, false), dotted);
AttachLegend("none", SW, SW);

NewPad("$|t|\ung{GeV^2}$", "all contributions -- effect in $\d\si/\d t\ung{\%}$", xTicks=LeftTicks(Step=0.05, step=0.01), yTicks=RightTicks(Step=2, step=1));
draw(yscale(100)*shift(0, -1), rGetObj("mc_optics:ypxp.root", "ratio_uca"), black, "0");
draw(yscale(100)*shift(0, -1), rGetObj("mc_optics:ypxp,smp.root", "ratio_uca"), red, "+");
draw(yscale(100)*shift(0, -1), rGetObj("mc_optics:ypxp,smm.root", "ratio_uca"), blue, "-");
limits((0, -10), (0.4, 10), Crop);
yaxis(XEquals(0.2, false), dashed);
yaxis(XEquals(0.1225, false), dotted);
AttachLegend("optics:ypxp", SW, SW);


Shipout("makePlots");
*/

//--------------------------------------------------

NewPad("$|t|\ung{GeV^2}$", "mean of $(t^{\rm reco} - t^{\rm true}) / t^{\rm true}\ung{\%}$", xTicks=LeftTicks(Step=0.05, step=0.01), yTicks=RightTicks(Step=2, step=1));

draw(yscale(100), rGetObj("mc_none.root", "de_t_sm_rel_vs_t"), "vl,ec,d0", black, "without acceptance cut");
draw(yscale(100), rGetObj("mc_none.root", "de_t_re_rel_vs_t"), "vl,ec,d0", blue, "$t$-systematics: none");
draw(yscale(100), rGetObj("mc_optics:ypxp.root", "de_t_re_rel_vs_t"), "vl,ec,d0", red, "$t$-systematics: optics (x+, y+)");
limits((0, -2), (0.4, 8), Crop);
yaxis(XEquals(0.1225, false), dotted);
AttachLegend(NE, NE);

Shipout("dt_rel_vs_t");

//--------------------------------------------------

NewPad("$|t|\ung{GeV^2}$", "misalignment and optics systemtatics -- effect in $\d\si/\d t\ung{\%}$", xTicks=LeftTicks(Step=0.05, step=0.01), yTicks=RightTicks(Step=0.2, step=0.1));

draw(yscale(100)*shift(0, -1), rGetObj("mc_none.root", "ratio_uca"), black, "none");
draw(yscale(100)*shift(0, -1), rGetObj("mc_misal.root", "ratio_uca"), blue, "misal. (x+, y+)");
draw(yscale(100)*shift(0, -1), rGetObj("mc_optics:yp.root", "ratio_uca"), red, "optics (y+)");
draw(yscale(100)*shift(0, -1), rGetObj("mc_optics:xp.root", "ratio_uca"), heavygreen, "optics (x+)");
draw(yscale(100)*shift(0, -1), rGetObj("mc_enoff.root", "ratio_uca"), brown, "energy offset ($+0.1\%$ in both arms)");

limits((0, -0.7), (0.4, 1.3), Crop);
yaxis(XEquals(0.2, false), dashed);
yaxis(XEquals(0.1225, false), dotted);
AttachLegend(SW, SW);


NewPad("$|t|\ung{GeV^2}$", "misalignment and optics systemtatics -- effect in $\d\si/\d t\ung{\%}$", xTicks=LeftTicks(Step=0.05, step=0.01), yTicks=RightTicks(Step=2, step=1));

draw(yscale(100)*shift(0, -1), rGetObj("mc_none.root", "ratio_uca"), black, "none");
draw(yscale(100)*shift(0, -1), rGetObj("mc_optics:xp.root", "ratio_uca"), heavygreen, "optics (x+)");
draw(yscale(100)*shift(0, -1), rGetObj("mc_optics:ypxp.root", "ratio_uca"), orange, "optics (x+, y+)");
draw(yscale(100)*shift(0, -1), rGetObj("mc_misal_optics:ypxp.root", "ratio_uca"), magenta, "misal. (x+, y+), optics (x+, y+)");
draw(yscale(100)*shift(0, -1), rGetObj("mc_optics:ypxm.root", "ratio_uca"), olive, "optics (x-, y+)");

limits((0, -10), (0.4, 10), Crop);
yaxis(XEquals(0.2, false), dashed);
yaxis(XEquals(0.1225, false), dotted);
AttachLegend(SW, SW);

Shipout("t_syst");

//--------------------------------------------------

NewPad("$|t|\ung{GeV^2}$", "acceptance correction error -- effect in $\d\si/\d t\ung{\%}$");
draw(yscale(100), rGetObj("differences.root", "ac_s_smp"), "d0,ec,vl", red, "$\si(\th^*_y)$ = nom + 1RMS");
draw(yscale(100), rGetObj("differences.root", "ac_s_smz"), "d0,ec,vl", black, "$\si(\th^*_y)$ = nom");
draw(yscale(100), rGetObj("differences.root", "ac_s_smm"), "d0,ec,vl", blue, "$\si(\th^*_y)$ = nom - 1RMS");
limits((0, -0.5), (0.4, 0.5), Crop);
xaxis(YEquals(0, false), dashed);
yaxis(XEquals(0.1225, false), dotted);
AttachLegend("$t$-systematics: optics (x+, y+)");

Shipout("acc_cor");

//--------------------------------------------------

NewPad("$|t|\ung{GeV^2}$", "smearing correction error -- effect in $\d\si/\d t\ung{\%}$");
draw(yscale(100), rGetObj("differences.root", "uc_s_smp"), "d0,ec,vl", red, "$\si(\th^*_{x,y})$ = nom + 1RMS");
draw(yscale(100), rGetObj("differences.root", "uc_s_smz"), "ec,vl", black, "$\si(\th^*_{x,y})$ = nom");
draw(yscale(100), rGetObj("differences.root", "uc_s_smm"), "d0,ec,vl", blue, "$\si(\th^*_{x,y})$ = nom - 1RMS");
limits((0, -1), (0.4, +1), Crop);
xaxis(YEquals(0, false), dashed);
yaxis(XEquals(0.1225, false), dotted);

Shipout("unsm_cor");
