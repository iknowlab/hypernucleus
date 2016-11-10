
/* クラス:文字列操作用関数を格納 */
class StrControl{
	
public:	/* アクセス修飾子 */
	
	/* 静的メンバ:指定された文字がある位置のポインタを返す */
	static char* strPoint(char *input, char *schr, int index){
		
		/* コメント：引数の説明 */
		/*           ARG1 - 対象文字列 */
		/*           ARG2 - 検索する文字 */
		/*           ARG3 - 何番目の指定文字を検索するか指定する数値 */
		
		/* 変数宣言 */
		int cnum;	/* 指定文字カウンタ */
		char cp[2];	/* 対象文字 */
		char *rtn;	/* 文字ポインタ */
		
		/* ポインタを初期化 */
		rtn = NULL;
		
		/* 指定文字が１文字であるか確認 */
		if(strlen(schr) != 1)
			return rtn;
		
		/* 初期化 */
		cnum = 0;
		
		/* 文字列全体を検索する */
		for(int i=0; i<strlen(input); i++){
			
			/* 文字列から１文字取得 */
			sprintf(cp, "%c", input[i]);
			
			/* 指定文字と照らし合わせる */
			if(strcmp(cp, schr) == 0){
				
				/* 見つかればカウンタを加算 */
				cnum++;
				
				/* 指定した数値以上であればポインタを取得 */
				if(cnum > index){
					rtn = input + i;
					break;
				}
				
			}
		}
		
		/* 戻り値をセット */
		return rtn;
		
	}
	
	/* 静的メンバ:文字列の左右をトリミング */
	static char* strTriming(char *input){
		
		/* コメント：引数と戻り値の説明 */
		/*           ARG1 - 対象文字列 */
		/*           RTN  - トリミング後の先頭文字のポインタ */
		
		int pos;	/* 文字位置 */
		
		/* 文字列全体から検索 */
		for(pos=0; pos<strlen(input); pos++){
			
			/* 先頭からスペース文字とタブ文字を読み飛ばす */
			if(input[pos] == ' ' || input[pos] == '\t')
				continue;
			
			/* 何か文字を見つけたらループを終了 */
			break;
			
		}
		
		/* 引数の文字列の最終文字から検索 */
		for(int i=strlen(input)-1; i>pos; i--){
			
			/* スペース文字、タブ文字、改行文字でない文字ならば */
			if(input[i] != ' ' && input[i] != '\t' && input[i] != '\n'){
				
				/* １つ先の文字をNULL文字にする */
				input[i+1] = '\0';
				
				/* ループ終了 */
				break;
			}
			
		}
		
		/* 戻り値セット */
		return (input + pos);
		
	}
	
};

/* クラス:信頼度の計算 */
class ConfidenceLevel{
	
	/* コメント：正確な定義を研究し次第書き込む */
	
};

/* クラス:グラフの描写機能、信頼度計算クラスを継承 */
class Distribution : public ConfidenceLevel{
	
private:	/* アクセス修飾子 */
	
	/* フィールド変数 */
	int LineColor[2];	/* ヒストグラムの線色番号 */
	char CampusName[64];	/* キャンパス名 */
	char GraphName[2][64];	/* Statisticsタイトル */
	char GraphTitle[256];	/* グラフのタイトル */
	char XAxisLabel[64];	/* X軸ラベル名 */
	char YAxisLabel[64];	/* Y軸ラベル名 */
	char FileName[4][256];	/* 読み込みFile名 */
	char LabelName[2][64];	/* 表示ラベル名 */
	bool Normalization;	/* 規格化機能設定 */
	
	/* コメント：規格化機能変数(Normalization)について */
	/*           true  - 百分率による規格化をする */
	/*           false - 規格化なし、縦軸は飛跡数になる */
	
	/* メンバ:尤度Fileの読み込みとグラフの描写 */
	bool CreateGraph(){
		
		/* 変数宣言 */
		int item;	/* 読み込み項目数カウンタ */
		int max;	/* ループ終了値 */
		int MainNum;	/* 片方に対する尤度比数 */
		int SubNum;	/* もう一方に対する尤度比数 */
		double unsigned const div = 100.;	/* 分割数 */
		double xiLike[4096];	/* 片方の尤度 */
		double kLike[4096];	/* もう一方の尤度 */
		double LR;	/* 尤度比 */
		double AVE_Xi;	/* 片方の尤度比平均値 */
		double AVE_K;	/* もう一方の尤度比平均値 */
		char ID[16];	/* 飛跡のID */
		char linebuffer[4096];	/* 余も込み行格納 */
		FILE *p1,*p2;	/* Fileポインタ */
		
		/* rootの設定 */
		gROOT -> SetStyle("Plain");
		
		/* Y軸ラベル名を決定 */
		strcpy(YAxisLabel, "Percent [%]");
		
		/* １次元ヒストオブジェクト */
		TH1F *h1 = new TH1F(GraphName[0], GraphTitle, div, 0, 1);
		TH1F *h2 = new TH1F(GraphName[1], GraphTitle, div, 0, 1);
		
		/* X軸ラベルを定義 */
		h1 -> SetXTitle(XAxisLabel);
		
		/* 片方のFileを開く */
		if ((p1 = fopen(FileName[0], "r")) == NULL) {
			printf("input_file[%s] not open\n", FileName[0]);
			return false;
		}
		
		/* 項目数の初期化 */
		item=0;
		
		/* 尤度比の読み込み */
		while(fgets(linebuffer,sizeof(linebuffer),p1)!=NULL){
			sscanf(linebuffer,"%s %lf %*d", ID, &xiLike[item]);
			
			/* 空文字でなければ項目数を加算 */
			if(strlen(linebuffer) > 0)
				item++;
			
		}
		
		/* Fileを閉じる */
		fclose(p1);
		
		/* もう一方のFileを開く */
		if ((p2 = fopen(FileName[1], "r")) == NULL) { 
			printf("input_file[%s] not open\n", FileName[1]);
			return false; 
		} 
		
		/* 項目数の初期化 */
		item=0;
		
		/* 尤度比の読み込み */
		while(fgets(linebuffer,sizeof(linebuffer),p2)!=NULL){
			sscanf(linebuffer,"%s %lf %*d",ID,&kLike[item]);
			
			/* 空文字でなければ項目数を加算 */
			if(strlen(linebuffer) > 0)
				item++;
			
		}
		
		/* Fileを閉じる */
		fclose(p2);
		
		/* 項目数を保存する */
		max = item;
		
		/* 尤度比数の初期化 */
		MainNum = 0;
		
		/* 取得項目分ループ */
		for(item=0; item<max; item++){
			
			/* 0で割ることを避ける */
			if(xiLike[item]==0 && kLike[item]==0)
				continue;
			
			/* 尤度比を計算 */
			LR = xiLike[item] / (xiLike[item]+kLike[item]);
			
			/* 1であれば0.999999に固定する */
			/* コメント：1はヒストグラムの範囲外になるため */
			/*           強制的に1以内に収める */
			if(LR == 1.0)
				LR = 0.999999;
			
			/* 尤度比を加算して行く */
			AVE_Xi = AVE_Xi + LR;
			
			/* 尤度比数を加算 */
			MainNum++;
			
			/* 尤度比を格納 */
			h1 -> Fill(LR);
			
		}
		
		/* 平均値を計算 */
		if(max != 0)
			AVE_Xi = AVE_Xi/((double) max);
		else
			AVE_Xi = 0.;
		
		/* 片方のFileを開く */
		if ((p1 = fopen(FileName[2], "r")) == NULL) {
			printf("input_file[%s] not open\n", FileName[2]);
			return false;
		}
		
		/* 項目数の初期化 */
		item=0;
		
		/* 尤度比の読み込み */
		while(fgets(linebuffer,sizeof(linebuffer),p1)!=NULL){
			sscanf(linebuffer,"%s %lf %*d",ID,&xiLike[item]);
			
			/* 空文字でなければ項目数を加算 */
			if(strlen(linebuffer) > 0)
				item++;
			
		}
		
		/* Fileを閉じる */
		fclose(p1);
		
		/* もう一方のFileを開く */
		if ((p2 = fopen(FileName[3], "r")) == NULL) {
			printf("input_file[%s] not open\n", FileName[3]);
			return false;
		}
		
		/* 項目数の初期化 */
		item=0;
		
		/* 尤度比の読み込み */
		while(fgets(linebuffer,sizeof(linebuffer),p2)!=NULL){
			sscanf(linebuffer,"%s %lf %*d",ID,&kLike[item]);
			
			/* 空文字でなければ項目数を加算 */
			if(strlen(linebuffer) > 0)
				item++;
			
		}
		
		/* Fileを閉じる */
		fclose(p2);
		
		/* 項目数を保存する */
		max=item;
		
		/* 尤度比数の初期化 */
		SubNum=0;
		
		/* 取得項目分ループ */
		for(item=0; item<max; item++){
			
			/* 0で割ることを避ける */
			if(xiLike[item]==0 && kLike[item]==0)
				continue;
			
			/* 尤度比を計算 */
			LR = xiLike[item]/(xiLike[item]+kLike[item]);
			
			/* 1であれば0.999999に固定する */
			if(LR == 1.0)
				LR = 0.999999;
			
			/* 尤度比を加算して行く */
			AVE_K = AVE_K + LR;
			
			/* 尤度比数を加算 */
			SubNum++;

			/* 尤度比を格納 */
			h2 -> Fill(LR);
			
		}

		/* integral(from bin1 to bin) */
		 Double_t pion_all = h1->Integral(1,div);
		 Double_t kaon_all = h2->Integral(1,div);

                 cout << " First entries = (for integral function)" << pion_all << endl;
		/* calc eff (histgram,bin,integral,files)*/
		 Efficiency(h1,h2,div,0,1,pion_all,kaon_all,"eff0.txt","fake0.txt","EF0.txt"); // ここだよ！！

		/* 平均値を計算 */
		if(max != 0)
			AVE_K = AVE_K/((double) max);
		else
			AVE_K = 0.;
		
		/* キャンパスを作成 */
		TCanvas *c1 = new TCanvas(CampusName, "Likelihood Ratio Distribution", 0, 0, 700, 700);
		
		/* 線色の正義 */
		h1 -> SetLineColor(LineColor[0]);
		h2 -> SetLineColor(LineColor[1]);
		
		/* ラベルの設定 */
		TText *ta = new TText(0.81, 1., LabelName[0]);
		ta -> SetTextSize(0.04);
		ta -> SetTextColor(LineColor[0]);
		TText *tb = new TText(0.06, 1., LabelName[1]);
		tb -> SetTextSize(0.04);
		tb -> SetTextColor(LineColor[1]);
		
		/* 線の太さを定義 */
		h1 -> SetLineWidth(3);
		h2 -> SetLineWidth(3);
		
		/* 規格化設定の確認 */
		if(Normalization){
			
			/* Y軸のタイトルを定義 */
			h1 -> SetYTitle(YAxisLabel);
			
			/* Y軸オフセット */
			h1 -> GetYaxis()->SetTitleOffset(1.0);
			
			/* 規格化してから描写 */
			TH1F *H1 = h1->DrawNormalized("",100);
		}
		
		/* 規格化なしで描写 */
		else
			h1->Draw();
		
		/* 規格化設定の確認と、一方のグラフを描写 */
		if(Normalization)
			TH1F *H2 = h2->DrawNormalized("same",100);
		else
			h2->Draw("same");
		
		/* ラベルの追記 */
		ta -> Draw();
		tb -> Draw();
		
		/* 項目数の表示 */
		printf("Entries : '%s' = %d\t'%s' = %d\n", GraphName[0], MainNum, GraphName[1], SubNum);
		
		/* 平均値の表示 */
		printf("AVE     : '%s' = %.4f\t'%s' = %.4f\n", GraphName[0], AVE_Xi, GraphName[1], AVE_K);
		
	}
	
public:	/* アクセス修飾子 */
	
	/* メンバ:ΞとKのグラフ設定 */
	void MakeXiKaonGraph(char *gfilename){
		char chr[2];	/* ユーザ返答 */
		
		/* 線色番号 */
		LineColor[0] = 4;
		LineColor[1] = 3;
		
		/* キャンパス名 */
		if(sizeof(gfilename) <= 64)
			sprintf(CampusName, "%s", gfilename);
		else
			sprintf(CampusName, "c1");
		
		/* Statisticsタイトル */
		sprintf(GraphName[0], "PID(#Xi)");
		sprintf(GraphName[1], "PID(#Xi) of Kaon");
		
		/* グラフタイトル */
		sprintf(GraphTitle, "Geant4 Xi/Kaon Simulation");
		
		/* File名 */
		sprintf(FileName[0], "xlike.dat");
		sprintf(FileName[1], "klike.dat");
		sprintf(FileName[2], "xlike2.dat");
		sprintf(FileName[3], "klike2.dat");
		
		/* 表示ラベル名 */
		sprintf(LabelName[0], "G4Xi");
		sprintf(LabelName[1], "G4kaon");
		
		/* 規格化の有無を確認する */
		printf("Set Normaliation? :\n");
		printf("answer by \"y\"(=yes) or \"n\"(=no) :  ");
		scanf("%s", chr);
		
		/* 無限ループ */
		do{
			
			/* 規格化あり */
			if(strcmp(chr, "y") == 0)
				Normalization = true;
			
			/* 規格化なし */
			else if(strcmp(chr, "n") == 0)
				Normalization = false;
			
			/* その他 */
			else{
				printf("Please answer by \"y\" or \"n\"");
				continue;
			}
			
			break;
			
		}while(1);
		
		/* X軸タイトル名 */
		sprintf(XAxisLabel, "PID(#Xi)");
		
		/* グラフの作成 */
		if(this->CreateGraph() != true)
			printf("Sorry ... Can not create a graph!!\n");

	}
	
	/* メンバ:Ξとπのグラフ設定 */
	void MakeXiPionGraph(char *gfilename){
		char chr[2];	/* ユーザ返答 */
		
		/* 線色番号 */
		LineColor[0] = 4;
		LineColor[1] = 2;
		
		/* キャンパス名 */
		if(sizeof(gfilename) <= 64)
			sprintf(CampusName, "%s", gfilename);
		else
			sprintf(CampusName, "c1");
		
		/* Statisticsタイトル */
		sprintf(GraphName[0], "PID(#Xi)");
		sprintf(GraphName[1], "PID(#Xi) of Pion");
		
		/* グラフタイトル */
		sprintf(GraphTitle, "Geant4 Xi/Pion Simulation");
		
		/* File名 */
		sprintf(FileName[0], "xlike.dat");
		sprintf(FileName[1], "pilike.dat");
		sprintf(FileName[2], "xlike2.dat");
		sprintf(FileName[3], "pilike2.dat");
		
		/* 表示ラベル名 */
		sprintf(LabelName[0], "G4Xi");
		sprintf(LabelName[1], "G4pion");
		
		/* 規格化の有無を確認する */
		printf("Set Normaliation? :\n");
		printf("answer by \"y\"(=yes) or \"n\"(=no) :  ");
		scanf("%s", chr);
		
		/* 無限ループ */
		do{
			
			/* 規格化あり */
			if(strcmp(chr, "y") == 0)
				Normalization = true;
			
			/* 規格化なし */
			else if(strcmp(chr, "n") == 0)
				Normalization = false;
			
			/* その他 */
			else{
				printf("Please answer by \"y\" or \"n\"");
				continue;
			}
			
			break;
			
		}while(1);
		
		/* X軸タイトル名 */
		sprintf(XAxisLabel, "PID(#Xi)");
		
		/* グラフの作成 */
		if(this->CreateGraph() != true)
			printf("Sorry ... Can not create a graph!!\n");
		
	}
	
	/* メンバ:ProtonとKのグラフ設定 */
	void MakeProtonKaonGraph(char *gfilename){
		char chr[2];	/* ユーザ返答 */
		
		/* 線色番号 */
		LineColor[0] = 14;
		LineColor[1] = 3;
		
		/* キャンパス名 */
		if(sizeof(gfilename) <= 64)
			sprintf(CampusName, "%s", gfilename);
		else
			sprintf(CampusName, "c1");
		
		/* Statisticsタイトル */
		sprintf(GraphName[0], "PID(Proton)");
		sprintf(GraphName[1], "PID(Proton) of Kaon");
		
		/* グラフタイトル */
		sprintf(GraphTitle, "Geant4 Proton/Kaon Simulation");
		
		/* File名 */
		sprintf(FileName[0], "Plike.dat");
		sprintf(FileName[1], "klike.dat");
		sprintf(FileName[2], "Plike2.dat");
		sprintf(FileName[3], "klike2.dat");
		
		/* 表示ラベル名 */
		sprintf(LabelName[0], "G4Proton");
		sprintf(LabelName[1], "G4kaon");
		
		/* 規格化の有無を確認する */
		printf("Set Normaliation? :\n");
		printf("answer by \"y\"(=yes) or \"n\"(=no) :  ");
		scanf("%s", chr);
		
		/* 無限ループ */
		do{
			
			/* 規格化あり */
			if(strcmp(chr, "y") == 0)
				Normalization = true;
			
			/* 規格化なし */
			else if(strcmp(chr, "n") == 0)
				Normalization = false;
			
			/* その他 */
			else{
				printf("Please answer by \"y\" or \"n\"");
				continue;
			}
			
			break;
			
		}while(1);
		
		/* X軸タイトル名 */
		sprintf(XAxisLabel, "PID(Proton)");
		
		/* グラフの作成 */
		if(this->CreateGraph() != true)
			printf("Sorry ... Can not create a graph!!\n");
		
	}
	
	/* メンバ:Protonとπのグラフ設定 */
	void MakeProtonPionGraph(char *gfilename){
		char chr[2];	/* ユーザ返答 */
		
		/* 線色番号 */
		LineColor[0] = 14;
		LineColor[1] = 2;
		
		/* キャンパス名 */
		if(sizeof(gfilename) <= 64)
			sprintf(CampusName, "%s", gfilename);
		else
			sprintf(CampusName, "c1");
		
		/* Statisticsタイトル */
		sprintf(GraphName[0], "PID(Proton)");
		sprintf(GraphName[1], "PID(Proton) of pion");
		
		/* グラフタイトル */
		sprintf(GraphTitle, "Geant4 Proton/Pion Simulation");
		
		/* File名 */
		sprintf(FileName[0], "Plike.dat");
		sprintf(FileName[1], "pilike.dat");
		sprintf(FileName[2], "Plike2.dat");
		sprintf(FileName[3], "pilike2.dat");
		
		/* 表示ラベル名 */
		sprintf(LabelName[0], "G4Proton");
		sprintf(LabelName[1], "G4pion");
		
		/* 規格化の有無を確認する */
		printf("Set Normaliation? :\n");
		printf("answer by \"y\"(=yes) or \"n\"(=no) :  ");
		scanf("%s", chr);
		
		/* 無限ループ */
		do{
			
			/* 規格化あり */
			if(strcmp(chr, "y") == 0)
				Normalization = true;
			
			/* 規格化なし */
			else if(strcmp(chr, "n") == 0)
				Normalization = false;
			
			/* その他 */
			else{
				printf("Please answer by \"y\" or \"n\"");
				continue;
			}
			
			break;
			
		}while(1);
		
		/* X軸タイトル名 */
		sprintf(XAxisLabel, "PID(Proton)");
		
		/* グラフの作成 */
		if(this->CreateGraph() != true)
			printf("Sorry ... Can not create a graph!!\n");
		
	}
	
	/* メンバ:Kとπのグラフ設定 */
	void MakeKaonPionGraph(char *gfilename){
		char chr[2];	/* ユーザ返答 */
		
		/* 線色番号 */
		LineColor[0] = 3;
		LineColor[1] = 2;
		
		/* キャンパス名 */
		if(sizeof(gfilename) <= 64)
			sprintf(CampusName, "%s", gfilename);
		else
			sprintf(CampusName, "c1");
		
		/* Statisticsタイトル */
		sprintf(GraphName[0], "PID(Kaon)");
		sprintf(GraphName[1], "PID(Kaon) of pion");
		
		/* グラフタイトル */
		sprintf(GraphTitle, "Geant4 Kaon/Pion Simulation");
		
		/* File名 */
		sprintf(FileName[0], "klike.dat");
		sprintf(FileName[1], "pilike.dat");
		sprintf(FileName[2], "klike2.dat");
		sprintf(FileName[3], "pilike2.dat");
		
		/* 表示ラベル名 */
		sprintf(LabelName[0], "G4kaon");
		sprintf(LabelName[1], "G4pion");
		
		/* 規格化の有無を確認する */
		printf("Set Normaliation? :\n");
		printf("answer by \"y\"(=yes) or \"n\"(=no) :  ");
		scanf("%s", chr);
		
		/* 無限ループ */
		do{
			
			/* 規格化あり */
			if(strcmp(chr, "y") == 0)
				Normalization = true;
			
			/* 規格化なし */
			else if(strcmp(chr, "n") == 0)
				Normalization = false;
			
			/* その他 */
			else{
				printf("Please answer by \"y\" or \"n\"");
				continue;
			}
			
			break;
			
		}while(1);
		
		/* X軸タイトル名 */
		sprintf(XAxisLabel, "PID(kaon)");
		
		/* グラフの作成 */
		if(this->CreateGraph() != true)
			printf("Sorry ... Can not create a graph!!\n");
		
	}
	
	/* メンバ:ユーザ設定 */
	bool UserSetupGraph(char *gfilename){
		char chr[2];	/* 規格化設定 */
		char head[64];	/* 設定項目名 */
		char linebuffer[4096];	/* 読み込み行格納 */
		FILE *tbl;	/* FIleポインタ */
		
		/* キャンパス名 */
		if(sizeof(gfilename) <= 64)
			sprintf(CampusName, "%s", gfilename);
		else
			sprintf(CampusName, "c1");
		
		/* ユーザ設定Fileを開く */
		if((tbl = fopen("UserDefine.tbl", "r"))==NULL){
			printf("Set up file[UserDefine.tbl] notopen!!\n");
			return false;
		}
		
		/* 読み込み開始 */
		while(fgets(linebuffer, sizeof(linebuffer), tbl) != NULL){
			
			/* 設定項目名の行を探す */
			if(linebuffer[0] == '[')
				sscanf(linebuffer, "%s", head);
			else
				continue;
			
			/* 共通の設定 */
			if(strcmp(head, "[Common]") == 0){
				
				/* 行の読み込みとグラフタイトル名の取得 */
				fgets(linebuffer, sizeof(linebuffer), tbl);
				strcpy(linebuffer, StrControl::strPoint(linebuffer, ":", 0) + 1);
				strcpy(linebuffer, StrControl::strTriming(linebuffer));
				sprintf(GraphTitle, "%s", linebuffer);
				
				/* 行の読み込みと規格化設定の取得 */
				fgets(linebuffer, sizeof(linebuffer), tbl);
				strcpy(linebuffer, StrControl::strPoint(linebuffer, ":", 0) + 1);
				strcpy(linebuffer, StrControl::strTriming(linebuffer));
				sscanf(linebuffer, "%s", chr);
				
				/* 規格化変数に保存 */
				if(strcmp(chr, "y") == 0)
					Normalization = true;
				else if(strcmp(chr, "n") == 0)
					Normalization = false;
				else{
					printf("Nomarization factor must be answered by \"y\" or \"n\"");
					return false;
				}
				
				/* 行の読み込みとX軸タイトル名の取得 */
				fgets(linebuffer, sizeof(linebuffer), tbl);
				strcpy(linebuffer, StrControl::strPoint(linebuffer, ":", 0) + 1);
				strcpy(linebuffer, StrControl::strTriming(linebuffer));
				sprintf(XAxisLabel, "%s", linebuffer);
				
			}
			
			/* 主グラフ設定 */
			else if(strcmp(head, "[Main]") == 0){
				
				/* 行の読み込みと線色番号の取得 */
				fgets(linebuffer, sizeof(linebuffer), tbl);
				strcpy(linebuffer, StrControl::strPoint(linebuffer, ":", 0) + 1);
				strcpy(linebuffer, StrControl::strTriming(linebuffer));
				sscanf(linebuffer, "%d", &LineColor[0]);
				
				/* 行の読み込みとStatistics名の取得 */
				fgets(linebuffer, sizeof(linebuffer), tbl);
				strcpy(linebuffer, StrControl::strPoint(linebuffer, ":", 0) + 1);
				strcpy(linebuffer, StrControl::strTriming(linebuffer));
				sprintf(GraphName[0], "%s", linebuffer);
				
				/* 行の読み込みと片方のFile名の取得 */
				fgets(linebuffer, sizeof(linebuffer), tbl);
				strcpy(linebuffer, StrControl::strPoint(linebuffer, ":", 0) + 1);
				strcpy(linebuffer, StrControl::strTriming(linebuffer));
				sprintf(FileName[0], "%s", linebuffer);
				
				/* 行の読み込みと一方のFile名の取得 */
				fgets(linebuffer, sizeof(linebuffer), tbl);
				strcpy(linebuffer, StrControl::strPoint(linebuffer, ":", 0) + 1);
				strcpy(linebuffer, StrControl::strTriming(linebuffer));
				sprintf(FileName[1], "%s", linebuffer);
				
				/* 行の読み込みと表示ラベル名の取得 */
				fgets(linebuffer, sizeof(linebuffer), tbl);
				strcpy(linebuffer, StrControl::strPoint(linebuffer, ":", 0) + 1);
				strcpy(linebuffer, StrControl::strTriming(linebuffer));
				sprintf(LabelName[0], "%s", linebuffer);
				
			}
			
			/* 副グラフ設定 */
			else if(strcmp(head, "[Sub]") == 0){
				
				/* 行の読み込みと線色番号の取得 */
				fgets(linebuffer, sizeof(linebuffer), tbl);
				strcpy(linebuffer, StrControl::strPoint(linebuffer, ":", 0) + 1);
				strcpy(linebuffer, StrControl::strTriming(linebuffer));
				sscanf(linebuffer, "%d", &LineColor[1]);
				
				/* 行の読み込みとStatistics名の取得 */
				fgets(linebuffer, sizeof(linebuffer), tbl);
				strcpy(linebuffer, StrControl::strPoint(linebuffer, ":", 0) + 1);
				strcpy(linebuffer, StrControl::strTriming(linebuffer));
				sprintf(GraphName[1], "%s", linebuffer);
				
				/* 行の読み込みと片方のFile名の取得 */
				fgets(linebuffer, sizeof(linebuffer), tbl);
				strcpy(linebuffer, StrControl::strPoint(linebuffer, ":", 0) + 1);
				strcpy(linebuffer, StrControl::strTriming(linebuffer));
				sprintf(FileName[2], "%s", linebuffer);
				
				/* 行の読み込みと一方のFile名の取得 */
				fgets(linebuffer, sizeof(linebuffer), tbl);
				strcpy(linebuffer, StrControl::strPoint(linebuffer, ":", 0) + 1);
				strcpy(linebuffer, StrControl::strTriming(linebuffer));
				sprintf(FileName[3], "%s", linebuffer);
				
				/* 行の読み込みと表示ラベル名の取得 */
				fgets(linebuffer, sizeof(linebuffer), tbl);
				strcpy(linebuffer, StrControl::strPoint(linebuffer, ":", 0) + 1);
				strcpy(linebuffer, StrControl::strTriming(linebuffer));
				sprintf(LabelName[1], "%s", linebuffer);
				
			}
		}	/* while [読み込み開始] end */
		
		/* グラフの作成 */
		if(this->CreateGraph() != true)
			printf("Sorry ... Can not create a graph!!\n");
		
	}
	
};

/* 関数の準備 */
void Efficiency(TH1F*, TH1F*, Int_t, Double_t, Double_t, Double_t, Double_t, const char*, const char*, const char*);

/* メインメソッド */
void MakeLRgraph(){
	int mode;	/* 処理モード */
	char gfname[256];	/* キャンパス名 */

	/* read parm */
	 Int_t    bin1 =    1;
	 Int_t    bin  =  275;
	 Double_t min  = -155;
	 Double_t max  =  120; 
	
	/* グラフ設定オブジェクト */
	Distribution *calc = new Distribution();
	
	/* 処理モードの選択 */
	printf("Select the number below :\n");
	printf("--- For Default Graph ---------------------------------\n");
	printf("\t1) [Xi , kaon]\t\t2) [Xi, pion]\n");
	printf("\t3) [Proton , kaon]\t4) [Proton, pion]\n");
	printf("\t5) [kaon, pion]\n");
	printf("--- For User Definition -----------------------------------\n");
	printf("\t6) User Defined or Real Data Graph\n");
	printf("[Answer ] :  ");
	scanf("%d", &mode);
	
	/* キャンパス名の入力 */
	printf("Input graph file name :  ");
	scanf("%s", gfname);
	
	/* 選択に応じた処理分岐 */
	switch(mode){
	case 1:
		calc->MakeXiKaonGraph(gfname);
		break;
	case 2:
		calc->MakeXiPionGraph(gfname);
		break;
	case 3:
		calc->MakeProtonKaonGraph(gfname);
		break;
	case 4:
		calc->MakeProtonPionGraph(gfname);
		break;
	case 5:
		calc->MakeKaonPionGraph(gfname);
		break;
	case 6:
		calc->UserSetupGraph(gfname);
		break;
	default:
		printf("Please select a number!!\n");
		
	}
	
}


/* efficiency function */
void Efficiency(TH1F* h_pion, TH1F* h_kaon, Int_t bin, Double_t min, Double_t max, Double_t pion_all, Double_t kaon_all, const char* title1, const char* title2, const char* title3)
{
  std::ofstream outE(title1);
  std::ofstream outF(title2);
  std::ofstream outEF(title3);

  for(Int_t i = 0; i < bin; i++){
  Int_t j = i + 1;
  Double_t x0 = h_kaon->GetXaxis()->GetBinCenter(i+1);
  Double_t xd = (max-min)/bin;
  Double_t xi = x0-xd/2;
  Double_t total11 = h_pion->Integral(j,bin+1);
  Double_t total12 = h_kaon->Integral(j,bin+1);

//  if(total11 == 0 && total12 == 0 ) break; 

  Double_t Total   = total11 + total12;
  Double_t eff     = (total11/pion_all)*100;
  Double_t fake    = (total12/kaon_all)*100;

  Double_t pi_error = sqrt(pion_all);
  Double_t k_error  = sqrt(kaon_all);
  Double_t error11  = sqrt(total11);
  Double_t error12  = sqrt(total12);
  Double_t s_e = sqrt((eff/100/pion_all*pi_error)*(eff/100/pion_all*pi_error)+(error11/pion_all)*(error11/pion_all))*100;
  Double_t s_f = sqrt((fake/100/kaon_all*k_error)*(fake/100/kaon_all*k_error)+(error12/kaon_all)*(error12/kaon_all))*100;
  Double_t s_x = 0;

//  cout << "pion total = " << total11 << endl;
//  cout << "kaon total = " << total12 << endl;
//  cout << "Total      = " << Total   << endl;

//  cout << "threshold : "  << xi      << endl;
//  cout << "efficiency : " << eff     << " % " << endl;
//  cout << "fake       : " << fake    << " % " << endl;

   outE << xi << " "
       << eff << " "
       << s_x << " "
       << s_e << endl;
  outF << xi << " "
       << fake << " "
       << s_x << " "
       << s_f << endl;
  outEF << eff << " "
        << fake << " "
        << s_e << " "
        << s_f << endl;

  }

 return;
}

