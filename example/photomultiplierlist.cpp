// this file is distributed under 
// MIT license
#include <iostream>
#include <DataGeneral/Photomultiplier.h>
#include <Postgres/postgres_data.h>
using namespace std;
using namespace DataAccess;
using namespace JPetData;
int main(){
	DBConfigData cfg;
	cfg.hostname="127.0.0.1";
	cfg.port="5432";
	cfg.db_name="postgres";
	cfg.username="postgres";
	cfg.password="pass";
	PhotomultipliersData data(make_shared<PQData>(cfg),1);
	cout << data.size()<<endl;
	for(const PhotomultiplierData&item:data.GetList())
		cout<<item.name()<<endl;
}