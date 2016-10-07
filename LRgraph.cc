#include <TH1.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TRint.h>
#include <fstream>
#include <TPaveStats.h>
#include <iostream>
#include <TROOT.h>

using namespace std;

int main(int argc,char **argv)//main
{
	if(argc<2){
		fprintf(stderr,"usage)\n%s likelihooddata(*)\n",argv[0]);
		return 0;
	}

	TRint app("app",&argc,argv,0,0,kTRUE);
	
	int item;
	char ID[16];
//	char CampusName[64];
	char GraphName[app.Argc()-1][64];
	sprintf(GraphName[0], "PID(%s)", app.Argv(1));
	sprintf(GraphName[1], "PID(%s)", app.Argv(2));
	char GraphTitle[256];
	sprintf(GraphTitle, "RealTrack LikelihoodRatio");
	char XAxisLabel[64];
	sprintf(XAxisLabel, "PID");
	char YAxisLabel[64];
//	char LabelName[app.Argc()-1][64];
	double range,dx,dy,dz;
	double likeli[app.Argc()-1][4096];
	double max[app.Argc()-1];
	double LR;
	char linebuffer[1024];
	string str;

	gROOT -> SetStyle("Plain");
	strcpy(YAxisLabel, "Percent [%]");

	TH1F *h1 = new TH1F(GraphName[0], GraphTitle, 20, 0, 1);
	TCanvas *z1 = new TCanvas("z1","Likelihood Ratio Distribution", 0, 0, 700, 700);
//	fprintf(stderr,"%d\t%s\t%d\t%s\n",app.Argc(),app.Argv(0),argc,argv[0]);

	h1 -> SetXTitle(XAxisLabel);
	
	for(int i=0;i<(app.Argc()-1);i++){
//		sprintf(buf,"hist_%s",app.Argv(i+1));
//		fprintf(stderr,"%s and %s\n",buf,fbname);
		ifstream data(app.Argv(i+1));
		if(!data){
			fprintf(stderr,"end!");
			return 0;
		}//if file_check
		item = 0;
		while(getline(data,str)){
			sscanf(str.c_str(),"%s %lf %*d",ID,&likeli[i][item]);
			if(!str.empty())item++;
		}//while file_read
//		h1->Fill(dx);
		max[i]=item;
		data.close();
	}//for argc

	for(int m=0;m<(app.Argc()-1);m++){
		for(int l=0;l<max[m];l++){
			LR = likeli[m][l] / (likeli[m][l]+likeli[m+1][l]);
			cerr << LR << endl;
			if(LR == 1.0)LR = 0.999999;
			h1 -> Fill(LR);
		}//for data num
		m++;
	}//for argc

	double norm = 100.0;//normalization
	Double_t scale;
	scale = norm/h1->Integral();
	h1->Scale(scale);
	h1->SetLineColor(2);//red

	z1->Update();//update Graph

	h1 -> SetLineWidth(3);
	h1 -> SetYTitle(YAxisLabel);
	h1 -> GetYaxis()->SetTitleOffset(1.0);

//	TH1F *H1 = h1->DrawNormalized("",100);
	h1 -> Draw();

/*
	TPaveStats *stat = (TPaveStats*)h[0]->FindObject("stats");
	stat->SetTextColor(2);
	for(int k=1;k<(app.Argc()-1);k++){
		double y1=0.775-0.200*(double)k;
		double y2=0.935-0.200*(double)k;
		TPaveStats *st[app.Argc()-1];
		st[k] = (TPaveStats*)h[k]->FindObject("stats");
		st[k]->SetTextColor(2+k);
		st[k]->SetY1NDC(y1);
		st[k]->SetY2NDC(y2);
	}
	z1->Modified();
*/

	app.Run();
	return 0;
}//main
