// this file is distributed under 
// MIT license
#include <iostream>
#include <JPetData/Frames.h>
#include <JPetData/Detectors.h>
#include <JPetData/HVconfig.h>
#include <Postgres/postgres_data.h>
using namespace std;
using namespace DataAccess;
using namespace JPetSetup;
int main(){
	DBConfigData cfg;
	cfg.hostname="127.0.0.1";
	cfg.port="5432";
	cfg.db_name="postgres";
	cfg.username="postgres";
	cfg.password="pass";
	auto src=make_shared<PQData>(cfg);
	Photomultipliers phm_table(src);
	HVPMConnections connections(src);
	for(const Frame&frame:Frames(src).GetList()){
		cout << frame.id()<<": "<<frame.description()<<endl;
		for(const Layer&layer:frame.CreateLayersFactory().GetList()){
			cout<<"\tlayer "<<layer.id()<<": "<<layer.name()<<"; r="<<layer.radius()<<endl;
			for(const Slot&slot:layer.CreateSlotsFactory().GetList()){
				cout <<"\t\tslot "<<slot.id()<<": "<<slot.name()<<endl;
				for(const HVPMConnection&con:connections.BySlotID(slot.id())){
					Photomultiplier phm=phm_table.ByID(con.photomultiplier_id());
					cout<<"\t\t\tphm "<<phm.id()<<": "<<phm.name()<<endl;
				}
			}
		}
		for(const Setup&setup:frame.CreateSetupFactory().GetList())
			cout<<"\tsetup "<<setup.id()<<": "<<setup.name()<<"; "<<setup.description()<<endl;
	}
}