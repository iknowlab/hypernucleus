#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

int main(int argc,char *fname[])
{
	int count;	/* ������Ր� */
	int numer;	/* ���q */
	int denomin;	/* ���� */
	int num;	/* �Q��File�ԍ� */
	int plane;	/* �v�Z�Ώې��� */
	int index;	/* ���e�͈͌����ԍ� */
	int pljoint;	/* plate�ڑ����� */
	double Lx;	/* �ޓx */
	double range;	/* ��� */
	double dx,dy,dz;	/* 2���ψ� */
	double dx_c,dy_c,dz_c;	/* �Q�Ƒ���2���ψ� */
	double length;	/* ���e�͈� */
	char linebuffer[2048];	/* �ǂݍ��ݍs */
	char id[64];	/* ��Ղ�ID */
	char chr[16];	/* ���q�̋�ʕ��� */
	FILE *read, *out, *cmp, *med;	/* File�|�C���^ */
	
	/* �ǂݍ���File���J�� */
	if ((read = fopen(fname[1], "r")) == NULL) {
		printf("input_file[%s] not open\n",fname[1]);
		exit(1);
	}
	
	/* �o��File���J��*/
	if ((out = fopen(fname[2], "w")) == NULL) {
		printf("output_file[%s] not open\n",fname[2]);
		exit(1);
	}
	
	/* ��ʕ����̓��� */
	printf("> File assigned character :  ");
	scanf("%s",chr);
	
	/* �����Ώې����̑I�� */
	printf("> Calculation Plane :\n");
	printf("> \t1) X'_plane\n");
	printf("> \t2) Y'_plane\n");
	printf("> \t3) Z'_plane\n");
	printf("> Answer by Index(1, 2, 3) :  ");
	scanf("%d",&plane);
	
	/* ���e�͈͐ݒ�File��I�� */
	if(plane == 1)
		sprintf(linebuffer,"ffbox.tbl.x");
	else if(plane == 2)
		sprintf(linebuffer,"ffbox.tbl.y");
	else if(plane == 3)
		sprintf(linebuffer,"ffbox.tbl.z");
	else{
		printf("> You should input 1, 2, or 3!!\n> Exit soon...\n");
		exit(1);
	}
	
	/* ���e�͈͐ݒ�File��ǂݍ��� */
	if ((med = fopen(linebuffer, "r")) == NULL) {
		printf("> [Error] :- %s - not open\n",linebuffer);
		exit(1);
	}
	
	/* ������Ր��������� */
	count = 0;
	
	/* ��Ր����̏������J�n */
	while(1){
		
		/* �Q��File�ԍ��������� */
		num = 0;
		
		/* �ޓx�̏����� */
		Lx = 1;
		
		/* �P��Օ��̖ޓx�����߂� */
		while(1){
			
			/* �ŏI�s�ɂȂ����珈�����I�� */
			if(fgets(linebuffer,sizeof(linebuffer),read)==NULL)
				goto Frame;
			
			/* ���������ł̓��[�v���I������ */
			if(strncmp(linebuffer,"--------------------------------------------------",49)==0)
				break;
			
			/* �P�s����2���ψʂ��擾 */
			sscanf(linebuffer,"%s %lf %lf %lf %lf %d",id,&range,&dx,&dy,&dz,&pljoint);
			
			/* plate�̐ڑ������͖������� */
			if(pljoint == 111){
				num++;
				continue;
			}

			/* ���0�f�[�^�͖������� */
			if((int)range==0||(int)range==100){
				continue;
			}
			
			/* �Ώې����̒l��X'�����Ɉڂ� */
			if(plane == 2)
				dx = dy;
			else if(plane == 3)
				dx = dz;
			
			/* �Q�ƗpFile�������� */
			sprintf(linebuffer,"ssd_%d%s.dat", num+1, chr);
			
			/* �ޓx�v�Z�̂��߂̎Q��File���J�� */
			if ((cmp = fopen(linebuffer, "r")) == NULL) {
				printf("> [Error] : - %s - not open\n",linebuffer);
				exit(1);
			}
			
			/* ���e�͈͂�ǂݍ��� */
			while(fgets(linebuffer,sizeof(linebuffer),med)!=NULL){
				sscanf(linebuffer,"%d %lf",&index,&length);
				
				/* File�ԍ��ɑΉ����鋖�e�͈͂��擾 */
				if((num+1) == index)
					break;
			}
			
			/* ���e�͈͐ݒ�File�̃X�g���[���������� */
			fseek(med, 0, SEEK_SET);
			
			/* �m���v�Z�Ɏg���J�E���^�������� */
			numer = 1;
			denomin = 1;
			
			/* �Q��File��2���ψʂ�ǂݍ��� */
			while(fgets(linebuffer,sizeof(linebuffer),cmp) != NULL){
				
				/* �ꐔ�����Z */
				denomin++;
				
				/* 2���ψʂ��擾 */
				sscanf(linebuffer,"%*s %*lf %lf %lf %lf",&dx_c,&dy_c,&dz_c);
				
				/* �Ώې����̒l��X'�����Ɉڂ� */
				if(plane == 2)
					dx_c = dy_c;
				else if(plane == 3)
					dx_c = dz_c;
				
				/* ���e�͈͓��Ɋ܂܂��2���ψʂ𐔂��� */
				if(dx >= dx_c-length && dx <= dx_c+length)
					numer++;
				
			}
			
			/* �Q��File����� */
			fclose(cmp);
			
			/* �ޓx���v�Z */
			/* �R�����g�F�l���������Ȃ肷���邱�Ƃ������邽�� */
			/*           10�{���Ă��� */
			Lx = Lx*((double)numer/denomin)*10;
			
			/* ����File�ԍ��� */
			num++;
			
		}	/* while [�P��Օ��̖ޓx�����߂�] end */
		
		/* �����󋵂�\�� */
		printf("%s\t%.10e\t%d\n",id,Lx,num);
		
		/* �Z�o���ꂽ�ޓx���o�� */
		fprintf(out,"%s\t%.10e\t%d\n",id,Lx,num);
		
		count++;
		
	}	/* while [��Ր����̏������J�n] end */
	
Frame:	/* ���x�� */
	
	/* File����� */
	fclose(read);
	fclose(out);
	fclose(med);
	
	/* main() �߂�l 0 (����) */
	return 0;
	
}
