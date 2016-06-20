#include "math.h"
#include "test_data_source.h"
using namespace std;
using namespace DataAccess;
test_data_source::test_data_source(const bool valid):f_valid(valid){ResetCounters();}
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
		case data_photomultiplier:{
			map<string,string> item;
			item["status"]="some status message";
			item["description"]="";
			item["maxhv"]="1000";
			item["id"]="1";
			item["name"]="PHM1";
			output.push_back(DataItem(item));
			item["id"]="2";
			item["name"]="PHM2";
			output.push_back(DataItem(item));
			item["id"]="3";
			item["name"]="PHM3";
			output.push_back(DataItem(item));
			item["id"]="4";
			item["name"]="PHM4";
			output.push_back(DataItem(item));
			item["id"]="5";
			item["name"]="PHM5";
			output.push_back(DataItem(item));
			item["id"]="6";
			item["name"]="PHM6";
			output.push_back(DataItem(item));
			item["id"]="7";
			item["name"]="PHM7";
			output.push_back(DataItem(item));
			item["id"]="8";
			item["name"]="PHM8";
			output.push_back(DataItem(item));
		};break;
		case data_frame:{
			map<string,string> item;
			item["id"]="1";
			item["version"]="2";
			item["description"]="old";
			item["status"]="inactive";
			item["isactive"]="f";
			output.push_back(DataItem(item));
			item["id"]="2";
			item["version"]="3";
			item["description"]="";
			item["status"]="";
			item["isactive"]="t";
			output.push_back(DataItem(item));
		};break;
		case data_layer:{
			map<string,string> item;
			item["id"]="1";
			item["frame_id"]="1";
			item["name"]="only";
			item["radius"]="1";
			item["isactive"]="t";
			output.push_back(DataItem(item));
			item["id"]="2";
			item["frame_id"]="2";
			item["name"]="first";
			item["radius"]="1";
			item["isactive"]="t";
			output.push_back(DataItem(item));
			item["id"]="3";
			item["frame_id"]="2";
			item["name"]="second";
			item["radius"]="1.2";
			item["isactive"]="t";
			output.push_back(DataItem(item));
		};break;
		case data_slot:{
			map<string,string> item;
			if(params[0]=="1"){
				item["id"]="1";
				item["layer_id"]="1";
				item["name"]="1";
				item["isactive"]="t";
				output.push_back(DataItem(item));
				item["id"]="2";
				item["layer_id"]="1";
				item["name"]="2";
				item["isactive"]="t";
			}
			if(params[0]=="2"){
				output.push_back(DataItem(item));
				item["id"]="3";
				item["layer_id"]="2";
				item["name"]="1";
				item["isactive"]="t";
				output.push_back(DataItem(item));
				item["id"]="4";
				item["layer_id"]="2";
				item["name"]="2";
				item["isactive"]="t";
				output.push_back(DataItem(item));
			}
			if(params[0]=="3"){
				item["id"]="5";
				item["layer_id"]="3";
				item["name"]="1";
				item["isactive"]="t";
				output.push_back(DataItem(item));
				item["id"]="6";
				item["layer_id"]="3";
				item["name"]="2";
				item["isactive"]="t";
				output.push_back(DataItem(item));
			}
		};break;
		case data_highvoltage:{
			map<string,string> item;
			item["id"]="1";
			item["description"]="the only HV block in test database";
			item["status"]="";
			output.push_back(DataItem(item));
		};break;
		case data_hvchannel:if(params[0]=="1"){
			map<string,string> item;
			item["highvoltage_id"]="1";
			item["status"]="";
			item["givespositivevoltage"]="t";
			item["minv"]="10";
			item["maxv"]="1000";
			item["maxi"]="500";
			
			item["id"]="1";
			item["channelidx"]="1";
			output.push_back(DataItem(item));
			item["id"]="2";
			item["channelidx"]="2";
			output.push_back(DataItem(item));
			item["id"]="3";
			item["channelidx"]="3";
			output.push_back(DataItem(item));
			item["id"]="4";
			item["channelidx"]="4";
			output.push_back(DataItem(item));
			item["id"]="5";
			item["channelidx"]="5";
			output.push_back(DataItem(item));
			item["id"]="6";
			item["channelidx"]="6";
			output.push_back(DataItem(item));
			item["id"]="7";
			item["channelidx"]="7";
			output.push_back(DataItem(item));
			item["id"]="8";
			item["channelidx"]="8";
			output.push_back(DataItem(item));
		};break;
		case data_setup:if(params[0]=="2"){
			map<string,string> item;
			item["id"]="1";
			item["frame_id"]="2";
			item["highvoltage_id"]="1";
			output.push_back(DataItem(item));
		};break;
		case data_hvpmconnection:{
			map<string,string> item;
			item["setup_id"]="1";
			item["isright"]="t";
			item["id"]="1";
			item["hvchannel_id"]="1";
			item["slot_id"]="3";
			item["photomultiplier_id"]="1";
			output.push_back(DataItem(item));
			item["id"]="2";
			item["hvchannel_id"]="2";
			item["slot_id"]="4";
			item["photomultiplier_id"]="2";
			output.push_back(DataItem(item));
			item["id"]="3";
			item["hvchannel_id"]="3";
			item["slot_id"]="5";
			item["photomultiplier_id"]="3";
			output.push_back(DataItem(item));
			item["id"]="4";
			item["hvchannel_id"]="4";
			item["slot_id"]="6";
			item["photomultiplier_id"]="3";
			output.push_back(DataItem(item));
			item["isright"]="f";
			item["id"]="5";
			item["hvchannel_id"]="5";
			item["slot_id"]="3";
			item["photomultiplier_id"]="5";
			output.push_back(DataItem(item));
			item["id"]="6";
			item["hvchannel_id"]="6";
			item["slot_id"]="4";
			item["photomultiplier_id"]="6";
			output.push_back(DataItem(item));
			item["id"]="7";
			item["hvchannel_id"]="7";
			item["slot_id"]="5";
			item["photomultiplier_id"]="7";
			output.push_back(DataItem(item));
			item["id"]="8";
			item["hvchannel_id"]="8";
			item["slot_id"]="6";
			item["photomultiplier_id"]="8";
			output.push_back(DataItem(item));
		};break;
		case data_hvconfig:if(params[0]=="1"){
			map<string,string> item;
			item["id"]="1";
			item["setup_id"]="1";
			item["description"]="";
			output.push_back(DataItem(item));
		};break;
		case data_hvconfigentry:if(params[0]=="1"){
			map<string,string> item;
			item["voltageset"]="500";
			item["hvconfig_id"]="1";
			
			item["id"]="1";
			item["hvpm_id"]="1";
			output.push_back(DataItem(item));
			item["id"]="2";
			item["hvpm_id"]="2";
			output.push_back(DataItem(item));
			item["id"]="4";
			item["hvpm_id"]="4";
			output.push_back(DataItem(item));
			item["id"]="5";
			item["hvpm_id"]="6";
			output.push_back(DataItem(item));
			item["id"]="6";
			item["hvpm_id"]="8";
			output.push_back(DataItem(item));
		};break;
		default:
			return false;
	}
	f_counters[request.operation]++;
	return true;
}
const size_t test_data_source::Count(const operationtype type)const{return f_counters.find(type)->second;}
void test_data_source::ResetCounters(){
	f_counters[data_obtain]=0;
	f_counters[data_insert]=0;
	f_counters[data_remove]=0;
}



test_hv_setter::test_hv_setter():f_update_counter(0){}
test_hv_setter::~test_hv_setter(){}
bool test_hv_setter::IsOn(size_t channel_no) const{
	if(f_data.find(channel_no)!=f_data.end())
		return f_data.find(channel_no)->second.first;
	else 
		return false;
}
double test_hv_setter::GetHV(size_t channel_no) const{
	if(f_data.find(channel_no)!=f_data.end())
		return f_data.find(channel_no)->second.second;
	else 
		return INFINITY;
}
void test_hv_setter::turnOn(size_t channel_no){
	if(f_data.find(channel_no)!=f_data.end()){
		f_data[channel_no].first=true;
	}else f_data[channel_no]=make_pair(false,INFINITY);
}
void test_hv_setter::turnOff(size_t channel_no){
	if(f_data.find(channel_no)!=f_data.end()){
		f_data[channel_no].first=false;
	}else f_data[channel_no]=make_pair(false,INFINITY);
}

bool test_hv_setter::SetHV(size_t channel_no, double hv){
	if(f_data.find(channel_no)!=f_data.end()){
		f_data[channel_no].second=hv;
	}else f_data[channel_no]=make_pair(false,hv);
}
void test_hv_setter::UpdateRequest(){
	f_update_counter++;
}



