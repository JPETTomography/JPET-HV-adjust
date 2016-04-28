// this file is distributed under 
// MIT license
#include <iostream>
#include <JPetData/Frames.h>
#include <Postgres/postgres_data.h>
using namespace std;
using namespace DataAccess;
int main(){
	DBConfigData cfg;
	cfg.hostname="127.0.0.1";
	cfg.port="5432";
	cfg.db_name="postgres";
	cfg.username="postgres";
	cfg.password="pass";
	auto src=make_shared<PQData>(cfg);

}