#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

int main(int argc,char *fname[])
{
	double L[2];	/* 尤度 */
	double range;	/* Range */
	char ID[2][64];	/* ID */
	char linebuffer[2048];	/* 読み込み行 */
	FILE *read1, *read2;	/* Fileポインタ */
	
	/* Rangeの入力 */
	printf("Input the range of this event track (um) :  ");
	scanf("%lf",&range);
	
	/* ２つの尤度Fileを開く */
	if ((read1 = fopen(fname[1],"r")) == NULL) {
		printf("input_file[%s] not open\n",fname[1]);
		exit(1);
	}
	if ((read2 = fopen(fname[2],"r")) == NULL) {
		printf("input_file[%s] not open\n",fname[2]);
		exit(1);
	}
	
	/* 尤度比の計算 */
	while(1){
		
		/* 片方のFileから尤度を取得 */
		if(fgets(linebuffer,sizeof(linebuffer),read1)==NULL)
			break;
		sscanf(linebuffer,"%s %lf %*d", ID[0], &L[0]);
		
		/* もう一方のFileから尤度を取得 */
		if(fgets(linebuffer,sizeof(linebuffer),read2)==NULL)
			break;
		sscanf(linebuffer,"%s %lf %*d", ID[1], &L[1]);
		
		/* ２つのFileのIDが一致しない場合はエラーとする */
		if(strcmp(ID[0], ID[1])!=0){
			printf("Different Type of File Loaded -> [%s] & [%s] !!\n", fname[1], fname[2]);
			exit(1);
		}
		
		/* 尤度比の出力 */
		printf("%.0f\t%lf\n", range, L[0]/(L[0]+L[1]));
		
	}
	
	/* Fileを閉じる */
	fclose(read1);
	fclose(read2);
	
	/* 戻り値 0 (正常) */
	return 0;
	
}

