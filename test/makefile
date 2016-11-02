all: shower_test shower_test2 shower_test3

shower_test: shower_test.cc
	g++ `root-config --libs` -lMinuit `root-config --cflags` -O3\
		-L/afs/cern.ch/exp/totem/scratch/jkaspar/software/offline/424/lib/slc5_amd64_gcc434/ -lTotemAnalysisTotemNtuplizer\
		shower_test.cc -o shower_test

shower_test2: shower_test2.cc
	g++ `root-config --libs` -lMinuit `root-config --cflags` -O3\
		-L/afs/cern.ch/exp/totem/scratch/jkaspar/software/offline/424/lib/slc5_amd64_gcc434/ -lTotemAnalysisTotemNtuplizer\
		shower_test2.cc -o shower_test2

shower_test3: shower_test3.cc
	g++ `root-config --libs` -lMinuit `root-config --cflags` -O3\
		-L/afs/cern.ch/exp/totem/scratch/jkaspar/software/offline/424/lib/slc5_amd64_gcc434/ -lTotemAnalysisTotemNtuplizer\
		shower_test3.cc -o shower_test3
:wa
