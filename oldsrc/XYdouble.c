#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

int main(int argc,char *fname[])
{
	int num[2];	/* �ώZ�� */
	double L[2];	/* �ޓx */
	char ID[2][64];	/* ID */
	char linebuffer[2048];	/* �ǂݍ��ݍs */
	FILE *read1, *read2;	/* File�|�C���^ */
	
	/* �Q�̖ޓxFile���J�� */
	if ((read1 = fopen(fname[1],"r")) == NULL) {
		printf("input_file[%s] not open\n",fname[1]);
		exit(1);
	}
	if ((read2 = fopen(fname[2],"r")) == NULL) {
		printf("input_file[%s] not open\n",fname[2]);
		exit(1);
	}
	
	/* �f�[�^�̓ǂݍ��݂Ɩޓx�̊|���Z */
	while(1){
		
		/* �Е���File����ޓx���擾 */
		if(fgets(linebuffer,sizeof(linebuffer),read1) == NULL)
			break;
		sscanf(linebuffer,"%s %lf %d", ID[0], &L[0], &num[0]);
		
		/* ���������File����ޓx���擾 */
		if(fgets(linebuffer,sizeof(linebuffer),read2) == NULL)
			break;
		sscanf(linebuffer,"%s %lf %d", ID[1], &L[1], &num[1]);
		
		/* �Q��File��ID����v���Ȃ��ꍇ�̓G���[�Ƃ��� */
		if(strcmp(ID[0], ID[1])!=0){
			printf("Different tipe File [%s] & [%s] !!\n", fname[1], fname[2]);
			exit(1);
		}
		
		/* �Q�̖ޓx���|�����킹�o�� */
		printf("%s\t%lf\t%d\n", ID[0], L[0]*L[1], num[0]+num[1]);
		
	}
	
	/* File����� */
	fclose(read1);
	fclose(read2);
	
	/* �߂�l 0 (����) */
	return 0;
	
}

