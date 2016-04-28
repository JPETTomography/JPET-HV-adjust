// this file is distributed under 
// MIT license
#ifndef ______DATA_CALIBRATION_H________
#	define ______DATA_CALIBRATION_H________
#include <DataGeneral/data_access.h>
class TFormula;
namespace Calibration{
	class CalibrationType{
	public:
		CalibrationType(const std::string&n,const size_t count,const std::string&f);
		CalibrationType(const std::string&&n,const size_t count,const std::string&&f);
		CalibrationType(const CalibrationType&source);
		virtual ~CalibrationType();
		const size_t id()const;
		const size_t param_count()const;
		const std::string&name()const;
		const std::string&formula()const;
	protected:
		friend class DataAccess::Factory<CalibrationType>;
		enum{type=DataAccess::data_calibrationtype};
		CalibrationType(const DataAccess::DataItem&item,const std::shared_ptr<DataAccess::IDataSource>);
		DataAccess::RequestParameters params_to_insert()const; 
		DataAccess::RequestParameters params_to_delete()const;
	private:
		size_t m_id,m_count;
		std::string m_name,m_formula;
	};
	class CalibrationTypes:public DataAccess::Factory<CalibrationType>{
	public:
		CalibrationTypes(const std::shared_ptr<DataAccess::IDataSource> src);
		virtual ~CalibrationTypes();
	};
	
	typedef std::vector<size_t> id_set;
	typedef std::vector<double> parameter_set;
	typedef std::vector<std::string> field_set;
	class Calibration{
	public:
		virtual ~Calibration();
		const std::string&name()const;
		const std::string&formula()const;
		const std::vector<double>&params()const;
		const std::string&encoded_params()const;
		double operator()(const parameter_set&X)const;
		double operator()(const parameter_set&&X)const;
	protected:
		Calibration(const DataAccess::DataItem&row,const field_set&field_names);
		Calibration(const CalibrationType&type,const parameter_set&values);
		Calibration(const Calibration&source);
		Calibration();
	private:
		Calibration(const std::string& n,const size_t count,const std::string& f,const std::string&params);
		std::string m_name,m_formula,m_encoded_params;
		parameter_set m_params;
		TFormula*m_tformula;
		double*buf;
		void init_formula();
		void deinit_formula();
	};
	class CalibrationForEquipment:public Calibration{
	public:
		CalibrationForEquipment(const id_set&eq_id,const DataAccess::DataItem&row,const field_set&field_names);
		CalibrationForEquipment(const id_set&eq_id,const CalibrationType&type,const parameter_set&values);
		CalibrationForEquipment(const CalibrationForEquipment&source);
		virtual ~CalibrationForEquipment();
		const size_t id()const;
	protected:
		const size_t type_id()const;
		const id_set&equipment_ids()const;
	private:
		size_t m_cal_id,m_type_id;
		id_set m_eq_id;
	};
	class CalibrationForEquipmentAndRun:public Calibration{
	public:
		CalibrationForEquipmentAndRun(const size_t cal_id,size_t run_id);
		CalibrationForEquipmentAndRun(const id_set&eq_id,size_t run_id,const DataAccess::DataItem&row,const field_set&field_names);
		CalibrationForEquipmentAndRun(const CalibrationForEquipmentAndRun&source);
		virtual ~CalibrationForEquipmentAndRun();
		const size_t id()const;
		const size_t run_id()const;
	protected:
		const id_set&equipment_ids()const;
	private:
		size_t m_cal_id,m_run_id;
		id_set m_eq_id;
	};
};
#endif