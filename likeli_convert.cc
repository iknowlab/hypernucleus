/* make inoh.2017.1.5 */
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <vector>

using namespace std;

struct likelidata{
	char name[64];
	double ll;
	int cnt;
};

void usage(){
	cerr << "convert to Log-Likelihood from Likelihood or reverse." << endl;
	cerr << "exe imputfile(~like~.dat) outputfile(~like~.dat)" << endl;
	exit(1);
}

int main(int argc, char **argv){
	if(argc<2)usage();
	ifstream ifs;
	string str,hoge;
	vector< likelidata > vec_ll;
	likelidata one_ll;
	int conv_flag;
	ofstream ofs;
	char buf[1024];
		
	ifs.open(argv[1]);
	if(!ifs){
		cerr << "read err!" << endl;
		exit(1);
	}//if
	
	while(ifs && getline(ifs,str)){
		stringstream ss(str);
		ss >> one_ll.name >> one_ll.ll >> one_ll.cnt;
		vec_ll.push_back(one_ll);	
	}

	cerr << "convert to Log-Likelihood from Likelihood or reverse." << endl;
	cerr << "likeli -> log-likeli : 1" << endl;
	cerr << "log-likeli -> likeli : 2" << "\t";
	cin >> hoge;
	conv_flag=atoi(hoge.c_str());

	ofs.open(argv[2]);
	if(!ofs){
		cerr << "write err!" << endl;
		exit(1);
	}//if

	for(int i=0;i<vec_ll.size();i++){
		if(conv_flag==1){
			vec_ll[i].ll=log(vec_ll[i].ll);
		}else{
			vec_ll[i].ll=exp(vec_ll[i].ll);
		}
		sprintf(buf,"%s\t%10e\t%d\n",vec_ll[i].name,vec_ll[i].ll,vec_ll[i].cnt);
		ofs << buf;
	}//for
	cerr << "final output line: " << buf;
}//main
