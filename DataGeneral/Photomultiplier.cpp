// this file is distributed under 
// MIT license
#include <DataGeneral/Photomultiplier.h>
using namespace std;
using namespace DataAccess;
namespace JPetData {
	PhotomultiplierData::PhotomultiplierData()
	:f_key(0),f_run_id(0),f_setup_id(0),f_active(false),f_status(""),f_name(""),f_max_HV(0){}
	PhotomultiplierData::PhotomultiplierData(const PhotomultiplierData& source)
	:f_key(source.f_key),f_run_id(source.f_run_id),
	f_setup_id(source.f_setup_id),f_active(source.f_active),
	f_status(source.f_status),f_name(source.f_name),f_max_HV(source.f_max_HV){}
	const string PhotomultiplierData::keyfield(){return "photomultiplier_id";}
	PhotomultiplierData::PhotomultiplierData(const DataItem& row)
	:f_key(row.num_field<size_t>("photomultiplier_id")),f_run_id(row.num_field<size_t>("run_id")),
	f_setup_id(row.num_field<size_t>("setup_id")),f_active(row["photomultiplier_isactive"]=="t"),
	f_status(row["photomultiplier_status"]),f_name(row["photomultiplier_name"]),
	f_max_HV(row.num_field<double>("photomultiplier_maxhv")){}
	PhotomultiplierData::~PhotomultiplierData(){}
	const size_t PhotomultiplierData::id() const{return f_key;}
	const size_t PhotomultiplierData::run_id() const{return f_run_id;}
	const size_t PhotomultiplierData::setup_id() const{return f_setup_id;}
	const bool PhotomultiplierData::isActive() const{return f_active;}
	const string& PhotomultiplierData::status() const{return f_status;}
	const string& PhotomultiplierData::name() const{return f_name;}
	const double& PhotomultiplierData::max_HV() const{return f_max_HV;}

	PhotomultipliersData::PhotomultipliersData(const shared_ptr< IDataSource > src, const size_t run_id)
	:Factory< JPetData::PhotomultiplierData >(src,{to_string(run_id)}),f_run_id(run_id){}
	PhotomultipliersData::~PhotomultipliersData(){}
	const size_t PhotomultipliersData::RunID() const{return f_run_id;}
};