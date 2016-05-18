// this file is distributed under 
// MIT license
#ifndef _______HV_____CONFIG______H___________
#	define _______HV_____CONFIG______H___________
#include <DataGeneral/data_access.h>
namespace JPetSetup{
	class HVconfigEntry{
	public:
		HVconfigEntry(const HVconfigEntry&source);
		HVconfigEntry&operator=(const HVconfigEntry&source);
		HVconfigEntry(const size_t hvpm,double value);
		virtual ~HVconfigEntry();
		const size_t id()const;
		const size_t HVPMConnection_id()const;
		const size_t hvconfig_id()const;
		const double HV()const;
	protected:
		friend class DataAccess::Factory<HVconfigEntry>;
		enum{type=DataAccess::data_hvconfigentry};
		HVconfigEntry(const DataAccess::DataItem&item,const std::shared_ptr<DataAccess::IDataSource>);
		DataAccess::RequestParameters params_to_insert()const; 
		DataAccess::RequestParameters params_to_delete()const;
	private:
		size_t f_id,f_hvpm_id,f_hvconfig_id;
		double f_hv_value;
	};
	class HVconfig{
	public:
		HVconfig(const HVconfig&source);
		HVconfig&operator=(const HVconfig&source);
		HVconfig(const std::string&descr);
		HVconfig(const std::string&&descr):HVconfig(descr){}
		virtual ~HVconfig();
		const size_t id()const;
		const std::string&description()const;
		DataAccess::Factory<HVconfigEntry> CreateEntriesFactory()const;
	protected:
		friend class DataAccess::Factory<HVconfig>;
		enum{type=DataAccess::data_hvconfig};
		HVconfig(const DataAccess::DataItem&item,const std::shared_ptr<DataAccess::IDataSource>);
		DataAccess::RequestParameters params_to_insert()const; 
		DataAccess::RequestParameters params_to_delete()const;
	private:
		size_t f_id;
		std::string f_description;
		std::shared_ptr<DataAccess::IDataSource> f_source;
	};
	class HVconfigTable:public DataAccess::Factory<HVconfig>{
	public:
		HVconfigTable(const std::shared_ptr< DataAccess::IDataSource > src);
		virtual ~HVconfigTable();
		const HVconfig Last()const;
	};
	
	
	
	class HVChannel{
	public:
		HVChannel(const HVChannel&source);
		HVChannel&operator=(const HVChannel&source);
		virtual ~HVChannel();
		const size_t id()const;
		const size_t highvoltage_id()const;
		const size_t idx()const;
		const std::string&status()const;
		const bool givespositivevoltage()const;
		const double minv()const;
		const double maxv()const;
		const double maxi()const;
	protected:
		friend class DataAccess::Factory<HVChannel>;
		enum{type=DataAccess::data_hvchannel};
		HVChannel(const DataAccess::DataItem&item,const std::shared_ptr<DataAccess::IDataSource>src);
		//DataAccess::RequestParameters params_to_insert()const; 
		//DataAccess::RequestParameters params_to_delete()const;
	private:
		size_t f_id,f_highvoltage_id,f_idx;
		std::string f_status;
		bool f_givespositive_hv;
		double f_minv,f_maxv,f_maxi;
	};
	class HighVoltage{
	public:
		HighVoltage(const HighVoltage&source);
		HighVoltage&operator=(const HighVoltage&source);
		virtual ~HighVoltage();
		const size_t id()const;
		const std::string&description()const;
		const std::string&status()const;
		DataAccess::Factory<HVChannel> CreateChannelsFactory()const;
	protected:
		friend class DataAccess::Factory<HighVoltage>;
		enum{type=DataAccess::data_highvoltage};
		HighVoltage(const DataAccess::DataItem&item,const std::shared_ptr<DataAccess::IDataSource>src);
		//DataAccess::RequestParameters params_to_insert()const; 
		//DataAccess::RequestParameters params_to_delete()const;
	private:
		size_t f_id;
		std::string f_description,f_status;
		std::shared_ptr<DataAccess::IDataSource> f_data_source;
	};
	class HighVoltageTable:public DataAccess::Factory<HighVoltage>{
	public:
		HighVoltageTable(std::shared_ptr<DataAccess::IDataSource>source);
		virtual ~HighVoltageTable();
		
	};
	
	enum JPET_side{side_left,side_right};
	class HVPMConnection{
	public:
		HVPMConnection(const HVPMConnection&source);
		HVPMConnection&operator=(const HVPMConnection&source);
		virtual ~HVPMConnection();
		const size_t id()const;
		const size_t hvchannel_id()const;
		const size_t photomultiplier_id()const;
		const size_t setup_id()const;
		const size_t slot_id()const;
		const JPET_side side()const;
	protected:
		friend class DataAccess::Factory<HVPMConnection>;
		enum{type=DataAccess::data_hvpmconnection};
		HVPMConnection(const DataAccess::DataItem&item,const std::shared_ptr<DataAccess::IDataSource>);
		//DataAccess::RequestParameters params_to_insert()const; 
		//DataAccess::RequestParameters params_to_delete()const;
	private:
		size_t f_id,f_hvchannel_id,f_photomultiplier_id,f_setup_id,f_slot_id;
		JPET_side f_side;
	};
	class HVPMConnections:public DataAccess::Factory<HVPMConnection>{
	public:
		HVPMConnections(const std::shared_ptr<DataAccess::IDataSource> source);
		virtual ~HVPMConnections();
		const std::vector<HVPMConnection> ByID(const size_t id)const;
		const std::vector<HVPMConnection> ByHVChannelID(const size_t id)const;
		const std::vector<HVPMConnection> ByPhotomultiplierID(const size_t id)const;
		const std::vector<HVPMConnection> BySetupID(const size_t id)const;
		const std::vector<HVPMConnection> BySlotID(const size_t id)const;
	};
	
};
#endif