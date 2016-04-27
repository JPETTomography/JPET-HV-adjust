// this file is distributed under 
// MIT license
#ifndef ________PHMGAIN_H_______
#	define ________PHMGAIN_H_______
#include <data_access.h>
#include <CalibrationGeneral.h>
namespace Calibration{
	class PhotomultiplierGain:public CalibrationForEquipment{
	public:
		PhotomultiplierGain(const size_t phm_id,const CalibrationType&type,const parameter_set&parameters);
		virtual ~PhotomultiplierGain();
		const size_t phm_id()const;
	protected:
		friend class DataAccess::Factory<PhotomultiplierGain>;
		PhotomultiplierGain(const DataAccess::DataItem& row);
		PhotomultiplierGain();
		enum{type=DataAccess::calibration_phmampl};
		static const std::string keyfield();
		const DataAccess::RequestParameters params_to_insert()const; 
		const DataAccess::RequestParameters params_to_delete()const;
	};
	class PhotomultiplierGains:public DataAccess::Factory<PhotomultiplierGain>{
	public:
		PhotomultiplierGains(const std::shared_ptr<DataAccess::IDataSource> src, const size_t phm_id);
		virtual ~PhotomultiplierGains();
		const size_t PhotomultiplierID()const;
	private:
		size_t f_phm_id;
	};
	
	class PhotomultiplierGain4Run:public CalibrationForEquipmentAndRun{
	public:
		PhotomultiplierGain4Run(const size_t cal_id, size_t run_id);
		virtual ~PhotomultiplierGain4Run();
		const size_t connection_id()const;
		const size_t phm_id()const;
	protected:
		friend class DataAccess::Factory<PhotomultiplierGain4Run>;
		PhotomultiplierGain4Run(const DataAccess::DataItem& row);
		PhotomultiplierGain4Run();
		enum{type=DataAccess::calibration_phmampl_connected};
		static const std::string keyfield();
		const DataAccess::RequestParameters params_to_insert()const;
		const DataAccess::RequestParameters params_to_delete()const;
	private:
		size_t f_key;
	};
	class PhotomultiplierGains4Run:public DataAccess::Factory<PhotomultiplierGain4Run>{
	public:
		PhotomultiplierGains4Run(const std::shared_ptr<DataAccess::IDataSource> src, const size_t run_id);
		virtual ~PhotomultiplierGains4Run();
		const size_t RunID()const;
	private:
		size_t f_run_id;
	};
};
#endif