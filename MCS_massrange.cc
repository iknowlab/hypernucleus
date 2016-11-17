#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

// 関数：プロトタイプ宣言
double MainProgram(double, double, int, double, double);
void MCScalc(const char *write_filename, double Range, double Step, double IZ, double RR, double D, double Mass, double Meas_Error);
void writerc(int, double, double, double, double, double);

//Sub関数：物理量の計算
double TotalEnergy(double Mass , double KE){
	double E = Mass + KE;
	return E;
}

double Momentum(double Mass, double KE){
	double E = TotalEnergy(Mass,KE);
	return sqrt(E * E - Mass * Mass);
}
double Beta(double Mass, double KE){
	double P = Momentum(Mass,KE);
	double E = TotalEnergy(Mass,KE);
	return P / E;
}

//エラー出力関数
int readerr(const char *filename){
	std::cerr << "read file error! (" << filename << ")" << std::endl;
	exit(2);
}

//散乱角θ0を計算
double Theta0(double Mass, double KE, double z, double t, double Density){

	double X0 = 29690.*(3.6/Density);
	// Radiation Length of ET-7C/D emulsion
	
	double P = Momentum(Mass,KE);
	double B = Beta(Mass,KE);
	double Theta_plane_rms = 13.6*z*sqrt(t/X0)*(1+0.038*log(t/X0))/(P*B);
	return Theta_plane_rms;
}

int main(int argc, char *argv[])
{
	/*********************************************************************************************/
	/*    Range-Energy Calculation :   by K.NAKAZAWA                                             */
	/*    Translated the original Fortran_file "er31.txt" & "re31.txt" to C_file "er31.c"        */
	/*		by Y.Ariga , Toho-u, fundamental physics laboratory in 10, 2009 .                      */
	/*		selected freely mass value.																				*/
	/*    by T.Inoh , Toho-u, particle physics lab. in 10, 2015.										*/
	/*********************************************************************************************/

	/* how to */
	if(argc<2)
	{
		std::cerr << " use )\tMCS_massrange pid.conf" << std::endl;
		std::cerr << "PID Config File Is Not Found." << std::endl;
		std::cerr << "Dialogic Operation Mode..." << std::endl;
	}//if

	/* Program Range -> Energy */

	int IA,IZ;
	int k=0,a;
	double Mass,RR,D;
	double M_Min,M_Max,Step,Range,MeasError;
	char write_filename[2048];
	int N_Mass = 1;
	std::vector<double> Mass_Bin;
	std::ifstream prm;
	std::string tag,str;
	double param;
	int prmtrg=0;
	int prm_i;
	
	/* value */
	RR = 1.0;
	D = 3.60;
//	D = 3.25;

/* read parameter */
	if(argc==2){
		std::cerr << "Read " << argv[1] << "..." << std::endl;

		prm.open(argv[1]);
		if(prm.fail()){
			std::cerr << "cant open \"" << argv[1] << "\" !" << std::endl;
			argc = 1;
		}
		while(prm && getline(prm,str)){
			if(str.compare("[ParticleInfo]")==0){
				while(getline(prm,str) && !str.empty()){
					std::stringstream ss(str);
					ss >> tag >> param;
	//				std::cerr << tag << " " << param <<std::endl;
					if(tag.compare("Z_Charge")==0)IZ = (int)param;
					else if(tag.compare("M_Min")==0)M_Min = param;
					else if(tag.compare("M_Max")==0)M_Max = param;
					else if(tag.compare("Step")==0)Step = param;
					else if(tag.compare("Range")==0)Range = param;
				}//while
				prmtrg++;		
			}//if
		
			if(str.compare("[Mass_Num]")==0){
				prm_i=0;
				while(getline(prm,str) && !str.empty()){
					std::stringstream ss(str);
					ss >> prm_i >> param;
					Mass_Bin.push_back(param);
					prm_i++;
				}//while
				prmtrg++;
				N_Mass=prm_i;
			}//if
		
			if(str.compare("[Meas._Error]")==0){
				while(getline(prm,str) && !str.empty()){
					std::stringstream ss(str);
					ss >> tag >> param;
					if(tag.compare("MeasErr")==0)MeasError = param;
				}//while
				prmtrg++;
			}//if

			if(prmtrg == 3)break;
		}//while

		if(prmtrg!=3){
			std::cerr << "read err \"pid.conf\" !" << std::endl;
			argc = 1;//argcを減らす(false)
		}//if

	}//if
	if(argc<2){
		/* insert datum */	
		std::string h,ho,hog,hoge,ha,hag,hage;	

		std::cerr << "Abs:Z(charge)?" << std::endl;
		std::cin >> hoge;
		IZ = atoi(hoge.c_str());
	
		std::cerr << "Is a number of Mass one or else ?" << std::endl;
		std::cerr << "one:0\telse:1" << std::endl;
		std::cin >> h;
		a=atoi(h.c_str());

		if(a==0){
			std::cerr << "Mass (MeV) ?" <<std::endl;
			std::cin >> ho;
			M_Min = atof(ho.c_str());
			M_Max = atof(ho.c_str());
		}//if
		else{
			std::cerr << "Mass_Min?(MeV)" <<std::endl;
			std::cin >> ho;
			M_Min = atof(ho.c_str());
			std::cerr << "Mass_Max?(MeV)" <<std::endl;
			std::cin >> hog;
			M_Max = atof(hog.c_str());
		}//else

		std::cerr << "MCS Step ?" << std::endl;
		std::cin >> ha;
		Step = atof(ha.c_str());

		std::cerr << "MCS Range ?" << std::endl;
		std::cin >> hag;
		Range = atof(hag.c_str());
	
		std::cerr << "Measurement Error ?" << std::endl;
		std::cerr << "ex) ariga_mode -> 0.76\ninoh_mode -> 0.62" << std::endl;
		std::cin >> hage;
		MeasError = atof(hage.c_str());
	
	/* calc per 50MeV/c^2 */

		if(pow(M_Max - M_Min,2.) > 2500){
		N_Mass = (int)( M_Max - M_Min )/(double)50 + 1;
		}

		for(int i=0;i<N_Mass;i++){
			Mass_Bin.push_back(M_Min + (double)i * 50.0);
//			std::cerr << "debugmode: " << Mass_bin[i] <<std::endl;
		}
		Mass_Bin[N_Mass-1] = M_Max;

		/* specific mass flag */
		double P_flag[4];
		for(int i=0;i<4;i++)P_flag[i]=0.;
		if(M_Max>	139.57	&&	139.57	>M_Min)P_flag[0]=139.57;//pion
		if(M_Max>	493.677	&&	493.677	>M_Min)P_flag[1]=493.677;//kaon
		if(M_Max>	938.27	&&	938.27	>M_Min)P_flag[2]=938.27;//proton
		if(M_Max>	1321.31	&&	1321.31	>M_Min)P_flag[3]=1321.31;//xi

	/* 8<--------------- write file -------------->8 */
	/* calc (specific mass only) */
		if(P_flag[0]!=0)MCScalc("mcRMS_0139.dat",Range,Step,IZ,RR,D,P_flag[0],MeasError);
		if(P_flag[1]!=0)MCScalc("mcRMS_0493.dat",Range,Step,IZ,RR,D,P_flag[1],MeasError);
		if(P_flag[2]!=0)MCScalc("mcRMS_0938.dat",Range,Step,IZ,RR,D,P_flag[2],MeasError);
		if(P_flag[3]!=0)MCScalc("mcRMS_1321.dat",Range,Step,IZ,RR,D,P_flag[3],MeasError);

	}//if
	/* read parameter end */

	/* 8<--------------- write file -------------->8 */
	// 散乱計算結果

	for(int j=0;j<N_Mass;j++)
	{
		/* various mass */
		sprintf(write_filename,"mcRMS_%04d.dat",(int)Mass_Bin[j]);
		
		MCScalc(write_filename,Range,Step,IZ,RR,D,Mass_Bin[j],MeasError);

	}//for

	/* notice end of program */
	std::cout << "RMSfile is made." << std::endl;

	/* if you need... */
	if(argc<2)writerc(IZ ,M_Min ,M_Max ,Step ,Range, MeasError);

	return 0;
}//main end

void MCScalc(const char *write_filename,double Range,double Step,double IZ,double RR,double D,double Mass,double Meas_Error){
	
	double visE=0.;
	double KE,E,P,B,rms;
	double EndRange;
	char linebuffer[2048];

	std::ofstream writing_file;
	writing_file.open(write_filename);
	if(writing_file.fail())readerr(write_filename);

		EndRange=Range;
		Range=Step*2.0;
		if(EndRange <= 0.)readerr("zeroRange");

		for (Range=Range; Range<=EndRange; Range=Range+Step) {
			double r = 0.0;
			double r0 = 0.0;
			double KE0 = 0.0;
			double dKE = 10.0;
			double Range_m = Range - Step;
			KE = 0.0;
			
			if(Mass > 0.){

				while (dKE > 0.00005) {
					if(Range_m > r0) 
						KE += dKE;
					else
						KE -= dKE;
				
					r = MainProgram(Mass, KE, IZ, RR, D);
				
					if ((Range_m-r0)*(Range_m-r0) <= (r-r0)*(r-r0)) {
						dKE /= 100.;
						KE = KE0 + (KE-KE0)*(Range_m-r0)/(r-r0);
						r = MainProgram(Mass, KE, IZ, RR, D);
					}//if
					
					r0 = r;
					KE0 = KE;
				}//while
				
				visE = visE + KE + 8.;
				E = TotalEnergy(Mass,KE);
				P = Momentum(Mass,KE);
				B = Beta(Mass,KE);
				
				// Scattering : 垂直成分における位置変位
				rms = tan(Theta0(Mass, KE, IZ, Step*2., D))*(Step*2.)/((double)(2.*sqrt(3)));
				
				/* add Measurement Error */
//				rms = sqrt(rms*rms + Meas_Error*Meas_Error);
/* 乱数出力に影響を及ぼすため、測定誤差の追加をやめる。 */

				/* write */
				sprintf(linebuffer,"%lf\t%.0f\t%lf\n", Mass, Range, rms);
				writing_file << linebuffer;
			}//if
		}//for
//	printf("2>  mass: %.3f ,\tKE: %.4f MeV,\tP: %.2f MeV/c,\tBeta: %.5f,\tdelta_plane_RMS: %lf\n",Mass,KE,P,B,rms);
//	printf("> Total Visible Energy :  %.2f\n",visE);
	
	writing_file.close();

}

// 運動エネルギー -> Range
double MainProgram(double Mass, double KE, int IZ, double RR, double D){
	double CF=1.00,E,P,R,Range,B,Rp,FX,Cz;
	double KEM,D0=3.815,Rs,Rw,F,Mp=938.272;
	double R1,R2,CPS=0.99512,CPM=0.9726;
	double LKEM,KKEM;
	
	KEM = KE / Mass;
	E = TotalEnergy(Mass,KE);
	P = Momentum(Mass,KE);
	B = Beta(Mass,KE);
	LKEM = log10(KEM);
	
	if(KEM < 0.0001)
		Rs = 479.210*pow(KEM,0.675899);
	else{
		Rs = 6.05595+1.38639*LKEM -0.302838*LKEM*LKEM;
		KKEM = LKEM*LKEM*LKEM;
		Rs = Rs - 0.0602134*KKEM +0.0359347*KKEM*LKEM;
		KKEM = KKEM*LKEM*LKEM;
		Rs = Rs + 0.0195023*KKEM +0.00348314*KKEM*LKEM;
		KKEM = KKEM*LKEM*LKEM;
		Rs = Rs + 0.000185264*KKEM;
		Rs = pow(10.0,Rs);
	}
	if(KEM <0.0001){
		Rw = 6.2813+1.5342*LKEM-0.15997*LKEM*LKEM;
		Rw = Rw - 0.025245*LKEM*LKEM*LKEM;
		Rw = Rs + pow(10.0,Rw);
	}
	else{
		Rw = 6.47455+1.41016*LKEM -0.275959*LKEM*LKEM;
		KKEM = LKEM*LKEM*LKEM;
		Rw = Rw + 0.00837953*KKEM +0.0989985*KKEM*LKEM;
		KKEM = KKEM*LKEM*LKEM;
		Rw = Rw + 0.0495298*KKEM +0.0102625*KKEM*LKEM;
		KKEM = KKEM*LKEM*LKEM;
		Rw = Rw +0.000765385*KKEM;
		Rw = pow(10.0,Rw);
	}
	
	F = ( RR*D-1.) / ( RR*D0-1.) + RR * ( D0-D ) * Rs / ( RR*D0-1.)/ Rw;
	Rp = Rs / F;
	if( IZ > 1.0){
		FX = 137.0 * B / IZ;
		if(FX < 0.4)
			Cz = -0.0891805 * FX + 0.301246 * FX*FX;
		else if(FX <= 2.32)
			Cz = -0.02923 + 0.14894 * FX + 0.011031*FX*FX - 0.012393*FX*FX*FX;
		else
			Cz = 0.220931;
	}
	else
		Cz = 0.0;
	
	R1 = CPS*Rp/(IZ*IZ)/Mp*Mass;
	R2 = CPM*Mass/Mp*Cz*pow(IZ,(2/(double)3));
	R = R1 + R2;
	Range = R * CF;
	
	return Range;
}

void writerc(int Z, double min, double max, double Step, double Range, double measerr){
	double checkbin;
	int n,trg;

	n = (int)(max - min)/(double)50 + 1;
	
	if(min<	139.57	&&	139.57	<max)n++;
	if(min<	493.677	&&	493.677	<max)n++;
	if(min<	938.27	&&	938.27	<max)n++;
	if(min<	1321.31	&& 1321.31	<max)n++;

	std::ofstream writefile;
	writefile.open("pid.conf");
	if(writefile.fail()){
		std::cerr<< "error!" <<std::endl;
		exit(2);
	}
	
	writefile << "[ParticleInfo]" << std::endl;
	writefile << "Z_Charge " << Z << std::endl;
	writefile << "M_Min " << min << std::endl;
	writefile << "M_Max " << max << std::endl;
	writefile << "Step " << Step << std::endl;
	writefile << "Range " << Range << std::endl;
	writefile << std::endl;

	writefile << "[Mass_Num]" << std::endl;

	double massbin[n];
	trg=0;
	if(n==1)writefile << "0" << " " << (int)min << std::endl;
	else{
		for(int i=0;i<n;i++){
			massbin[i] = min + (double)(i-trg)*50.;
			checkbin = min + (double)(i+1-trg)*50.;
			writefile << i << " " << massbin[i] << std::endl;
			if(massbin[i]<139.57 && 139.57 <checkbin){
				i++;
				massbin[i]=139.57;
				writefile << i << " " << (int)massbin[i] << std::endl;
				trg++;
			}
			if(massbin[i]<493.677 && 493.677 <checkbin){
				i++;
				massbin[i]=493.677;
				writefile << i << " " << (int)massbin[i] << std::endl;
				trg++;
			}
			if(massbin[i]<938.27 && 937.27 <checkbin){
				i++;
				massbin[i]=938.27;
				writefile << i << " " << (int)massbin[i] << std::endl;
				trg++;
			}
			if(massbin[i]<1321.31 && 1331.31 <checkbin){
				i++;
				massbin[i]=1321.31;
				writefile << i << " " << (int)massbin[i] << std::endl;
				trg++;
			}
		}
	}//else
	writefile << "\n[Meas._Error]" << std::endl;
	writefile << "MeasErr\t" << measerr << std::endl;

	writefile.close();

	std::cerr << "pid.conf file is made !" << std::endl;
}
