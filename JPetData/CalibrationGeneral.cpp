// this file is distributed under 
// MIT license
#include <iostream>
#include <iterator>
#include <sstream>
#include <math_h/error.h>
//#include <TFormula.h>
#include <JPetData/CalibrationGeneral.h>
using namespace std;
using namespace MathTemplates;
using namespace DataAccess;
namespace Calibration {
	CalibrationType::CalibrationType(const DataItem& item,const std::shared_ptr<DataAccess::IDataSource>)
	:m_id(item.num_field<size_t>("type_id")),m_count(item.num_field<size_t>("param_count")),m_name(item["name"]),m_formula(item["formula"]){}
	CalibrationType::CalibrationType(const string& n, const size_t count, const string& f)
	:m_id(0),m_count(count),m_name(n),m_formula(f){}
	CalibrationType::CalibrationType(const string&& n, const size_t count, const string&& f)
	:m_id(0),m_count(count),m_name(n),m_formula(f){}
	CalibrationType::CalibrationType(const CalibrationType& source)
	:m_id(source.m_id),m_count(source.m_count),m_name(source.m_name),m_formula(source.m_formula){}
	const size_t CalibrationType::id() const{return m_id;}
	const string& CalibrationType::name() const{return m_name;}
	const size_t CalibrationType::param_count()const{return m_count;}
	const string& CalibrationType::formula() const{return m_formula;}
	CalibrationType::~CalibrationType(){}
	RequestParameters CalibrationType::params_to_insert()const{
		return {m_name,to_string(m_count),m_formula};
	}
	RequestParameters CalibrationType::params_to_delete() const{
		return {to_string(m_id)};
	}

	CalibrationTypes::CalibrationTypes(const shared_ptr<IDataSource> src):Factory<CalibrationType>(src,{}){}
	CalibrationTypes::~CalibrationTypes(){}
	
	Calibration::Calibration():m_name(""),m_formula(""),m_encoded_params(""){}
	Calibration::Calibration(const string&n,const size_t count,const string& f,const string&params)
	:m_name(n),m_formula(f),m_encoded_params(params){
		stringstream ss(params);
		vector<string> tokens;
		copy(istream_iterator<string>(ss),
		     istream_iterator<string>(),
		     back_inserter(tokens));
		for(const string&token:tokens)
			if(m_params.size()<count)
				m_params.push_back(stod(token));
			if(m_params.size()<count)
				throw Exception<Calibration>("The number of decoded parameters is bad");
			init_formula();
	}
	Calibration::Calibration(const DataItem&row,const vector<string>&field_names)
	:Calibration(row[field_names[1]],row.num_field<size_t>(field_names[2]),row[field_names[3]],row[field_names[4]]){}
	Calibration::Calibration(const CalibrationType&type,const parameter_set&values):m_name(type.name()),m_formula(type.formula()){
		if(0==type.id())throw Exception<Calibration>("attempt to create calibration of type that is not present in database");
		if(values.size()!=type.param_count())
			throw Exception<Calibration>("parameters count is not good for selected calibration type");
		m_encoded_params="";
		for(double p:values){
			m_params.push_back(p);
			m_encoded_params+=to_string(p)+" ";
		}
		init_formula();
	}
	Calibration::Calibration(const Calibration& source)
	:m_name(source.m_name),m_formula(source.m_formula),m_encoded_params(source.m_encoded_params){
		for(double p:source.m_params)m_params.push_back(p);
		init_formula();
	}
	Calibration::~Calibration(){deinit_formula();}
	const string& Calibration::name() const{return m_name;}
	const string& Calibration::formula() const{return m_formula;}
	const vector<double>&Calibration::params() const{return m_params;}
	const string& Calibration::encoded_params() const{return m_encoded_params;}
	void Calibration::init_formula(){
		//buf=new double[m_params.size()];
		//for(size_t i=0;i<m_params.size();i++)buf[i]=m_params[i];
		//m_tformula= new TFormula(formula().c_str(),formula().c_str());
	}
	void Calibration::deinit_formula(){
		//delete[] buf;
		//delete m_tformula;
	}
	double Calibration::operator()(const parameter_set& X) const{
		//double x[X.size()];
		//for(size_t i=0,n=X.size();i<n;i++)x[i]=X[i];
		//return m_tformula->EvalPar(x,buf);
		return 0.0;
	}
	double Calibration::operator()(const parameter_set&& X) const{return operator()(X);}
	
	CalibrationForEquipment::CalibrationForEquipment(const id_set&eq_id,const DataItem&row,const field_set&field_names)
	:Calibration(row, field_names),m_cal_id(row.num_field<size_t>(field_names[0])),m_type_id(row.num_field<size_t>(field_names[5])){for(const auto&item:eq_id)m_eq_id.push_back(item);}
	CalibrationForEquipment::CalibrationForEquipment(const id_set&eq_id,const CalibrationType&type,const parameter_set&values)
	:Calibration(type, values),m_cal_id(0),m_type_id(type.id()){for(const auto&item:eq_id)m_eq_id.push_back(item);}
	CalibrationForEquipment::CalibrationForEquipment(const CalibrationForEquipment&source)
	:Calibration(source),m_cal_id(source.m_cal_id),m_type_id(source.m_type_id){for(const auto&item:source.m_eq_id)m_eq_id.push_back(item);}
	CalibrationForEquipment::~CalibrationForEquipment(){}
	const size_t CalibrationForEquipment::id()const{return m_cal_id;}
	const size_t CalibrationForEquipment::type_id()const{return m_type_id;}
	const id_set& CalibrationForEquipment::equipment_ids()const{return m_eq_id;}
	
	CalibrationForEquipmentAndRun::CalibrationForEquipmentAndRun(const id_set&eq_id,size_t run_id,const DataItem&row,const field_set&field_names)
	:Calibration(row, field_names),m_cal_id(row.num_field<size_t>(field_names[0])),m_run_id(run_id){for(const auto&item:eq_id)m_eq_id.push_back(item);}
	CalibrationForEquipmentAndRun::CalibrationForEquipmentAndRun(const size_t cal_id, size_t run_id)
	:Calibration(),m_cal_id(cal_id),m_run_id(run_id){}
	CalibrationForEquipmentAndRun::CalibrationForEquipmentAndRun(const CalibrationForEquipmentAndRun& source)
	:Calibration(source),m_cal_id(source.m_cal_id),m_run_id(source.m_run_id){for(const auto&item:source.m_eq_id)m_eq_id.push_back(item);}
	CalibrationForEquipmentAndRun::~CalibrationForEquipmentAndRun(){}
	const size_t CalibrationForEquipmentAndRun::id() const{return m_cal_id;}
	const size_t CalibrationForEquipmentAndRun::run_id()const{return m_run_id;}
	const id_set&CalibrationForEquipmentAndRun::equipment_ids()const{return m_eq_id;}
	
};