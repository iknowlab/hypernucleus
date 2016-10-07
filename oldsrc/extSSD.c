#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

int main()
{
	int index;	/* File�ʂ��ԍ� */
	double R;	/* Range */
	double x;	/* X���W */
	double y;	/* Y���W */
	double z;	/* Z���W */
	double EX;	/* ���o����ʒu */
	char ident[4];	/* ���q�̊������� */
	char ID[64];	/* ��Ղ�ID */
	char linebuffer[2048];	/* �P�s�ǂݍ��� */
	FILE *read,*out,*step;	/* File�|�C���^ */
	
	/* ���q��File�̋�ʕ�������� */
	printf("Input a character to assign the indexes to files :  ");
	scanf("%s", ident);
	
	/* ���͕����̊m�F */
	if(strlen(ident) > 1){
		printf("Please input a charactor!! Not a string.\n");
		exit(0);
	}
	
	/* �ǂݍ���File�����w�� */
	sprintf(linebuffer, "reference_%s.dlt", ident);
	
	/* �ǂݍ���File���J�� */
	if((read = fopen(linebuffer,"r"))==NULL){
		printf("> Input_FILE(%s) not found!\n", linebuffer);
		exit(1);
	}
	
	/* step�ݒ�File���J�� */
	if((step = fopen("R.tbl","r"))==NULL){
		printf("> Extraction_FILE(R.tbl) not found!\n");
		exit(1);
	}
	
	/* File�ʂ��ԍ��������� */
	index = 0;
	
	/* �ݒ�File�̕��ʈʒu��ǂݍ��� */
	while(fgets(linebuffer,sizeof(linebuffer),step) != NULL){
		sscanf(linebuffer, "%lf", &EX);
		
		/* �ʂ��ԍ������Z */
		index++;
		
		/* �ʂ��ԍ���t���ďo��File�������� */
		sprintf(linebuffer,"ssd_%d%s.dat", index, ident);
		
		/* �o��File���J�� */
		if((out = fopen(linebuffer,"w"))==NULL){
			printf("> Output_FILE(%s) not open!\n",linebuffer);
			exit(1);
		}
		
		/* �r���o�߂̕\�� */
		printf("> ... Extraction for R = %.3f\tFILE :  -[ %s ]-\n",EX,linebuffer);
		
		/* �ǂݍ���File����A2���ψʂ��擾 */
		while(fgets(linebuffer,sizeof(linebuffer),read) != NULL){
			sscanf(linebuffer,"%s %lf %lf %lf %lf %*lf",ID,&R,&x,&y,&z);
			
			/* ���o���ʒu�Ɉ�v����Ƃ� */
			if(R>=EX-0.1 && R<=EX+0.1){
				
				/* ���������łȂ����2���ψʂ��o�͂��� */
				if(strncmp(ID,"--------------------------------------------------",30)!=0)
					fprintf(out,"%s\t%lf\t%lf\t%lf\t%lf\n",ID,R,x,y,z);
			}
		}
		
		/* �X�g���[���������ʒu�ɖ߂� */
		fseek(read, 0, SEEK_SET);
		
		/* �o��File����� */
		fclose(out);
		
	}	/* while [�ݒ�File�̕��ʈʒu��ǂݍ���] end */
	
	/* �ǂݍ���File����� */
	fclose(read);
	
	/* step�ݒ�File����� */
	fclose(step);
	
	/* main() �߂�l0 (����) */
	return 0;
	
}
