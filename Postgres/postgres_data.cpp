// this file is distributed under 
// MIT license
#include <iostream>
#include <Postgres/postgres_data.h>
using namespace std;
using namespace pqxx;
namespace DataAccess{
	PQData::PQData(const DBConfigData& cfg)
	:f_connection(new connection("dbname="+cfg.db_name+" user="+cfg.username+" password="+cfg.password+" host="+cfg.hostname+" port="+cfg.port)),
	f_work(new work(*f_connection)){
		if(f_connection->is_open())
			cout << "Opened database successfully: " << f_connection->dbname() << endl;
		else
			cout << "Can't open database" << endl;
	}
	PQData::~PQData(){
		if(f_connection->is_open()){
			f_connection->disconnect();
			f_connection->deactivate();
			f_work->commit();
		}
		
	}
	const bool PQData::Request(const RequestType request, const RequestParameters& params, vector< DataItem >&){
		string funcname="";
		switch(request.data){
			case calibrationtype:
				switch(request.operation){
				case data_obtain: funcname="getcalibrationtypes"; break;
				case data_insert: funcname="insert_calibrationtype"; break;
				case data_remove: return false; break;
				};
				break;
			case calibration_phmampl:
				switch(request.operation){
				case data_obtain: funcname="getcalibrations_phmampl_allphm"; break;
				case data_insert: funcname="insert_calibration_phmampl"; break;
				case data_remove: return false; break;
				};
				break;
			case calibration_phmampl_connected:
				switch(request.operation){
				case data_obtain: funcname="getcalibrations_phmampl_setupandphm"; break;
				case data_insert: funcname="connect_calibration_phmampl"; break;
				case data_remove: return false; break;
				};
				break;
			case photomultiplierdata:
				switch(request.operation){
				case data_obtain: funcname="getphotomultipliersdata"; break;
				case data_insert: return false; break;
				case data_remove: return false; break;
				};
				break;
			default:
				return false;
		}
		string par_vals="";
		if(params.size()>0){
			par_vals=params[0];
			for(size_t i=1;i<params.size();i++)
				par_vals+=","+params[i];
		}
		result l_result=f_work->exec("SELECT * FROM "+funcname+"("+par_vals+");");
		f_work->commit();
		return true;
	}
};