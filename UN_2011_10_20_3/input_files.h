#include <string>
#include <vector>

//----------------------------------------------------------------------------------------------------

std::vector<std::string> input_files;

void InitInputFiles()
{
	input_files.clear();
	
	std::string prefix = "rfio:///castor/cern.ch/user/j/jkaspar/RP_reco/ntuples/";
	//std::string prefix = "/mnt/lhcanalysis/2012/validation/";

	input_files.push_back(prefix + "6944_ntuple.root");
	input_files.push_back(prefix + "6945_ntuple.root");
	input_files.push_back(prefix + "6947_ntuple.root");
}
