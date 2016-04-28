// this file is distributed under 
// MIT license
#include <JPetData/Detectors.h>
using namespace std;
using namespace DataAccess;
namespace JPetSetup {
	Photomultiplier::Photomultiplier(const Photomultiplier& source)
	:f_id(source.f_id),f_name(source.f_name),f_status(source.f_status),
	f_description(source.f_description),f_max_hv(source.f_max_hv){}
	Photomultiplier::Photomultiplier(const DataItem&item,const shared_ptr<IDataSource>)
	:f_id(item.num_field<size_t>("id")),f_name(item["name"]),f_status(item["status"]),
	f_description(item["description"]),f_max_hv(item.num_field<double>("maxhv")){}
	Photomultiplier::~Photomultiplier(){}
	const size_t Photomultiplier::id() const{return f_id;}
	const string& Photomultiplier::name() const{return f_name;}
	const string& Photomultiplier::description() const{return f_description;}
	const string& Photomultiplier::status() const{return f_status;}
	const double Photomultiplier::max_hv() const{return f_max_hv;}
	Photomultipliers::Photomultipliers(const shared_ptr<IDataSource> src)
	:Factory< JPetSetup::Photomultiplier >(src, {}){}
	Photomultipliers::~Photomultipliers(){}
	const Photomultiplier Photomultipliers::ByID(const size_t id){
		auto vec=GetFieldEq("id",id);
		if(vec.size()>0)return vec[0];
		throw;
	}
};