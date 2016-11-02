import root;
import pad_layout;

//----------------------------------------------------------------------------------------------------

string datasets[] = { "NN_2011_10_20_1", "NN_2011_10_20_2", "NN_2011_10_20_3" };

//string diagonals[] = { "45b_56t", "45t_56b" };
//string diagonals_long[] = { "45 bottom - 56 top", "45 top - 56 bottom" };
string diagonals[] = { "merged" };
string diagonals_long[] = { "both diagonals" };

string topDir = "../";

xSizeDef = 8cm;

//----------------------------------------------------------------------------------------------------

real A_ref = 508.7, B_ref = 19.98;

string ref_str = format("%.1f ", A_ref) + format("\,\exp(-%.2f\,|t|)", B_ref);

//----------------------------------------------------------------------------------------------------

void DrawRelDiff(transform t = identity(), rObject o, pen p, string label="")
{
	if (o.InheritsFrom("TH1"))
	{
		int N = o.iExec("GetNbinsX");
		for (int i = 1; i <= N; ++i)
		{
			real xc = o.rExec("GetBinCenter", i);
			real xw = o.rExec("GetBinWidth", i);
	
			real y = o.rExec("GetBinContent", i);
			real y_unc = o.rExec("GetBinError", i);
	
			real y_ref = A_ref * exp(-B_ref * xc);
	
			real y_rel = (y - y_ref) / y_ref;
			real y_rel_unc = y_unc / y_ref;
	
			draw(t * ((xc-xw/2, y_rel)--(xc+xw/2, y_rel)), p+0.1pt);	
			draw(t * ((xc, y_rel-y_rel_unc)--(xc, y_rel+y_rel_unc)), p+0.1pt);	
			draw(t * (xc, y_rel), mCi+1pt+p);
		}
	}

	if (o.InheritsFrom("TGraph"))
	{
		guide g_u, g_b;
		
		int N = o.iExec("GetN");
		for (int i = 1; i <= N; ++i)
		{
			real xa[] = {0.};
			real ya[] = {0.};
			o.vExec("GetPoint", i, xa, ya);
			real x = xa[0];
			real y = ya[0];
	
			real y_unc = o.rExec("GetErrorY", i);
	
			real y_ref = A_ref * exp(-B_ref * x);
	
			real y_rel = (y - y_ref) / y_ref;
			real y_rel_unc = y_unc / y_ref;
	
			//draw(t * ((x, y_rel-y_rel_unc)--(x, y_rel+y_rel_unc)), p+0.1pt);	
			//draw(t * (x, y_rel), mCi+1pt+p);
			g_u = g_u -- (x, y_rel+y_rel_unc);
			g_b = g_b -- (x, y_rel-y_rel_unc);
		}

		g_b = reverse(g_b);
		filldraw(t*(g_u--g_b--cycle), p, nullpen);
	}

	if (o.InheritsFrom("TF1"))
	{
		guide g;

		real t_max = o.rExec("GetXmax");

		for (real t=0; t <= t_max; t += 0.01)
		{
			real y = o.rExec("Eval", t);
			real y_ref = A_ref * exp(-B_ref * t);
			real y_rel = (y - y_ref) / y_ref;
			g = g--(t, y_rel);
		}

		draw(g, p);
	}


	AddToLegend(label, p);
}

//----------------------------------------------------------------------------------------------------

frame fLegend;

for (int dsi : datasets.keys)
{
	NewRow();

	string dataset = datasets[dsi];

	NewPad(false);
	label("{\SetFontSizesXX "+replace(datasets[dsi], "_", "\_")+"}");

	for (int dgni : diagonals.keys)
	{
		string f = topDir+dataset+"/normalize_"+diagonals[dgni]+".root";

		NewPad("$|t|\ung{GeV^2}$", "$\d\si/\d t \ung{mb/GeV^2}$");
		scale(Linear, Log);	
		currentpad.xTicks = LeftTicks(0.05, 0.01);

		draw(rGetObj(f, "cross section/fit1"), "eb", black);
		draw(rGetObj(f, "cross section/fit1|ff1"), "def", red+opacity(0.7)+2pt);

		limits((0, 1e-1), (0.3, 1e3), Crop);

		// ------------------------------

		NewPad("$|t|\ung{GeV^2}$", "$(\d\si/\d t - \hbox{ref}) / \hbox{ref}\ ,\ \ \hbox{ref} = "+ref_str+"$");
		currentpad.xTicks = LeftTicks(0.05, 0.01);
		currentpad.yTicks = RightTicks(0.05, 0.01);

		DrawRelDiff(rGetObj(f, "cross section/fit1"), black);
		DrawRelDiff(rGetObj(f, "cross section/fit1|ff1"), red+opacity(0.7)+2pt);
		
		limits((0, -0.15), (0.22, 0.15), Crop);
		xaxis(YEquals(0, false), dotted);
	}

	/*
	fLegend = BuildLegend();
	currentpicture.legend.delete();
	AttachLegend(diagonals_long[di], NW, NW);
	
	NewPad(false);
	add(fLegend);
	*/
}

