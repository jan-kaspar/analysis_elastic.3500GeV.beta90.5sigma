/** Code by Jan Welti **/

/*
#include "TotemAnalysis/TotemNtuplizer/interface/T1Event.h"
#include "TotemAnalysis/TotemNtuplizer/interface/TriggerDataFormats.h"
#include "TotemAnalysis/TotemNtuplizer/interface/RawDataFormats.h"
#include "TotemAnalysis/T2MakeNtples/interface/T2Event.h"

#include "TTree.h"
#include "TH1.h"
#include "TH2F.h"
#include "TH1F.h"
#include "TCanvas.h"
*/

#include "TFile.h"
#include "TGraph.h"

#include <map>
#include <cstdlib>
#include <fstream>

/*
#include <iostream>
#include <bitset>
#include <cstring>
#include <string.h>

std::ofstream daqout;
std::ofstream lumiout;

bool firstfile=true;
bool lastfile=false;
bool lastcorrupted=false;

std::vector<double> intlumi;
std::vector<double> length;
std::vector<double> daqeff;
*/

std::map<unsigned int,double> lumimap0;			// big
std::map<unsigned int,double> lumimap891;		// big non-colliding
std::map<unsigned int,double> lumimap1785;		// big non-colliding
std::map<unsigned int,double> lumimap1900;		// below small colliding
std::map<unsigned int,double> lumimap2050;
std::map<unsigned int,double> lumimap2250;
std::map<unsigned int,double> lumimap2450;

TGraph *g_lumi_0;
TGraph *g_lumi_0_corr;			// with pedestal removed
TGraph *g_lumi_891;	
TGraph *g_lumi_1785;
TGraph *g_lumi_1900;
TGraph *g_lumi_2050;
TGraph *g_lumi_2250;
TGraph *g_lumi_2450;


const double x = 0.40; // 40+-3 % of sum of non-colliding bunch lumi should be subtracted from measured lumi (luminosity pedestal)

/*
char filename[80];

unsigned int timefirst=0;
unsigned int timelast=0;

int trueevent=0;
int daqevent=0;
int firstevent=0;
int firstdaq=0;
double triggereventfirst=0;
double triggereventlast=0;
double triggernumfirst=0;
double triggernumlast=0;

int events=0;

double fulllumi=0;
*/

int PrepareRun(const char *suffix = "")
{
	/*
    sprintf(filename,"lumi%s.txt", suffix);
    lumiout.open(filename);
	
    sprintf(filename,"daq%s.txt", suffix);
    daqout.open(filename);
	*/
	
	g_lumi_0 = new TGraph();
	g_lumi_0_corr = new TGraph();
	g_lumi_891 = new TGraph();
	g_lumi_1785 = new TGraph();
	g_lumi_1900 = new TGraph();
	g_lumi_2050 = new TGraph();
	g_lumi_2250 = new TGraph();
	g_lumi_2450 = new TGraph();

	g_lumi_0->SetName("g_lumi_0");
	g_lumi_0_corr->SetName("g_lumi_0_corr");
	g_lumi_891->SetName("g_lumi_891");
	g_lumi_1785->SetName("g_lumi_1785");
	g_lumi_1900->SetName("g_lumi_1900");
	g_lumi_2050->SetName("g_lumi_2050");
	g_lumi_2250->SetName("g_lumi_2250");
	g_lumi_2450->SetName("g_lumi_2450");
    
    // Prepare luminosity maps
    //ifstream lumidb("/afs/cern.ch/user/j/jwelti/TriggerEff/AllOct/Modular/fill_2232lumiDB_lumibylsXing.csv");
    ifstream lumidb("/afs/cern.ch/exp/totem/scratch/jkaspar/software/offline/424/user/elastic_analysis/low_t/fill_2232lumiDB_lumibylsXing.csv");

    unsigned int count=0;
    std::string value1,value2,value3,value4,value5,value6,value7,value8,value9,value10,value11,value12,value13,value14,value15,value16,value17;

    getline(lumidb,value1,'\n');
    while(lumidb.good()) {
        count++;
        getline(lumidb,value1,',');
        getline(lumidb,value2,',');
        getline(lumidb,value3,',');
        getline(lumidb,value4,',');
        getline(lumidb,value5,',');
        getline(lumidb,value6,',');
        getline(lumidb,value7,',');
        getline(lumidb,value8,',');
        getline(lumidb,value9,',');
        getline(lumidb,value10,',');
        getline(lumidb,value11,',');
        getline(lumidb,value12,',');
        getline(lumidb,value13,',');
        getline(lumidb,value14,',');
        getline(lumidb,value15,',');
        getline(lumidb,value16,',');
        getline(lumidb,value17,'\n');
        if (atoi(value1.c_str())==0)
			break;

		unsigned int idx = atoi(value1.c_str());
        lumimap0[idx] = atof(value2.c_str());
        lumimap891[idx] = atof(value3.c_str());
        lumimap1785[idx] = atof(value4.c_str());
        lumimap1900[idx] = atof(value5.c_str());
        lumimap2050[idx] = atof(value8.c_str());
        lumimap2250[idx] = atof(value12.c_str());
        lumimap2450[idx] = atof(value16.c_str());
        
		g_lumi_0->SetPoint(g_lumi_0->GetN(), idx, lumimap0[idx]);
		g_lumi_891->SetPoint(g_lumi_891->GetN(), idx, lumimap891[idx]);
		g_lumi_1785->SetPoint(g_lumi_1785->GetN(), idx, lumimap1785[idx]);
		g_lumi_1900->SetPoint(g_lumi_1900->GetN(), idx, lumimap1900[idx]);
		g_lumi_2050->SetPoint(g_lumi_2050->GetN(), idx, lumimap2050[idx]);
		g_lumi_2250->SetPoint(g_lumi_2250->GetN(), idx, lumimap2250[idx]);
		g_lumi_2450->SetPoint(g_lumi_2450->GetN(), idx, lumimap2450[idx]);
		
		g_lumi_0_corr->SetPoint(g_lumi_0_corr->GetN(), idx, lumimap0[idx] - x*(lumimap891[idx] + lumimap1785[idx]));
    }  
    
    return 0;
}

#if 0
int EndRun(const char *suffix)
{
	double effLumiSum = 0.;

    std::cout << "* Integrated luminosities of periods: " << std::endl;
    for (unsigned int i=0; i<intlumi.size(); i++) {
        std::cout << "    " << intlumi[i] << std::endl;
        //lumiout << intlumi[i] << std::endl;
        fulllumi+=intlumi[i];
    }
    std::cout << "* Full integrated luminosity: " << fulllumi << std::endl;

    std::cout << "\n* Lengths of periods (s): " << std::endl;
    double fulltime=0;
    for (unsigned int i=0; i<length.size(); i++) {
        std::cout << "    " << length[i] << std::endl;
        fulltime+=length[i];
    }
    std::cout << "\n* DAQ efficiencies: " << std::endl;
    double meandaqeff=0;
    for (unsigned int i=0; i<daqeff.size(); i++) {
        std::cout << "    " << daqeff[i] << std::endl;
        //daqout << daqeff[i] << std::endl;
        meandaqeff+=daqeff[i]*length[i];

		effLumiSum += intlumi[i] * daqeff[i];
    }
    std::cout << "Mean DAQ eff: " << meandaqeff/fulltime << std::endl;

	printf("\n* effective luminosity sum: %E\n", effLumiSum);

    //lumiout.close();
    //daqout.close();

    return 0;
}
#endif

double CalculateLumi(unsigned int firsttime, unsigned int lasttime)
{
    double lumi = 0;
    int n = 0;
    firsttime+=710; // difference between time in DAQ and CMS lumi measurement is 710 s
    lasttime+=710;

    std::map<unsigned int,double>::iterator low=lumimap0.lower_bound(firsttime);
    std::map<unsigned int,double>::iterator high=lumimap0.upper_bound(lasttime);
    
    for (int i=0; i<3; i++) { // shift edges in order to do some averaging especially to get a more reasonable estimate for short periods
        low--;
        high++;
    }    

    for(std::map<unsigned int,double>::iterator i=low; i!=high; i++) {
        lumi+=i->second;
        lumi-=x*lumimap891[i->first];
        lumi-=x*lumimap1785[i->first];
        n++;
    }

    lumi/=n;

    lumi*=lasttime-firsttime;

    return lumi;
}

#if 0
int AnalyzeFile(const char *filein)
{
    TFile *f;
    std::cout << "Opening file " << filein << "..." << std::endl;
    f = TFile::Open(filein);
    if(!f) {
        std::cout << "File NOT opened!" << std::endl;
        firstfile=true;
        lastfile=false;
        if(!lastcorrupted) {
            intlumi.push_back(CalculateLumi(timefirst,timelast));
            std::cout << "Integrated luminosity: " << CalculateLumi(timefirst,timelast) << std::endl;
            daqeff.push_back((double)(triggereventlast-triggereventfirst)/(triggernumlast-triggernumfirst));
            length.push_back(timelast-timefirst);
            std::cout << "DAQ efficiency: " << (double)(triggereventlast-triggereventfirst)/(triggernumlast-triggernumfirst) << std::endl;
        }
        lastcorrupted=true;
        return 1;
    }
    std::cout << "File opened!" << std::endl;

    TTree *t1 = (TTree*)f->Get("TotemNtuple");
    if(!t1) {
        std::cout << "Cannot find Ntuple!" << std::endl;
        return 1;
    }

    T2Event *the_t2ev=new T2Event();
    TBranch *t2branch  = t1->GetBranch("branchT2EV.");
    if(!t2branch) {
        std::cout << "Cannot find Ntuple!" << std::endl;
        return 1;
    }
    t2branch->SetAddress(&the_t2ev);
    t2branch->GetEntry(1);

    T1Event *the_t1ev=new T1Event();
    TBranch *t1branch  = t1->GetBranch("branchT1EV.");
    if(!t1branch) {
        std::cout << "Cannot find Ntuple!" << std::endl;
        return 1;
    }
    t1branch->SetAddress(&the_t1ev);
    t1branch->GetEntry(1);

    TriggerData *trig = new TriggerData();
    TBranch *trigbranch = t1->GetBranch("trigger_data.");
    if(!trigbranch) {
        std::cout << "Cannot find Ntuple!" << std::endl;
        return 1;
    }
    trigbranch->SetAddress(&trig);
    trigbranch->GetEntry(0);

    EventMetaData *evinfo = new EventMetaData();
    TBranch *evbranch = t1->GetBranch("event_info.");
    if(!evbranch) {
        std::cout << "Cannot find Ntuple!" << std::endl;
        return 1;
    }
    evbranch->SetAddress(&evinfo);
    evbranch->GetEntry(0);

    if(firstfile) {
        timefirst = evinfo->timestamp;
        firstfile=false;
        firstdaq = evinfo->daq_event_number;
        firstevent = evinfo->event_no;
        triggereventfirst = trig->event_num;
        triggernumfirst = trig->trigger_num;
    }
    evbranch->GetEntry(evbranch->GetEntries()-1);
    trigbranch->GetEntry(trigbranch->GetEntries()-1);
    timelast = evinfo->timestamp;
    daqevent = evinfo->daq_event_number;
    trueevent = evinfo->event_no;
    triggereventlast = trig->event_num;
    triggernumlast = trig->trigger_num;

    if (lastfile) {
        firstfile=true;
        lastfile=false;
        intlumi.push_back(CalculateLumi(timefirst,timelast));
        std::cout << "Integrated luminosity: " << CalculateLumi(timefirst, timelast) << std::endl;
        daqeff.push_back((double)(triggereventlast-triggereventfirst)/(triggernumlast-triggernumfirst));
        length.push_back(timelast-timefirst);
        std::cout << "DAQ efficiency: " << (double)(triggereventlast-triggereventfirst)/(triggernumlast-triggernumfirst) << std::endl;
        lastcorrupted=true;
    } else
		lastcorrupted=false;

	cout.flush();

	delete f;
    return 0;
}
#endif
