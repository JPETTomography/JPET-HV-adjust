// this file is distributed under 
// MIT license
#include <sstream>
#include <math_h/error.h>
#include <data_access.h>
using namespace std;
using namespace MathTemplates;
namespace DataAccess{
	DataItem::DataItem(const map<string,string>&row){
		for(const auto item:row)
			f_data[item.first]=item.second;
	}
	DataItem::DataItem(const DataItem&source){
		for(const auto item:source.f_data)
			f_data[item.first]=item.second;
	}
	DataItem::~DataItem(){}
	const string& DataItem::operator[](const string&name)const{
		auto found=f_data.find(name);
		if(found!=f_data.end())
			return found->second;
		else
			throw Exception<DataItem>("DataItem: field not found");
	}
	const string& DataItem::operator[](const string&&name) const{return operator[](name);}
	const size_t DataItem::operator()(const string&name) const{
		size_t res=0;
		istringstream(operator[](name))>>res;
		return res;
	}
	const size_t DataItem::operator()(const string&&name) const{return operator()(name);}
	DataSet::DataSet(const shared_ptr<IDataSource> source, const datatype type, const RequestParameters& getter_params)
		:f_source(source),f_type(type){
			for(const auto&item:getter_params)
				f_update_params.push_back(item);
			Update();
	}
	DataSet::~DataSet(){}
	bool DataSet::Update(){
		f_data.clear();
		return f_source->Request({.data=f_type,.operation=data_obtain},f_update_params,f_data);
	}
	bool DataSet::Insert(const RequestParameters& par){
		vector<DataItem> tmp;
		return f_source->Request({.data=f_type,.operation=data_insert},par,tmp)&&Update();
	}
	bool DataSet::Delete(const RequestParameters& par){
		vector<DataItem> tmp;
		return f_source->Request({.data=f_type,.operation=data_remove},par,tmp)&&Update();
	}
	const size_t DataSet::size() const{return f_data.size();}
	DataSet::const_iterator DataSet::begin() const{return f_data.begin();}
	DataSet::const_iterator DataSet::cbegin() const{return f_data.cbegin();}
	DataSet::const_iterator DataSet::end() const{return f_data.end();}
	DataSet::const_iterator DataSet::cend() const{return f_data.cend();}
	const DataItem& DataSet::operator[](size_t index) const{
		if(index>=f_data.size())
			throw Exception<DataSet>("DataSet range check error");
		return f_data[index];
	}
};