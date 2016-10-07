#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

int main(int argc,char *fname[])
{
	int EX;	/* 抽出するRange */
	int Range;	/* Range */
	char linebuffer[2048];	/* 読み込み行 */
	FILE *read;	/* Fileポインタ */
	
	/* 抽出するRangeを入力 */
	printf("Extraction Point? :\tR = ");
	scanf("%d",&EX);
	
	/* 読み込みFileを開く */
	if((read = fopen(fname[1],"r"))==NULL){
		printf("Input_FILE(%s) not found!\n",fname[1]);
		exit(1);
	}
	
	/* 行の読み込み */
	while(fgets(linebuffer,sizeof(linebuffer),read) != NULL){
		
		/* しきい線の場合は読み直し */
		if(strncmp(linebuffer, "--------------------------------------------------", 30) == 0)
			continue;
		
		/* Rangeの取得 */
		sscanf(linebuffer,"%*s %d %*lf", &Range);
		
		/* 指定されたRangeと一致したら、その行を表示 */
		if(Range == EX)
			printf("%s", linebuffer);
		
	}
	
	/* Fileを閉じる */
	fclose(read);
	
	/* 戻り値 0 (正常) */
	return 0;
	
}
