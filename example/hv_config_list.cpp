// this file is distributed under 
// MIT license
#include <iostream>
#include <JPetData/HVconfig.h>
#include <Postgres/postgres_data.h>
using namespace std;
using namespace DataAccess;
using namespace JPetSetup;
int main(){
	auto src=make_shared<PQData>(DBConfigData("host=127.0.0.1;database=postgres;login=postgres;pwd=pass"));
	
	for(const HVconfig&config:HVconfigTable(src,1).SelectAll()){
		cout<<config.id()<<":"<<config.description()<<endl;
		for(const HVconfigEntry&entry:config.CreateEntriesFactory().SelectAll())
			cout<<" \t hvpm:" << entry.HVPMConnection_id()<<"; hv="<<entry.HV()<<endl;
	}
}