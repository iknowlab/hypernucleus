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
	MyApp(int argc, char* argv[]) :
	TRint("MyApp", &argc, argv,0,0,kTRUE),
	mg1(),
	c()
	{
		if(Argc() == 1){
			cout << "Call \"Graph Data File\". Error!" << endl;
			exit(1);
		}//if

		Read();
	}

	virtual ~MyApp(){
	}

	void Read(){
		TGraph *g[Argc()-1];
		int k;

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

			const Int_t NRGBs = 5;
			const Int_t NCont = 255;
			Int_t MyPalette[NCont];
			Double_t red[NRGBs] = {0.0,0.0,0.87,1.0,1.0};
			Double_t green[NRGBs] = {0.0,0.81,1.0,0.20,0.0};
			Double_t blue[NRGBs] = {0.51,1.00,0.12,0.00,0.00};
			Double_t stop[NRGBs] = {0.0,0.25,0.50,0.75,1.00};
			Int_t FI = TColor::CreateGradientColorTable(NRGBs,stop,red,green,blue,NCont);
			for(int i=0;i<NCont;++i){ MyPalette[i] = FI+i; }

			g[k-1]->SetTitle(Argv(k));
			g[k-1]->SetMarkerStyle(21);
			g[k-1]->SetMarkerSize(0.6);
			g[k-1]->SetLineWidth(1);
			g[k-1]->SetMarkerColor(MyPalette[k*NCont/Argc()]);
			g[k-1]->SetLineColor(1);//MyPalette[k*NCont/Argc()]);
			
			mg1.Add(g[k-1]);

		}//for

	gStyle->SetTitleTextColor(kBlue+4);
	gStyle->SetLineWidth(2);

	}
		
	void Runn(){
		c.cd();
		
		c.SetFillColor(0);
		c.SetFrameFillStyle(1);
		c.SetFrameFillColor(kGray);
		c.SetFrameLineColor(1);

		mg1.Draw("APL");
		mg1.SetTitle("RMS by Range");
		mg1.SetMinimum(0.);
		mg1.GetYaxis()->SetTitle("#delta_{X'} [#mum]");
		mg1.GetXaxis()->SetTitle("Range [#mum]");
		gPad->Modified();
		TLegend *tl = c.BuildLegend(0.55,0.75,0.9,0.9,"2nd diff");

		mg1.GetXaxis()->SetTitleColor(kBlue+4);
		mg1.GetYaxis()->SetLabelColor(kBlue+4);
		mg1.GetXaxis()->SetAxisColor(1);
		mg1.GetYaxis()->SetTitleColor(kBlue+4);
		mg1.GetXaxis()->SetLabelColor(kBlue+4);
		mg1.GetYaxis()->SetAxisColor(1);

		tl->SetFillStyle(0);
		tl->SetTextColor(kBlue+4);
		tl->SetLineColor(1);
		tl->SetLineWidth(2);
	
//		c.WaitPrimitive();
	}

private:
	TMultiGraph mg1;
	TCanvas c;
};

int main(int argc, char* argv[]){
	MyApp app(argc,argv);
	app.Runn();
	app.Run();
	return 0;
}
