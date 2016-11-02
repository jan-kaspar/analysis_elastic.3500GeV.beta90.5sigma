#include <string>
#include <vector>

//----------------------------------------------------------------------------------------------------

std::vector<std::string> input_files;

void InitInputFiles()
{
	input_files.clear();

	std::string prefix = "rfio:///castor/cern.ch/totem/offline/Analysis/2011/Physics/October/v_3.6_reco_v_3.6/";
	//std::string prefix = "/mnt/lhcanalysis/2012/validation/";
	
	input_files.push_back(prefix + "val6936/val6936_totem_ntuple.root");
	input_files.push_back(prefix + "val6938/val6938_totem_ntuple.root");
	input_files.push_back(prefix + "val6940/val6940_totem_ntuple.root"); // RPs stop working within this run ?? VERIFY !!
	input_files.push_back(prefix + "val6941/val6941_totem_ntuple.root");
	input_files.push_back(prefix + "val6942/val6942_totem_ntuple.root");
	input_files.push_back(prefix + "val6943/val6943_totem_ntuple.root"); // here data for one diagonal only ?? VERIFY !!
}
