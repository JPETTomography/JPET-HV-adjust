// this file is distributed under 
// MIT license
#ifndef ______PHOTOMULTIPLIER_H_________
#	define ______PHOTOMULTIPLIER_H_________
#include <DataGeneral/data_access.h>
namespace JPetData{
	class PhotomultiplierData{
	public:
		PhotomultiplierData(const PhotomultiplierData&source);
		virtual ~PhotomultiplierData();
		const size_t id()const;
		const size_t run_id()const;
		const size_t setup_id()const;
		const bool isActive()const;
		const std::string&status()const;
		const std::string&name()const;
		const double&max_HV()const;
	protected:
		friend class DataAccess::Factory<PhotomultiplierData>;
		enum{type=DataAccess::photomultiplierdata};
		PhotomultiplierData(const DataAccess::DataItem& row);
	private:
		size_t f_key,f_run_id,f_setup_id;
		bool f_active;
		std::string f_status;
		std::string f_name;
		double f_max_HV;
	};
	class PhotomultipliersData:public DataAccess::Factory<PhotomultiplierData>{
	public:
		PhotomultipliersData(const std::shared_ptr<DataAccess::IDataSource> src, const size_t run_id);
		virtual ~PhotomultipliersData();
		const size_t RunID()const;
	private:
		size_t f_run_id;
	};
};

#endif