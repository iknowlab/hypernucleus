#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

int main(int argc,char *fname[])
{
	int count;	/* 処理飛跡数 */
	int numer;	/* 分子 */
	int denomin;	/* 分母 */
	int num;	/* 参照File番号 */
	int plane;	/* 計算対象成分 */
	int index;	/* 許容範囲検索番号 */
	int pljoint;	/* plate接続部分 */
	double Lx;	/* 尤度 */
	double range;	/* 飛程 */
	double dx,dy,dz;	/* 2次変位 */
	double dx_c,dy_c,dz_c;	/* 参照側の2次変位 */
	double length;	/* 許容範囲 */
	char linebuffer[2048];	/* 読み込み行 */
	char id[64];	/* 飛跡のID */
	char chr[16];	/* 粒子の区別文字 */
	FILE *read, *out, *cmp, *med;	/* Fileポインタ */
	
	/* 読み込みFileを開く */
	if ((read = fopen(fname[1], "r")) == NULL) {
		printf("input_file[%s] not open\n",fname[1]);
		exit(1);
	}
	
	/* 出力Fileを開く*/
	if ((out = fopen(fname[2], "w")) == NULL) {
		printf("output_file[%s] not open\n",fname[2]);
		exit(1);
	}
	
	/* 区別文字の入力 */
	printf("> File assigned character :  ");
	scanf("%s",chr);
	
	/* 処理対象成分の選択 */
	printf("> Calculation Plane :\n");
	printf("> \t1) X'_plane\n");
	printf("> \t2) Y'_plane\n");
	printf("> \t3) Z'_plane\n");
	printf("> Answer by Index(1, 2, 3) :  ");
	scanf("%d",&plane);
	
	/* 許容範囲設定Fileを選択 */
	if(plane == 1)
		sprintf(linebuffer,"ffbox.tbl.x");
	else if(plane == 2)
		sprintf(linebuffer,"ffbox.tbl.y");
	else if(plane == 3)
		sprintf(linebuffer,"ffbox.tbl.z");
	else{
		printf("> You should input 1, 2, or 3!!\n> Exit soon...\n");
		exit(1);
	}
	
	/* 許容範囲設定Fileを読み込む */
	if ((med = fopen(linebuffer, "r")) == NULL) {
		printf("> [Error] :- %s - not open\n",linebuffer);
		exit(1);
	}
	
	/* 処理飛跡数を初期化 */
	count = 0;
	
	/* 飛跡数分の処理を開始 */
	while(1){
		
		/* 参照File番号を初期化 */
		num = 0;
		
		/* 尤度の初期化 */
		Lx = 1;
		
		/* １飛跡分の尤度を求める */
		while(1){
			
			/* 最終行になったら処理を終了 */
			if(fgets(linebuffer,sizeof(linebuffer),read)==NULL)
				goto Frame;
			
			/* しきい線ではループを終了する */
			if(strncmp(linebuffer,"--------------------------------------------------",49)==0)
				break;
			
			/* １行分の2次変位を取得 */
			sscanf(linebuffer,"%s %lf %lf %lf %lf %d",id,&range,&dx,&dy,&dz,&pljoint);
			
			/* plateの接続部分は無視する */
			if(pljoint == 111){
				num++;
				continue;
			}

			/* 飛程0データは無視する */
			if((int)range==0||(int)range==100){
				continue;
			}
			
			/* 対象成分の値をX'成分に移す */
			if(plane == 2)
				dx = dy;
			else if(plane == 3)
				dx = dz;
			
			/* 参照用File名を決定 */
			sprintf(linebuffer,"ssd_%d%s.dat", num+1, chr);
			
			/* 尤度計算のための参照Fileを開く */
			if ((cmp = fopen(linebuffer, "r")) == NULL) {
				printf("> [Error] : - %s - not open\n",linebuffer);
				exit(1);
			}
			
			/* 許容範囲を読み込む */
			while(fgets(linebuffer,sizeof(linebuffer),med)!=NULL){
				sscanf(linebuffer,"%d %lf",&index,&length);
				
				/* File番号に対応する許容範囲を取得 */
				if((num+1) == index)
					break;
			}
			
			/* 許容範囲設定Fileのストリームを初期化 */
			fseek(med, 0, SEEK_SET);
			
			/* 確率計算に使うカウンタを初期化 */
			numer = 1;
			denomin = 1;
			
			/* 参照Fileの2次変位を読み込む */
			while(fgets(linebuffer,sizeof(linebuffer),cmp) != NULL){
				
				/* 母数を加算 */
				denomin++;
				
				/* 2次変位を取得 */
				sscanf(linebuffer,"%*s %*lf %lf %lf %lf",&dx_c,&dy_c,&dz_c);
				
				/* 対象成分の値をX'成分に移す */
				if(plane == 2)
					dx_c = dy_c;
				else if(plane == 3)
					dx_c = dz_c;
				
				/* 許容範囲内に含まれる2次変位を数える */
				if(dx >= dx_c-length && dx <= dx_c+length)
					numer++;
				
			}
			
			/* 参照Fileを閉じる */
			fclose(cmp);
			
			/* 尤度を計算 */
			/* コメント：値が小さくなりすぎることをさけるため */
			/*           10倍しておく */
			Lx = Lx*((double)numer/denomin)*10;
			
			/* 次のFile番号へ */
			num++;
			
		}	/* while [１飛跡分の尤度を求める] end */
		
		/* 処理状況を表示 */
		printf("%s\t%.10e\t%d\n",id,Lx,num);
		
		/* 算出された尤度を出力 */
		fprintf(out,"%s\t%.10e\t%d\n",id,Lx,num);
		
		count++;
		
	}	/* while [飛跡数分の処理を開始] end */
	
Frame:	/* ラベル */
	
	/* Fileを閉じる */
	fclose(read);
	fclose(out);
	fclose(med);
	
	/* main() 戻り値 0 (正常) */
	return 0;
	
}
