// this file is distributed under 
// MIT license
#include <math_h/error.h>
#include <JPetData/HVconfig.h>
using namespace std;
using namespace DataAccess;
namespace JPetSetup{
	HVconfigEntry::HVconfigEntry(const HVconfigEntry& source)
	:f_id(source.f_id),f_hvpm_id(source.f_hvpm_id),
	f_hvconfig_id(source.f_hvconfig_id),f_hv_value(source.f_hv_value){}
	HVconfigEntry& HVconfigEntry::operator=(const HVconfigEntry& source){
		f_id=source.f_id;
		f_hvpm_id=source.f_hvpm_id;
		f_hvconfig_id=source.f_hvconfig_id;
		f_hv_value=source.f_hv_value;
		return *this;
	}
	HVconfigEntry::HVconfigEntry(const size_t hvpm, double value)
	:f_id(0),f_hvpm_id(hvpm),f_hvconfig_id(0),f_hv_value(value){}
	HVconfigEntry::HVconfigEntry(const DataItem& item, const shared_ptr<IDataSource>)
	:f_id(item.num_field<size_t>("id")),f_hvpm_id(item.num_field<size_t>("hvpm_id")),
	f_hvconfig_id(item.num_field<size_t>("hvconfig_id")),f_hv_value(item.num_field<double>("voltageset")){}
	HVconfigEntry::~HVconfigEntry(){}
	const size_t HVconfigEntry::id() const{return f_id;}
	const size_t HVconfigEntry::hvconfig_id() const{return f_hvconfig_id;}
	const size_t HVconfigEntry::HVPMConnection_id() const{return f_hvpm_id;}
	const double HVconfigEntry::HV() const{return f_hv_value;}
	RequestParameters HVconfigEntry::params_to_insert() const{
		return {to_string(HVPMConnection_id()),to_string(HV())};
	}
	RequestParameters HVconfigEntry::params_to_delete() const{
		return {to_string(id())};
	}
	
	HVconfig::HVconfig(const HVconfig& source)
	:f_id(source.f_id),f_setup_id(source.f_setup_id),
	f_description(source.f_description),f_source(source.f_source){}
	HVconfig& HVconfig::operator=(const HVconfig& source){
		f_id=source.f_id;
		f_setup_id=source.f_setup_id;
		f_description=source.f_description;
		f_source=source.f_source;
		return *this;
	}
	HVconfig::HVconfig(const size_t setup_id,const string&descr)
	:f_id(0),f_setup_id(setup_id),f_description(descr){}
	HVconfig::HVconfig(const DataItem& item, const shared_ptr<IDataSource>src)
	:f_id(item.num_field<size_t>("id")),f_setup_id(item.num_field<size_t>("setup_id")),f_description(item["description"]),f_source(src){}
	HVconfig::~HVconfig(){}
	const size_t HVconfig::id() const{return f_id;}
	const size_t HVconfig::setup_id() const{return f_setup_id;}
	const string& HVconfig::description() const{return f_description;}
	RequestParameters HVconfig::params_to_insert() const{return {"'"+f_description+"'"};}
	RequestParameters HVconfig::params_to_delete() const{return {to_string(f_id)};}
	DataTableInterface<HVconfigEntry> HVconfig::CreateEntriesInterface() const{
		return DataTableInterface<HVconfigEntry>(f_source,{to_string(id())});
	}
	HVconfigTable::HVconfigTable(const shared_ptr<IDataSource>src,const size_t setup_id):DataTableInterface<JPetSetup::HVconfig>(src,{to_string(setup_id)}){}
	HVconfigTable::~HVconfigTable(){}
	
	
	HVChannel::HVChannel(const HVChannel& source)
	:f_id(source.f_id),f_highvoltage_id(source.f_highvoltage_id),
	f_idx(source.f_idx),f_status(source.f_status),
	f_givespositive_hv(source.f_givespositive_hv),
	f_minv(source.f_minv),f_maxv(source.f_maxv),f_maxi(source.f_maxi){}
	HVChannel& HVChannel::operator=(const HVChannel& source){
		f_id=source.f_id;
		f_highvoltage_id=source.f_highvoltage_id;
		f_idx=source.f_idx;
		f_status=source.f_status;
		f_givespositive_hv=source.f_givespositive_hv;
		f_minv=source.f_minv;
		f_maxv=source.f_maxv;
		f_maxi=source.f_maxi;
		return *this;
	}
	HVChannel::HVChannel(const DataItem& item, const shared_ptr< IDataSource > src)
	:f_id(item.num_field<size_t>("id")),f_highvoltage_id(item.num_field<size_t>("highvoltage_id")),
	f_idx(item.num_field<size_t>("channelidx")),f_status(item["status"]),
	f_givespositive_hv(item.flag("givespositivevoltage")),
	f_minv(item.num_field<double>("minv")),f_maxv(item.num_field<double>("maxv")),f_maxi(item.num_field<size_t>("maxi")){}
	HVChannel::~HVChannel(){}
	const size_t HVChannel::id() const{return f_id;}
	const size_t HVChannel::highvoltage_id() const{return f_highvoltage_id;}
	const size_t HVChannel::idx() const{return f_idx;}
	const string& HVChannel::status() const{return f_status;}
	const bool HVChannel::givespositivevoltage() const{return f_givespositive_hv;}
	const double HVChannel::minv() const{return f_minv;}
	const double HVChannel::maxv() const{return f_maxv;}
	const double HVChannel::maxi() const{return f_maxi;}
	
	HighVoltage::HighVoltage(const HighVoltage& source)
	:f_id(source.f_id),f_description(source.f_description),
	f_status(source.f_status),f_data_source(source.f_data_source){}
	HighVoltage& HighVoltage::operator=(const HighVoltage& source){
		f_id=source.f_id;
		f_description=source.f_description;
		f_status=source.f_status;
		f_data_source=source.f_data_source;
		return *this;
	}
	HighVoltage::HighVoltage(const DataItem& item, const shared_ptr< IDataSource > src)
	:f_id(item.num_field<size_t>("id")),f_description(item["description"]),
	f_status(item["status"]),f_data_source(src){}
	HighVoltage::~HighVoltage(){}
	const size_t HighVoltage::id() const{return f_id;}
	const string& HighVoltage::description() const{return f_description;}
	const string& HighVoltage::status() const{return f_status;}
	DataTableInterface<HVChannel> HighVoltage::CreateChannelsInterface() const{
		return DataTableInterface<HVChannel>(f_data_source,{to_string(id())});
	}
	
	HighVoltageTable::HighVoltageTable(std::shared_ptr<DataAccess::IDataSource>source)
	:DataTableInterface<HighVoltage>(source,{}){}
	HighVoltageTable::~HighVoltageTable(){}
	const HighVoltage HighVoltageTable::ByID(const size_t id) const{
		auto vec=Select([id](const DataItem&row){return row.num_field<size_t>("id")==id;});
		if(vec.size()>0)return vec[0];
		throw MathTemplates::Exception<HighVoltageTable>("HighVoltage not found id="+to_string(id));
	}
	
	HVPMConnection::HVPMConnection(const HVPMConnection& source)
	:f_id(source.f_id),f_hvchannel_id(source.f_hvchannel_id),
	f_photomultiplier_id(source.f_photomultiplier_id),f_setup_id(source.f_setup_id),
	f_slot_id(source.f_slot_id),f_side(source.f_side){}
	HVPMConnection& HVPMConnection::operator=(const HVPMConnection& source){
		f_id=source.f_id;
		f_hvchannel_id=source.f_hvchannel_id;
		f_photomultiplier_id=source.f_photomultiplier_id;
		f_setup_id=source.f_setup_id;
		f_slot_id=source.f_slot_id;
		f_side=source.f_side;
		return *this;
	}
	HVPMConnection::HVPMConnection(const DataItem& item, const shared_ptr< IDataSource >)
	:f_id(item.num_field<size_t>("id")),f_hvchannel_id(item.num_field<size_t>("hvchannel_id")),
	f_photomultiplier_id(item.num_field<size_t>("photomultiplier_id")),f_setup_id(item.num_field<size_t>("setup_id")),
	f_slot_id(item.num_field<size_t>("slot_id")),f_side(item.flag("isrightside")?side_right:side_left){}
	HVPMConnection::~HVPMConnection(){}
	const size_t HVPMConnection::id() const{return f_id;}
	const size_t HVPMConnection::hvchannel_id() const{return f_hvchannel_id;}
	const size_t HVPMConnection::photomultiplier_id() const{return f_photomultiplier_id;}
	const size_t HVPMConnection::setup_id() const{return f_setup_id;}
	const size_t HVPMConnection::slot_id() const{return f_slot_id;}
	const JPET_side HVPMConnection::side() const{return f_side;}

	
	HVPMConnections::HVPMConnections(const shared_ptr<IDataSource> source)
	:DataTableInterface< JPetSetup::HVPMConnection >(source,{}){}
	HVPMConnections::~HVPMConnections(){}
	const HVPMConnection HVPMConnections::ByID(const size_t id) const{
		auto vec=Select([id](const DataItem&row){return row.num_field<size_t>("id")==id;});
		if(vec.size()>0)return vec[0];
		throw MathTemplates::Exception<HVPMConnections>("HVPM connection not found id="+to_string(id));
	}
	const vector<HVPMConnection> HVPMConnections::ByHVChannelID(const size_t id) const{
		return Select([id](const DataItem&row){return row.num_field<size_t>("hvchannel_id")==id;});
	}
	const vector<HVPMConnection> HVPMConnections::ByPhotomultiplierID(const size_t id) const{
		return Select([id](const DataItem&row){return row.num_field<size_t>("photomultiplier_id")==id;});
	}
	const vector<HVPMConnection> HVPMConnections::BySetupID(const size_t id) const{
		return Select([id](const DataItem&row){return row.num_field<size_t>("setup_id")==id;});
	}
	const vector<HVPMConnection> HVPMConnections::BySlotID(const size_t id) const{
		return Select([id](const DataItem&row){return row.num_field<size_t>("slot_id")==id;});
	}
};