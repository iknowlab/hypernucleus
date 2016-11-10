/* make inoh 2016.9.29 */

/* include root header */
#include<TRint.h>
#include<TCanvas.h>
#include<TGraph.h>
#include<TMultiGraph.h>
#include<TColor.h>
#include<TAxis.h>
#include<TFrame.h>
#include<TStyle.h>
#include<TLegend.h>
#include<TROOT.h>

/* include c++ header */
#include<sstream>
#include<fstream>
#include<iostream>
#include<stdlib.h>
#include<string>
#include<string.h>
#include<math.h>
#include<vector>

using namespace std;
void usage(){
	cerr << "Usage : exe LogLikelihood_file" << endl;
	exit(1);
}

void readerr(const char *filename){
	cerr << "read file error (" << filename << ")" << endl;
	exit(1);
}

void writeerr(const char *filename){
	 cerr << "write file error (" << filename << ")" << endl;
	exit(1);
}

int main(int argc, char **argv){
	
	if(argc<2){
		usage();
	}//if
	
	TRint app("app", &argc, argv, 0, 0, kTRUE);
	
	TMultiGraph *mg = new TMultiGraph();
	TCanvas *c = new TCanvas("c","",600,450);
	TGraph *g[128];
	
	int i=1;
	int j=0;
	int k;
	int index=0;
	double mass,likelihood;	
	string tag,str,s_name;
	vector<string> s_id;
	
	ifstream ifs;

	ifs.open(app.Argv(i));
	fprintf(stderr, "%s\n", app.Argv(i));
	if(ifs.fail())readerr(app.Argv(i));
	while(ifs && getline(ifs,str)){
		stringstream ss(str);
		ss >> tag >> s_name;
		if(tag.compare("[SampleName]")==0){
			s_id.push_back(s_name);
//			g.reserve(j+10);
			g[j] = new TGraph();

			while(ifs && getline(ifs,str) && !str.empty()){
				stringstream ss(str);
				ss >> mass >> likelihood;
//				fprintf(stderr,"%lf\t%lf\n", mass, likelihood);
				g[j]->TGraph::SetPoint(index, mass, likelihood);
				index++;
			}
			j++;
			index = 0;
		}//if
	}//while
	ifs.close();

	/* color settings */
	const Int_t NRGBs = 5;
	const Int_t NCont = 255;
	Int_t MyPalette[NCont];
	Double_t red[NRGBs] = {0.0,0.0,0.87,1.0,1.0};
	Double_t green[NRGBs] = {0.0,0.81,1.0,0.20,0.0};
	Double_t blue[NRGBs] = {0.51,1.00,0.12,0.00,0.00};
	Double_t stop[NRGBs] = {0.0,0.34,0.61,0.84,1.00};
	Int_t FI = TColor::CreateGradientColorTable(NRGBs,stop,red,green,blue,NCont);
	for(int i=0;i<NCont;++i){ MyPalette[i] = FI+i; }

	/* j is number of Sample */
	for(k=0;k<j;k++){
		cerr<<s_id[k].c_str()<<endl;
		g[k]->SetTitle(s_id[k].c_str());
		g[k]->SetMarkerStyle(20);
		g[k]->SetMarkerSize(0.5);
		g[k]->SetLineWidth(0.8);
		g[k]->SetMarkerColor(MyPalette[k*NCont/j]);
		g[k]->SetLineColor(MyPalette[k*NCont/j]);
		
		mg->Add(g[k]);
	}//for

	/* Draw */
	c->cd();
	mg->Draw("APL");
	mg->SetTitle("Maximum Log-Likelihood Estimation");
	mg->GetYaxis()->SetTitle("Log-Likelihood");
	mg->GetXaxis()->SetTitle("Mass [MeV]");
	gPad->Modified();
	TLegend *tl = c->BuildLegend(0.88, 0.55, 0.99, 0.95, "Sample");

	((TRint*)gROOT->GetApplication())->SetPrompt("Rooooooooot [%d]");
	app.Run();
}
