#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

int main(int argc,char *fname[])
{
	int num[2];	/* 積算数 */
	double L[2];	/* 尤度 */
	char ID[2][64];	/* ID */
	char linebuffer[2048];	/* 読み込み行 */
	FILE *read1, *read2;	/* Fileポインタ */
	
	/* ２つの尤度Fileを開く */
	if ((read1 = fopen(fname[1],"r")) == NULL) {
		printf("input_file[%s] not open\n",fname[1]);
		exit(1);
	}
	if ((read2 = fopen(fname[2],"r")) == NULL) {
		printf("input_file[%s] not open\n",fname[2]);
		exit(1);
	}
	
	/* データの読み込みと尤度の掛け算 */
	while(1){
		
		/* 片方のFileから尤度を取得 */
		if(fgets(linebuffer,sizeof(linebuffer),read1) == NULL)
			break;
		sscanf(linebuffer,"%s %lf %d", ID[0], &L[0], &num[0]);
		
		/* もう一方のFileから尤度を取得 */
		if(fgets(linebuffer,sizeof(linebuffer),read2) == NULL)
			break;
		sscanf(linebuffer,"%s %lf %d", ID[1], &L[1], &num[1]);
		
		/* ２つのFileのIDが一致しない場合はエラーとする */
		if(strcmp(ID[0], ID[1])!=0){
			printf("Different tipe File [%s] & [%s] !!\n", fname[1], fname[2]);
			exit(1);
		}
		
		/* ２つの尤度を掛け合わせ出力 */
		printf("%s\t%lf\t%d\n", ID[0], L[0]*L[1], num[0]+num[1]);
		
	}
	
	/* Fileを閉じる */
	fclose(read1);
	fclose(read2);
	
	/* 戻り値 0 (正常) */
	return 0;
	
}

