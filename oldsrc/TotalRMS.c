#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

/* 関数定義main()関数以降に記述 */
void CalcRMS(int, int, char*);

int main(){
	int loop;	/* ループカウンタ */
	int select;	/* ユーザ選択 */
	int line;	/* 行数 */
	char ID[64];	/* File ID */
	char linebuffer[2048];	/* 読み込み行 */
	FILE *stp;	/* Fileポインタ */
	
ReStart:	/* ラベル再実行 */
	
	/* File IDの入力 */
	printf("Input the file assigned ID :  ");
	scanf("%s",ID);
	
	/* 入力値のチェック */
	if(strlen(ID) > 1){
		printf("Please input a charactor!! Not a string.\n");
		goto ReStart;
	}
	
	/* R.tblを参照 */
	if((stp = fopen("R.tbl", "r"))==NULL){
		printf("Range table file[R.tbl] not open!!\n");
		exit(1);
	}
	
	/* 行数を初期化 */
	line = 0;
	
	/* 行数を数える */
	while(fgets(linebuffer, sizeof(linebuffer), stp)!=NULL)
		line++;
	

ReSelect:	/* ラベル再選択 */
	
	/* 計算対象に成分を選択 */
	printf("Select a coodinates :  \n");
	printf("\t1) X'-palne\t2) Y'-palne\n");
	printf("\t3) X' & Y'-plane both\n");
	printf("[Select number] :  ");
	scanf("%d", &select);
	
	/* 入力数値のチェック */
	if(select > 3 || select <1){
		printf("Please input by number 1, 2 or 3!!\n");
		goto ReSelect;
	}
	
	/* RMSの計算処理を開始 */
	for(loop=line; loop>0; loop--)
		CalcRMS(select-1, loop, ID);
	
	/* main() 戻り値0 (正常) */
	return 0;
	
}

void CalcRMS(int mode, int index, char *id){
	long int Max;	/* 2次変位の最大数 */
	double deltaX;	/* 2次変位X'-plane */
	double deltaY;	/* 2次変位Y'-plane */
	double R;	/* Range */
	double SUM;	/* 2次変位の２乗値の合計 */
	double RMS;	/* RMS */
	char linebuffer[2048];	/* 読み込み行 */
	FILE *read, *out;	/* Fileポインタ */
	
	/* 読み込みFile名を指定 */
	sprintf(linebuffer,"ssd_%d%s.dat", index, id);
	
	/* 読み込みFileを開く */
	if ((read = fopen(linebuffer,"r")) == NULL) {
		printf("FILE[%s] not open!!\n",linebuffer);
		exit(1);
	}
	
	/* 出力Fileをアペンドモードで開く */
	if ((out = fopen("RMS.txt","a")) == NULL) {
		printf("output_file[RMS.txt] not open\n");
		exit(1);
	}
	
	/* 処理経過を表示 */
	printf(" > Execute \"%s\" ...\t",linebuffer);
	
	/* 初期クリア */
	SUM = 0;
	Max = 0;
	
	/* 2次変位の読み込み */
	while(fgets(linebuffer,sizeof(linebuffer),read) != NULL){
		sscanf(linebuffer," %*s %lf %lf %lf %*lf", &R, &deltaX, &deltaY);
		
		/* X'成分選択の場合 */
		if(mode == 0){
			SUM = SUM + deltaX*deltaX;
			Max++;
		}
		/* Y'成分選択の場合 */
		else if(mode == 1){
			SUM = SUM + deltaY*deltaY;
			Max++;
		}
		/* X' Y'両成分選択の場合 */
		else{
			SUM = SUM + deltaY*deltaY + deltaX*deltaX;
			Max += 2;
		}
		
	}
	
	/* RMSを求める */
	RMS = sqrt(SUM/((double) Max));
	
	/* 計算された値を表示 */
	printf("N : %d\tRMS = %lf\n", Max, RMS);
	
	/* 出力Fileへの書き込み */
	fprintf(out,"%.0f\t%d\t%lf\n", R, Max, RMS);
	
	/* Fileを閉じる */
	fclose(read);
	fclose(out);
	
}

