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
		data_calibrationtype,data_calibration_phmampl,data_calibration_phmampl_connected,
		data_slot,data_layer,data_frame,data_setup,data_run,
		data_photomultiplier,
		data_highvoltage,data_hvchannel,data_hvpmconnection,data_hvconfigentry,data_hvconfig,
		
		data_dummy
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
		inline const std::string&operator[](const std::string&&name)const{return operator[](name);}
		const bool flag(const std::string&name)const;
		inline const bool flag(const std::string&&name)const{return flag(name);}
		template<typename numt>
		const numt num_field(const std::string&name)const{
			numt res=0;
			std::istringstream(operator[](name))>>res;
			return res;
		}
		template<typename numt>
		const numt num_field(const std::string&&name)const{return num_field<numt>(name);}
		template<typename numt>
		const bool field_eq(const std::string&name,const numt value)const{
			return num_field<numt>(name)==value;
		}
		template<typename numt>
		const bool field_eq(const std::string&&name,const numt value)const{
			return field_eq<numt>(name,value);
		}
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
		std::shared_ptr<DataSet> m_data;
		std::shared_ptr<IDataSource> f_src;
	protected:
		virtual RequestParameters additional_add_parameters(){return RequestParameters();}
		virtual RequestParameters additional_delete_parameters(){return RequestParameters();}
		template<typename numt>
		const std::vector<DataItemRepresenter> GetFieldEq(const std::string&name,const numt v)const{
			std::vector<DataItemRepresenter> res;
			for(const DataItem&item: *m_data){
				if(item.field_eq<numt>(name,v))
					res.push_back(DataItemRepresenter(item,f_src));
			}
			return res;
		}
		template<typename numt>
		const std::vector<DataItemRepresenter> GetFieldEq(const std::string&&name,const numt v)const{return GetFieldEq<numt>(name,v);}
	public:
		Factory(const std::shared_ptr<IDataSource> src,const RequestParameters&params){
			m_data=std::make_shared<DataSet>(src,datatype(DataItemRepresenter::type),params);
		}
		Factory(const std::shared_ptr<IDataSource> src,const RequestParameters&&params):Factory(src,params){}
		Factory(const Factory&source):m_data(source.m_data),f_src(source.f_src){}
		virtual ~Factory(){}
		const size_t size()const{return m_data->size();}
		const std::vector<DataItemRepresenter> GetList()const{
			std::vector<DataItemRepresenter> res;
			for(const auto&item: *m_data)
				res.push_back(DataItemRepresenter(item,f_src));
			return res;
		}
		bool Add(const DataItemRepresenter&&item){
			auto params=item.params_to_insert();
			for(const auto&item:additional_add_parameters())
				params.push_back(item);
			return m_data->Insert(params);
		}
		bool Delete(const DataItemRepresenter&item){
			auto params=item.params_to_delete();
			for(const auto&item:additional_delete_parameters())
				params.push_back(item);
			return m_data->Delete(params);
		}
	};
};
#endif