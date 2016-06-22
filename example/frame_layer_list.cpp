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
	auto src=make_shared<PQData>("host=127.0.0.1 dbname=test user=postgres password=pass");
	Photomultipliers phm_table(src);
	HVPMConnections connections(src);
	for(const Frame&frame:Frames(src).SelectAll()){
		cout << frame.id()<<": "<<frame.description()<<endl;
		for(const Layer&layer:frame.CreateLayersFactory().SelectAll()){
			cout<<"\tlayer "<<layer.id()<<": "<<layer.name()<<"; r="<<layer.radius()<<endl;
			for(const Slot&slot:layer.CreateSlotsFactory().SelectAll()){
				cout <<"\t\tslot "<<slot.id()<<": "<<slot.name()<<endl;
				for(const HVPMConnection&con:connections.BySlotID(slot.id())){
					Photomultiplier phm=phm_table.ByID(con.photomultiplier_id());
					cout<<"\t\t\tphm "<<phm.id()<<": "<<phm.name()<<endl;
				}
			}
		}
		for(const Setup&setup:frame.CreateSetupFactory().SelectAll())
			cout<<"\tsetup "<<setup.id()<<": "<<setup.name()<<"; "<<setup.description()<<endl;
	}
}