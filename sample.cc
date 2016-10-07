#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

/* make 2016.06.10 inoh */
/* PDG_gen_ssdで出力されるリファレンスの交差検証用ファイルを生成する */

struct reference {
	char ID[16];
	double range,dx,dy,dz;
};

typedef struct {
	/* particle mass */
	double mass;
	double range;
	/* (delta_X')_rms */
	double ssdx;
	/* theta_rms */
	double theta;
} struct_rmsdata;

/* return gaussian distribution random number */
double gaussdistro(double rms){
	double r;//random 0~1
	double ph;//phai 0~2pi
	double t;//inverse function (gaussian squared)
	double x;//result value

	r =(double)rand()/RAND_MAX;
	ph =(double)rand()/(RAND_MAX/(2*M_PI));
	
	/* {f(x)}^(-1) at f(x) = e^{(x^2+y^2)/rms} */
	t = rms * sqrt( -2.0 * log(r) );
	
	/* 2dimensions -> 1dimension */
	x = t*cos(ph);

	return x;
}

/* return 0~1 */
double RandomUniform(){
	double uni;
	return uni = 2.*(double)rand()/(RAND_MAX)-1.0;
}

void howto(){
	std::cerr << "exe SampleNumber(int) G4rms_?.dat" << std::endl;
	std::cerr << "G4rms.awk makes G4rms(_?).dat" << std::endl;
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

	/* ファイルを開くことに失敗しなければ(成功すれば) */
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
		}
	}//if
	else{
		std::cerr << "Imput " << need << " ";
		std::cin >> str;
		param = atof(str.c_str());
	}//else
	prm.close();

	return param;
}

int main(int argc, char *argv[]){

	/* how to */
	if(argc<2)howto();

	/* time random */
	srand((unsigned)time(NULL));
	
	/* read param(argv[1])*/
	int samplenum;
	samplenum = atoi(argv[1]);

	/* read param */
	double step,error;
	step = Read_Param("Step");
	error = Read_Param("MeasErr");
//	std::cerr << "debug mode: " << step << " " << error << std::endl;

	/* read */
	char linebuffer[1024];
	std::string RF;
	std::string write_line_buffer;
	std::ifstream reading_file[argc-2];/* argc = exe + addfile */
	
	/* rmsfile(datafile,filenum */
	struct_rmsdata rmsdata[128][argc-2];
	
	int i, j;

	/* read file */
	for(i=0;i<argc-2;i++){
		reading_file[i].open(argv[i+2]);
		if(reading_file[i].fail())readerr(argv[i+2]);
		j=0;
		std::cout << "read " << argv[i+1] << " ..." << std::endl;

	/* read a row */
		while(reading_file[i] && getline(reading_file[i], RF))
		{
			sscanf(RF.c_str(),"%lf %lf %lf",&rmsdata[j][i].mass,&rmsdata[j][i].range,&rmsdata[j][i].ssdx);
			rmsdata[j][i].theta = rmsdata[j][i].ssdx * (double)sqrt(3.)/step;
			j++;
		}//while
		reading_file[i].close();
	}//for

//	std::cerr << "debug mode: " << j << " mass: "<< rmsdata[j-1][i-1].mass << std::endl;
//	std::cerr << "debug mode: " << rmsdata[0][0].ssdx << " --> " << rmsdata[j-1][i-1].ssdx << std::endl;
	
	/* calc and write */
	std::ofstream writing_file[argc-2];	/* [rmssum] * [argc-1] */
	int k;
	int max_j=j;	/* memorize max_j */
	double dx,theta_p;
	char ssdID[64];
	char particleid[64];

	/* write samplefile */
	for(i=0;i<argc-2;i++){

		/* G4rms_?.dat -> the sixth letter "?" is inserted in %s */
		sprintf(linebuffer,"mc_sample_%04d.dlt",(int)rmsdata[i][0].mass);
		writing_file[i].open(linebuffer);
		if(writing_file[i].fail())readerr(linebuffer);

		k=0;

/* --------------- monte carlo simulation ---------------- */
		while(k<=samplenum)
		{
			for(j=0;j<max_j;j++)
			{
				/* gaussian random */
				rmsdata[j][i].theta = sqrt(3)*(double)rmsdata[j][i].ssdx/step;//convert theta
				theta_p = gaussdistro(rmsdata[j][i].theta);//theta plane

				/* delta = (1/2)y_plane = z1*x*theta/sqrt(12) + z2*x*theta/2 */
				dx = (2.*step)*(double)gaussdistro(rmsdata[j][i].theta)/sqrt(12.) 
					+ (2.*step)*(double)theta_p/2.;

				/* convert delta_plane from y_plane */
				dx = (double)dx/2.0;

				/* add measurement error */
				dx += gaussdistro(error);

				/* output samplefile */
				sprintf(ssdID,"%04d%05d",(int)rmsdata[j][i].mass,k);
				writing_file[i] << ssdID << "\t" << rmsdata[j][i].range << "\t" << dx << "\t0.0" << "\t0.0\t" << theta_p << "\t000"<< std::endl;
				
			}//for

			writing_file[i] << "----------------------------------------------------------" << std::endl;
			k++;
		
		}//while
//		std::cerr << "debug mode: " << rmsdata[j][i].range << rmssum[j][i] << std::endl;
		writing_file[i].close();
		std::cout << "writing " << linebuffer << " is end." << std::endl;

	}//for

	return 0;
}
