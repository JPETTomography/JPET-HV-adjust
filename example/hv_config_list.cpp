// this file is distributed under 
// MIT license
#include <iostream>
#include <JPetData/HVconfig.h>
#include <Postgres/postgres_data.h>
using namespace std;
using namespace DataAccess;
using namespace JPetSetup;
int main(){
	auto src=make_shared<PQData>("host=127.0.0.1 dbname=test user=postgres password=pass");
	for(const HVconfig&config:HVconfigTable(src,1).SelectAll()){
		cout<<config.id()<<":"<<config.description()<<endl;
		for(const HVconfigEntry&entry:config.CreateEntriesInterface().SelectAll())
			cout<<" \t hvpm:" << entry.HVPMConnection_id()<<"; hv="<<entry.HV()<<endl;
	}
}