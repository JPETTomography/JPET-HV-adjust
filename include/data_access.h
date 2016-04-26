// this file is distributed under 
// MIT license
#ifndef ______DATA_ACCESS_H________
#	define ______DATA_ACCESS_H________
#include <vector>
#include <map>
#include <string>
#include <memory>
namespace DataAccess{
	enum RequestType{
		empty_request,
		insertcalibrationtype,
		insertcalibration_phm_ampl,
		connect_calibration_phm_ampl,
		getcalibrationtype,
		getcalibrationtypes,
		getcalibrations_phm_ampl,
		getconnectedcalibrations_phm_ampl
	};
	class DataItem{
	public:
		DataItem(const std::map<std::string,std::string>&row);
		virtual ~DataItem();
		const std::string&operator[](std::string&&name)const;
	private:
		DataItem(const DataItem&){}
		DataItem&operator=(const DataItem&){return *this;}
		
		std::map<std::string,std::string> f_data;
	};
	typedef std::vector<std::string> RequestParameters;
	class IDataSource{
	public:
		virtual ~IDataSource(){}
		virtual const bool Request(const RequestType request, const RequestParameters&params,std::vector<DataItem>&)=0;
	};
	class DataSet{
	public:
		DataSet(const std::shared_ptr<IDataSource> source,const RequestType getter,const RequestParameters&getter_params,const RequestType inserter=empty_request,const RequestType deleter=empty_request);
		virtual ~DataSet();
		
		typedef std::vector<DataItem>::const_iterator const_iterator;
		const_iterator begin()const;
		const_iterator cbegin()const;
		const_iterator end() const;
		const_iterator cend() const;
		const DataItem&operator[](size_t index)const;
		
		bool Insert(const RequestParameters&par);
		bool Delete(const RequestParameters&par);
	private:
		DataSet(const DataSet&){}
		DataSet&operator=(const DataSet&){return *this;}
		
		std::shared_ptr<IDataSource> f_source;
		RequestType f_getter,f_inserter,f_deleter;
		RequestParameters f_update_params;
		std::vector<DataItem> f_data;
		bool Update();
	};
};
#endif