first: mc

mc: mc.cc
	g++ `root-config --libs` -lMinuit `root-config --cflags` -O3 -Wall mc.cc -o mc
