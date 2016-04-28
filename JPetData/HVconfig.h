// this file is distributed under 
// MIT license
#ifndef _______HV_____CONFIG______H___________
#	define _______HV_____CONFIG______H___________
#include <DataGeneral/data_access.h>
namespace JPetSetup{
	class HVconfigEntry{
	public:
		HVconfigEntry(const HVconfigEntry&source);
		HVconfigEntry(const size_t hvpm,const size_t hvconfig,double value);
		virtual ~HVconfigEntry();
		const size_t HVPMConnection_id()const;
		const size_t hvconfig_id()const;
		const double HV()const;
	protected:
		friend class DataAccess::Factory<HVconfigEntry>;
		enum{type=DataAccess::data_hvconfig};
		HVconfigEntry(const DataAccess::DataItem&item,const std::shared_ptr<DataAccess::IDataSource>);
		DataAccess::RequestParameters params_to_insert()const; 
		//DataAccess::RequestParameters params_to_delete()const;
	private:
		size_t f_hvpm_id,f_hvconfig_id;
		double f_hv_value;
	};
	class HVconfig{
	public:
		HVconfig(const HVconfig&source);
		HVconfig(std::string&descr);
		virtual ~HVconfig();
		const size_t id()const;
		const std::string&description()const;
		DataAccess::Factory<HVconfigEntry> CreateEntriesFactory()const;
	protected:
		friend class DataAccess::Factory<HVconfig>;
		enum{type=DataAccess::data_hvconfig};
		HVconfig(const DataAccess::DataItem&item,const std::shared_ptr<DataAccess::IDataSource>);
		DataAccess::RequestParameters params_to_insert()const; 
		//DataAccess::RequestParameters params_to_delete()const;
	private:
		size_t f_id;
		std::string f_description;
		std::shared_ptr<DataAccess::IDataSource> f_source;
	};
	class HVconfigTable:public DataAccess::Factory<HVconfig>{
	public:
		HVconfigTable(const std::shared_ptr< DataAccess::IDataSource > src);
		virtual ~HVconfigTable();
	};
};
#endif