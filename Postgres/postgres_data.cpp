// this file is distributed under 
// MIT license
#include <iostream>
#include <Postgres/postgres_data.h>
using namespace std;
using namespace pqxx;
namespace DataAccess{
	PQData::PQData(const DBConfigData& cfg)
	:f_connection(
		"dbname="+cfg.db_name+
		" user="+cfg.username+
		" password="+cfg.password+
		" host="+cfg.hostname+
		" port="+cfg.port
	),f_work(f_connection){}
	PQData::~PQData(){
		f_work.commit();
		f_connection.disconnect();
	}
	const bool PQData::Request(const RequestType request, const RequestParameters& params, vector<DataItem>&out){
		if(f_connection.is_open()){
			string funcname="";
			string tablename="";
			string par_vals="";
			switch(request.data){
			case data_calibrationtype:
				switch(request.operation){
				case data_obtain: funcname="getcalibrationtypes"; break;
				case data_insert: funcname="insert_calibrationtype"; break;
				case data_remove: return false; break;
				};
			break;
			case data_calibration_phmampl:
				switch(request.operation){
				case data_obtain: funcname="getcalibrations_phmampl_allphm"; break;
				case data_insert: funcname="insert_calibration_phmampl"; break;
				case data_remove: return false; break;
				};
			break;
			case data_calibration_phmampl_connected:
				switch(request.operation){
				case data_obtain: funcname="getcalibrations_phmampl_setupandphm"; break;
				case data_insert: funcname="connect_calibration_phmampl"; break;
				case data_remove: return false; break;
				};
			break;
			default:
				return false;
			}
			result l_result;
			if(funcname!=""){
				if(par_vals!="")throw;
				if(params.size()>0){
					par_vals=params[0];
					for(size_t i=1;i<params.size();i++)
						par_vals+=","+params[i];
				}
				l_result=f_work.exec("SELECT * FROM "+funcname+"("+par_vals+");");
			}else{
				l_result=f_work.exec("SELECT * FROM "+tablename+" where "+par_vals+";");
			}
			for(const auto&item:l_result){
				map<string,string> toinsert;
				for(const auto&field:item)
					if(field.size()>0)
						toinsert[field.name()]=field.as<string>();
				out.push_back(toinsert);
			}
			return true;
		}
		return false;
	}
};