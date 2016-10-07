#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(){
	int index;	/* �ʂ��ԍ� */
	int select;	/* ���[�U�I�� */
	double RMS;	/* RMS */
	char linebuffer[2048];	/* �ǂݍ��ݍs */
	FILE *read, *out, *stp;	/* File�|�C���^ */
	
ReSelect:	/* ���x���đI�� */
	
	/* �v�Z�Ώۂɐ�����I�� */
	printf("Select a coodinates :  \n");
	printf("\t1) X'-palne\n");
	printf("\t2) Y'-palne\n");
	printf("\t3) Z'-plane\n");
	printf("[Select number] :  ");
	scanf("%d", &select);
	
	/* ���͐��l�̃`�F�b�N */
	if(select > 3 || select <1){
		printf("Please input by number 1, 2 or 3!!\n");
		goto ReSelect;
	}
	
	/* R.tbl���Q�� */
	if((stp = fopen("R.tbl", "r"))==NULL){
		printf("Range table file[R.tbl] not open!!\n");
		exit(1);
	}
	
	/* ������ */
	index = 0;
	
	/* �s���𐔂��� */
	while(fgets(linebuffer, sizeof(linebuffer), stp)!=NULL)
		index++;
	
	/* �ǂݍ���File���J�� */
	if((read = fopen("RMS.txt","r"))==NULL){
		printf("Input File[RMS.txt] not open!!\n");
		exit(1);
	}
	
	/* X'�����I���̏ꍇ */
	if(select == 1)
		if((out = fopen("ffbox.tbl.x","w"))==NULL){
			printf("Output File[ffbox.tbl.x] not open!!\n");
			exit(1);
		}
		
	/* Y'�����I���̏ꍇ */
	else if(select == 2)
		if((out = fopen("ffbox.tbl.y","w"))==NULL){
			printf("Output File[ffbox.tbl.y] not open!!\n");
			exit(1);
		}
		
	/* Z'�����I���̏ꍇ */
	else if(select == 3)
		if((out = fopen("ffbox.tbl.z","w"))==NULL){
			printf("Output File[ffbox.tbl.z] not open!!\n");
			exit(1);
		}
		
	/* ���̑��I���̏ꍇ�G���[ */
	else{
		printf("Error to open an output file!!\n");
		exit(1);
	}
	
	/* RMS���擾���� */
	while(fgets(linebuffer,sizeof(linebuffer),read)!=NULL){
		sscanf(linebuffer,"%*lf %*d %lf", &RMS);
		
		/* 5�Ŋ��� */
		RMS /= 5.;
		
		/* �������l���o�� */
		fprintf(out,"%d\t%lf\n", index, RMS);
		
		/* �ԍ��̍X�V */
		index--;
	}
	
	/* File����� */
	fclose(read);
	fclose(out);
	
	/* main() �߂�l 0 (����) */
	return 0;
	
}
