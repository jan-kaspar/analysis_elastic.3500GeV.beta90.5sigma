void dump()
{
	TFile *inF = new TFile("6895.0000.root");
	TGraph *g = (TGraph *) inF->Get("Graph");
	g->Print();
}
