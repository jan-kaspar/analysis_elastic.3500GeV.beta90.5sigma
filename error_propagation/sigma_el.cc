void sigma_el()
{
	vector<double> le, he, err;

	le.push_back(0.007); he.push_back(0.02); err.push_back(1.04);
	le.push_back(0.02); he.push_back(0.11); err.push_back(0.86);
	le.push_back(0.11); he.push_back(0.15); err.push_back(1.16);
	le.push_back(0.15); he.push_back(0.25); err.push_back(4.45);
	le.push_back(0.25); he.push_back(0.35); err.push_back(8.28);
	le.push_back(0.35); he.push_back(0.40); err.push_back(12.26);

	double B = 20.;
	
	double fs = 0.;
	for (unsigned int i = 0; i < le.size(); i++) {
		double f = (exp(-B*le[i]) - exp(-B*he[i]));
		fs += f;
	}

	double ess = 0.;
	double ofs = 0.;

	for (unsigned int i = 0; i < le.size(); i++) {
		double f = (exp(-B*le[i]) - exp(-B*he[i]));
		double of = f / fs;
		double ee = of * err[i];

		ofs += of;
		//ess += ee*ee;
		ess += fabs(ee);

		printf("inverval %u (from %.3f to %.3f): f = %.4f, err = %.2f, err * f = %.3f\n", i, le[i], he[i], of, err[i], ee);
	}

	//double fe = sqrt(ess);
	double fe = ess;

	printf(">> sum of fractions: %.3f\n", fs);
	printf(">> sum of observed fractions: %.3f\n", ofs);
	printf(">> final error: %.2f\n", fe);
}
