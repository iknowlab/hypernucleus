#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

int main(int argc,char *fname[])
{
	int EX;	/* ���o����Range */
	int Range;	/* Range */
	char linebuffer[2048];	/* �ǂݍ��ݍs */
	FILE *read;	/* File�|�C���^ */
	
	/* ���o����Range����� */
	printf("Extraction Point? :\tR = ");
	scanf("%d",&EX);
	
	/* �ǂݍ���File���J�� */
	if((read = fopen(fname[1],"r"))==NULL){
		printf("Input_FILE(%s) not found!\n",fname[1]);
		exit(1);
	}
	
	/* �s�̓ǂݍ��� */
	while(fgets(linebuffer,sizeof(linebuffer),read) != NULL){
		
		/* ���������̏ꍇ�͓ǂݒ��� */
		if(strncmp(linebuffer, "--------------------------------------------------", 30) == 0)
			continue;
		
		/* Range�̎擾 */
		sscanf(linebuffer,"%*s %d %*lf", &Range);
		
		/* �w�肳�ꂽRange�ƈ�v������A���̍s��\�� */
		if(Range == EX)
			printf("%s", linebuffer);
		
	}
	
	/* File����� */
	fclose(read);
	
	/* �߂�l 0 (����) */
	return 0;
	
}
