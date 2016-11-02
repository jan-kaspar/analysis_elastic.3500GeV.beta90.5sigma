#include <string>
#include <vector>

//----------------------------------------------------------------------------------------------------

std::vector<std::string> input_files;

void InitInputFiles()
{
	input_files.clear();

	std::string prefix = "rfio:///castor/cern.ch/user/j/jkaspar/RP_reco/ntuples/";
	//std::string prefix = "/mnt/lhcanalysis/2012/validation/";
	
	input_files.push_back(prefix + "6936_ntuple.root");
	input_files.push_back(prefix + "6938_ntuple.root");

	// RPs stop working within this run
	//input_files.push_back(prefix + "val6940/val6940_ntuple.root");

	input_files.push_back(prefix + "6941_ntuple.root");
	input_files.push_back(prefix + "6942_ntuple.root");

	// here data for one diagonal only
	//input_files.push_back(prefix + "6943_totem_ntuple.root");
}
