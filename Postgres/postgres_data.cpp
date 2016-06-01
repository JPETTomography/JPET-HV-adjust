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
	),f_work(f_connection),f_changed(false){
		f_work.exec("begin;");
	}
	PQData::~PQData(){
		f_connection.disconnect();
	}
	const bool PQData::changed() const{return f_changed;}
	void PQData::Commit(){
		f_work.exec("commit;");
		f_changed=false;
	}
	string ReplaceAll(const string&source, const string& from, const string& to) {
		string str=source;
		size_t start_pos = 0;
		while((start_pos = str.find(from, start_pos)) != string::npos) {
			str.replace(start_pos, from.length(), to);
			start_pos += to.length();
		}
		return str;
	}
	const bool PQData::Request(const RequestType request, const RequestParameters&Par, vector<DataItem>&out){
		RequestParameters P;
		out.clear();
		for(const string&s:Par)P.push_back(ReplaceAll(s,",","."));
		if(f_connection.is_open()){
			string sql_request="";
			switch(request.data){
			case data_calibrationtype:
				switch(request.operation){
				case data_obtain: sql_request="select * from getcalibrationtypes();"; break;
				case data_insert: sql_request="select * from insert_calibrationtype("+P[0]+","+P[1]+","+P[2]+");"; break;
				case data_remove: return false; break;
				};
			break;
			case data_calibration_phmampl:
				switch(request.operation){
				case data_obtain: sql_request="select * from getcalibrations_phmampl_allphm("+P[0]+");"; break;
				case data_insert: sql_request="select * from insert_calibration_phmampl("+P[0]+","+P[1]+","+P[2]+");"; break;
				case data_remove: return false; break;
				};
			break;
			case data_calibration_phmampl_connected:
				switch(request.operation){
				case data_obtain: sql_request="select * from getcalibrations_phmampl_setupandphm("+P[0]+");"; break;
				case data_insert: sql_request="select * from connect_calibration_phmampl("+P[0]+","+P[1]+");"; break;
				case data_remove: return false; break;
				};
			break;
			case data_photomultiplier:
				switch(request.operation){
				case data_obtain: sql_request="select * from \"PhotoMultiplier\";"; break;
				case data_insert: return false; break;
				case data_remove: return false; break;
				};
			break;
			case data_hvconfig:
				switch(request.operation){
				case data_obtain: sql_request="select * from \"HVConfig\" where setup_id="+P[0]+";";break;
				case data_insert: sql_request="insert into \"HVConfig\" (setup_id,description)values("+P[0]+","+P[1]+");"; break;
				case data_remove: sql_request="delete from \"HVConfig\" where id="+P[0]+";"; break;
				};
			break;
			case data_hvconfigentry:
				switch(request.operation){
				case data_obtain: sql_request="select * from \"HVConfigEntry\" where hvconfig_id="+P[0]+";";break;
				case data_insert: sql_request="insert into \"HVConfigEntry\" (hvpm_id,voltageset,hvconfig_id)values("+P[0]+","+P[1]+","+P[2]+");"; break;
				case data_remove: sql_request="delete from \"HVConfigEntry\" where id="+P[0]+";"; break;
				};
			break;
			case data_frame:
				switch(request.operation){
				case data_obtain: sql_request="select * from \"Frame\";";break;
				case data_insert: return false; break;
				case data_remove: return false; break;
				};
			break;
			case data_layer:
				switch(request.operation){
				case data_obtain: sql_request="select * from \"Layer\" where frame_id="+P[0]+";";break;
				case data_insert: return false; break;
				case data_remove: return false; break;
				};
			break;
			case data_slot:
				switch(request.operation){
				case data_obtain: sql_request="select * from \"Slot\" where layer_id="+P[0]+";";break;
				case data_insert: return false; break;
				case data_remove: return false; break;
				};
			break;
			case data_setup:
				switch(request.operation){
				case data_obtain: sql_request="select * from \"Setup\" where frame_id="+P[0]+";";break;
				case data_insert: return false; break;
				case data_remove: return false; break;
				};
			break;
			case data_hvpmconnection:
				switch(request.operation){
				case data_obtain: sql_request="select * from \"HVPMConnection\";";break;
				case data_insert: return false; break;
				case data_remove: return false; break;
				};
			break;
			case data_highvoltage:
				switch(request.operation){
				case data_obtain: sql_request="select * from \"HighVoltage\";";break;
				case data_insert: return false; break;
				case data_remove: return false; break;
				};
			break;
			case data_hvchannel:
				switch(request.operation){
				case data_obtain: sql_request="select * from \"HVChannel\";";break;
				case data_insert: return false; break;
				case data_remove: return false; break;
				};
			break;
			default:
				return false;
			}
				for(const auto&item:f_work.exec(sql_request)){
					map<string,string> toinsert;
					for(const auto&field:item)if(field.size()>0)
							toinsert[field.name()]=field.as<string>();
					out.push_back(toinsert);
				}
				if(data_obtain!=request.operation)f_changed=true;
				return true;
		}
		return false;
	}
};