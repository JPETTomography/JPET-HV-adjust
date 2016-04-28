// this file is distributed under 
// MIT license
#include <DataGeneral/Calibration.h>
using namespace std;
using namespace DataAccess;
namespace Calibration {
	PhotomultiplierGain::PhotomultiplierGain(const DataItem& row)
	:CalibrationForEquipment({row.num_field<size_t>("id_phm")},row,{"cal_id","name","param_count","formula","param_values","id_type"}){}
	PhotomultiplierGain::PhotomultiplierGain(const size_t phm_id, const CalibrationType& type, const parameter_set& parameters)
	:CalibrationForEquipment({phm_id},type,parameters){}
	PhotomultiplierGain::PhotomultiplierGain(const PhotomultiplierGain& source)
	:CalibrationForEquipment(source){}
	PhotomultiplierGain::~PhotomultiplierGain(){}
	const size_t PhotomultiplierGain::phm_id() const{return equipment_ids()[0];}
	const RequestParameters PhotomultiplierGain::params_to_insert() const{
		if(id()>0) return {};
		else return {to_string(phm_id()),to_string(type_id()),"'"+encoded_params()+"'"};
	}
	const RequestParameters PhotomultiplierGain::params_to_delete() const{
		if(id()>0)return {to_string(id())};
		else return {};
	}
	
	PhotomultiplierGains::PhotomultiplierGains(const shared_ptr<IDataSource> src, const size_t phm_id):Factory<PhotomultiplierGain>(src,{to_string(phm_id)}),f_phm_id(phm_id){}
	const size_t PhotomultiplierGains::PhotomultiplierID() const{return f_phm_id;}
	PhotomultiplierGains::~PhotomultiplierGains(){}
	
	
	
	
	
	
	PhotomultiplierGain4Run::PhotomultiplierGain4Run(const PhotomultiplierGain4Run& source)
	:CalibrationForEquipmentAndRun(source),f_key(source.f_key){}
	PhotomultiplierGain4Run::PhotomultiplierGain4Run(const DataItem& row)
	:CalibrationForEquipmentAndRun({row.num_field<size_t>("id_phm")},row.num_field<size_t>("id_run"),row,{"cal_id","name","param_count","formula","param_values"}){}
	PhotomultiplierGain4Run::PhotomultiplierGain4Run(const size_t cal_id, size_t run_id)
	:CalibrationForEquipmentAndRun(cal_id, run_id){}
	PhotomultiplierGain4Run::~PhotomultiplierGain4Run(){}
	const size_t PhotomultiplierGain4Run::phm_id() const{return equipment_ids()[0];}
	const size_t PhotomultiplierGain4Run::connection_id() const{return f_key;}
	const RequestParameters PhotomultiplierGain4Run::params_to_insert() const{
		if(f_key>0)return {};
		else return {to_string(id()),to_string(run_id())};
	}
	const RequestParameters PhotomultiplierGain4Run::params_to_delete() const{
		if(f_key>0)return {to_string(f_key)};
		else return {};
	}
	
	PhotomultiplierGains4Run::PhotomultiplierGains4Run(const shared_ptr< IDataSource > src, const size_t run_id)
	:Factory<PhotomultiplierGain4Run>(src,{to_string(run_id)}),f_run_id(run_id){}
	PhotomultiplierGains4Run::~PhotomultiplierGains4Run(){}
	const size_t PhotomultiplierGains4Run::RunID() const{return f_run_id;}
};