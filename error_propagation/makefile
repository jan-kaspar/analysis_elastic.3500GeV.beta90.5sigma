all: prop_dist prop_deriv bayes

prop_dist: prop_dist.cc common.h formulae.h
	g++ `root-config --libs` -lMinuit `root-config --cflags` -O3 prop_dist.cc -o prop_dist

prop_deriv: prop_deriv.cc common.h formulae.h
	g++ `root-config --libs` -lMinuit `root-config --cflags` -O3 prop_deriv.cc -o prop_deriv

bayes: bayes.cc common.h
	g++ `root-config --libs` -lMinuit `root-config --cflags` -O3 bayes.cc -o bayes
