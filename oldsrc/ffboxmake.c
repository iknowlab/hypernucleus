#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(){
	int index;	/* 通し番号 */
	int select;	/* ユーザ選択 */
	double RMS;	/* RMS */
	char linebuffer[2048];	/* 読み込み行 */
	FILE *read, *out, *stp;	/* Fileポインタ */
	
ReSelect:	/* ラベル再選択 */
	
	/* 計算対象に成分を選択 */
	printf("Select a coodinates :  \n");
	printf("\t1) X'-palne\n");
	printf("\t2) Y'-palne\n");
	printf("\t3) Z'-plane\n");
	printf("[Select number] :  ");
	scanf("%d", &select);
	
	/* 入力数値のチェック */
	if(select > 3 || select <1){
		printf("Please input by number 1, 2 or 3!!\n");
		goto ReSelect;
	}
	
	/* R.tblを参照 */
	if((stp = fopen("R.tbl", "r"))==NULL){
		printf("Range table file[R.tbl] not open!!\n");
		exit(1);
	}
	
	/* 初期化 */
	index = 0;
	
	/* 行数を数える */
	while(fgets(linebuffer, sizeof(linebuffer), stp)!=NULL)
		index++;
	
	/* 読み込みFileを開く */
	if((read = fopen("RMS.txt","r"))==NULL){
		printf("Input File[RMS.txt] not open!!\n");
		exit(1);
	}
	
	/* X'成分選択の場合 */
	if(select == 1)
		if((out = fopen("ffbox.tbl.x","w"))==NULL){
			printf("Output File[ffbox.tbl.x] not open!!\n");
			exit(1);
		}
		
	/* Y'成分選択の場合 */
	else if(select == 2)
		if((out = fopen("ffbox.tbl.y","w"))==NULL){
			printf("Output File[ffbox.tbl.y] not open!!\n");
			exit(1);
		}
		
	/* Z'成分選択の場合 */
	else if(select == 3)
		if((out = fopen("ffbox.tbl.z","w"))==NULL){
			printf("Output File[ffbox.tbl.z] not open!!\n");
			exit(1);
		}
		
	/* その他選択の場合エラー */
	else{
		printf("Error to open an output file!!\n");
		exit(1);
	}
	
	/* RMSを取得する */
	while(fgets(linebuffer,sizeof(linebuffer),read)!=NULL){
		sscanf(linebuffer,"%*lf %*d %lf", &RMS);
		
		/* 5で割る */
		RMS /= 5.;
		
		/* 割った値を出力 */
		fprintf(out,"%d\t%lf\n", index, RMS);
		
		/* 番号の更新 */
		index--;
	}
	
	/* Fileを閉じる */
	fclose(read);
	fclose(out);
	
	/* main() 戻り値 0 (正常) */
	return 0;
	
}
