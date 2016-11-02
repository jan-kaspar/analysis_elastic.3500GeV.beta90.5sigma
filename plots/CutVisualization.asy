import root;
import pad_layout;

string basedir = "../";
string dataSets[] = { "NN_2011_10_20_1", "NN_2011_10_20_2", "NN_2011_10_20_3" };
real t_min[] = {7.3e-3, 5.7e-3, 4.6e-3};

TH2_palette = Gradient(blue, heavygreen, yellow, red);

string cqa[] = {"\th_x^{*R}", "\th_y^{*R}", "\th_x^{*R}", "\th_x^{*L}",           "y^{R,N}",           "y^{L,N}", "\th_x^*"};
string cqb[] = {"\th_x^{*L}", "\th_y^{*L}",     "x^{*R}",     "x^{*L}", "y^{R,F} - y^{R,N}", "y^{L,F} - y^{L,N}", "x^{*R} - x^{*L}"};

real sca[] = {1e6, 1e6, 1e6, 1e6, 1, 1, 1e6};
real scb[] = {1e6, 1e6, 1e3, 1e3, 1, 1, 1e3};

string una[] = {"\mu rad", "\mu rad", "\mu rad", "\mu rad", "mm", "mm", "\mu rad" };
string unb[] = {"\mu rad", "\mu rad", "\mu m", "\mu m", "mm", "mm", "\mu m" };

real rfa[] = {-200,    0, -200, -200,   0, -30, -200};
real rta[] = {+200, +200, +200, +200, +30,   0, +200};

real rfb[] = {-200,    0, -650, -650,  0, -3, -30};
real rtb[] = {+200, +200, +650, +650, +3,  0, +30};

real ri[] = {+1, -1, +1, +1, -1, -1, +1};


for (int dsi : dataSets.keys) {
	write(">> " + dataSets[dsi]);

	for (int i = 1; i <= 7; ++i) {
		NewPad(false);
		label(format("cut %i", i));
		
		NewPad("$"+cqa[i-1]+"\ung{"+una[i-1]+"}$", "$"+cqb[i-1]+"\ung{"+unb[i-1]+"}$");
		scale(Linear, Linear, Log);
		draw(scale(sca[i-1], scb[i-1]), rGetObj(basedir+dataSets[dsi]+"/distributions_45b_56t.root", "elastic cuts/"+format("plot_after_cq%i#0", i)), "def");
		draw(scale(sca[i-1], scb[i-1]), rGetObj(basedir+dataSets[dsi]+"/distributions_45b_56t.root", "elastic cuts/"+format("plot_after_cq%i#1", i)), "def");
		draw(scale(sca[i-1], scb[i-1]), rGetObj(basedir+dataSets[dsi]+"/distributions_45b_56t.root", "elastic cuts/"+format("plot_after_cq%i#2", i)), "def");
		limits((rfa[i-1], rfb[i-1]), (rta[i-1], rtb[i-1]), Crop);
		
		NewPad("$"+format("cq_{%i}", i)+"$");
		draw(scale(scb[i-1], 1), rGetObj(basedir+dataSets[dsi]+"/distributions_45b_56t.root", format("elastic cuts/h_cq%i", i)), "vl,ec,lM,lR", blue);
		AttachLegend("45 B -- 56 T");
		
	
		//-----
		
		NewPad("$"+cqa[i-1]+"\ung{"+una[i-1]+"}$", "$"+cqb[i-1]+"\ung{"+unb[i-1]+"}$");
		scale(Linear, Linear, Log);
		draw(scale(sca[i-1], scb[i-1]), rGetObj(basedir+dataSets[dsi]+"/distributions_45t_56b.root", "elastic cuts/"+format("plot_after_cq%i#0", i)), "def");
		draw(scale(sca[i-1], scb[i-1]), rGetObj(basedir+dataSets[dsi]+"/distributions_45b_56t.root", "elastic cuts/"+format("plot_after_cq%i#1", i)), "def");
		draw(scale(sca[i-1], scb[i-1]), rGetObj(basedir+dataSets[dsi]+"/distributions_45b_56t.root", "elastic cuts/"+format("plot_after_cq%i#2", i)), "def");
		if (ri[i-1] == +1)
			limits((rfa[i-1], rfb[i-1]), (rta[i-1], rtb[i-1]), Crop);
		else
			limits((-rta[i-1], -rtb[i-1]), (-rfa[i-1], -rfb[i-1]), Crop);
		
		NewPad("$"+format("cq_{%i}", i)+"$");
		draw(scale(scb[i-1], 1), rGetObj(basedir+dataSets[dsi]+"/distributions_45t_56b.root", format("elastic cuts/h_cq%i", i)), "vl,ec,lM,lR", red);
		AttachLegend("45 T -- 56 B");
		
		NewRow();
	}

	Shipout("CutVisualization_"+dataSets[dsi]);
}
