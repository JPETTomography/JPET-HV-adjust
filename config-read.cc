// this file is distributed under 
// MIT license
#include <fstream>
#include "config-read.h"
using namespace std;
connstr_config::~connstr_config(){}
const connstr_config& connstr_config::instance(){
	static connstr_config s_instance;
	return s_instance;
}
const string connstr_config::DB(){
	return instance().f_db;
}
const string connstr_config::HV(){
	return instance().f_hv;
}
connstr_config::connstr_config(){
	ifstream input;
	input.open("connstr.txt");
	if(input){
		getline(input,f_db);
		getline(input,f_hv);
		input.close();
	}
}


