import root;
import pad_layout;

string f = "unfolding_45b_56t.root";
string fm = "unfolding_minus_45b_56t.root";
string fp = "unfolding_plus_45b_56t.root";
string fc = "unfolding_comparison.root";

NewPad("$|t|\ung{GeV^2}$", "unfolding correction$\ung{\%}$", yTicks=RightTicks(Step=2,step=1));
draw(scale(1., 100)*shift(0, -1), rGetObj(f, "cf,ub/corr_final"), "vl,ec,d0", black);
draw(scale(1., 100)*shift(0, -1), rGetObj(fp, "cf,ub/corr_final"), "vl,ec,d0", blue);
draw(scale(1., 100)*shift(0, -1), rGetObj(fm, "cf,ub/corr_final"), "vl,ec,d0", red);
limits((0, -10), (1, +2), Crop);

NewPad("$|t|\ung{GeV^2}$", "unfolding correction error$\ung{\%}$", yTicks=RightTicks(Step=0.2,step=0.1));
draw(scale(1., 100), rGetObj(fc, "hp"), "vl,ec,d0", blue);
draw(scale(1., 100), rGetObj(fc, "hm"), "vl,ec,d0", red);
limits((0, -0.8), (1, +0.8), Crop);
xaxis(YEquals(0, false), black);
