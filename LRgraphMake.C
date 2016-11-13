
/* �N���X:�����񑀍�p�֐����i�[ */
class StrControl{
	
public:	/* �A�N�Z�X�C���q */
	
	/* �ÓI�����o:�w�肳�ꂽ����������ʒu�̃|�C���^��Ԃ� */
	static char* strPoint(char *input, char *schr, int index){
		
		/* �R�����g�F�����̐��� */
		/*           ARG1 - �Ώە����� */
		/*           ARG2 - �������镶�� */
		/*           ARG3 - ���Ԗڂ̎w�蕶�����������邩�w�肷�鐔�l */
		
		/* �ϐ��錾 */
		int cnum;	/* �w�蕶���J�E���^ */
		char cp[2];	/* �Ώە��� */
		char *rtn;	/* �����|�C���^ */
		
		/* �|�C���^�������� */
		rtn = NULL;
		
		/* �w�蕶�����P�����ł��邩�m�F */
		if(strlen(schr) != 1)
			return rtn;
		
		/* ������ */
		cnum = 0;
		
		/* ������S�̂��������� */
		for(int i=0; i<strlen(input); i++){
			
			/* �����񂩂�P�����擾 */
			sprintf(cp, "%c", input[i]);
			
			/* �w�蕶���ƏƂ炵���킹�� */
			if(strcmp(cp, schr) == 0){
				
				/* ������΃J�E���^�����Z */
				cnum++;
				
				/* �w�肵�����l�ȏ�ł���΃|�C���^���擾 */
				if(cnum > index){
					rtn = input + i;
					break;
				}
				
			}
		}
		
		/* �߂�l���Z�b�g */
		return rtn;
		
	}
	
	/* �ÓI�����o:������̍��E���g���~���O */
	static char* strTriming(char *input){
		
		/* �R�����g�F�����Ɩ߂�l�̐��� */
		/*           ARG1 - �Ώە����� */
		/*           RTN  - �g���~���O��̐擪�����̃|�C���^ */
		
		int pos;	/* �����ʒu */
		
		/* ������S�̂��猟�� */
		for(pos=0; pos<strlen(input); pos++){
			
			/* �擪����X�y�[�X�����ƃ^�u������ǂݔ�΂� */
			if(input[pos] == ' ' || input[pos] == '\t')
				continue;
			
			/* �����������������烋�[�v���I�� */
			break;
			
		}
		
		/* �����̕�����̍ŏI�������猟�� */
		for(int i=strlen(input)-1; i>pos; i--){
			
			/* �X�y�[�X�����A�^�u�����A���s�����łȂ������Ȃ�� */
			if(input[i] != ' ' && input[i] != '\t' && input[i] != '\n'){
				
				/* �P��̕�����NULL�����ɂ��� */
				input[i+1] = '\0';
				
				/* ���[�v�I�� */
				break;
			}
			
		}
		
		/* �߂�l�Z�b�g */
		return (input + pos);
		
	}
	
};

/* �N���X:�M���x�̌v�Z */
class ConfidenceLevel{
	
	/* �R�����g�F���m�Ȓ�`�����������揑������ */
	
};

/* �N���X:�O���t�̕`�ʋ@�\�A�M���x�v�Z�N���X���p�� */
class Distribution : public ConfidenceLevel{
	
private:	/* �A�N�Z�X�C���q */
	
	/* �t�B�[���h�ϐ� */
	int LineColor[2];	/* �q�X�g�O�����̐��F�ԍ� */
	char CampusName[64];	/* �L�����p�X�� */
	char GraphName[2][64];	/* Statistics�^�C�g�� */
	char GraphTitle[256];	/* �O���t�̃^�C�g�� */
	char XAxisLabel[64];	/* X�����x���� */
	char YAxisLabel[64];	/* Y�����x���� */
	char FileName[4][256];	/* �ǂݍ���File�� */
	char LabelName[2][64];	/* �\�����x���� */
	bool Normalization;	/* �K�i���@�\�ݒ� */
	
	/* �R�����g�F�K�i���@�\�ϐ�(Normalization)�ɂ��� */
	/*           true  - �S�����ɂ��K�i�������� */
	/*           false - �K�i���Ȃ��A�c���͔�Ր��ɂȂ� */
	
	/* �����o:�ޓxFile�̓ǂݍ��݂ƃO���t�̕`�� */
	bool CreateGraph(){
		
		/* �ϐ��錾 */
		int item;	/* �ǂݍ��ݍ��ڐ��J�E���^ */
		int max;	/* ���[�v�I���l */
		int MainNum;	/* �Е��ɑ΂���ޓx�䐔 */
		int SubNum;	/* ��������ɑ΂���ޓx�䐔 */
		double unsigned const div = 100.;	/* ������ */
		double xiLike[4096];	/* �Е��̖ޓx */
		double kLike[4096];	/* ��������̖ޓx */
		double LR;	/* �ޓx�� */
		double AVE_Xi;	/* �Е��̖ޓx�䕽�ϒl */
		double AVE_K;	/* ��������̖ޓx�䕽�ϒl */
		char ID[16];	/* ��Ղ�ID */
		char linebuffer[4096];	/* �]�����ݍs�i�[ */
		FILE *p1,*p2;	/* File�|�C���^ */
		
		/* root�̐ݒ� */
		gROOT -> SetStyle("Plain");
		
		/* Y�����x���������� */
		strcpy(YAxisLabel, "Percent [%]");
		
		/* �P�����q�X�g�I�u�W�F�N�g */
		TH1F *h1 = new TH1F(GraphName[0], GraphTitle, div, 0, 1);
		TH1F *h2 = new TH1F(GraphName[1], GraphTitle, div, 0, 1);
		
		/* X�����x�����` */
		h1 -> SetXTitle(XAxisLabel);
		
		/* �Е���File���J�� */
		if ((p1 = fopen(FileName[0], "r")) == NULL) {
			printf("input_file[%s] not open\n", FileName[0]);
			return false;
		}
		
		/* ���ڐ��̏����� */
		item=0;
		
		/* �ޓx��̓ǂݍ��� */
		while(fgets(linebuffer,sizeof(linebuffer),p1)!=NULL){
			sscanf(linebuffer,"%s %lf %*d", ID, &xiLike[item]);
			
			/* �󕶎��łȂ���΍��ڐ������Z */
			if(strlen(linebuffer) > 0)
				item++;
			
		}
		
		/* File����� */
		fclose(p1);
		
		/* ���������File���J�� */
		if ((p2 = fopen(FileName[1], "r")) == NULL) { 
			printf("input_file[%s] not open\n", FileName[1]);
			return false; 
		} 
		
		/* ���ڐ��̏����� */
		item=0;
		
		/* �ޓx��̓ǂݍ��� */
		while(fgets(linebuffer,sizeof(linebuffer),p2)!=NULL){
			sscanf(linebuffer,"%s %lf %*d",ID,&kLike[item]);
			
			/* �󕶎��łȂ���΍��ڐ������Z */
			if(strlen(linebuffer) > 0)
				item++;
			
		}
		
		/* File����� */
		fclose(p2);
		
		/* ���ڐ���ۑ����� */
		max = item;
		
		/* �ޓx�䐔�̏����� */
		MainNum = 0;
		
		/* �擾���ڕ����[�v */
		for(item=0; item<max; item++){
			
			/* 0�Ŋ��邱�Ƃ������ */
			if(xiLike[item]==0 && kLike[item]==0)
				continue;
			
			/* �ޓx����v�Z */
			LR = xiLike[item] / (xiLike[item]+kLike[item]);
			
			/* 1�ł����0.999999�ɌŒ肷�� */
			/* �R�����g�F1�̓q�X�g�O�����͈̔͊O�ɂȂ邽�� */
			/*           �����I��1�ȓ��Ɏ��߂� */
			if(LR == 1.0)
				LR = 0.999999;
			
			/* �ޓx������Z���čs�� */
			AVE_Xi = AVE_Xi + LR;
			
			/* �ޓx�䐔�����Z */
			MainNum++;
			
			/* �ޓx����i�[ */
			h1 -> Fill(LR);
			
		}
		
		/* ���ϒl���v�Z */
		if(max != 0)
			AVE_Xi = AVE_Xi/((double) max);
		else
			AVE_Xi = 0.;
		
		/* �Е���File���J�� */
		if ((p1 = fopen(FileName[2], "r")) == NULL) {
			printf("input_file[%s] not open\n", FileName[2]);
			return false;
		}
		
		/* ���ڐ��̏����� */
		item=0;
		
		/* �ޓx��̓ǂݍ��� */
		while(fgets(linebuffer,sizeof(linebuffer),p1)!=NULL){
			sscanf(linebuffer,"%s %lf %*d",ID,&xiLike[item]);
			
			/* �󕶎��łȂ���΍��ڐ������Z */
			if(strlen(linebuffer) > 0)
				item++;
			
		}
		
		/* File����� */
		fclose(p1);
		
		/* ���������File���J�� */
		if ((p2 = fopen(FileName[3], "r")) == NULL) {
			printf("input_file[%s] not open\n", FileName[3]);
			return false;
		}
		
		/* ���ڐ��̏����� */
		item=0;
		
		/* �ޓx��̓ǂݍ��� */
		while(fgets(linebuffer,sizeof(linebuffer),p2)!=NULL){
			sscanf(linebuffer,"%s %lf %*d",ID,&kLike[item]);
			
			/* �󕶎��łȂ���΍��ڐ������Z */
			if(strlen(linebuffer) > 0)
				item++;
			
		}
		
		/* File����� */
		fclose(p2);
		
		/* ���ڐ���ۑ����� */
		max=item;
		
		/* �ޓx�䐔�̏����� */
		SubNum=0;
		
		/* �擾���ڕ����[�v */
		for(item=0; item<max; item++){
			
			/* 0�Ŋ��邱�Ƃ������ */
			if(xiLike[item]==0 && kLike[item]==0)
				continue;
			
			/* �ޓx����v�Z */
			LR = xiLike[item]/(xiLike[item]+kLike[item]);
			
			/* 1�ł����0.999999�ɌŒ肷�� */
			if(LR == 1.0)
				LR = 0.999999;
			
			/* �ޓx������Z���čs�� */
			AVE_K = AVE_K + LR;
			
			/* �ޓx�䐔�����Z */
			SubNum++;

			/* �ޓx����i�[ */
			h2 -> Fill(LR);
			
		}

		/* integral(from bin1 to bin) */
		 Double_t pion_all = h1->Integral(1,div);
		 Double_t kaon_all = h2->Integral(1,div);

                 cout << " First entries = (for integral function)" << pion_all << endl;
		/* calc eff (histgram,bin,integral,files)*/
		 Efficiency(h1,h2,div,0,1,pion_all,kaon_all,"eff0.txt","fake0.txt","EF0.txt"); // ��������I�I

		/* ���ϒl���v�Z */
		if(max != 0)
			AVE_K = AVE_K/((double) max);
		else
			AVE_K = 0.;
		
		/* �L�����p�X���쐬 */
		TCanvas *c1 = new TCanvas(CampusName, "Likelihood Ratio Distribution", 0, 0, 700, 700);
		
		/* ���F�̐��` */
		h1 -> SetLineColor(LineColor[0]);
		h2 -> SetLineColor(LineColor[1]);
		
		/* ���x���̐ݒ� */
		TText *ta = new TText(0.81, 1., LabelName[0]);
		ta -> SetTextSize(0.04);
		ta -> SetTextColor(LineColor[0]);
		TText *tb = new TText(0.06, 1., LabelName[1]);
		tb -> SetTextSize(0.04);
		tb -> SetTextColor(LineColor[1]);
		
		/* ���̑������` */
		h1 -> SetLineWidth(3);
		h2 -> SetLineWidth(3);
		
		/* �K�i���ݒ�̊m�F */
		if(Normalization){
			
			/* Y���̃^�C�g�����` */
			h1 -> SetYTitle(YAxisLabel);
			
			/* Y���I�t�Z�b�g */
			h1 -> GetYaxis()->SetTitleOffset(1.0);
			
			/* �K�i�����Ă���`�� */
			TH1F *H1 = h1->DrawNormalized("",100);
		}
		
		/* �K�i���Ȃ��ŕ`�� */
		else
			h1->Draw();
		
		/* �K�i���ݒ�̊m�F�ƁA����̃O���t��`�� */
		if(Normalization)
			TH1F *H2 = h2->DrawNormalized("same",100);
		else
			h2->Draw("same");
		
		/* ���x���̒ǋL */
		ta -> Draw();
		tb -> Draw();
		
		/* ���ڐ��̕\�� */
		printf("Entries : '%s' = %d\t'%s' = %d\n", GraphName[0], MainNum, GraphName[1], SubNum);
		
		/* ���ϒl�̕\�� */
		printf("AVE     : '%s' = %.4f\t'%s' = %.4f\n", GraphName[0], AVE_Xi, GraphName[1], AVE_K);
		
	}
	
public:	/* �A�N�Z�X�C���q */
	
	/* �����o:����K�̃O���t�ݒ� */
	void MakeXiKaonGraph(char *gfilename){
		char chr[2];	/* ���[�U�ԓ� */
		
		/* ���F�ԍ� */
		LineColor[0] = 4;
		LineColor[1] = 3;
		
		/* �L�����p�X�� */
		if(sizeof(gfilename) <= 64)
			sprintf(CampusName, "%s", gfilename);
		else
			sprintf(CampusName, "c1");
		
		/* Statistics�^�C�g�� */
		sprintf(GraphName[0], "PID(#Xi)");
		sprintf(GraphName[1], "PID(#Xi) of Kaon");
		
		/* �O���t�^�C�g�� */
		sprintf(GraphTitle, "Geant4 Xi/Kaon Simulation");
		
		/* File�� */
		sprintf(FileName[0], "xlike.dat");
		sprintf(FileName[1], "klike.dat");
		sprintf(FileName[2], "xlike2.dat");
		sprintf(FileName[3], "klike2.dat");
		
		/* �\�����x���� */
		sprintf(LabelName[0], "G4Xi");
		sprintf(LabelName[1], "G4kaon");
		
		/* �K�i���̗L�����m�F���� */
		printf("Set Normaliation? :\n");
		printf("answer by \"y\"(=yes) or \"n\"(=no) :  ");
		scanf("%s", chr);
		
		/* �������[�v */
		do{
			
			/* �K�i������ */
			if(strcmp(chr, "y") == 0)
				Normalization = true;
			
			/* �K�i���Ȃ� */
			else if(strcmp(chr, "n") == 0)
				Normalization = false;
			
			/* ���̑� */
			else{
				printf("Please answer by \"y\" or \"n\"");
				continue;
			}
			
			break;
			
		}while(1);
		
		/* X���^�C�g���� */
		sprintf(XAxisLabel, "PID(#Xi)");
		
		/* �O���t�̍쐬 */
		if(this->CreateGraph() != true)
			printf("Sorry ... Can not create a graph!!\n");

	}
	
	/* �����o:���ƃ΂̃O���t�ݒ� */
	void MakeXiPionGraph(char *gfilename){
		char chr[2];	/* ���[�U�ԓ� */
		
		/* ���F�ԍ� */
		LineColor[0] = 4;
		LineColor[1] = 2;
		
		/* �L�����p�X�� */
		if(sizeof(gfilename) <= 64)
			sprintf(CampusName, "%s", gfilename);
		else
			sprintf(CampusName, "c1");
		
		/* Statistics�^�C�g�� */
		sprintf(GraphName[0], "PID(#Xi)");
		sprintf(GraphName[1], "PID(#Xi) of Pion");
		
		/* �O���t�^�C�g�� */
		sprintf(GraphTitle, "Geant4 Xi/Pion Simulation");
		
		/* File�� */
		sprintf(FileName[0], "xlike.dat");
		sprintf(FileName[1], "pilike.dat");
		sprintf(FileName[2], "xlike2.dat");
		sprintf(FileName[3], "pilike2.dat");
		
		/* �\�����x���� */
		sprintf(LabelName[0], "G4Xi");
		sprintf(LabelName[1], "G4pion");
		
		/* �K�i���̗L�����m�F���� */
		printf("Set Normaliation? :\n");
		printf("answer by \"y\"(=yes) or \"n\"(=no) :  ");
		scanf("%s", chr);
		
		/* �������[�v */
		do{
			
			/* �K�i������ */
			if(strcmp(chr, "y") == 0)
				Normalization = true;
			
			/* �K�i���Ȃ� */
			else if(strcmp(chr, "n") == 0)
				Normalization = false;
			
			/* ���̑� */
			else{
				printf("Please answer by \"y\" or \"n\"");
				continue;
			}
			
			break;
			
		}while(1);
		
		/* X���^�C�g���� */
		sprintf(XAxisLabel, "PID(#Xi)");
		
		/* �O���t�̍쐬 */
		if(this->CreateGraph() != true)
			printf("Sorry ... Can not create a graph!!\n");
		
	}
	
	/* �����o:Proton��K�̃O���t�ݒ� */
	void MakeProtonKaonGraph(char *gfilename){
		char chr[2];	/* ���[�U�ԓ� */
		
		/* ���F�ԍ� */
		LineColor[0] = 14;
		LineColor[1] = 3;
		
		/* �L�����p�X�� */
		if(sizeof(gfilename) <= 64)
			sprintf(CampusName, "%s", gfilename);
		else
			sprintf(CampusName, "c1");
		
		/* Statistics�^�C�g�� */
		sprintf(GraphName[0], "PID(Proton)");
		sprintf(GraphName[1], "PID(Proton) of Kaon");
		
		/* �O���t�^�C�g�� */
		sprintf(GraphTitle, "Geant4 Proton/Kaon Simulation");
		
		/* File�� */
		sprintf(FileName[0], "Plike.dat");
		sprintf(FileName[1], "klike.dat");
		sprintf(FileName[2], "Plike2.dat");
		sprintf(FileName[3], "klike2.dat");
		
		/* �\�����x���� */
		sprintf(LabelName[0], "G4Proton");
		sprintf(LabelName[1], "G4kaon");
		
		/* �K�i���̗L�����m�F���� */
		printf("Set Normaliation? :\n");
		printf("answer by \"y\"(=yes) or \"n\"(=no) :  ");
		scanf("%s", chr);
		
		/* �������[�v */
		do{
			
			/* �K�i������ */
			if(strcmp(chr, "y") == 0)
				Normalization = true;
			
			/* �K�i���Ȃ� */
			else if(strcmp(chr, "n") == 0)
				Normalization = false;
			
			/* ���̑� */
			else{
				printf("Please answer by \"y\" or \"n\"");
				continue;
			}
			
			break;
			
		}while(1);
		
		/* X���^�C�g���� */
		sprintf(XAxisLabel, "PID(Proton)");
		
		/* �O���t�̍쐬 */
		if(this->CreateGraph() != true)
			printf("Sorry ... Can not create a graph!!\n");
		
	}
	
	/* �����o:Proton�ƃ΂̃O���t�ݒ� */
	void MakeProtonPionGraph(char *gfilename){
		char chr[2];	/* ���[�U�ԓ� */
		
		/* ���F�ԍ� */
		LineColor[0] = 14;
		LineColor[1] = 2;
		
		/* �L�����p�X�� */
		if(sizeof(gfilename) <= 64)
			sprintf(CampusName, "%s", gfilename);
		else
			sprintf(CampusName, "c1");
		
		/* Statistics�^�C�g�� */
		sprintf(GraphName[0], "PID(Proton)");
		sprintf(GraphName[1], "PID(Proton) of pion");
		
		/* �O���t�^�C�g�� */
		sprintf(GraphTitle, "Geant4 Proton/Pion Simulation");
		
		/* File�� */
		sprintf(FileName[0], "Plike.dat");
		sprintf(FileName[1], "pilike.dat");
		sprintf(FileName[2], "Plike2.dat");
		sprintf(FileName[3], "pilike2.dat");
		
		/* �\�����x���� */
		sprintf(LabelName[0], "G4Proton");
		sprintf(LabelName[1], "G4pion");
		
		/* �K�i���̗L�����m�F���� */
		printf("Set Normaliation? :\n");
		printf("answer by \"y\"(=yes) or \"n\"(=no) :  ");
		scanf("%s", chr);
		
		/* �������[�v */
		do{
			
			/* �K�i������ */
			if(strcmp(chr, "y") == 0)
				Normalization = true;
			
			/* �K�i���Ȃ� */
			else if(strcmp(chr, "n") == 0)
				Normalization = false;
			
			/* ���̑� */
			else{
				printf("Please answer by \"y\" or \"n\"");
				continue;
			}
			
			break;
			
		}while(1);
		
		/* X���^�C�g���� */
		sprintf(XAxisLabel, "PID(Proton)");
		
		/* �O���t�̍쐬 */
		if(this->CreateGraph() != true)
			printf("Sorry ... Can not create a graph!!\n");
		
	}
	
	/* �����o:K�ƃ΂̃O���t�ݒ� */
	void MakeKaonPionGraph(char *gfilename){
		char chr[2];	/* ���[�U�ԓ� */
		
		/* ���F�ԍ� */
		LineColor[0] = 3;
		LineColor[1] = 2;
		
		/* �L�����p�X�� */
		if(sizeof(gfilename) <= 64)
			sprintf(CampusName, "%s", gfilename);
		else
			sprintf(CampusName, "c1");
		
		/* Statistics�^�C�g�� */
		sprintf(GraphName[0], "PID(Kaon)");
		sprintf(GraphName[1], "PID(Kaon) of pion");
		
		/* �O���t�^�C�g�� */
		sprintf(GraphTitle, "Geant4 Kaon/Pion Simulation");
		
		/* File�� */
		sprintf(FileName[0], "klike.dat");
		sprintf(FileName[1], "pilike.dat");
		sprintf(FileName[2], "klike2.dat");
		sprintf(FileName[3], "pilike2.dat");
		
		/* �\�����x���� */
		sprintf(LabelName[0], "G4kaon");
		sprintf(LabelName[1], "G4pion");
		
		/* �K�i���̗L�����m�F���� */
		printf("Set Normaliation? :\n");
		printf("answer by \"y\"(=yes) or \"n\"(=no) :  ");
		scanf("%s", chr);
		
		/* �������[�v */
		do{
			
			/* �K�i������ */
			if(strcmp(chr, "y") == 0)
				Normalization = true;
			
			/* �K�i���Ȃ� */
			else if(strcmp(chr, "n") == 0)
				Normalization = false;
			
			/* ���̑� */
			else{
				printf("Please answer by \"y\" or \"n\"");
				continue;
			}
			
			break;
			
		}while(1);
		
		/* X���^�C�g���� */
		sprintf(XAxisLabel, "PID(kaon)");
		
		/* �O���t�̍쐬 */
		if(this->CreateGraph() != true)
			printf("Sorry ... Can not create a graph!!\n");
		
	}
	
	/* �����o:���[�U�ݒ� */
	bool UserSetupGraph(char *gfilename){
		char chr[2];	/* �K�i���ݒ� */
		char head[64];	/* �ݒ荀�ږ� */
		char linebuffer[4096];	/* �ǂݍ��ݍs�i�[ */
		FILE *tbl;	/* FIle�|�C���^ */
		
		/* �L�����p�X�� */
		if(sizeof(gfilename) <= 64)
			sprintf(CampusName, "%s", gfilename);
		else
			sprintf(CampusName, "c1");
		
		/* ���[�U�ݒ�File���J�� */
		if((tbl = fopen("UserDefine.tbl", "r"))==NULL){
			printf("Set up file[UserDefine.tbl] notopen!!\n");
			return false;
		}
		
		/* �ǂݍ��݊J�n */
		while(fgets(linebuffer, sizeof(linebuffer), tbl) != NULL){
			
			/* �ݒ荀�ږ��̍s��T�� */
			if(linebuffer[0] == '[')
				sscanf(linebuffer, "%s", head);
			else
				continue;
			
			/* ���ʂ̐ݒ� */
			if(strcmp(head, "[Common]") == 0){
				
				/* �s�̓ǂݍ��݂ƃO���t�^�C�g�����̎擾 */
				fgets(linebuffer, sizeof(linebuffer), tbl);
				strcpy(linebuffer, StrControl::strPoint(linebuffer, ":", 0) + 1);
				strcpy(linebuffer, StrControl::strTriming(linebuffer));
				sprintf(GraphTitle, "%s", linebuffer);
				
				/* �s�̓ǂݍ��݂ƋK�i���ݒ�̎擾 */
				fgets(linebuffer, sizeof(linebuffer), tbl);
				strcpy(linebuffer, StrControl::strPoint(linebuffer, ":", 0) + 1);
				strcpy(linebuffer, StrControl::strTriming(linebuffer));
				sscanf(linebuffer, "%s", chr);
				
				/* �K�i���ϐ��ɕۑ� */
				if(strcmp(chr, "y") == 0)
					Normalization = true;
				else if(strcmp(chr, "n") == 0)
					Normalization = false;
				else{
					printf("Nomarization factor must be answered by \"y\" or \"n\"");
					return false;
				}
				
				/* �s�̓ǂݍ��݂�X���^�C�g�����̎擾 */
				fgets(linebuffer, sizeof(linebuffer), tbl);
				strcpy(linebuffer, StrControl::strPoint(linebuffer, ":", 0) + 1);
				strcpy(linebuffer, StrControl::strTriming(linebuffer));
				sprintf(XAxisLabel, "%s", linebuffer);
				
			}
			
			/* ��O���t�ݒ� */
			else if(strcmp(head, "[Main]") == 0){
				
				/* �s�̓ǂݍ��݂Ɛ��F�ԍ��̎擾 */
				fgets(linebuffer, sizeof(linebuffer), tbl);
				strcpy(linebuffer, StrControl::strPoint(linebuffer, ":", 0) + 1);
				strcpy(linebuffer, StrControl::strTriming(linebuffer));
				sscanf(linebuffer, "%d", &LineColor[0]);
				
				/* �s�̓ǂݍ��݂�Statistics���̎擾 */
				fgets(linebuffer, sizeof(linebuffer), tbl);
				strcpy(linebuffer, StrControl::strPoint(linebuffer, ":", 0) + 1);
				strcpy(linebuffer, StrControl::strTriming(linebuffer));
				sprintf(GraphName[0], "%s", linebuffer);
				
				/* �s�̓ǂݍ��݂ƕЕ���File���̎擾 */
				fgets(linebuffer, sizeof(linebuffer), tbl);
				strcpy(linebuffer, StrControl::strPoint(linebuffer, ":", 0) + 1);
				strcpy(linebuffer, StrControl::strTriming(linebuffer));
				sprintf(FileName[0], "%s", linebuffer);
				
				/* �s�̓ǂݍ��݂ƈ����File���̎擾 */
				fgets(linebuffer, sizeof(linebuffer), tbl);
				strcpy(linebuffer, StrControl::strPoint(linebuffer, ":", 0) + 1);
				strcpy(linebuffer, StrControl::strTriming(linebuffer));
				sprintf(FileName[1], "%s", linebuffer);
				
				/* �s�̓ǂݍ��݂ƕ\�����x�����̎擾 */
				fgets(linebuffer, sizeof(linebuffer), tbl);
				strcpy(linebuffer, StrControl::strPoint(linebuffer, ":", 0) + 1);
				strcpy(linebuffer, StrControl::strTriming(linebuffer));
				sprintf(LabelName[0], "%s", linebuffer);
				
			}
			
			/* ���O���t�ݒ� */
			else if(strcmp(head, "[Sub]") == 0){
				
				/* �s�̓ǂݍ��݂Ɛ��F�ԍ��̎擾 */
				fgets(linebuffer, sizeof(linebuffer), tbl);
				strcpy(linebuffer, StrControl::strPoint(linebuffer, ":", 0) + 1);
				strcpy(linebuffer, StrControl::strTriming(linebuffer));
				sscanf(linebuffer, "%d", &LineColor[1]);
				
				/* �s�̓ǂݍ��݂�Statistics���̎擾 */
				fgets(linebuffer, sizeof(linebuffer), tbl);
				strcpy(linebuffer, StrControl::strPoint(linebuffer, ":", 0) + 1);
				strcpy(linebuffer, StrControl::strTriming(linebuffer));
				sprintf(GraphName[1], "%s", linebuffer);
				
				/* �s�̓ǂݍ��݂ƕЕ���File���̎擾 */
				fgets(linebuffer, sizeof(linebuffer), tbl);
				strcpy(linebuffer, StrControl::strPoint(linebuffer, ":", 0) + 1);
				strcpy(linebuffer, StrControl::strTriming(linebuffer));
				sprintf(FileName[2], "%s", linebuffer);
				
				/* �s�̓ǂݍ��݂ƈ����File���̎擾 */
				fgets(linebuffer, sizeof(linebuffer), tbl);
				strcpy(linebuffer, StrControl::strPoint(linebuffer, ":", 0) + 1);
				strcpy(linebuffer, StrControl::strTriming(linebuffer));
				sprintf(FileName[3], "%s", linebuffer);
				
				/* �s�̓ǂݍ��݂ƕ\�����x�����̎擾 */
				fgets(linebuffer, sizeof(linebuffer), tbl);
				strcpy(linebuffer, StrControl::strPoint(linebuffer, ":", 0) + 1);
				strcpy(linebuffer, StrControl::strTriming(linebuffer));
				sprintf(LabelName[1], "%s", linebuffer);
				
			}
		}	/* while [�ǂݍ��݊J�n] end */
		
		/* �O���t�̍쐬 */
		if(this->CreateGraph() != true)
			printf("Sorry ... Can not create a graph!!\n");
		
	}
	
};

/* �֐��̏��� */
void Efficiency(TH1F*, TH1F*, Int_t, Double_t, Double_t, Double_t, Double_t, const char*, const char*, const char*);

/* ���C�����\�b�h */
void MakeLRgraph(){
	int mode;	/* �������[�h */
	char gfname[256];	/* �L�����p�X�� */

	/* read parm */
	 Int_t    bin1 =    1;
	 Int_t    bin  =  275;
	 Double_t min  = -155;
	 Double_t max  =  120; 
	
	/* �O���t�ݒ�I�u�W�F�N�g */
	Distribution *calc = new Distribution();
	
	/* �������[�h�̑I�� */
	printf("Select the number below :\n");
	printf("--- For Default Graph ---------------------------------\n");
	printf("\t1) [Xi , kaon]\t\t2) [Xi, pion]\n");
	printf("\t3) [Proton , kaon]\t4) [Proton, pion]\n");
	printf("\t5) [kaon, pion]\n");
	printf("--- For User Definition -----------------------------------\n");
	printf("\t6) User Defined or Real Data Graph\n");
	printf("[Answer ] :  ");
	scanf("%d", &mode);
	
	/* �L�����p�X���̓��� */
	printf("Input graph file name :  ");
	scanf("%s", gfname);
	
	/* �I���ɉ������������� */
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
