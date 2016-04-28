// this file is distributed under 
// MIT license
#include <JPetData/HVconfig.h>
using namespace std;
using namespace DataAccess;
namespace JPetSetup{
	HVconfigEntry::HVconfigEntry(const HVconfigEntry& source)
	:f_hvpm_id(source.f_hvpm_id),f_hvconfig_id(source.f_hvconfig_id),f_hv_value(source.f_hv_value){}
	HVconfigEntry::HVconfigEntry(const size_t hvpm, const size_t hvconfig, double value)
	:f_hvpm_id(hvpm),f_hvconfig_id(hvconfig),f_hv_value(value){}
	HVconfigEntry::HVconfigEntry(const DataItem& item, const shared_ptr<IDataSource>)
	:f_hvpm_id(item.num_field<size_t>("id")),f_hvconfig_id(item.num_field<size_t>("hvconfig_id")),
	f_hv_value(item.num_field<double>("voltageset")){}
	HVconfigEntry::~HVconfigEntry(){}
	const size_t HVconfigEntry::hvconfig_id() const{return f_hvconfig_id;}
	const size_t HVconfigEntry::HVPMConnection_id() const{return f_hvpm_id;}
	const double HVconfigEntry::HV() const{return f_hv_value;}
	RequestParameters HVconfigEntry::params_to_insert() const{
		return {to_string(HVPMConnection_id()),to_string(hvconfig_id()),to_string(HV())};
	}
	
	HVconfig::HVconfig(const HVconfig& source)
	:f_id(source.f_id),f_description(source.f_description),f_source(){}
	HVconfig::HVconfig(string&descr)
	:f_id(0),f_description(descr){}
	HVconfig::HVconfig(const DataItem& item, const shared_ptr<IDataSource>src)
	:f_id(item.num_field<size_t>("id")),f_description(item["description"]),f_source(src){}
	HVconfig::~HVconfig(){}
	const size_t HVconfig::id() const{return f_id;}
	const string& HVconfig::description() const{f_description;}
	RequestParameters HVconfig::params_to_insert() const{return {"'"+description()+"'"};}
	Factory<HVconfigEntry> HVconfig::CreateEntriesFactory() const{
		return Factory<HVconfigEntry>(f_source,{to_string(id())});
	}
	HVconfigTable::HVconfigTable(const shared_ptr<IDataSource>src):Factory<JPetSetup::HVconfig>(src,{}){}
	HVconfigTable::~HVconfigTable(){}
};