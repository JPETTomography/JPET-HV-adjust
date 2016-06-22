// this file is distributed under 
// MIT license
#include <iostream>
#include <string>
#include "config-read.h"
using namespace std;
int main(int argc,char**argv){
	cout<<"DB: "<<connstr_config::DB()<<endl;
	cout<<"HV: "<<connstr_config::HV()<<endl;
}