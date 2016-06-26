// this file is distributed under 
// MIT license
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
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
	if(argc==4){
		string framename(argv[1]),setupname(argv[2]),hvconfigname(argv[3]);
		auto db=make_shared<PQData>(connstr_config::DB());
		auto hv=make_shared<CAEN>(connstr_config::HV());
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
								try{
									HighVoltage highvoltage=HighVoltageTable(db).ByID(setupsearch[0].highvoltage_id());
									HVTable hvtable(hvconfigsearch[0],setupsearch[0],framesearch[0],highvoltage,db,hv);
									cout<<"Writting HV"<<endl;
									hvtable.SynchroHardwarewithDB();
									for(bool matches=false;!matches;){
										cout<<"waiting..."<<endl;
										this_thread::sleep_for(chrono::seconds(10));
										cout<<"Reading HV..."<<endl;
										hvtable.read_hardware();
										matches=true;
										for(size_t index=0;(index<hvtable.SlotInfo().size())&&matches;index++){
											double hv_conf=hvtable.HVConfigEntries()[index].HV();
											if(isfinite(hv_conf)){//not configured channels are skipped
												cout<<"IDX="<<hvtable.SlotInfo()[index].hvchannel.idx()<<";";
												cout<<hv_conf<<";";
												double hv_actual=hvtable.HardwareHV()[index];
												cout<<hv_actual<<";";
												if(isfinite(hv_actual)){
													if(pow(hv_actual-hv_conf,2)<pow(max_difference,2))cout<<"OK.";
													else{
														cout<<"notOK!";
														matches=false;
													}
												}else{
													cout <<"HV hardware unknown error (infinite value was read). Code=2"<<endl;
													return 2;
												}
												cout<<endl;
											}
										}
									}
									cout<<"Success"<<endl;
									return 0;
								}catch(libhvException* ex){
									//yes, they throw pointers to exception
									//and don't override what()
									cout<<"HVERROR!!!!!: "<< ex->getMessage()<<endl;
									cout<<"Code = 3"<<endl;
									return 3;
								}
							}else cout<<"hvconfig name is not inique"<<endl;
						}else cout<<"hvconfig not found"<<endl;
					}else cout <<"setup name is not unique"<<endl;
				}else cout<<"setup not found"<<endl;
			}cout<<"frame name is not unique"<<endl;
		}else cout<<"frame not found"<<endl;
	}else cout<<"wrong parameters"<<endl;
	cout<<"Code = 1"<<endl;
	return 1;
}