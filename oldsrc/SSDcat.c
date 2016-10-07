#include<stdio.h>
#include<stdlib.h>

int main(){
	int index;	/* File�ԍ� */
	int max;	/* ���[�v�I���l */
	char firstId[8];	/* ���q�P�̕��� */
	char secondId[8];	/* ���q�Q�̕��� */
	char linebuffer[2048];	/* �s�̓ǂݍ��� */
	FILE *fc, *stp;	/* File�|�C���^ */
	
	/* ���������̓��� */
	printf("> Input an assigned character to combine 2 files on the same position :\n");
	
	/* ���q�P�̕�������� */
	printf("> for particle 1 :  ");
	scanf("%s", firstId);
	
	/* ���q�Q�̕�������� */
	printf("> for particle 2 :  ");
	scanf("%s", secondId);
	
	/* R.tbl���Q�� */
	if((stp = fopen("R.tbl", "r"))==NULL){
		printf("Range table file[R.tbl] not open!!\n");
		exit(1);
	}
	
	/* �s���������� */
	max = 0;
	
	/* �s���𐔂��� */
	while(fgets(linebuffer, sizeof(linebuffer), stp)!=NULL)
		max++;
	
	/* File�̌����J�n */
	for(index=1; index<=max; index++){
		
		/* ���q�P��File�� */
		sprintf(linebuffer, "ssd_%d%s.dat", index, firstId);
		
		/* ���q�P��File���J�� */
		if((fc = fopen(linebuffer, "r"))==NULL){
			printf("Load file[%s] not open!!\n", linebuffer);
			break;
		}
		
		/* �O���R�}���h */
		sprintf(linebuffer, "cat ssd_%d%s.dat > ssd_%dd.dat\n", index, firstId, index);
		
		/* �R�}���h��\�� */
		printf("> %s", linebuffer);
		
		/* ���s�\���q�P��File���R�s�[ */
		system(linebuffer);
		
		/* ���q�Q��File�� */
		sprintf(linebuffer, "ssd_%d%s.dat", index, secondId);
		
		/* ���q�Q��File���J�� */
		if((fc = fopen(linebuffer, "r"))==NULL){
			printf("Load file[%s] not open!!\n", linebuffer);
			break;
		}
		
		/* �O���R�}���h */
		sprintf(linebuffer, "cat ssd_%d%s.dat >> ssd_%dd.dat\n", index, secondId, index);
		
		/* �R�}���h��\�� */
		printf("> %s", linebuffer);
		
		/* ���s�\���q�Q��File��ǋL */
		system(linebuffer);
		
	}
	
	/* main() �߂�l 0 (����) */
	return 0;
	
}

