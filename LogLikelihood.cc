/* make inoh 2016/5/13 */
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
	std::cerr << "Usage : exe ssdname(x,P,k,p) sample__.dlt ./LIKE/_like_.dat" << std::endl;
	std::cerr << "needfile : two or more referencefiles, R.tbl" << std::endl;
	exit(1);
}

int readerror(const char* readfile){
	std::cerr << "imput file error ("<< readfile << ")" << std::endl;
	exit(1);
}

int writeerror(){
	std::cerr << "write file error" << std::endl;
	exit(1);
}

typedef struct {
	char ID[16];
	double Range,dx,dy,dz;
	} dlt;

int main(int argc, char **argv){
	if(argc<4)usage();/* how to use this prgrm */

	int i,j,k,l,m,n;
	int flag=0; /* eof flag*/
	int index[128];
	double srange,sdx,sdy,sdz;
	int pljoint; /* sample.dlt */
	char sid[16];
	double RMS[2][128];	/* RMS */
	char linebuffer[2048];	/* １行読み込み */
	dlt SDfile[128][65536];	/* べくたーめんどい */

	std::string RF,write_line_buffer;

	std::ifstream reading_file;
	reading_file.open("RMS.dat");
	if(reading_file.fail())readerror("RMS.dat");

	std::ofstream writing_file;
	writing_file.open(argv[3]);
	if(writing_file.fail())writeerror();
	
	j=0;
	while(reading_file && getline(reading_file, RF))
	{
		sscanf(RF.c_str(),"%lf %lf",&RMS[0][j],&RMS[1][j]);
		RMS[1][j] /= 5.;	/* devided by 5 */
		j++;
	}//while
	reading_file.close();
//	std::cerr << "debug mode " << RMS[1][0] << " ---- " << RMS[1][j-1] << std::endl;

	/* ssd file read */
	std::cerr << "read ssd*.dat..." << std::endl;
	std::ifstream readssd[j];

	for(k=0;k<j;k++)
	{
		sprintf(linebuffer,"./ssd/ssd_%03d%s.dat", k, argv[1]);
		readssd[k].open(linebuffer);
		if(readssd[k].fail())readerror(linebuffer);
		l=0;		
		while(readssd[k] && getline(readssd[k], RF))
		{
			sscanf(RF.c_str(),"%s %lf %lf %lf %lf", SDfile[k][l].ID, &SDfile[k][l].Range, &SDfile[k][l].dx, &SDfile[k][l].dy, &SDfile[k][l].dz);
			l++;
		}//while
		index[k]=l; /* max l num */
		readssd[k].close();

	}//for

//	std::cerr << "debug value " << SDfile[0][index[0]-1].Range << "," << index[0]-1 << std::endl;
	std::cerr << "read " << argv[2] << "..." << std::endl;

	std::ifstream sampleread;
	sampleread.open(argv[2]);
	if(sampleread.fail())readerror(argv[2]);

	/* 処理飛跡数を初期化 */
	int count = 0;
	double Lx = 1.;	
	int numer,denomin;

	/* 飛跡数分の処理を開始 */
	while(1)
	{
		/* 参照File番号を初期化(しきい線で管理される) */
		k = 0; /* set first range */
		
		/* 尤度の初期化 */
		Lx = 1.;
		
		/* １飛跡分の尤度を求める */
		while(1){
			
			std::getline(sampleread, RF);

			/* 最終行になったら処理を終了 */
			if(sampleread.eof())
			{
				flag=1;
				break;//フラッグを立てて終了
			}//if

			/* しきい線ではループを終了する */
			if(strncmp(RF.c_str(),"--------------------------------------------------",49)==0)break;
			
			/* １行分の2次変位を取得 */
			sscanf(RF.c_str(),"%s %lf %lf %lf %lf %d",sid,&srange,&sdx,&sdy,&sdz,&pljoint);
			
			/* plateの接続部分は無視する */
			if(pljoint == 111){
				k++;
				continue;/* read next line */
			}//if

//			if((int)srange==0||(int)srange==100)break;//debug mode: 飛程0データは無視する
						
			/* 確率計算に使うカウンタを初期化 */
			numer = 1;
			denomin = 1;
			m=0;

			/* 参照Fileの2次変位を読み込む */
			while(m<index[k])
			{
				
				/* 母数を加算 */
				denomin++;
				
				/* 許容範囲内に含まれる2次変位を数える */
				if((sdx>=SDfile[k][m].dx-RMS[1][k])&&(sdx<=SDfile[k][m].dx+RMS[1][k]))numer++;
//				std::cout << sdx << ":" << SDfile[k][m].dx << ":" << RMS[1][k] <<std::endl;
				
				/* SDfileを1つ進める */
				m++;
			
			}//while
			
			/* 尤度を計算 */
			Lx = Lx*((double)numer/denomin);
			//std::cerr << Lx << std::endl;
			
			/* 次のFile番号へ */
			k++;
			
		}	/* while [１飛跡分の尤度を求める] end */
		
		/* while exit */	
		if(flag==1)break;

		/* 処理状況を表示 */
//		printf("%s\t%.10e\t%d\n",sid,Lx,k);
		
		/* Natural Logarythm of Likelihood value */
		double NLL;
		NLL = log(Lx);//Natural Log-likelihood log_e(Lx)
		
		/* 算出された尤度を出力 */
		sprintf(linebuffer,"%s\t%lf\t%d\n",sid,NLL,k);
//		sprintf(linebuffer,"%s\t%.10e\t%d\n",sid,Lx,k);
		std::cout << linebuffer;
		writing_file << linebuffer;		
		count++;

	}	/* while [飛跡数分の処理を開始] end */

	return 0;
}
