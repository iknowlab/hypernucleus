/*g3関数は未使用、補正は無*/
/* 2016.10.7 */

#include <cstdio>
#include <random>
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>

using namespace std;
typedef struct{
	double mass,range,ssdx,theta;
} struct_rmsdata;

void howto(){
	std::cerr << "exe rmsfile" << std::endl;
	std::cerr << "1)\tG4rms.awk makes rmsfile" << std::endl;
	std::cerr << "2)\tMCS_massrange makes rmsfile" << std::endl;
	exit(1);
}

void readerr(const char *filename){
	std::cerr << "read file error! (" << filename << ")" << std::endl;
	exit(1);
}

double Read_Param(const char *need){
	std::ifstream prm;
	std::string tag,str;
	double param;
	prm.open("pid.conf");

   if(!prm.fail()){
      while(prm && getline(prm,str)){
         if(str.compare("[ParticleInfo]")==0){
            while(getline(prm,str) && !str.empty()){
               std::stringstream ss(str);
					ss >> tag >> param;
					if(tag.compare(need)==0){
						prm.close();
						return param;
					}//if
				}//while
			}//if
			if(str.compare("[Meas._Error]")==0){
				while(getline(prm,str) && !str.empty()){
					std::stringstream ss(str);
					ss >> tag >> param;
					if(tag.compare(need)==0){
						prm.close();
						return param;
					}//if
				}//while
			}//if
		}//while
	}//if
	else{
		std::cerr << "Imput " << need << " ";
		std::cin >> str;
		param = atof(str.c_str());
		return param;
	}//else
	prm.close();
	return param;
}

double f(double th, double a){
	double d = 2.;
	double b = (d - 3. + a )/a;
	double u0_th = acos(1.-3/a);
	double g1 = exp(-a*(1.-cos(u0_th)));
	double g2 = 1./pow(b-cos(u0_th),d);
	//std::cerr << g2/g1 << std::endl;

	if( (th*th) < (u0_th*u0_th) )
	{
		return g2*(double)exp(-a*(1.-cos(th)))/(g1+g2);
	}
	else
	{
		return (double)g1/((g1+g2)*pow(b-cos(th),d));
	}
}

/* oOoOoOoOoOoOoOoOo--------------- monte carlo simulation ----------------oOoOoOoOoOoOoOoOo */	
class MCS_Distribution{
private:
	double th_lim = (double)M_PI;
	double eps = 0.0001;
	double m_a;
	double m_range;
	double m_mass;
	double m_step;
	double m_error;
//	double cell_length = 29215.3;/* 29215.3micron */

public:
	double th;
	double rms;
	MCS_Distribution (double a,double range, double mass, double step, double error)
		: m_a(a)
		, m_range(range)
		, m_mass(mass)
		, m_step(step)
		, m_error(error)
	{
		vector<double> i;
		vector<double> v;
		ofstream writing;
		mt19937 gen;
		char linebuffer[1024];
		for(double x=-th_lim; x<th_lim; x+=eps){
			i.push_back(x);
			v.push_back(f(x,a));
		}/* for end */

		std::piecewise_linear_distribution<> dist(i.begin(), i.end(), v.begin());
	
		sprintf(linebuffer, "./ssd/ssd_%05d_%04d.dat",(int)m_range,(int)m_mass);
		writing.open(linebuffer);
		if(!writing)readerr(linebuffer);

		rms = 0;

		for(int j=0; j<10000; j++){
			th = dist(gen);
			double dx = (2.* m_step)*(double)th/sqrt(3.);
			dx /= 2.;

			normal_distribution<double> gauss(0,error);
			dx += gauss(gen);

			rms += dx*dx;

			writing << "test" << j <<"\t" << m_step << "\t" << dx << "\t0.0\t0.0\t" << th << std::endl;
		}/* for  */
\
		writing.close();
	}
};/* class end */

int main(int argc, char *argv[])
{
	if(argc<2)howto();
	double step,error,dx,theta_p,cutval,a,inv_a,rmsout;
	int i,j,k;
	std::string RF,write_line_buffer;
	std::ifstream reading_file[argc-1];
	std::ofstream writing_rms;
	struct_rmsdata data[128][argc-1];

	step = Read_Param("Step");
	error = Read_Param("Meas._Err");

	for(i=0;i<argc-1;i++){/* read file */
		reading_file[i].open(argv[i+1]);
		if(reading_file[i].fail())readerr(argv[i+1]);
		j=0;
		std::cerr << "read " << argv[i+1] << "..." << std::endl;

		while(reading_file[i] && getline(reading_file[i], RF))
		{
			sscanf(RF.c_str(),"%lf %lf %lf %lf",&data[j][i].mass,&data[j][i].range,&data[j][i].ssdx);
			data[j][i].theta = data[j][i].ssdx * (double)sqrt(3)/step;
			j++;
		}
		reading_file[i].close();
	}/* read file end  */

	int max_j = j;
	double rmssum[j][argc-1];
	double progress;

	std::cerr << "commandline: mkdir ssd" << std::endl;
	system("mkdir ssd");

	for(i=0;i<argc-1;i++){
		for(j=0;j<max_j;j++)
		{
			inv_a = 2.0*(1.0-cos(data[j][i].theta));
			a = (double)1./inv_a;
			/* a, range, mass, step, error */
			MCS_Distribution mcsd(a,data[j][i].range,data[j][i].mass,step,error);
			rmssum[j][i] = mcsd.rms;
			progress = 100.*(i*(max_j) + (j+1))/((argc-1)*(max_j));
			fprintf(stderr, "%2.0lf%\r", progress);
//			std::cerr << "test\t" << mcsd.th << std::endl;
		}
	}

	/* RMSfile will be edited */	
	writing_rms.open("RMS.dat");
	if(writing_rms.fail())readerr("RMS.dat");

	for(j=0;j<max_j;j++){
		double RMS_byRange = 0.;
		for(i=0;i<argc-1;i++)RMS_byRange += rmssum[j][i];/* For function by Mass */
		RMS_byRange = sqrt(RMS_byRange/((double)(argc-1)*10000.0));
//		cerr << data[j][0].range << "\t" << RMS_byRange << endl;
		/* Range:RMS in file */
		writing_rms << data[j][0].range << "\t" << RMS_byRange << endl;
	}/* For function by Range END */

	return 0;
}
