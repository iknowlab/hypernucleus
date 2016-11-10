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
			Double_t stop[NRGBs] = {0.0,0.34,0.61,0.84,1.00};
			Int_t FI = TColor::CreateGradientColorTable(NRGBs,stop,red,green,blue,NCont);
			for(int i=0;i<NCont;++i){ MyPalette[i] = FI+i; }

			g[k-1]->SetTitle(Argv(k));
			g[k-1]->SetMarkerStyle(3);
			g[k-1]->SetMarkerSize(0.8);
			g[k-1]->SetMarkerColor(MyPalette[k*NCont/Argc()]);
			g[k-1]->SetLineColor(MyPalette[k*NCont/Argc()]);
		
			mg1.Add(g[k-1]);
	
			/* black lab mode */
			gStyle->SetTitleTextColor(0);
			
		}//for

	}
		
	void Runn(){
		c.cd();

		/* black lab mode */
//		c.SetFrameFillStyle(3019);
		c.SetFillColor(1);
		c.SetFrameFillColor(kGray+1);
		c.SetFrameLineColor(17);

		mg1.Draw("APL");
		mg1.SetTitle("RMS by Range");
		mg1.SetMinimum(0.);
		mg1.GetYaxis()->SetTitle("#delta_{X'} [#mum]");
		mg1.GetXaxis()->SetTitle("Range [#mum]");
		gPad->Modified();
		TLegend *tl = c.BuildLegend(0.55,0.75,0.9,0.9,"2nd diff");

		/* black lab mode */
		mg1.GetXaxis()->SetTitleColor(1);
		mg1.GetXaxis()->SetLabelColor(1);
		mg1.GetXaxis()->SetAxisColor(17);
		mg1.GetYaxis()->SetTitleColor(1);
		mg1.GetYaxis()->SetLabelColor(1);
		mg1.GetYaxis()->SetAxisColor(17);
		tl->SetFillStyle(0);
		tl->SetTextColor(0);
		tl->SetLineColor(17);
		tl->SetLineWidth(0.1);

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
