/*g3関数は未使用、補正は無*/
/* 2016.9.21 */

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

void howto(char **argv){
	std::cerr << argv[0] <<" RMSfile" << std::endl;
	std::cerr << "method 1)\tMCS_massrange -> RMSfile" << std::endl;
	std::cerr << "method 2)\tG4rms.awk -> RMSfile" << std::endl;
	cerr << "need config \"step, meas.err\"" << endl;
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
	double d = 2;
	double b = (d - 3. + a)/a;
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
//		return (double)g1/((g1+g2)*pow((1.+pow(u0_th,(double)1./d))-cos(th),d));
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
	vector<double> i;
	vector<double> v;
	MCS_Distribution (double a,double range, double mass, double step, double error)
		: m_a(a)
		, m_range(range)
		, m_mass(mass)
		, m_step(step)
		, m_error(error)
	{
		//ofstream writing;
		mt19937 gen;
		char linebuffer[1024];
		for(double x=-th_lim; x<th_lim; x+=eps){
			i.push_back(x);
			v.push_back(f(x,a));
		}/* for end */

		//sprintf(linebuffer, "./ssd/ssd_%05d_%04d.dat",(int)m_range,(int)m_mass);
		//writing.open(linebuffer);
		//if(!writing)readerr(linebuffer);
		//writing.close();
	}
};/* class end */

int main(int argc, char *argv[])
{
	if(argc<2)howto(argv);
	double step,error,dx;
	double inv_a,a;
	double th,rms;
	int i,j;
	mt19937 gen;
	string RF;
	ifstream reading_file[argc-1];
	struct_rmsdata data[128][argc-1];

	step = Read_Param("Step");
	error = Read_Param("Meas._Err");
	normal_distribution<double> gauss(0,error);

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

	MCS_Distribution *mcsd[max_j];
	piecewise_linear_distribution<double> *dist[max_j];

	for(i=0;i<argc-1;i++){
		for(j=0;j<max_j;j++)
		{
			inv_a = 2.0*(1.0-cos(data[j][i].theta));
			a = (double)1./inv_a;
			/* a, range, mass, step, error */
			mcsd[j] = new MCS_Distribution(a,data[j][i].range,data[j][i].mass,step,error);
			dist[j] = new piecewise_linear_distribution<double>(mcsd[j]->i.begin(), mcsd[j]->i.end(), mcsd[j]->v.begin());
		}
	}//for

	cerr << "pdf is made. now generating..." << endl;

	for(i=0;i<argc-1;i++){
		for(int mt=0;mt<1000;mt++){
			for(j=0;j<max_j;j++){
				th = (*dist[j])(gen);
				dx = (2.* step)*(double)tan(th)/sqrt(3.);
				dx /= 2.;
				dx += gauss(gen);
				cout << mt << "\t" <<data[j][i].range << "\t" <<dx << "\t0\t" << "0\t" << "000" << endl;
			}//for
			cout << "--------------------------------------------------" << endl;
		}
	}//for

	return 0;
}
