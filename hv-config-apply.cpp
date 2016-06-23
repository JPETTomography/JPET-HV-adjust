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
#include <HV/CAEN.h>
#include "config-read.h"
using namespace std;
using namespace DataAccess;
using namespace JPetSetup;
using namespace Hardware;
using namespace HVAdjust;
const double max_difference=2;
int main(int argc,char**argv){
	if(argc==3){
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
									for(size_t index=0;index<hvtable.SlotInfo().size();index++){
										cout<<"IDX="<<hvtable.SlotInfo()[index].hvchannel.idx()<<"...";
										if(isfinite(hvtable.HVConfigEntries()[index].HV())){
											cout<<"comparing...";
											if(isfinite(hvtable.HardwareHV()[index])){
												bool res=(pow(hvtable.HVConfigEntries()[index].HV()-hvtable.HardwareHV()[index],2)<pow(max_difference,2));
												if(res)cout<<"OK.";
												else cout<<"not OK.";
												matches&=res;
											}else{
												matches=false;
												cout <<"HV hardware unknown error (infinite value was read). Code=2"<<endl;
												return 2;
											}
											cout<<endl;
										}
										cout<<"not configured"<<endl;
									}
									cout<<"Success"<<endl;
									return 0;
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