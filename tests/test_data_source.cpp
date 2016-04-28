#include "test_data_source.h"
using namespace std;
using namespace DataAccess;
test_data_source::test_data_source(const bool valid):f_valid(valid){
	f_counters[data_obtain]=0;
	f_counters[data_insert]=0;
	f_counters[data_remove]=0;
}
test_data_source::~test_data_source(){}
const bool test_data_source::Request(const RequestType request, const RequestParameters& params, vector< DataItem >& output){
	if(!f_valid)return false;
	switch(request.data){
		case data_dummy:{
			map<string,string> item;
			item["field1"]="value1";
			item["field2"]="value2";
			output.push_back(DataItem(item));
			item["field1"]="value3";
			item["field2"]="value4";
			output.push_back(DataItem(item));
		}break;
		case data_calibrationtype:{
			map<string,string> item;
			item["type_id"]="1";
			item["name"]="linear";
			item["param_count"]="2";
			item["formula"]="[0]+[1]*x";
			output.push_back(DataItem(item));
			item["type_id"]="2";
			item["name"]="parabolic";
			item["param_count"]="3";
			item["formula"]="[0]+[1]*x+[2]*x*x";
			output.push_back(DataItem(item));
		}break;
		case data_calibration_phmampl:{
			map<string,string> item;
			item["cal_id"]="1";
			item["id_type"]="2";
			item["id_phm"]="1";
			item["name"]="parabolic";
			item["param_count"]="3";
			item["formula"]="[0]+[1]*x+[2]*x*x";
			item["param_values"]="1 1 1 ";
			output.push_back(DataItem(item));
			item["cal_id"]="2";
			item["id_type"]="2";
			item["id_phm"]="1";
			item["name"]="parabolic";
			item["param_count"]="3";
			item["formula"]="[0]+[1]*x+[2]*x*x";
			item["param_values"]="2 2 2 ";
			output.push_back(DataItem(item));
		};break;
		case data_calibration_phmampl_connected:{
			map<string,string> item;
			item["id"]="1";
			item["cal_id"]="1";
			item["id_run"]="1";
			item["id_phm"]="1";
			item["name"]="parabolic";
			item["param_count"]="3";
			item["formula"]="[0]+[1]*x+[2]*x*x";
			item["param_values"]="1 1 1 ";
			output.push_back(DataItem(item));
			item["id"]="2";
			item["cal_id"]="2";
			item["id_run"]="1";
			item["id_phm"]="1";
			item["name"]="parabolic";
			item["param_count"]="3";
			item["formula"]="[0]+[1]*x+[2]*x*x";
			item["param_values"]="2 2 2 ";
			output.push_back(DataItem(item));
		};break;
	}
	f_counters[request.operation]++;
	return true;
}
const size_t test_data_source::Count(const operationtype type) const{return f_counters.find(type)->second;}
