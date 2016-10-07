#include<stdio.h>
#include<stdlib.h>

int main(){
	int index;	/* File番号 */
	int max;	/* ループ終了値 */
	char firstId[8];	/* 粒子１の文字 */
	char secondId[8];	/* 粒子２の文字 */
	char linebuffer[2048];	/* 行の読み込み */
	FILE *fc, *stp;	/* Fileポインタ */
	
	/* 割当文字の入力 */
	printf("> Input an assigned character to combine 2 files on the same position :\n");
	
	/* 粒子１の文字を入力 */
	printf("> for particle 1 :  ");
	scanf("%s", firstId);
	
	/* 粒子２の文字を入力 */
	printf("> for particle 2 :  ");
	scanf("%s", secondId);
	
	/* R.tblを参照 */
	if((stp = fopen("R.tbl", "r"))==NULL){
		printf("Range table file[R.tbl] not open!!\n");
		exit(1);
	}
	
	/* 行数を初期化 */
	max = 0;
	
	/* 行数を数える */
	while(fgets(linebuffer, sizeof(linebuffer), stp)!=NULL)
		max++;
	
	/* Fileの結合開始 */
	for(index=1; index<=max; index++){
		
		/* 粒子１のFile名 */
		sprintf(linebuffer, "ssd_%d%s.dat", index, firstId);
		
		/* 粒子１のFileを開く */
		if((fc = fopen(linebuffer, "r"))==NULL){
			printf("Load file[%s] not open!!\n", linebuffer);
			break;
		}
		
		/* 外部コマンド */
		sprintf(linebuffer, "cat ssd_%d%s.dat > ssd_%dd.dat\n", index, firstId, index);
		
		/* コマンドを表示 */
		printf("> %s", linebuffer);
		
		/* 実行―粒子１のFileをコピー */
		system(linebuffer);
		
		/* 粒子２のFile名 */
		sprintf(linebuffer, "ssd_%d%s.dat", index, secondId);
		
		/* 粒子２のFileを開く */
		if((fc = fopen(linebuffer, "r"))==NULL){
			printf("Load file[%s] not open!!\n", linebuffer);
			break;
		}
		
		/* 外部コマンド */
		sprintf(linebuffer, "cat ssd_%d%s.dat >> ssd_%dd.dat\n", index, secondId, index);
		
		/* コマンドを表示 */
		printf("> %s", linebuffer);
		
		/* 実行―粒子２のFileを追記 */
		system(linebuffer);
		
	}
	
	/* main() 戻り値 0 (正常) */
	return 0;
	
}

