// this file is distributed under 
// MIT license
#include <iostream>
#include <string>
#include <fstream>
#include <math.h>
#include <JPetData/Frames.h>
#include <JPetData/HVconfig.h>
#include <JPetData/HVSetter.h>
#include <JPetData/Detectors.h>
#include <Postgres/postgres_data.h>
#include <libhvexception.h>
#include <HV/CAEN.h>
#include "config-read.h"
using namespace std;
using namespace DataAccess;
using namespace JPetSetup;
using namespace Hardware;
using namespace HVAdjust;
const double max_difference=2;
int main(int argc,char**argv){
	if(argc==5){
		string framename(argv[1]),setupname(argv[2]),hvconfigname(argv[3]),filename(argv[4]);
		auto db=make_shared<PQData>(connstr_config::DB());
		auto hv=make_shared<DummyHV>();
		vector<Frame> framesearch=Frames(db).Select([&framename](const DataItem&item){
			return item["description"]==framename;
		});
		if(framesearch.size()>0){
			if(framesearch.size()==1){
				vector<Setup> setupsearch=framesearch[0].CreateSetupInterface().Select([&setupname](const DataItem&item){
					return item["setname"]==setupname;
				});
				if(setupsearch.size()>0){
					if(setupsearch.size()==1){
						vector<HVconfig> hvconfigsearch=HVconfigTable(db,setupsearch[0].id()).Select([&hvconfigname](const DataItem&item){
							return item["description"]==hvconfigname;
						});
						if(hvconfigsearch.size()>0){
							if(hvconfigsearch.size()==1){
								ofstream file;
								file.open(filename);
								if(file){
									HVTable hvtable(hvconfigsearch[0],setupsearch[0],framesearch[0],HighVoltageTable(db).ByID(setupsearch[0].highvoltage_id()),db,hv);
									SaveHV(file,hvtable);
									file.close();
									return 0;
								}else cout<<"cannot open file"<<endl;
							}else cout<<"hvconfig name is not inique"<<endl;
						}else cout<<"hvconfig not found"<<endl;
					}else cout <<"setup name is not unique"<<endl;
				}else cout<<"setup not found"<<endl;
			}else cout<<"frame name is not unique"<<endl;
		}else cout<<"frame not found"<<endl;
	}else cout<<"wrong parameters"<<endl;
	return 1;
}