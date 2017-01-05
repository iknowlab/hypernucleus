#include<stdio.h>
#include<stdlib.h>
#include <math.h>

int main(int argc,char *fname[]){
	int loop;	/* ���[�v�J�E���^ */
	int line[2];	/* �ǂݍ��ݍs�� */
	int freq;	/* ����x�� */
	int Ncmp;	/* ���葤�f�[�^�� */
	int Nrd;	/* �V�~�����[�V�������f�[�^�� */
	int div;	/* ���z�̕����� */
	int rst;	/* ��������� */
	int ndf; /* ���R�x */
	double P1;	/* �Е��̖ޓx */
	double P2;	/* ��������̖ޓx */
	double LikeRatio[100000];	/* �ޓx�� */
	double Ek[30];	/* ���Ғl */
	double Ok[30];	/* ����x���̕ۑ� */
	double Lthresh[1000];	/* �v�f������臒l */
	double SUM_Chi2;	/* ��2��l */
	char linebuffer[2048];	/* �ǂݍ��ݍs */
	char id[16];	/* ID */
	FILE *rf1,*rf2,*cf1,*cf2;	/* File�|�C���^ */
	
	/* ��P������File���J�� */
	if ((rf1 = fopen(fname[1], "r")) == NULL) { 
		printf("input_file[%s] not open\n",fname[1]); 
		exit(1); 
	}
	
	/* ��Q������File���J�� */
	if ((rf2 = fopen(fname[2], "r")) == NULL) { 
		printf("input_file[%s] not open\n",fname[2]); 
		exit(1); 
	}
	
	/* ��R������File���J�� */
	if ((cf1 = fopen(fname[3], "r")) == NULL) {
		printf("input_file[%s] not open\n",fname[3]);
		exit(1);
	}
	
	/* ��S������File���J�� */
	if ((cf2 = fopen(fname[4], "r")) == NULL) {
		printf("input_file[%s] not open\n",fname[4]);
		exit(1);
	}
	
	/* �ǂݍ���File�̕\�� */
	printf(">******* Caluclation for Mean /& Observation frequency *****************\n");
	printf("> Using FILE :\n");
	printf(">	[%s] /& [%s]  for Comprition data\n",fname[3],fname[4]);
	printf("> Using FILE :\n");
	printf(">	[%s] /& [%s]  for Chi2test data\n>\n",fname[1],fname[2]);
	printf(">***********************************************************************\n>\n");
	
	/* �s��(�f�[�^��)�𐔂��� */
	line[0] = 0;
	while(fgets(linebuffer,sizeof(linebuffer),cf1)!=NULL)
		line[0]++;
	
	/* �s��(�f�[�^��)�𐔂��� */
	line[1] = 0;
	while(fgets(linebuffer,sizeof(linebuffer),cf2)!=NULL)
		line[1]++;
	
	/* �����s���o�Ȃ���΃G���[ */
	if(line[0]!=line[1]){
		printf("> Errors: The number of particles are different!!\n");
		printf(">         n[%s] = %d,\tn[%s] = %d\n", fname[3], line[0], fname[4], line[1]);
		printf("> Exit anyway!!\n");
		exit(0);
	}
	
	/* �f�[�^���̕ۑ� */
	Ncmp = line[0];
	
	/* �X�g���[����߂� */
	fseek(cf1, 0, SEEK_SET);
	fseek(cf2, 0, SEEK_SET);
	
	/* �s���������� */
	line[0] = 0;
	
	/* �ޓx���i�[���ꂽFile��ǂݍ��� */
	while(fgets(linebuffer,sizeof(linebuffer),cf1)!=NULL){
		
		/* �Е��̖ޓx���擾 */
		sscanf(linebuffer,"%*s %lf %*d",&P1);
		
		/* ����̖ޓx���擾 */
		fgets(linebuffer,sizeof(linebuffer),cf2);
		sscanf(linebuffer,"%*s %lf %*d",&P2);
		
		/* �ޓx����v�Z */
		LikeRatio[line[0]] = P1/(P1+P2);
		
		/* �s�����Z */
		line[0]++;
	}
	
	/* �q�X�g�̕����� */
	printf("> Input the number of divisions :  ");
	scanf("%d",&div);
	
	/* ��������ݒ� */
	printf("> Input the restriction factor  :  ");
	scanf("%d",&rst);
	
	/* ���z�̕����w�� */
	Lthresh[0] = 0.;
	Lthresh[div] = 1.;
	
	/* �~���ƂȂ�l��ݒ� */
	for(loop=1; loop<div; loop++)
		Lthresh[loop] = Lthresh[loop-1] + 1./((double) div);
	
	/* �e�����v�f�ɂ�������Ғl���v�Z */
	for(loop=1; loop<=div; loop++){
		
		/* �����Ɉ�v����f�[�^���̃N���A */
		freq = 0;
		
		/* �f�[�^�������[�v */
		for(int i=0; i<=Ncmp; i++){
			if(LikeRatio[i] <= Lthresh[loop]
			&& LikeRatio[i] > Lthresh[loop-1] && loop!=1)
				freq++;
			else if(LikeRatio[i] <= Lthresh[loop]
			&& LikeRatio[i] >= Lthresh[loop-1] && loop==1)
				freq++;
		}
		
			
		/* ���Ғl�̌v�Z */
		Ek[loop-1] = ((double) freq)/((double) Ncmp);
	}
	
	/* �s��(�f�[�^��)�𐔂��� */
	line[0] = 0;
	while(fgets(linebuffer,sizeof(linebuffer),rf1)!=NULL)
		line[0]++;
	
	/* �s��(�f�[�^��)�𐔂��� */
	line[1] = 0;
	while(fgets(linebuffer,sizeof(linebuffer),rf2)!=NULL)
		line[1]++;
	
	/* �����s���o�Ȃ���΃G���[ */
	if(line[0] != line[1]){
		printf("> Errors: The number of particles are different!!\n");
		printf(">         n[%s] = %d,\tn[%s] = %d\n", fname[1], line[0], fname[2], line[1]);
		printf("> Exit anyway!!\n");
		exit(0);
	}
	
	/* �f�[�^���̕ۑ� */
	Nrd = line[0];
	
	/* �X�g���[����߂� */
	fseek(rf1, 0, SEEK_SET);
	fseek(rf2, 0, SEEK_SET);
	
	/* �s���������� */
	line[0] = 0;
	
	/* �ޓx���i�[���ꂽFile��ǂݍ��� */
	while(fgets(linebuffer,sizeof(linebuffer),rf1)!=NULL){
		
		/* �Е��̖ޓx���擾 */
		sscanf(linebuffer,"%s %lf %*d",id,&P1);
		
		/* ����̖ޓx���擾 */
		fgets(linebuffer,sizeof(linebuffer),rf2);
		sscanf(linebuffer,"%*s %lf %*d",&P2);
		
		/* �ޓx����v�Z */
		LikeRatio[line[0]] = P1/(P1+P2);
		
		/* 0�Ŋ���f�[�^�͖��� */
		if(P1==0 && P2==0){
			Nrd--;
			line[0]--;
			printf("> L[%s] -> Nan value !!\n",id);
		}
		
		/* �s�����Z */
		line[0]++;
	}
	
	/* ���R�x�̏����� */
	ndf=0;
	
	/* �e�����v�f�ɂ�����x�����v�Z */
	for(loop=1; loop<=div; loop++){
		
		/* �x���̏����� */
		freq = 0;
		
		/* �x�����v�Z */
		for(int j=0;j<Nrd;j++){
			
			/* 0�ɋ߂��l��0.000001�ɌŒ� */
			if(LikeRatio[j] < 0.000001)
				LikeRatio[j] = 0.000001;
			
			/* �͈͓��̃f�[�^�𐔂��� */
			if(LikeRatio[j] <= Lthresh[loop] && LikeRatio[j] > Lthresh[loop-1])
				freq++;
			
		}
		
		/* ���R�x�̌v�Z */
		if(freq>0)ndf++;
		
		/* �x����ۑ� */
		Ok[loop-1] = (double) freq;
		
	}
	
	/* ���ғx�����v�Z */
	for(loop=1; loop<=div; loop++)
		Ek[loop-1] = Ek[loop-1]*((double) Nrd);
	
	/* ���ғx����\�� */
	printf("> -- [Parameters] --\n");
	printf("> Mean frequency:\n");
	for(loop=1; loop<=div; loop++){
		if(Ek[loop-1] == 0)
			Ek[loop-1] = 0.000001;
		
		printf(">       E(%lf -> %lf) = %lf\n",
			Lthresh[loop-1],Lthresh[loop],Ek[loop-1]);

	}
	
	/* ���s */
	printf(">\n");
	
	/* ����f�[�^�̓x����\�� */
	printf("> Observation frequency:\n");
	for(loop=1; loop<=div; loop++)
		printf(">       O(%lf -> %lf) = %lf\n",
			Lthresh[loop-1],Lthresh[loop],Ok[loop-1]);
	printf(">\n");
	printf("> --------------------------------------------------\n");
		
	/* ��2��l�̃N���A */
	SUM_Chi2 = 0;
	
	/* ��2��l�̌v�Z */
	for(loop=1; loop<=div; loop++)
		SUM_Chi2 = SUM_Chi2 + (Ok[loop-1] - Ek[loop-1])*(Ok[loop-1] - Ek[loop-1])/Ek[loop-1];
	
	/* ��2��l�̌v�Z���ʂ�\�� */
//	printf("> Chi^2/ndf = %.3f/%d = %.3f\n",SUM_Chi2,div-rst,SUM_Chi2/((double) (div-rst)));
	printf("> Chi^2/ndf = %.3f/%d = %.3f\n",SUM_Chi2,ndf,SUM_Chi2/((double)ndf));
	
	/* File����� */
	fclose(rf1);
	fclose(rf2);
	fclose(cf1);
	fclose(cf2);
	
	/* main()�߂�l 0 (����) */
	return 0;
	
}
