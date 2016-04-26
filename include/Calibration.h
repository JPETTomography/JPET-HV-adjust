// this file is distributed under 
// MIT license
#ifndef ______DATA_CALIBRATION_H________
#	define ______DATA_CALIBRATION_H________
#include <data_access.h>
namespace Calibration{
	class CalibrationType{
		friend class DataAccess::Factory<CalibrationType>;
	public:
		CalibrationType(const std::string&&n,const size_t count,const std::string&&f);
		virtual ~CalibrationType();
		const size_t id()const;
		const size_t param_count()const;
		const std::string&name()const;
		const std::string&formula()const;
	protected:
		enum{type=DataAccess::calibrationtype};
		static const std::string keyfield();
		CalibrationType();
		CalibrationType(const DataAccess::DataItem&item);
		const DataAccess::RequestParameters params_to_insert()const; 
	private:
		size_t m_id,m_count;
		std::string m_name,m_formula;
	};
	
};
#endif