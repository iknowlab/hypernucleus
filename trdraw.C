/* make inoh 2016 */
#include <TCanvas.h>
#include <TRint.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <TColor.h>
#include <TAxis.h>
#include <TFrame.h>
#include <TPave.h>
#include <TStyle.h>
#include <TLegend.h>

using namespace std;

class MyApp : public TRint {
public:
	MyApp(int argc, char* filename[], int slide)
	:	TRint("MyApp", &argc, filename,0,0,kTRUE)
	,	mg1()
	,	c()
	{
		if(Argc() == 1){
			cout << "Call \"Graph Data File\". Error!" << endl;
			exit(1);
		}//if

		Read(slide);
	}

	virtual ~MyApp(){
	}

	void Read(int slide){
		TGraph *g[Argc()-1];
		int k;

	/* color settings */
		const Int_t NCont = 50;
		Int_t MyPalette[NCont];
		int bin;
		if(slide==0)bin=5;
		else bin=4;
		const Int_t NRGBs = bin;
		Double_t red[NRGBs];
		Double_t green[NRGBs];
		Double_t blue[NRGBs];
		Double_t stop[NRGBs];
		if(slide==0){
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
	/* color settings end */

		for(k=1;k<Argc();k++){
			cerr << "read " << Argv(k) << "..." << endl;
			ifstream data(Argv(k));
			if(!data){
				cout << "read error!" << Argv(k) << endl;
				exit(1);
			}//if

			string str;
			int j=0;
			double range,dx;
			g[k-1] = new TGraph();
			while(data && getline(data,str)){

				stringstream ss(str);
				ss >> range >> dx;
//				cout << range << " " << dx << endl;
				g[k-1] -> TGraph::SetPoint(j,range,dx);
				j++;

			}//while

			data.close();

			g[k-1]->SetTitle(Argv(k));
			g[k-1]->SetMarkerStyle(21);
			g[k-1]->SetMarkerSize(0.6);
			g[k-1]->SetLineWidth(1);
			g[k-1]->SetMarkerColor(MyPalette[(k-1)*NCont/(Argc()-2)]);
			if(k==Argc()-1)g[k-1]->SetMarkerColor(MyPalette[NCont-2]);
			g[k-1]->SetLineColor(1);//MyPalette[(k-1)*NCont/(Argc()-1)]);
			
			mg1.Add(g[k-1]);

		}//for

	gStyle->SetTitleTextColor(kBlue+4);
	gStyle->SetLineWidth(2);

	}
		
	void Runn(int slide){
		c.cd();

		if(slide!=0)SetWS(600,600);	
		c.SetFillColor(0);
//		c.SetFrameFillStyle(1);
		c.SetFrameFillColor(kGray);
		c.SetFrameLineColor(1);
		c.SetGrid();

		mg1.Draw("APL");
		mg1.SetTitle("RMS by Range");
		if(slide!=0)mg1.SetTitle("Caluculation of #delta_{0}, Range#leq6mm, Step 100 #mum");
		mg1.SetMinimum(0.);
		mg1.GetYaxis()->SetTitle("#delta_{X'} [#mum]");
		mg1.GetXaxis()->SetTitle("Range [#mum]");
		gPad->Modified();

		mg1.GetXaxis()->SetTitleColor(kBlue+4);
		mg1.GetYaxis()->SetLabelColor(kBlue+4);
		mg1.GetXaxis()->SetAxisColor(1);
		mg1.GetYaxis()->SetTitleColor(kBlue+4);
		mg1.GetXaxis()->SetLabelColor(kBlue+4);
		mg1.GetYaxis()->SetAxisColor(1);

		if(slide==0){
			TLegend *tl = c.BuildLegend(0.55,0.75,0.9,0.9,"2nd diff");
			tl->SetFillStyle(0);
			tl->SetTextColor(kBlue+4);
			tl->SetLineColor(1);
			tl->SetLineWidth(2);
		}
//		c.WaitPrimitive();
	}

void SetWP(Double_t wx,Double_t wy){
	c.SetWindowPosition(wx,wy);
}

void SetWS(Double_t x,Double_t y){
	c.SetWindowSize(x,y);
}

private:
	TMultiGraph mg1;
	TCanvas c;
};

int main(int argc, char* argv[]){
	if(argc<2){
		cerr << "TrDraw [option] datfile(s)" << endl;
		cerr << "datfile is writen by 2law (Range : dx)" << endl;
		cerr << "Option: [-s] for showing on a slide" << endl;
		exit(1);
	}//if
	
	/* use for slide */
	int slide = 0;
	int param = 0;
	if(strncmp(argv[1],"-s",2)==0){
		slide++;
		param++;
	}
	char *filename[argc-param];
	filename[0]=argv[0];//imput program name
	for(int index=1;index<argc-param;index++){
		filename[index]=argv[index+param];
	}//for
	argc -= param;
	
	MyApp app(argc,filename,slide);
	app.Runn(slide);
	app.Run();
	return 0;
}
