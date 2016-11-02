#include <string>
#include <vector>

//----------------------------------------------------------------------------------------------------

std::vector<std::string> input_files;

void InitInputFiles()
{
	input_files.clear();

	std::string prefix = "rfio:///castor/cern.ch/user/j/jkaspar/RP_reco/ntuples/";
	//std::string prefix = "/mnt/lhcanalysis/2012/validation/";

	input_files.push_back(prefix + "6884_ntuple.root");
	input_files.push_back(prefix + "6885_ntuple.root");
	input_files.push_back(prefix + "6887_ntuple.root");
	input_files.push_back(prefix + "6895_ntuple.root");
	input_files.push_back(prefix + "6896_ntuple.root");	// short
	input_files.push_back(prefix + "6897_ntuple.root");
	input_files.push_back(prefix + "6899_ntuple.root");
	input_files.push_back(prefix + "6900_ntuple.root");
	input_files.push_back(prefix + "6903_ntuple.root");
	input_files.push_back(prefix + "6904_ntuple.root");
	input_files.push_back(prefix + "6905_ntuple.root");
	input_files.push_back(prefix + "6908_ntuple.root");	// short
	input_files.push_back(prefix + "6909_ntuple.root");
	input_files.push_back(prefix + "6910_ntuple.root");
	input_files.push_back(prefix + "6911_ntuple.root");
	input_files.push_back(prefix + "6912_ntuple.root");
	input_files.push_back(prefix + "6913_ntuple.root");
	input_files.push_back(prefix + "6914_ntuple.root");
	input_files.push_back(prefix + "6915_ntuple.root");
	input_files.push_back(prefix + "6916_ntuple.root");	// short
	input_files.push_back(prefix + "6917_ntuple.root");
	input_files.push_back(prefix + "6918_ntuple.root");
	input_files.push_back(prefix + "6919_ntuple.root");
	input_files.push_back(prefix + "6920_ntuple.root");
	input_files.push_back(prefix + "6921_ntuple.root");
	input_files.push_back(prefix + "6922_ntuple.root");
	input_files.push_back(prefix + "6923_ntuple.root");
	input_files.push_back(prefix + "6924_ntuple.root");
	input_files.push_back(prefix + "6925_ntuple.root");
	input_files.push_back(prefix + "6926_ntuple.root");
	input_files.push_back(prefix + "6927_ntuple.root");
	input_files.push_back(prefix + "6929_ntuple.root");
	
	// RPs moving !!
	//input_files.push_back(prefix + "6930_ntuple.root");
}
