import root;
import pad_layout;

NewPad("${x^{*R} - x^{*L}\over \sqrt 2\ \si(x^*)}$", 8cm, 6cm);
scale(Linear, Log);
draw(rGetObj("plot.root", "c1|hb_cq7"), "vl", black, "data");
draw(rGetObj("plot.root", "c1|f_sig"), "def", heavygreen, "signal fit");
draw(rGetObj("plot.root", "c1|f_bckg"), "def", blue, "background fit");
draw(rGetObj("plot.root", "c1|f_both"), "def", red, "signal+bckg.~fit");
limits((-6, 1e-1), (6, 1e4), Crop);
AttachLegend(S+0.01E, S);
