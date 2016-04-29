// this file is distributed under 
// MIT license
#include <iostream>
#include <fstream>
#include <math_h/error.h>
#include <math_h/chains.h>
#include <JPetData/Frames.h>
#include <JPetData/Detectors.h>
#include <JPetData/HVconfig.h>
#include <Postgres/postgres_data.h>
using namespace std;
using namespace MathTemplates;
using namespace DataAccess;
using namespace JPetSetup;
shared_ptr<Photomultipliers> phm_table;
shared_ptr<HVPMConnections> connections;
const string&PhmName(const HVconfigEntry&a){
	return phm_table->ByID(connections->ByID(a.HVPMConnection_id())[0].photomultiplier_id()).name();
}
bool operator>(const HVconfigEntry&a,const HVconfigEntry&b){
	return PhmName(a)>PhmName(b);
}
bool operator<(const HVconfigEntry&a,const HVconfigEntry&b){
	return PhmName(a)<PhmName(b);
}
int main(int argc, char** argv) {
	DBConfigData cfg;
	cfg.hostname="127.0.0.1";
	cfg.port="5432";
	cfg.db_name="postgres";
	cfg.username="postgres";
	cfg.password="pass";
	auto src=make_shared<PQData>(cfg);
	
	phm_table=make_shared<Photomultipliers>(src);
	connections=make_shared<HVPMConnections>(src);
	HVconfigTable hv_configs(src);
	
	auto previous=hv_configs.Last();
	cout<<"Last config"<<endl;
	cout<<"id="<<previous.id()<<"; descr="<<previous.description()<<endl;
	SortedChain<HVconfigEntry> previousitems;
	for(const HVconfigEntry&item:previous.CreateEntriesFactory().GetList())
		previousitems<<item;
	cout<<previousitems.size()<<" items"<<endl;
	
	for(const HVconfigEntry&item:previousitems)
		cout<< PhmName(item) << " - "<< item.HV()<<endl;
	
	vector<double> read_hv_values;
	ifstream file;
	file.open(string(argv[2]));
	if(file){
		cout<<"reading file"<<endl;
		double hv=0;
		while(file>>hv)read_hv_values.push_back(hv);
		file.close();
		cout<<"done"<<endl;
	}else cout <<"file not found"<<endl;
	
	if(read_hv_values.size()!=previousitems.size())
		throw Exception<vector<double>>("file size is not equal to config size");
	bool compare=true;
	for(size_t i=0;compare&&(i<previousitems.size());i++)
		compare&=(previousitems[i].HV()==read_hv_values[i]);
	
	if(compare)cout<<"The configuration has not changed"<<endl;
	else{
		hv_configs.Add(HVconfig(previous.id()+1,"newly created config"));
		auto next=hv_configs.Last();
		if(next.id()<=previous.id())
			throw Exception<HVconfigTable>("the id of new config is less than for previous or it's not created");
		auto factory=next.CreateEntriesFactory();
		for(size_t i=0;i<previousitems.size();i++)
			factory.Add(HVconfigEntry(
				previousitems[i].HVPMConnection_id(),
				read_hv_values[i]
			));
		
		cout<<"New config"<<endl;
		cout<<"id="<<next.id()<<"; descr="<<next.description()<<endl;
		for(const HVconfigEntry&item:factory.GetList())
			cout<< PhmName(item) << " - "<< item.HV()<<endl;
	}
}