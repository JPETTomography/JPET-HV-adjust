// this file is distributed under 
// MIT license
#include <math_h/error.h>
#include <Calibration.h>
using namespace std;
namespace Calibration {
	CalibrationType::CalibrationType()
	:m_id(0),m_count(0),m_name(""),m_formula(""){}
	const string CalibrationType::keyfield(){return "type_id";}
	CalibrationType::CalibrationType(const DataAccess::DataItem& item)
	:m_id(0),m_count(0),m_name(item["name"]),m_formula(item["formula"]){
		istringstream str1(item["type_id"]),str2(item["param_count"]);
		str1>>m_id;
		str2>>m_count;
	}
	CalibrationType::CalibrationType(const string&& n, const size_t count, const string&& f)
	:m_id(0),m_count(0),m_name(""),m_formula(""){}
	const size_t CalibrationType::id() const{return m_id;}
	const string& CalibrationType::name() const{return m_name;}
	const size_t CalibrationType::param_count()const{return m_count;}
	const string& CalibrationType::formula() const{return m_formula;}
	CalibrationType::~CalibrationType(){}
	const DataAccess::RequestParameters CalibrationType::params_to_insert()const{
		if(0==m_id)return {m_name,to_string(m_count),m_formula};
		else throw MathTemplates::Exception<CalibrationType>("reinserting!");
	}


};