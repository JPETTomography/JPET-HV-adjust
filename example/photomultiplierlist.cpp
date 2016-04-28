// this file is distributed under 
// MIT license
#include <iostream>
#include <JPetData/Photomultiplier.h>
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
	auto src=make_shared<PQData>(cfg);
	PhotomultipliersData data1(src,1);
	cout << data1.size()<<endl;
	for(const PhotomultiplierData&item:data1.GetList())
		cout<<item.setup_id()<<" - "<<item.name()<<endl;
	PhotomultipliersData data2(src,2);
	cout << data2.size()<<endl;
	for(const PhotomultiplierData&item:data2.GetList())
		cout<<item.setup_id()<<" - "<<item.name()<<endl;
	PhotomultipliersData data3(src,3);
	cout << data3.size()<<endl;
	for(const PhotomultiplierData&item:data3.GetList())
		cout<<item.setup_id()<<" - "<<item.name()<<endl;
	PhotomultipliersData data4(src,1);
	cout << data4.size()<<endl;
	for(const PhotomultiplierData&item:data4.GetList())
		cout<<item.setup_id()<<" - "<<item.name()<<endl;
}