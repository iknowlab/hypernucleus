#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

int main()
{
	int index;	/* File通し番号 */
	double R;	/* Range */
	double x;	/* X座標 */
	double y;	/* Y座標 */
	double z;	/* Z座標 */
	double EX;	/* 抽出する位置 */
	char ident[4];	/* 粒子の割当文字 */
	char ID[64];	/* 飛跡のID */
	char linebuffer[2048];	/* １行読み込み */
	FILE *read,*out,*step;	/* Fileポインタ */
	
	/* 粒子別Fileの区別文字を入力 */
	printf("Input a character to assign the indexes to files :  ");
	scanf("%s", ident);
	
	/* 入力文字の確認 */
	if(strlen(ident) > 1){
		printf("Please input a charactor!! Not a string.\n");
		exit(0);
	}
	
	/* 読み込みFile名を指定 */
	sprintf(linebuffer, "reference_%s.dlt", ident);
	
	/* 読み込みFileを開く */
	if((read = fopen(linebuffer,"r"))==NULL){
		printf("> Input_FILE(%s) not found!\n", linebuffer);
		exit(1);
	}
	
	/* step設定Fileを開く */
	if((step = fopen("R.tbl","r"))==NULL){
		printf("> Extraction_FILE(R.tbl) not found!\n");
		exit(1);
	}
	
	/* File通し番号を初期化 */
	index = 0;
	
	/* 設定Fileの分別位置を読み込む */
	while(fgets(linebuffer,sizeof(linebuffer),step) != NULL){
		sscanf(linebuffer, "%lf", &EX);
		
		/* 通し番号を加算 */
		index++;
		
		/* 通し番号を付けて出力File名を決定 */
		sprintf(linebuffer,"ssd_%d%s.dat", index, ident);
		
		/* 出力Fileを開く */
		if((out = fopen(linebuffer,"w"))==NULL){
			printf("> Output_FILE(%s) not open!\n",linebuffer);
			exit(1);
		}
		
		/* 途中経過の表示 */
		printf("> ... Extraction for R = %.3f\tFILE :  -[ %s ]-\n",EX,linebuffer);
		
		/* 読み込みFileから、2次変位を取得 */
		while(fgets(linebuffer,sizeof(linebuffer),read) != NULL){
			sscanf(linebuffer,"%s %lf %lf %lf %lf %*lf",ID,&R,&x,&y,&z);
			
			/* 取り出す位置に一致するとき */
			if(R>=EX-0.1 && R<=EX+0.1){
				
				/* しきい線でなければ2次変位を出力する */
				if(strncmp(ID,"--------------------------------------------------",30)!=0)
					fprintf(out,"%s\t%lf\t%lf\t%lf\t%lf\n",ID,R,x,y,z);
			}
		}
		
		/* ストリームを初期位置に戻す */
		fseek(read, 0, SEEK_SET);
		
		/* 出力Fileを閉じる */
		fclose(out);
		
	}	/* while [設定Fileの分別位置を読み込む] end */
	
	/* 読み込みFileを閉じる */
	fclose(read);
	
	/* step設定Fileを閉じる */
	fclose(step);
	
	/* main() 戻り値0 (正常) */
	return 0;
	
}
