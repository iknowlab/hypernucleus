#include <stdlib.h>
#include <iostream>

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

#include <TImage.h>
#include <TPad.h>
#include <TROOT.h>

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
//	cout << argc << endl;

	//TRint
	TRint app("app",&argc,filename,0,0,kTRUE);
	
	char buf[32];
	char id[16];
	double range,dx,dy,dz;
	
	TH1D *h[app.Argc()-1];
	TH1 *hm[app.Argc()-1];
	
	TCanvas *z1 = new TCanvas("SuperMatsuo","SuperMatsuo",600,450);
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
		
		while(data>>dx)h[i]->Fill(dx);
		
		data.close();

/* mario mode */
		hm[i] = h[i]->DrawCopy("same");
		hm[i]->SetFillColor(10);
		hm[i]->SetFillStyle(1001);
		if(i!=0)hm[i]->SetFillStyle(0);
		if(i==0)hm[i]->Draw();
		else hm[i]->Draw("same");
		h[i]->SetFillColor(kRed+2);
		h[i]->SetFillStyle(3025);
		if(i!=0)h[i]->SetFillStyle(0);

		if(i==0){
			h[i]->Draw("SAMES");
			h[i]->GetXaxis()->SetAxisColor(1);
			h[i]->GetXaxis()->SetTitleColor(kBlue+4);
			h[i]->GetXaxis()->SetLabelColor(kBlue+4);
			h[i]->GetYaxis()->SetAxisColor(1);
			h[i]->GetYaxis()->SetTitleColor(kBlue+4);
			h[i]->GetYaxis()->SetLabelColor(kBlue+4);
		}
		else {
			h[i]->Draw("SAMES");
			h[i]->SetLineWidth(2);	
		}
	}//for

	double norm = 1.00;//normalization
	Double_t scale[app.Argc()-1];
	Double_t scalem[app.Argc()-1];

	for(int j=0;j<(app.Argc()-1);j++){
		scale[j] = norm/h[j]->Integral();
		scalem[j] = norm/hm[j]->Integral();
		h[j]->Scale(scale[j]);
		hm[j]->Scale(scalem[j]);
		h[j]->SetTitle("2nd difference distribution");
//		h[j]->SetLineColor(MyPalette[(j+1)*NCont/app.Argc()]);
//		h[j]->SetLineWidth(2);
//		h[j]->SetFillStyle(0);
	}//for

/* chi2test(h1:h2) */
	cerr << h[0]->Chi2Test(h[1],"WWPCHI2/NDF",0) << endl;

//	gStyle->SetOptStat();
	gStyle->SetTitleTextColor(kBlue+4);
	gStyle->SetLineWidth(2);
	gStyle->SetLineColor(1);

	z1->SetFillColor(10);
//	z1->SetFrameFillStyle(1);
	z1->SetFrameFillColor(kBlue-9);
//	z1->SetFrameFillColor(kGray);
	z1->SetFrameLineColor(1);

//	gPad->BuildLegend(0.65,0.75,0.9,0.9);
	z1->Update();//update Graph

	/* kokomade okay */

	TPaveStats *stat = (TPaveStats*)h[0]->FindObject("stats");
	hm[0]->SetStats(0);
	stat->SetTextColor(10);
	stat->SetLineColor(kBlue-9);
//	stat->SetTextColor(MyPalette[NCont/app.Argc()]);
	stat->SetFillStyle(0);
//	stat->SetFillColor(kGray+2);
	stat->SetX1NDC(0.655);
	stat->SetX2NDC(0.895);
	stat->SetY1NDC(0.725);
	stat->SetY2NDC(0.885);
	stat->SetTextFont(102);

	for(int k=1;k<(app.Argc()-1);k++){
		double x1=0.655;//-0.200*(double)k;
		double x2=0.895;//-0.200*(double)k;
		double y1=0.725-0.200*(double)k;
		double y2=0.885-0.200*(double)k;
		TPaveStats *st[app.Argc()-1];
		st[k] = (TPaveStats*)h[k]->FindObject("stats");
		st[k]->SetLineColor(kBlue-9);
		st[k]->SetTextColor(10);
//		st[k]->SetTextColor(MyPalette[(k+1)*NCont/app.Argc()]);
		st[k]->SetFillStyle(0);
		st[k]->SetFillColor(kGray+2);
		st[k]->SetX1NDC(x1);
		st[k]->SetX2NDC(x2);
		st[k]->SetY1NDC(y1);
		st[k]->SetY2NDC(y2);
		st[k]->SetTextFont(102);
	}

#ifdef matsuo
	TImage *img = TImage::Open("~/MTO.png");
	TImage *img1 = TImage::Open("~/SBY.png");
	TImage *img2 = TImage::Open("~/OGW.png");

	TPad *p1 = new TPad("mto","mto",0.78,0.25,0.90,0.50);/* x_min,y_min,x_max,y_max */
//	TPad *p2 = new TPad("sby","sby",0.15,0.55,0.35,0.75);
//	TPad *p3 = new TPad("ogw","ogw",0.15,0.35,0.35,0.55);
	p1->SetFillColor(kBlue-9);
//	p2->SetFillColor(kBlue-9);
//	p3->SetFillColor(kBlue-9);

	p1->Draw();
	p1->cd();
	img->Draw();
	z1->cd();
//	p2->Draw();
//	p2->cd();
//	img1->Draw();
//	z1->cd();
//	p3->Draw();
//	p3->cd();
//	img2->Draw();
//	z1->cd();
#endif	

	gPad->Modified();
	z1->Update();
//	z1->SaveAs("ssdhist.png");
	app.Run();
	return 0;
}//main
