// this file is distributed under 
// MIT license
#include <iostream>
#include <fstream>
#include <JPetData/Frames.h>
#include <JPetData/Detectors.h>
#include <JPetData/HVconfig.h>
#include <Postgres/postgres_data.h>
using namespace std;
using namespace DataAccess;
using namespace JPetSetup;
int main(int argc, char** argv) {
	if(argc<2){
		cout << "wrong parameters"<<endl;
		return -1;
	}
	DBConfigData cfg;
	cfg.hostname="127.0.0.1";
	cfg.port="5432";
	cfg.db_name="postgres";
	cfg.username="postgres";
	cfg.password="pass";
	auto src=make_shared<PQData>(cfg);
	Photomultipliers phm_table(src);
	HVPMConnections connections(src);
	HVconfigTable hv_configs(src);
	cout<<"searching last config"<<endl;
	HVconfig previous=hv_configs.Last();
	cout<<"id="<<previous.id()<<"; descr="<<previous.description()<<endl;
	auto previousitems=previous.CreateEntriesFactory().GetList();
	cout<<previousitems.size()<<" items"<<endl;
	ifstream file;
	file.open(string(argv[2]));
	if(file){
		cout<<"file found"<<endl;
		file.close();
	}else cout <<"file not found"<<endl;
}