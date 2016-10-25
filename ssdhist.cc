#include <stdlib.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TRint.h>
#include <fstream>
#include <TPaveStats.h>
#include <TColor.h>
#include <TAxis.h>
#include <TPave.h>
#include <TFrame.h>
using namespace std;

int main(int argc,char **argv)//main
{
	if(argc<2){
		fprintf(stderr,"usage)\n%s ssdfile(*)\n",argv[0]);
		fprintf(stderr,"option: [-b nbins Low High]\n");
		return 0;
	}

	//create color table
	const Int_t NRGBs = 5;
	const Int_t NCont = 255;
	Int_t MyPalette[NCont];
	Double_t red[NRGBs] = {0.0,0.0,0.87,1.0,1.0};
	Double_t green[NRGBs] = {0.0,0.81,1.0,0.2,0.0};
	Double_t blue[NRGBs] = {0.51,1.0,0.12,0.0,0.0};
	Double_t stop[NRGBs] = {0.0,0.25,0.50,0.75,1.0};
	Int_t FI = TColor::CreateGradientColorTable(NRGBs,stop,red,green,blue,NCont);
	for(int i=0;i<NCont;++i){ MyPalette[i] = FI+i;}

	double bin,max,min;
	bin = 20.0;
	min = -10.;
	max = 10.;
	int param = 0;
	if(strncmp(argv[1],"-b",2)==0){
		bin = atof(argv[2]);
		min = atof(argv[3]);
		max = atof(argv[4]);
		param = 4;
	}
	char *filename[argc-param];
	for(int index=0;index<argc-param;index++){
		filename[index]=argv[index+param];
	}//for
	argc -= param;

	//TRint
	TRint app("app",&argc,filename,0,0,kTRUE);
	
	char buf[32];
	char id[16];
	double range,dx,dy,dz;
	TH1D *h[app.Argc()-1];
	TCanvas *z1 = new TCanvas("z1","",600,450);

//	fprintf(stderr,"%d\t%s\t%d\t%s\n",app.Argc(),app.Argv(0),argc,argv[0]);

	for(int i=0;i<(app.Argc()-1);i++){
		sprintf(buf,"hist_%s",app.Argv(i+1));
//		fprintf(stderr,"%s and %s\n",buf,fbname);

		ifstream data(app.Argv(i+1));
		if(!data){
			fprintf(stderr,"%s end!",app.Argv(i+1));
			return 0;
		}

		h[i] = new TH1D(buf,"2nd_diff_Distro;SD^{X'}",bin,min,max);
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
		else h[i]->Draw("SAMES");

	}//for

	double norm = 1.00;//normalization
	Double_t scale[app.Argc()-1];
	
	for(int j=0;j<(app.Argc()-1);j++){
		scale[j] = norm/h[j]->Integral();
		h[j]->Scale(scale[j]);
		h[j]->SetTitle("2nd difference distribution");
		h[j]->SetLineColor(MyPalette[(j+1)*NCont/app.Argc()]);
		h[j]->SetLineWidth(2);
		h[j]->SetFillStyle(0);
	}//for

	gStyle->SetTitleTextColor(kBlue+4);
	gStyle->SetLineWidth(2);
	gStyle->SetLineColor(1);

	z1->SetFillColor(0);
	z1->SetFrameFillStyle(1);
	z1->SetFrameFillColor(kGray);
	z1->SetFrameLineColor(1);

//	gPad->BuildLegend(0.65,0.75,0.9,0.9);
	z1->Update();//update Graph

	TPaveStats *stat = (TPaveStats*)h[0]->FindObject("stats");
	stat->SetTextColor(MyPalette[NCont/app.Argc()]);
	stat->SetFillStyle(1);
	stat->SetFillColor(kGray+2);

	for(int k=1;k<(app.Argc()-1);k++){
		double y1=0.775-0.200*(double)k;
		double y2=0.935-0.200*(double)k;
		TPaveStats *st[app.Argc()-1];
		st[k] = (TPaveStats*)h[k]->FindObject("stats");
		st[k]->SetTextColor(MyPalette[(k+1)*NCont/app.Argc()]);
		st[k]->SetFillStyle(1);
		st[k]->SetFillColor(kGray+2);
//		st[k]->SetX1NDC(0.78);
//		st[k]->SetX2NDC(0.98);
		st[k]->SetY1NDC(y1);
		st[k]->SetY2NDC(y2);
	}

	z1->Modified();
	app.Run();
	return 0;
}//main
