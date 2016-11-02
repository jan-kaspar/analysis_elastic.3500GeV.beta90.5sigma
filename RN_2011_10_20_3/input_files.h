#include <string>
#include <vector>

//----------------------------------------------------------------------------------------------------

std::vector<std::string> input_files;

void InitInputFiles()
{
	input_files.clear();
	
	std::string prefix = "rfio:///castor/cern.ch/totem/offline/Analysis/2011/Physics/October/v_3.6_reco_v_3.6/";
	//std::string prefix = "/mnt/lhcanalysis/2012/validation/";

	input_files.push_back(prefix + "val6944/val6944_totem_ntuple.root");
	input_files.push_back(prefix + "val6945/val6945_totem_ntuple.root");
	input_files.push_back(prefix + "val6947/val6947_totem_ntuple.root");
}
