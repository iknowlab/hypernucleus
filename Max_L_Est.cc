/* make inoh 2016.6.16 */
/* filename is fixed. use caution */
#include<sstream>
#include<fstream>
#include<iostream>
#include<stdlib.h>
#include<string>
#include<string.h>
#include<math.h>
#include<vector>

int usage(){
	std::cerr << "Usage : exe sample*.dlt ./LIKE/LogLikeli.dat" << std::endl;
	std::cerr << "needfile : ConfigFile, RMSdata, SampleFile, OutputFile," << std::endl;
	std::cerr << "dialogic operation mode..." <<std::endl;
	return 1;
}

void readerror(){
	std::cerr << "file error" << std::endl;
	exit(1);
}

void writeerror(){
	std::cerr << "write file error" << std::endl;
	exit(1);
}

typedef struct {
	char ID[32];
	double Range,dx,dy,dz,theta;
	} dlt;

typedef struct {
	char sampleid[64];
	} timpo;

int main(int argc, char **argv){
	
	if(argc<3){
		int nope = usage();
		std::cerr << "...not enough" << std::endl;
		exit(1);
	}//if

	int a=0;/* readflag */
	int i,j,k,l,m,n;
	int flag=0; /* eof flag*/
	int pljoint; /* sample.dlt */
	int index[256][128];/*Range:Mass*/
	
	/* RMS is divided this parameter */
	double divide = 5.0;
	
	double srange,sdx,sdy,sdz,stheta;
	double RMS[2][256];	/* RMS */
	char sid[32];
	char linebuffer[2048];	/* １行読み込み */
	dlt SDfile[256][128][16384];
	/* Particle RangeByStep Delta */
	timpo s_id[128][16384];

	/* sample read data */
	std::vector<std::string> line;
	//line.push_back();

/* 8<-------------------- read parameter -------------------->8*/	
	std::ifstream prm;
	std::string tag,str,hoge,hage,hageml;
	double massbin[128];/* Mass */
	double param,max,min,Step,Range,err,M_Min,M_Max;
	int Z;	
	int prmtrg=0;

//	std::cerr << "debug mode: " << argv[1] << std::endl;
	if(a!=1){/* when config file is not found, a equals "1" by howto() */

	/* open config file */
		prm.open("pid.conf");
		if(prm.fail()){
			std::cerr << "cant open \"pid config file\" !" << std::endl;
			a=usage();//argcを減らす(false)
		}//if
		while(prm && getline(prm,str))
		{
			if(a==1)break;
			if(str.compare("[ParticleInfo]")==0)
			{
				while(getline(prm,str) && !str.empty()){
					std::stringstream ss(str);
					ss >> tag >> param;
					if(tag.compare("Z_Charge")==0)Z = (int)param;
					else if(tag.compare("M_Min")==0)min = param;
					else if(tag.compare("M_Max")==0)max = param;
					else if(tag.compare("Step")==0)Step = param;
					else if(tag.compare("Range")==0)Range = param;
				}//while
				prmtrg++;		
			}//if
			if(str.compare("[Mass_Num]")==0){
				i=0;
				while(getline(prm,str) && !str.empty()){
					std::stringstream ss(str);
					ss >> i >> param;
					massbin[i] = param;
					i++;
				}//while
				prmtrg++;
				n=i;			
//				std::cerr << "debug mode: " << i-1 << ": " << massbin[i-1] <<std::endl;
			}//if
			if(str.compare("[Meas._Error]")==0){
				while(getline(prm,str) && !str.empty()){
					std::stringstream ss(str);
					ss >> tag >> param;
					if(tag.compare("MeasErr")==0)err = param;
				}//while
				prmtrg++;
//				std::cerr << "debug mode: " << "measerr " << param << std::endl;
			}//if
		}//while
		if(prmtrg!=3)
		{
			std::cerr << "read err \"pid cofing file\" !" << std::endl;
			a=1;//false
		}//else

		prm.close();
	}//if
	
	if(a==1){
		std::cerr << "Mass_Min (MeV) ?" << std::endl;
		std::cin >> hoge;
		M_Min = atof(hoge.c_str());
		std::cerr << "Mass_Max (MeV) ?" << std::endl;
		std::cin >> hage;
		M_Max = atof(hage.c_str());

		if(min<	139.57	&&	139.57	<max)n++;
		if(min<	493.677	&&	493.677	<max)n++;
		if(min<	938.27	&&	938.27	<max)n++;
		if(min<	1321.31	&& 1321.31	<max)n++;

		n = (int)(max - min)/(double)50 + 1;
		double checkbin;
		int trg=0;
		if(n==1)massbin[0]=(int)min;
		else{
			for(i=0;i<n;i++){
				massbin[i] = min + (double)(i-trg)*50.;
				checkbin = min + (double)(i+1-trg)*50.;
				if(massbin[i]<139.57 && 139.57 <checkbin){
					i++;
					massbin[i]=139.57;
					trg++;
				}
				else if(massbin[i]<493.677 && 493.677 <checkbin){
					i++;
					massbin[i]=493.677;
					trg++;
				}
				else if(massbin[i]<938.27 && 937.27 <checkbin){
					i++;
					massbin[i]=938.27;
					trg++;
				}
				else if(massbin[i]<1321.31 && 1331.31 <checkbin){
					i++;
					massbin[i]=1321.31;
					trg++;
				}
			}//for
		}//else
		
		/* Memorize Measurement Error */
		std::cerr << "Measurement Error ?" <<std::endl;
		std::cin >> hageml;
		err = atof(hageml.c_str());

	}//if

/* ------------------------ read parameter end -------------------------*/

	/* RMS.datは先のプログラムの全てreferenceの2次変位からRMSを得た物 */
	/* 尤度計算時の確率分布を作る際のbinは、このRMSの"1/5"の値を用いている。 */
	/* open to rms file to READ */
	std::string RF;

	/* for likelihood file */
	std::string write_line_buffer;

	/* RMS.dat */
	std::ifstream reading_file;
	reading_file.open("RMS.dat");
	if(reading_file.fail())readerror();
	
	/* open file to WRITE likelihood */
	std::ofstream writing_file;
	writing_file.open(argv[2]);
	if(writing_file.fail())writeerror();
	
	/* initialization */
	j=0;
	while(reading_file && getline(reading_file, RF))
	{

		sscanf(RF.c_str(),"%lf %lf",&RMS[0][j],&RMS[1][j]);
		RMS[1][j] /= divide;	/* divided by 5 */
		j++;

	}//while
	reading_file.close();

	/* debug */
//	std::cerr << "debug mode: " << RMS[1][0] << " ---- " << RMS[1][j-1] << std::endl;

	/* 8<-------------- ssd file read -------------->8 */
	std::cerr << "read ./ssd/ssd*.dat..." << std::endl;
	
	/* j:RangeNum n:MassNum */
	std::ifstream readssd[j][n];
	double rmsbin[j][n];
	int ssdnum = n;
	
	for(int typem=0;typem<ssdnum;typem++)
	{
		std::cerr << (int)massbin[typem] << "...\r";
		
		for(k=0;k<j;k++)
		{

			/* ./ssd/ssd_[range]_[mass].dat */
			sprintf(linebuffer,"./ssd/ssd_%05d_%04d.dat", (int)RMS[0][k], (int)massbin[typem]);

//			std::cerr << "debug mode: " << linebuffer << std::endl;
			
			readssd[k][typem].open(linebuffer);

			if(readssd[k][typem].fail())readerror();

			rmsbin[k][typem]=0.;

			l=0;
			
			while(readssd[k][typem] && getline(readssd[k][typem], RF))
			{
			  char buf[1024];
			  sscanf(RF.c_str(),"%*s %*lf %s %*lf %*lf %*lf", buf);
			  SDfile[k][typem][l].dx = atof(buf);
//				 SDfile[k][typem][l].ID, &SDfile[k][typem][l].Range, &SDfile[k][typem][l].dx, &SDfile[k][typem][l].dy, &SDfile[k][typem][l].dz,&SDfile[k][typem][l].theta);
				rmsbin[k][typem] += SDfile[k][typem][l].dx * SDfile[k][typem][l].dx;

				l++;

			}//while

			index[k][typem]=l; /* max l num */

			rmsbin[k][typem]=sqrt(rmsbin[k][typem] / (double)l );

			rmsbin[k][typem] /= 10.0;

			readssd[k][typem].close();

		}//for
	
	}//for

	/* debug */	
//	std::cerr << "debug mode: " << SDfile[0][ssdnum-1][index[0][ssdnum-1]-1].Range << "," << index[0][ssdnum-1]-1 << std::endl;

	/* open to READ sample file */
	std::cerr << "read " << argv[1] << "..." << std::endl;
	std::ifstream sampleread;
	sampleread.open(argv[1]);
	if(sampleread.fail())readerror();

	while(getline(sampleread,RF))line.push_back(RF);

	sampleread.close();

	/* 処理飛跡数を初期化 */

	/* sample line */
	int sl = 0;

	/* sample num */
	int count = 0;

	/* mass num */
	int mass =0;

	/* Likelihood delta X */
	double Lx = 1.;

	/* probability */
	int numer,denomin;

	/* 尤度保存用の行列を確保 */
	double likeli[ssdnum][16384];	/* MassNum,SampleNum */

	/* 質量データ数分の処理を開始 */
	while(1){

	/* initialization */
		sl = 0;
		count = 0;

	/* 飛跡数分の処理を開始 */
		while(sl<line.size())
		{
	/* 参照File番号を初期化(しきい線で管理される) */
			k = 0; /* set first range */
		
	/* 尤度の初期化 */
			Lx = 1.;
		
	/* １飛跡分の尤度を求める */
			while(sl<line.size()){
			
	/* しきい線ではループを終了する */
				if(strncmp(line[sl].c_str(),"--------------------------------------------------",49)==0){
					sl++;
					break;
				}//if
			
	/* １行分の2次変位を取得 */
				sscanf(line[sl].c_str(),"%s %lf %lf %lf %lf %d",sid,&srange,&sdx,&sdy,&sdz,&pljoint);//add theta
			
	/* plateの接続部分は無視する */
				if(pljoint == 111){
					k++;
					sl++;
					continue;/* read next line */
				}//if

	/* 確率計算に使うカウンタを初期化 */
				numer = 1;
				denomin = 1;
				m=0;

	/* 参照Fileの2次変位を読み込む */
				while(m < index[k][mass])
				{
				
	/* 母数を加算 */
					denomin++;
				
	/* 許容範囲内に含まれる2次変位を数える */
//					if((sdx>=SDfile[k][mass][m].dx-rmsbin[k][mass])&&(sdx<=SDfile[k][mass][m].dx+rmsbin[k][mass]))numer++;
					if((sdx>=SDfile[k][mass][m].dx-RMS[1][k])&&(sdx<=SDfile[k][mass][m].dx+RMS[1][k]))numer++;
//					if(sdx<SDfile[k][mass][m].dx && sdx>-SDfile[k][mass][m].dx)numer++;
//					std::cout << sdx << ":" << SDfile[k][mass][m].dx << ":" << RMS[1][k] <<std::endl;
				
	/* SDfileを1つ進める */
					m++;
			
				}//while
			
	/* 尤度を計算 */
				Lx = Lx*((double)numer/denomin);
//				std::cerr << Lx << std::endl;
			
	/* 次のFile番号へ */
				k++;
				sl++;
			
			}	/* while [１飛跡分の尤度を求める] end */

	/* 処理状況を表示 */
//			printf("%s\t%.10e\t%d\n",sid,Lx,k);
		
	/* Natural Logarythm of Likelihood value */
			double NLL;
			NLL = log(Lx);//Natural Log-likelihood log_e(Lx)
		
	/* 算出された尤度を出力 */
	//		sprintf(linebuffer,"%s\t%lf\t%d\n",sid,NLL,k);
	//		sprintf(linebuffer,"%s\t%.10e\t%d\n",sid,Lx,k);
	//		std::cout << linebuffer;
			likeli[mass][count]=NLL;
//			writing_file << linebuffer;		
			sprintf(s_id[mass][count].sampleid,"%s",sid);
			count++;

		}	/* while [飛跡数分の処理を開始] end */
	
	/* debug */		
//		std::cerr << "debug mode: " << s_id[mass][count-1].sampleid << ": " << (int)massbin[mass] << ": " << log(Lx) << std::endl;
	
	/* change mass number */
		mass++;
		if(mass == ssdnum)break;

	} /* while [質量データ数分の処理を開始] end */

	/* 8<------------- Maximum Likelihood Estimation ------------------->8 */
	std::cerr << "Calc Maximum Likelihood Estimation..." << std::endl;
	
	for(int smpl=0;smpl<count;smpl++){
	
	/* サンプル名を書き込む */
		writing_file << "[SampleName]\t"<<s_id[0][smpl].sampleid << std::endl;

	/* 尤度の最大値を初期化 */
		double maxlikeli,NLL_ErrMin,NLL_ErrMax,likelimass;
		double Mass_ErrMin=0.;
		double Mass_ErrMax=0.;

	/* 最低質量値での尤度を記録 */
		maxlikeli=likeli[0][smpl];

	/* 1つの飛跡分の最尤推定を行う:最大値の確定 */
		for(mass=0;mass<ssdnum;mass++){
	/* 2番目以降でなおかつ尤度が先の尤度を越える場合それを記録 */
			if(mass > 0 && maxlikeli < likeli[mass][smpl]){
				maxlikeli = likeli[mass][smpl];
				likelimass = massbin[mass];
			}//if

	/* ファイルに質量と尤度を書き込む */
			sprintf(linebuffer, "%4.2lf\t%lf", massbin[mass], likeli[mass][smpl]);
			writing_file << linebuffer << std::endl;
		}//for

	/* 改行(サンプル区切り) */
		writing_file << std::endl;

	/* 最大値から±1の質量値の確定 */
		for(mass=1;mass<ssdnum;mass++)
	/* 最大値から1/2を引いたものを越える場合、その一つ前の番号を記録 */
			if((maxlikeli - 0.5) < likeli[mass][smpl]){
				NLL_ErrMin = likeli[mass-1][smpl];
				/* 越えた値から越える1つ前の値で傾きを取る。越えた質量値から、傾きの逆数に0.5をかけた質量の変化量を引く */
//				Mass_ErrMin = massbin[mass] - (0.5 - (likeli[mass][smpl]-likeli[mass-1][smpl]))
//				* ((massbin[mass]-massbin[mass-1])/(likeli[mass][smpl]-likeli[mass-1][smpl]));
				Mass_ErrMin = massbin[mass-1];
				break;
			}//if and for-loop end

	/* 続きから */
		for(mass;mass<ssdnum;mass++)
	/* 最尤値を越えてなおかつ、最大値から1/2を引いたものを下回った場合、その番号を記録しループから抜ける */
			if(likelimass < massbin[mass] && (maxlikeli - 0.5) > likeli[mass][smpl]){
				NLL_ErrMax = likeli[mass-1][smpl];
				/* 越えた値から越える1つ前の値で傾きを取る。越えた質量値から、傾きの逆数に0.5をかけた質量の変化量を引く */
//				Mass_ErrMax = massbin[mass] - (0.5 - (likeli[mass][smpl]-likeli[mass-1][smpl])) 
//				* ((massbin[mass]-massbin[mass-1])/(likeli[mass][smpl]-likeli[mass-1][smpl]));
				Mass_ErrMax = massbin[mass];
				break;
			}//if and for-loop end

	/* 記録した最尤値における尤度、質量値、質量誤差をそれぞれアウトプットする */
		sprintf(linebuffer, "%s\t%lf\t%lf\t%.0lf\t%.0lf",s_id[0][smpl].sampleid,maxlikeli,likelimass,likelimass-Mass_ErrMin,Mass_ErrMax-likelimass);
		std::cout << linebuffer << std::endl;
		//writing_file << linebuffer << std::endl;

	}//for
	
	/* maximum likelihood estimation end */

	/* close file */
	writing_file.close();

	return 0;
}
