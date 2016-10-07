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
	std::cerr << "Usage : exe reference_?.dlt" << std::endl;
	std::cerr << "needfile : two or more referencefiles, R.tbl" << std::endl;
	exit(1);
}

int readerror(){
	std::cerr << "file error" << std::endl;
	exit(1);
}

struct dlt {
	char ID[16];
	double Range,dx,dy,dz;
	int Range_num;
	};

typedef struct {
	/* 気が向いたから作った */
	std::ofstream os;
	} SDfile;
	
int main(int argc, char **argv)
{

/* def */
	std::vector< dlt > reference;
	dlt diff_dat;

	int index[argc-1];	/* File通し番号 */
	int j,k,l,n;	/* need you ;) */
	double R;	/* Range */
	double x;	/* X座標 */
	double y;	/* Y座標 */
	double z;	/* Z座標 */
	double EX;	/* 抽出する位置 */
	char ident[4];	/* 粒子の割当文字 */
//	char ID[64];	/* 飛跡のID */
	char linebuffer[2048];	/* １行読み込み */

	std::string filename; /* argvの格納 */
	std::string RF; /* read a line */
	std::ifstream reading_file; /* ファイル */
	std::stringstream reading_line_buffer; /* 1行読み込み */
	
/* read files */
	if(argc < 2)usage();

	for(int i=0;i<argc-1;i++)index[i]=0;

	for(int i=0;i<argc-1;i++)
	{
		j = 0; if(i>0) j = index[i-1];
		filename = argv[i+1];
		std::ifstream reading_file(argv[i+1]);
		if(reading_file.fail())readerror();

		std::cerr << "reading " << filename << "..." << std::endl;
		while(reading_file && getline(reading_file, RF))
		{
			if(strncmp(RF.c_str(),"--------------------------------------------------",30)==0)continue;//しきい線をスキップ
			reference.push_back(diff_dat);

//			std::getline(reading_file, RF);
//			reading_line_buffer << RF;//convert
//			reading_line_buffer >> reference[j].ID >> reference[j].Range >> reference[j].dx >> reference[j].dy >> reference[j].dz;
			
			sscanf(RF.c_str(),"%s %lf %lf %lf %lf %*lf", reference[j].ID, &reference[j].Range, &reference[j].dx, &reference[j].dy, &reference[j].dz);

			j++;
		}//while
//	std::cerr << "check for debug: " << reference[j-1].ID << " ," << reference[j-1].Range <<std::endl;

	index[i]=j;
	reading_file.close();

	}//for

/* --------------------- calc RMS ------------------------- */
	double rms=0.;
	double ssd[256];
	
	/* step設定Fileを開く */
	reading_file.open("R.tbl");
	if(reading_file.fail())readerror();
//	std::cerr << "calc RMS..." <<std::endl;
	
	k=0;
	while(reading_file && getline(reading_file, RF))
	{
		reading_line_buffer << RF;
		reading_line_buffer >> ssd[k];
//		std::cout << reading_line_buffer.str() <<std::endl;
		k++;

/* std::stringstream initialization */
		reading_line_buffer.str("");//buffer clear
		reading_line_buffer.clear(std::stringstream::goodbit);//stream condition clear

	}//while
	reading_file.close();

	// "k" saves R.tbl's EOF line number.
	
	std::ofstream writing_file;
	writing_file.open("RMS.dat");
	if(writing_file.fail())readerror();
	
	for(l=0;l<k;l++)//each range
	{
	rms = 0.;
	n=0;
		for(int m=0;m<=index[argc-2];m++)//all data
		{
			if((reference[m].Range <= (ssd[l]+2.)) &&
				 (reference[m].Range >= (ssd[l]-2.)))
			{
				rms = rms + reference[m].dx * reference[m].dx;
				n++;
				reference[m].Range_num = l;//Range ssd[l]を対応付ける番号
			}//if
		}//for
	if(n!=0)rms = sqrt( rms / ( n ) );
	if(n==0)std::cout << ssd[l] << " was not found!" << std::endl;
	writing_file << ssd[l] << " " << rms << std::endl;
//	std::cout << "Range: " << ssd[l] << " RMS: " << rms << std::endl;

	}//for

	/* make directory */
	char cmd[] = "mkdir ssd";
	std::cerr << "command line: mkdir ssd" << std::endl;
	system(cmd);

/* make ssdfile */
	std::cerr << "output ssdfile..." << std::endl;
	/* output 2nddiff file */
	char filenamename[128];
	int a=0;
//	SDfile sdf[k][argc-1];
	std::ofstream ofstr[k][argc-1];
//	std::cout << k << "," << argc << std::endl;
	
	for(int x=0;x<argc-1;x++)
	{
		//std::ofstream SDfile[k];
		for(int p=0;p<k;p++)
		{

			sprintf(filenamename,"./ssd/ssd_%03d%c.dat",p,reference[index[x]-1].ID[0]);
			ofstr[p][x].open(filenamename);
		
		}//for
		while(a<index[x]){
			ofstr[(reference[a].Range_num)][x] << reference[a].ID << " " << reference[a].Range << " " << reference[a].dx << " " <<  reference[a].dy << " " << reference[a].dz << std::endl;
		a++;
		}//while

	}//for

	return 0;
	}
