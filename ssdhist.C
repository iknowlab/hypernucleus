#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <fstream>

#include <TH1.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TRint.h>
#include <TPaveStats.h>
#include <TColor.h>
#include <TAxis.h>
#include <TPave.h>
#include <TFrame.h>
//#include <>
using namespace std;

int main(int argc,char **argv)//main
{
	if(argc<2){
		fprintf(stderr,"usage)\n%s ssdfile(*)\n",argv[0]);
		fprintf(stderr,"option: [-b nbins Low High]\n");
		return 0;
	}

	/* bin fix mode*/
	double bin,max,min;
	bin = 20.0;
	min = -10.;
	max = 10.;
	int param = 0;

	if(argc>5){
		if(strncmp(argv[1],"-b",2)==0){
			bin = atof(argv[2]);
			min = atof(argv[3]);
			max = atof(argv[4]);
			param += 4;
		}//if
		if(strncmp(argv[2],"-b",2)==0){			
			bin = atof(argv[3]);
			min = atof(argv[4]);
			max = atof(argv[5]);
			param += 4;
		}//if
	}//if

	cerr << param << endl;

	/* for-slide mode */
	bool slide = false;
	if(argc >2 && strncmp(argv[1],"-s",2)==0){
		slide = true;
		param++;
	}//if
	if(argc >6 && strncmp(argv[5],"-s",2)==0){
		slide = true;
		param++;		
	}//if

	char *filename[argc-param];
	filename[0]=argv[0];
	for(int index=1;index<argc-param;index++){
		filename[index]=argv[index+param];
	}//for
	argc -= param;

	cerr << argc << endl;

	/* color settings */
	const Int_t NCont = 50;
	Int_t MyPalette[NCont];
	int color_n;
	if(!slide)color_n=5;
	else color_n=4;
	const Int_t NRGBs = color_n;
	Double_t red[NRGBs];
	Double_t green[NRGBs];
	Double_t blue[NRGBs];
	Double_t stop[NRGBs];
	if(!slide){
		red[0]=0.0;		red[1]=0.0;		red[2]=0.87;
		red[3]=1.0;		red[4]=1.0;
		green[0]=0.0;	green[1]=0.81;	green[2]=1.0;
		green[3]=0.20;	green[4]=0.0;
		blue[0]=0.51;	blue[1]=1.00;	blue[2]=0.12;
		blue[3]=0.00;	blue[4]=0.00;
		stop[0]=0.0;	stop[1]=0.25;	stop[2]=0.50;
		stop[3]=0.75;	stop[4]=1.00;
	}else{
		red[0]=1.00;	red[1]=0.50;	red[2]=0.00;	red[3]=0.00;
		green[0]=0.00;	green[1]=0.00;	green[2]=0.60;	green[3]=1.00;
		blue[0]=0.00;	blue[1]=1.00;	blue[2]=1.00;	blue[3]=0.00;
		stop[0]=0.00;	stop[1]=0.33;	stop[2]=0.64; stop[3]=1.00;
	}//(if)else end
//		cerr << red[0] << ":" << red[1] << ":" << red[2] << "..." << endl;
	Int_t FI = TColor::CreateGradientColorTable(NRGBs,stop,red,green,blue,NCont);
	for(int i=0;i<NCont;++i){
		MyPalette[i] = FI+i;
	}//for

	//TRint
	TRint app("app",&argc,filename,0,0,kTRUE);
	
	char buf[32];
	char id[16];
	double range,dx,dy,dz;
	TH1D *h[app.Argc()-1];
	TCanvas *z1 = new TCanvas("z1","",600,450);
	if(slide)z1->SetWindowSize(600,600);

//	fprintf(stderr,"%d\t%s\t%d\t%s\n",app.Argc(),app.Argv(0),argc,argv[0]);

	for(int i=0;i<(app.Argc()-1);i++){
		sprintf(buf,"hist_%s",app.Argv(i+1));
//		fprintf(stderr,"%s and %s\n",buf,fbname);

		ifstream data(app.Argv(i+1));
		if(!data){
			fprintf(stderr,"%s end!",app.Argv(i+1));
			return 0;
		}

		h[i] = new TH1D(buf,"2nd_diff_Distro;#delta_{X'}[#mum];Ratio",bin,min,max);
//		h[i] = new TH1D();
		
		while(data>>dx)h[i]->Fill(dx);
		
		data.close();

		if(i==0){
			h[i]->Draw();
			h[i]->GetXaxis()->SetAxisColor(1);
			h[i]->GetXaxis()->SetTitleColor(kBlue+4);
			h[i]->GetXaxis()->SetLabelColor(kBlue+4);
			h[i]->GetYaxis()->SetAxisColor(1);
			h[i]->GetYaxis()->SetTitleColor(kBlue+4);
			h[i]->GetYaxis()->SetLabelColor(kBlue+4);
		}
		else h[i]->Draw("SAME");

	}//for

	double norm = 1.00;//normalization
	Double_t scale[app.Argc()-1];
	
	for(int j=0;j<(app.Argc()-1);j++){
		scale[j] = norm/h[j]->Integral();
		h[j]->Scale(scale[j]);
		h[j]->SetTitle("2nd difference distribution");
		if(slide)h[j]->SetTitle("#delta_{X'}, Range=1000#mum, #Xi & K");
		h[j]->SetLineColor(MyPalette[(j)*NCont/(app.Argc()-1)]);
		h[j]->SetLineWidth(2);
		h[j]->SetFillStyle(0);
	}//for
	
	if(slide)gStyle->SetOptStat(0);
	if(!slide){
		gStyle->SetTitleTextColor(kBlue+4);
		gStyle->SetLineWidth(2);
		gStyle->SetLineColor(1);
	}

	z1->SetFillColor(10);
//	z1->SetFrameFillStyle(1);
	z1->SetFrameFillColor(kGray);
	z1->SetFrameLineColor(1);

//	gPad->BuildLegend(0.65,0.75,0.9,0.9);
	if(!slide)gPad->Modified();
	z1->Update();//update Graph

	if(!slide){
		TPaveStats *stat = (TPaveStats*)h[0]->FindObject("stats");
		stat->SetTextColor(MyPalette[NCont/app.Argc()]);
	//	stat->SetFillStyle(1);
		stat->SetFillColor(kGray+2);

		for(int k=1;k<(app.Argc()-1);k++){
			double y1=0.775-0.200*(double)k;
			double y2=0.935-0.200*(double)k;
			TPaveStats *st[app.Argc()-1];
			st[k] = (TPaveStats*)h[k]->FindObject("stats");
			st[k]->SetTextColor(MyPalette[(k+1)*NCont/app.Argc()]);
	//		st[k]->SetFillStyle(1);
			st[k]->SetFillColor(kGray+2);
	//		st[k]->SetX1NDC(0.78);
	//		st[k]->SetX2NDC(0.98);
			st[k]->SetY1NDC(y1);
			st[k]->SetY2NDC(y2);
		}//for
	}//if

//	gStyle->SetTextFont(142);
	z1->Modified();
	z1->Update();
//	z1->SaveAs("ssdhist.png");
	app.Run();
	return 0;

}//main
