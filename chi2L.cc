#include<stdio.h>
#include<stdlib.h>
#include <math.h>

int main(int argc,char *fname[]){
	int loop;	/* ループカウンタ */
	int line[2];	/* 読み込み行数 */
	int freq;	/* 測定度数 */
	int Ncmp;	/* 測定側データ数 */
	int Nrd;	/* シミュレーション側データ数 */
	int div;	/* 分布の分割数 */
	int rst;	/* 制約条件数 */
	int ndf; /* 自由度 */
	double P1;	/* 片方の尤度 */
	double P2;	/* もう一方の尤度 */
	double LikeRatio[100000];	/* 尤度比 */
	double Ek[30];	/* 期待値 */
	double Ok[30];	/* 測定度数の保存 */
	double Lthresh[1000];	/* 要素分割の閾値 */
	double SUM_Chi2;	/* χ2乗値 */
	char linebuffer[2048];	/* 読み込み行 */
	char id[16];	/* ID */
	FILE *rf1,*rf2,*cf1,*cf2;	/* Fileポインタ */
	
	/* 第１引数のFileを開く */
	if ((rf1 = fopen(fname[1], "r")) == NULL) { 
		printf("input_file[%s] not open\n",fname[1]); 
		exit(1); 
	}
	
	/* 第２引数のFileを開く */
	if ((rf2 = fopen(fname[2], "r")) == NULL) { 
		printf("input_file[%s] not open\n",fname[2]); 
		exit(1); 
	}
	
	/* 第３引数のFileを開く */
	if ((cf1 = fopen(fname[3], "r")) == NULL) {
		printf("input_file[%s] not open\n",fname[3]);
		exit(1);
	}
	
	/* 第４引数のFileを開く */
	if ((cf2 = fopen(fname[4], "r")) == NULL) {
		printf("input_file[%s] not open\n",fname[4]);
		exit(1);
	}
	
	/* 読み込みFileの表示 */
	printf(">******* Caluclation for Mean /& Observation frequency *****************\n");
	printf("> Using FILE :\n");
	printf(">	[%s] /& [%s]  for Comprition data\n",fname[3],fname[4]);
	printf("> Using FILE :\n");
	printf(">	[%s] /& [%s]  for Chi2test data\n>\n",fname[1],fname[2]);
	printf(">***********************************************************************\n>\n");
	
	/* 行数(データ数)を数える */
	line[0] = 0;
	while(fgets(linebuffer,sizeof(linebuffer),cf1)!=NULL)
		line[0]++;
	
	/* 行数(データ数)を数える */
	line[1] = 0;
	while(fgets(linebuffer,sizeof(linebuffer),cf2)!=NULL)
		line[1]++;
	
	/* 同じ行数出なければエラー */
	if(line[0]!=line[1]){
		printf("> Errors: The number of particles are different!!\n");
		printf(">         n[%s] = %d,\tn[%s] = %d\n", fname[3], line[0], fname[4], line[1]);
		printf("> Exit anyway!!\n");
		exit(0);
	}
	
	/* データ数の保存 */
	Ncmp = line[0];
	
	/* ストリームを戻す */
	fseek(cf1, 0, SEEK_SET);
	fseek(cf2, 0, SEEK_SET);
	
	/* 行数を初期化 */
	line[0] = 0;
	
	/* 尤度が格納されたFileを読み込む */
	while(fgets(linebuffer,sizeof(linebuffer),cf1)!=NULL){
		
		/* 片方の尤度を取得 */
		sscanf(linebuffer,"%*s %lf %*d",&P1);
		
		/* 一方の尤度を取得 */
		fgets(linebuffer,sizeof(linebuffer),cf2);
		sscanf(linebuffer,"%*s %lf %*d",&P2);
		
		/* 尤度比を計算 */
		LikeRatio[line[0]] = P1/(P1+P2);
		
		/* 行数加算 */
		line[0]++;
	}
	
	/* ヒストの分割数 */
	printf("> Input the number of divisions :  ");
	scanf("%d",&div);
	
	/* 制限数を設定 */
	printf("> Input the restriction factor  :  ");
	scanf("%d",&rst);
	
	/* 分布の幅を指定 */
	Lthresh[0] = 0.;
	Lthresh[div] = 1.;
	
	/* 敷居となる値を設定 */
	for(loop=1; loop<div; loop++)
		Lthresh[loop] = Lthresh[loop-1] + 1./((double) div);
	
	/* 各分割要素における期待値を計算 */
	for(loop=1; loop<=div; loop++){
		
		/* 条件に一致するデータ数のクリア */
		freq = 0;
		
		/* データ数文ループ */
		for(int i=0; i<=Ncmp; i++){
			if(LikeRatio[i] <= Lthresh[loop]
			&& LikeRatio[i] > Lthresh[loop-1] && loop!=1)
				freq++;
			else if(LikeRatio[i] <= Lthresh[loop]
			&& LikeRatio[i] >= Lthresh[loop-1] && loop==1)
				freq++;
		}
		
			
		/* 期待値の計算 */
		Ek[loop-1] = ((double) freq)/((double) Ncmp);
	}
	
	/* 行数(データ数)を数える */
	line[0] = 0;
	while(fgets(linebuffer,sizeof(linebuffer),rf1)!=NULL)
		line[0]++;
	
	/* 行数(データ数)を数える */
	line[1] = 0;
	while(fgets(linebuffer,sizeof(linebuffer),rf2)!=NULL)
		line[1]++;
	
	/* 同じ行数出なければエラー */
	if(line[0] != line[1]){
		printf("> Errors: The number of particles are different!!\n");
		printf(">         n[%s] = %d,\tn[%s] = %d\n", fname[1], line[0], fname[2], line[1]);
		printf("> Exit anyway!!\n");
		exit(0);
	}
	
	/* データ数の保存 */
	Nrd = line[0];
	
	/* ストリームを戻す */
	fseek(rf1, 0, SEEK_SET);
	fseek(rf2, 0, SEEK_SET);
	
	/* 行数を初期化 */
	line[0] = 0;
	
	/* 尤度が格納されたFileを読み込む */
	while(fgets(linebuffer,sizeof(linebuffer),rf1)!=NULL){
		
		/* 片方の尤度を取得 */
		sscanf(linebuffer,"%s %lf %*d",id,&P1);
		
		/* 一方の尤度を取得 */
		fgets(linebuffer,sizeof(linebuffer),rf2);
		sscanf(linebuffer,"%*s %lf %*d",&P2);
		
		/* 尤度比を計算 */
		LikeRatio[line[0]] = P1/(P1+P2);
		
		/* 0で割るデータは無視 */
		if(P1==0 && P2==0){
			Nrd--;
			line[0]--;
			printf("> L[%s] -> Nan value !!\n",id);
		}
		
		/* 行数加算 */
		line[0]++;
	}
	
	/* 自由度の初期化 */
	ndf=0;
	
	/* 各分割要素における度数を計算 */
	for(loop=1; loop<=div; loop++){
		
		/* 度数の初期化 */
		freq = 0;
		
		/* 度数を計算 */
		for(int j=0;j<Nrd;j++){
			
			/* 0に近い値は0.000001に固定 */
			if(LikeRatio[j] < 0.000001)
				LikeRatio[j] = 0.000001;
			
			/* 範囲内のデータを数える */
			if(LikeRatio[j] <= Lthresh[loop] && LikeRatio[j] > Lthresh[loop-1])
				freq++;
			
		}
		
		/* 自由度の計算 */
		if(freq>0)ndf++;
		
		/* 度数を保存 */
		Ok[loop-1] = (double) freq;
		
	}
	
	/* 期待度数を計算 */
	for(loop=1; loop<=div; loop++)
		Ek[loop-1] = Ek[loop-1]*((double) Nrd);
	
	/* 期待度数を表示 */
	printf("> -- [Parameters] --\n");
	printf("> Mean frequency:\n");
	for(loop=1; loop<=div; loop++){
		if(Ek[loop-1] == 0)
			Ek[loop-1] = 0.000001;
		
		printf(">       E(%lf -> %lf) = %lf\n",
			Lthresh[loop-1],Lthresh[loop],Ek[loop-1]);

	}
	
	/* 改行 */
	printf(">\n");
	
	/* 測定データの度数を表示 */
	printf("> Observation frequency:\n");
	for(loop=1; loop<=div; loop++)
		printf(">       O(%lf -> %lf) = %lf\n",
			Lthresh[loop-1],Lthresh[loop],Ok[loop-1]);
	printf(">\n");
	printf("> --------------------------------------------------\n");
		
	/* χ2乗値のクリア */
	SUM_Chi2 = 0;
	
	/* χ2乗値の計算 */
	for(loop=1; loop<=div; loop++)
		SUM_Chi2 = SUM_Chi2 + (Ok[loop-1] - Ek[loop-1])*(Ok[loop-1] - Ek[loop-1])/Ek[loop-1];
	
	/* χ2乗値の計算結果を表示 */
//	printf("> Chi^2/ndf = %.3f/%d = %.3f\n",SUM_Chi2,div-rst,SUM_Chi2/((double) (div-rst)));
	printf("> Chi^2/ndf = %.3f/%d = %.3f\n",SUM_Chi2,ndf,SUM_Chi2/((double)ndf));
	
	/* Fileを閉じる */
	fclose(rf1);
	fclose(rf2);
	fclose(cf1);
	fclose(cf2);
	
	/* main()戻り値 0 (正常) */
	return 0;
	
}
