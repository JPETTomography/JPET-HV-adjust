// this file is distributed under 
// MIT license
#include <iostream>
#include <JPetData/HVconfig.h>
#include <Postgres/postgres_data.h>
using namespace std;
using namespace DataAccess;
using namespace JPetSetup;
int main(){
	DBConfigData cfg;
	cfg.hostname="127.0.0.1";
	cfg.port="5432";
	cfg.db_name="postgres";
	cfg.username="postgres";
	cfg.password="pass";
	auto src=make_shared<PQData>(cfg);
	
	for(const HVconfig&config:HVconfigTable(src).GetList()){
		cout<<config.id()<<":"<<config.description()<<endl;
		for(const HVconfigEntry&entry:config.CreateEntriesFactory().GetList())
			cout<<" \t hvpm:" << entry.HVPMConnection_id()<<"; hv="<<entry.HV()<<endl;
	}
}