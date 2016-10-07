#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

/* made 2016.06.17 inoh */
/* this prgrm makes config(.conffile) */
/* for mc_mcs_identification */

int main(){
	
	double max,min,checkbin;
	double Step,Range;
	int Z,n,trg,k;
	std::string h,ho,hog,hoge,ha,hag,hage;	

	std::cerr << "Abs:Z(charge)?" << std::endl;
	std::cin >> hoge;
	Z = atoi(hoge.c_str());
	
	while(1){
		std::cerr << "What is a number of Mass ?" << std::endl;
		std::cerr << "many:0\tone:1\ttwo(LikelihoodRatio-mode):2" << std::endl;
		std::cin >> h;
		k=atoi(h.c_str());

		if(k==1){
			std::cerr << "Mass (MeV) ?" <<std::endl;
			std::cin >> ho;
			min = atof(ho.c_str());
			max = atof(ho.c_str());
			break;
		}
		else if(k==0){
			std::cerr << "Mass_Min?(MeV)" <<std::endl;
			std::cin >> ho;
			min = atof(ho.c_str());
			std::cerr << "Mass_Max?(MeV)" <<std::endl;
			std::cin >> hog;
			max = atof(hog.c_str());
			break;
		}
		else if(k==2){
			std::cerr << "1st Mass(light)[MeV]" << std::endl;
			std::cin >> ho;
			min = atof(ho.c_str());
			std::cerr << "2nd Mass(heavy)[MeV]" << std::endl;
			std::cin >> hog;
			max = atof(hog.c_str());
			break;
		}
		else continue;
	}//while

	std::cerr << "MCS Step ?" << std::endl;
	std::cin >> ha;
	Step = atof(ha.c_str());
	
	std::cerr << "MCS Range ?" << std::endl;
	std::cin >> hag;
	Range = atof(hag.c_str());

	/* define n */
	n = (int)(max - min)/(double)50 + 1;
	if(min<	139.57	&&	139.57	<max)n++;
	if(min<	493.677	&&	493.677	<max)n++;
	if(min<	938.27	&&	938.27	<max)n++;
	if(min<	1321.31	&& 1321.31	<max)n++;
	if(k==2)n=2;

	std::ofstream writefile;
	writefile.open("pid.conf");
	if(writefile.fail()){
		std::cerr<< "error!" <<std::endl;
		exit(2);
	}
	
	writefile << "[ParticleInfo]" << std::endl;
	writefile << "Z_Charge\t" << Z << std::endl;
	writefile << "M_Min\t" << min << std::endl;
	writefile << "M_Max\t" << max << std::endl;
	writefile << "Step\t" << Step << std::endl;
	writefile << "Range\t" << Range << std::endl;
	writefile << std::endl;

	writefile << "[Mass_Num]" << std::endl;

	double massbin[n];
	trg=0;
	if(n==1||k==2)writefile << "0\t" << (int)min << std::endl;
	if(k==2 && n==2)writefile << "1\t" << (int)max << std::endl;
	else if(k!=2){
		for(int i=0;i<n;i++){
			massbin[i] = min + (double)(i-trg)*50.;
			checkbin = min + (double)(i+1-trg)*50.;
			writefile << i << "\t" << massbin[i] << std::endl;
			if(massbin[i]<139.57 && 139.57 <checkbin){
				i++;
				massbin[i]=139.57;
				writefile << i << "\t" << (int)massbin[i] << std::endl;
				trg++;
			}
			if(massbin[i]<493.677 && 493.677 <checkbin){
				i++;
				massbin[i]=493.677;
				writefile << i << "\t" << (int)massbin[i] << std::endl;
				trg++;
			}
			if(massbin[i]<938.27 && 937.27 <checkbin){
				i++;
				massbin[i]=938.27;
				writefile << i << "\t" << (int)massbin[i] << std::endl;
				trg++;
			}
			if(massbin[i]<1321.31 && 1331.31 <checkbin){
				i++;
				massbin[i]=1321.31;
				writefile << i << "\t" << (int)massbin[i] << std::endl;
				trg++;
			}
		}//for
	}//else

	writefile << "\n[Meas._Error]" <<std::endl;
	std::cerr << "imput measurement error..." << std::endl;
	std::cerr << "ex) ariga_mode -> 0.76\ninoh_mode -> 0.60" << std::endl;
	std::cin >> hage;
	writefile << "MeasErr\t" << hage << std::endl;

writefile.close();
return 0;
}//main

