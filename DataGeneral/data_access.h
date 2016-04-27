// this file is distributed under 
// MIT license
#ifndef ______DATA_ACCESS_H________
#	define ______DATA_ACCESS_H________
#include <vector>
#include <list>
#include <map>
#include <string>
#include <memory>
#include <sstream>
namespace DataAccess{
	enum datatype{
		dummy,
		calibrationtype,
		calibration_phmampl,
		calibration_phmampl_connected,
		photomultiplierdata
	};
	enum operationtype{data_obtain,data_insert,data_remove};
	struct RequestType{
		datatype data;
		operationtype operation;
	};
	class DataItem{
	public:
		DataItem(const std::map<std::string,std::string>&row);
		DataItem(const DataItem&);
		virtual ~DataItem();
		const std::string&operator[](const std::string&name)const;
		const std::string&operator[](const std::string&&name)const;
		template<typename numt>
		const numt num_field(const std::string&name)const{
			numt res=0;
			std::istringstream(operator[](name))>>res;
			return res;
		}
		template<typename numt>
		const numt num_field(const std::string&&name)const{return num_field<numt>(name);}
	private:
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
		DataSet(const std::shared_ptr<IDataSource> source,const datatype type,const RequestParameters&getter_params);
		virtual ~DataSet();
		
		const size_t size()const;
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
		datatype f_type;
		RequestParameters f_update_params;
		std::vector<DataItem> f_data;
		bool Update();
	};
	template<class DataItemRepresenter>
	class Factory{
	private:
		DataAccess::DataSet m_data;
	public:
		Factory(const std::shared_ptr<IDataSource> src,const RequestParameters&&params)
		:m_data(src,datatype(DataItemRepresenter::type),params){}
		virtual ~Factory(){}
		const size_t size()const{return m_data.size();}
		const DataItemRepresenter Get(const size_t id)const{
			for(const auto&item:m_data){
				std::istringstream str(item[DataItemRepresenter::keyfield()]);
				size_t ID;str>>ID;
				if(id==ID)return DataItemRepresenter(item);
			}
			return DataItemRepresenter();
		}
		const std::vector<DataItemRepresenter> GetList()const{
			std::vector<DataItemRepresenter> res;
			for(const auto&item:m_data)res.push_back(DataItemRepresenter(item));
			return res;
		}
		bool Add(const DataItemRepresenter&&item){
			auto params=item.params_to_insert();
			if(params.size()>0)return m_data.Insert(params);
			else return false;
		}
		bool Delete(const DataItemRepresenter&item){
			auto params=item.params_to_delete();
			if(params.size()>0)return m_data.Delete(params);
			else return false;
		}
		bool Delete(const DataItemRepresenter&&item){return Delete(item);}
	};
};
#endif