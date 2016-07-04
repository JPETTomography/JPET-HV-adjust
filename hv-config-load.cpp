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
						HVconfigTable configtable(db,setupsearch[0].id());
						function<bool(const DataItem&)> condition=[&hvconfigname](const DataItem&item){
							return item["description"]==hvconfigname;
						};
						if(configtable.Select(condition).size()>0)cout<<"hvconfig allready exists"<<endl;
						else{
							configtable.Add(HVconfig(hvconfigname));
							HVconfig new_config=configtable.Select(condition)[0];
							HighVoltage highvoltage=HighVoltageTable(db).ByID(setupsearch[0].highvoltage_id());
							HVTable hvtable(new_config,setupsearch[0],framesearch[0],highvoltage,db,hv);
							ifstream file;
							file.open(filename);
							if(file){
								ReadHV(file,hvtable);
								db->Commit();
								file.close();
								return 0;
							}else cout <<"cannot open file"<<endl;
						}
					}else cout <<"setup name is not unique"<<endl;
				}else cout<<"setup not found"<<endl;
			}else cout<<"frame name is not unique"<<endl;
		}else cout<<"frame not found"<<endl;
	}else cout<<"wrong parameters"<<endl;
	return 1;
}