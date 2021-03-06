// this file is distributed under 
// MIT license
#include <iostream>
#include <string>
#include <fstream>
#include <libhv_global.h>
#include <HV/CAEN.h>
using namespace std;
using namespace Hardware;
int main(int argc,char**argv){
	string constr;
	{
		string filepath(argv[0]);
		filepath+=".txt";
		ifstream cfg;
		cfg.open(filepath);
		if(cfg){
			getline(cfg,constr);
			cfg.close();
		}
	}
	cout<<constr<<endl;
	CAEN hv(constr);
	try{
		for(size_t idx=800;idx<832;idx++)
			cout<<idx<<"\t"<<hv[hv.idx2index(idx)]->getChannelName()<<"\t"<<hv.IsOn(idx)<<"\t"<<hv.GetHV(idx)<<endl;
		for(size_t idx=800;idx<832;idx++)
			hv.turnOn(idx);
		for(size_t idx=800;idx<832;idx++)
			cout<<idx<<"\t"<<hv[hv.idx2index(idx)]->getChannelName()<<"\t"<<hv.IsOn(idx)<<"\t"<<hv.GetHV(idx)<<endl;
	}catch(libhvException* ex){
		cout<<"HVERROR: "<<ex->getMessage()<<endl;
	}
}