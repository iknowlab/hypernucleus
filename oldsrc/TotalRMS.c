#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

/* �֐���`main()�֐��ȍ~�ɋL�q */
void CalcRMS(int, int, char*);

int main(){
	int loop;	/* ���[�v�J�E���^ */
	int select;	/* ���[�U�I�� */
	int line;	/* �s�� */
	char ID[64];	/* File ID */
	char linebuffer[2048];	/* �ǂݍ��ݍs */
	FILE *stp;	/* File�|�C���^ */
	
ReStart:	/* ���x���Ď��s */
	
	/* File ID�̓��� */
	printf("Input the file assigned ID :  ");
	scanf("%s",ID);
	
	/* ���͒l�̃`�F�b�N */
	if(strlen(ID) > 1){
		printf("Please input a charactor!! Not a string.\n");
		goto ReStart;
	}
	
	/* R.tbl���Q�� */
	if((stp = fopen("R.tbl", "r"))==NULL){
		printf("Range table file[R.tbl] not open!!\n");
		exit(1);
	}
	
	/* �s���������� */
	line = 0;
	
	/* �s���𐔂��� */
	while(fgets(linebuffer, sizeof(linebuffer), stp)!=NULL)
		line++;
	

ReSelect:	/* ���x���đI�� */
	
	/* �v�Z�Ώۂɐ�����I�� */
	printf("Select a coodinates :  \n");
	printf("\t1) X'-palne\t2) Y'-palne\n");
	printf("\t3) X' & Y'-plane both\n");
	printf("[Select number] :  ");
	scanf("%d", &select);
	
	/* ���͐��l�̃`�F�b�N */
	if(select > 3 || select <1){
		printf("Please input by number 1, 2 or 3!!\n");
		goto ReSelect;
	}
	
	/* RMS�̌v�Z�������J�n */
	for(loop=line; loop>0; loop--)
		CalcRMS(select-1, loop, ID);
	
	/* main() �߂�l0 (����) */
	return 0;
	
}

void CalcRMS(int mode, int index, char *id){
	long int Max;	/* 2���ψʂ̍ő吔 */
	double deltaX;	/* 2���ψ�X'-plane */
	double deltaY;	/* 2���ψ�Y'-plane */
	double R;	/* Range */
	double SUM;	/* 2���ψʂ̂Q��l�̍��v */
	double RMS;	/* RMS */
	char linebuffer[2048];	/* �ǂݍ��ݍs */
	FILE *read, *out;	/* File�|�C���^ */
	
	/* �ǂݍ���File�����w�� */
	sprintf(linebuffer,"ssd_%d%s.dat", index, id);
	
	/* �ǂݍ���File���J�� */
	if ((read = fopen(linebuffer,"r")) == NULL) {
		printf("FILE[%s] not open!!\n",linebuffer);
		exit(1);
	}
	
	/* �o��File���A�y���h���[�h�ŊJ�� */
	if ((out = fopen("RMS.txt","a")) == NULL) {
		printf("output_file[RMS.txt] not open\n");
		exit(1);
	}
	
	/* �����o�߂�\�� */
	printf(" > Execute \"%s\" ...\t",linebuffer);
	
	/* �����N���A */
	SUM = 0;
	Max = 0;
	
	/* 2���ψʂ̓ǂݍ��� */
	while(fgets(linebuffer,sizeof(linebuffer),read) != NULL){
		sscanf(linebuffer," %*s %lf %lf %lf %*lf", &R, &deltaX, &deltaY);
		
		/* X'�����I���̏ꍇ */
		if(mode == 0){
			SUM = SUM + deltaX*deltaX;
			Max++;
		}
		/* Y'�����I���̏ꍇ */
		else if(mode == 1){
			SUM = SUM + deltaY*deltaY;
			Max++;
		}
		/* X' Y'�������I���̏ꍇ */
		else{
			SUM = SUM + deltaY*deltaY + deltaX*deltaX;
			Max += 2;
		}
		
	}
	
	/* RMS�����߂� */
	RMS = sqrt(SUM/((double) Max));
	
	/* �v�Z���ꂽ�l��\�� */
	printf("N : %d\tRMS = %lf\n", Max, RMS);
	
	/* �o��File�ւ̏������� */
	fprintf(out,"%.0f\t%d\t%lf\n", R, Max, RMS);
	
	/* File����� */
	fclose(read);
	fclose(out);
	
}

