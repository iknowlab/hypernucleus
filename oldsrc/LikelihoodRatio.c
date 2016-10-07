#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

int main(int argc,char *fname[])
{
	double L[2];	/* �ޓx */
	double range;	/* Range */
	char ID[2][64];	/* ID */
	char linebuffer[2048];	/* �ǂݍ��ݍs */
	FILE *read1, *read2;	/* File�|�C���^ */
	
	/* Range�̓��� */
	printf("Input the range of this event track (um) :  ");
	scanf("%lf",&range);
	
	/* �Q�̖ޓxFile���J�� */
	if ((read1 = fopen(fname[1],"r")) == NULL) {
		printf("input_file[%s] not open\n",fname[1]);
		exit(1);
	}
	if ((read2 = fopen(fname[2],"r")) == NULL) {
		printf("input_file[%s] not open\n",fname[2]);
		exit(1);
	}
	
	/* �ޓx��̌v�Z */
	while(1){
		
		/* �Е���File����ޓx���擾 */
		if(fgets(linebuffer,sizeof(linebuffer),read1)==NULL)
			break;
		sscanf(linebuffer,"%s %lf %*d", ID[0], &L[0]);
		
		/* ���������File����ޓx���擾 */
		if(fgets(linebuffer,sizeof(linebuffer),read2)==NULL)
			break;
		sscanf(linebuffer,"%s %lf %*d", ID[1], &L[1]);
		
		/* �Q��File��ID����v���Ȃ��ꍇ�̓G���[�Ƃ��� */
		if(strcmp(ID[0], ID[1])!=0){
			printf("Different Type of File Loaded -> [%s] & [%s] !!\n", fname[1], fname[2]);
			exit(1);
		}
		
		/* �ޓx��̏o�� */
		printf("%.0f\t%lf\n", range, L[0]/(L[0]+L[1]));
		
	}
	
	/* File����� */
	fclose(read1);
	fclose(read2);
	
	/* �߂�l 0 (����) */
	return 0;
	
}

