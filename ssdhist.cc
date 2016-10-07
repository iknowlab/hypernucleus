#include <TH1.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TRint.h>
#include <fstream>
#include <TPaveStats.h>

using namespace std;

int main(int argc,char **argv)//main
{
	if(argc<2){
		fprintf(stderr,"usage)\n%s ssdfile(*)\n",argv[0]);
		return 0;
	}

	TRint app("app",&argc,argv,0,0,kTRUE);
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
			fprintf(stderr,"end!");
			return 0;
		}

		h[i] = new TH1D(buf,"2nd_diff_Distro;SD^{X'}",20,-10,10);
//		h[i] = new TH1D();
		
		while(data>>dx)h[i]->Fill(dx);
		
		data.close();

		if(i==0){
			h[i]->Draw();
		}
		else h[i]->Draw("SAMES");

	}//for

	double norm = 1.00;//normalization
	Double_t scale[app.Argc()-1];
	
	for(int j=0;j<(app.Argc()-1);j++){
		scale[j] = norm/h[j]->Integral();
		h[j]->Scale(scale[j]);
		h[j]->SetTitle("2nd difference distribution");
		h[j]->SetLineColor(2+j);
	}//for

//	gPad->BuildLegend(0.65,0.75,0.9,0.9);
	z1->Update();//update Graph

	TPaveStats *stat = (TPaveStats*)h[0]->FindObject("stats");
	stat->SetTextColor(2);

	for(int k=1;k<(app.Argc()-1);k++){
		double y1=0.775-0.200*(double)k;
		double y2=0.935-0.200*(double)k;
		TPaveStats *st[app.Argc()-1];
		st[k] = (TPaveStats*)h[k]->FindObject("stats");
		st[k]->SetTextColor(2+k);
//		st[k]->SetX1NDC(0.78);
//		st[k]->SetX2NDC(0.98);
		st[k]->SetY1NDC(y1);
		st[k]->SetY2NDC(y2);
	}

	z1->Modified();
	app.Run();
	return 0;
}//main
