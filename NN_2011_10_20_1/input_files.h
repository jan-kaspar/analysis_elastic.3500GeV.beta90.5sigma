#include <string>
#include <vector>

//----------------------------------------------------------------------------------------------------

std::vector<std::string> input_files;

void InitInputFiles()
{
	input_files.clear();

	//std::string prefix = "rfio:///castor/cern.ch/totem/offline/Analysis/2012/T1T2RP/inelastic_reco/ver3.4_test/validation/";
	std::string prefix = "/mnt/lhcanalysis/2012/validation/";

	input_files.push_back(prefix + "val6884/val6884_totem_ntuple.root");
	input_files.push_back(prefix + "val6885/val6885_totem_ntuple.root");
	input_files.push_back(prefix + "val6887/val6887_totem_ntuple.root");
	input_files.push_back(prefix + "val6895/val6895_totem_ntuple.root");
	input_files.push_back(prefix + "val6896/val6896_totem_ntuple.root");	// short
	input_files.push_back(prefix + "val6897/val6897_totem_ntuple.root");
	input_files.push_back(prefix + "val6899/val6899_totem_ntuple.root");
	input_files.push_back(prefix + "val6900/val6900_totem_ntuple.root");
	input_files.push_back(prefix + "val6903/val6903_totem_ntuple.root");
	input_files.push_back(prefix + "val6904/val6904_totem_ntuple.root");
	input_files.push_back(prefix + "val6905/val6905_totem_ntuple.root");
	input_files.push_back(prefix + "val6908/val6908_totem_ntuple.root");	// short
	input_files.push_back(prefix + "val6909/val6909_totem_ntuple.root");
	input_files.push_back(prefix + "val6910/val6910_totem_ntuple.root");
	input_files.push_back(prefix + "val6911/val6911_totem_ntuple.root");
	input_files.push_back(prefix + "val6912/val6912_totem_ntuple.root");
	input_files.push_back(prefix + "val6913/val6913_totem_ntuple.root");
	input_files.push_back(prefix + "val6914/val6914_totem_ntuple.root");
	input_files.push_back(prefix + "val6915/val6915_totem_ntuple.root");
	input_files.push_back(prefix + "val6916/val6916_totem_ntuple.root");	// short
	input_files.push_back(prefix + "val6917/val6917_totem_ntuple.root");
	input_files.push_back(prefix + "val6918/val6918_totem_ntuple.root");
	input_files.push_back(prefix + "val6919/val6919_totem_ntuple.root");
	input_files.push_back(prefix + "val6920/val6920_totem_ntuple.root");
	input_files.push_back(prefix + "val6921/val6921_totem_ntuple.root");
	input_files.push_back(prefix + "val6922/val6922_totem_ntuple.root");
	input_files.push_back(prefix + "val6923/val6923_totem_ntuple.root");
	input_files.push_back(prefix + "val6924/val6924_totem_ntuple.root");
	input_files.push_back(prefix + "val6925/val6925_totem_ntuple.root");
	input_files.push_back(prefix + "val6926/val6926_totem_ntuple.root");
	input_files.push_back(prefix + "val6927/val6927_totem_ntuple.root");
	input_files.push_back(prefix + "val6929/val6929_totem_ntuple.root");
	
	// RPs moving !!
	//input_files.push_back(prefix + "val6930/val6930_totem_ntuple.root");
}
