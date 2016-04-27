// this file is distributed under 
// MIT license
#include <math_h/error.h>
#include <iostream>
#include <sstream>
#include <TFormula.h>
#include <CalibrationGeneral.h>
using namespace std;
using namespace MathTemplates;
using namespace DataAccess;
namespace Calibration {
	CalibrationType::CalibrationType()
	:m_id(0),m_count(0),m_name(""),m_formula(""){}
	const string CalibrationType::keyfield(){return "type_id";}
	CalibrationType::CalibrationType(const DataItem& item)
	:m_id(item("type_id")),m_count(item("param_count")),m_name(item["name"]),m_formula(item["formula"]){}
	CalibrationType::CalibrationType(const string&& n, const size_t count, const string&& f)
	:m_id(0),m_count(0),m_name(""),m_formula(""){}
	const size_t CalibrationType::id() const{return m_id;}
	const string& CalibrationType::name() const{return m_name;}
	const size_t CalibrationType::param_count()const{return m_count;}
	const string& CalibrationType::formula() const{return m_formula;}
	CalibrationType::~CalibrationType(){}
	const RequestParameters CalibrationType::params_to_insert()const{
		if(0==m_id)return {m_name,to_string(m_count),m_formula};
		else throw MathTemplates::Exception<CalibrationType>("reinserting!");
	}
	CalibrationTypes::CalibrationTypes(const shared_ptr<IDataSource> src):Factory<CalibrationType>(src,{}){}
	CalibrationTypes::~CalibrationTypes(){}
	
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
	:Calibration(row[field_names[0]],row(field_names[1]),row[field_names[2]],row[field_names[3]]){}
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
		buf=new double[m_params.size()];
		for(size_t i=0;i<m_params.size();i++)buf[i]=m_params[i];
		m_tformula= new TFormula(formula().c_str(),formula().c_str());
	}
	void Calibration::deinit_formula(){
		delete[] buf;
		delete m_tformula;
	}
	double Calibration::operator()(const parameter_set& X) const{
		double x[X.size()];
		for(size_t i=0,n=X.size();i<n;i++)x[i]=X[i];
		return m_tformula->EvalPar(x,buf);
	}
	double Calibration::operator()(const parameter_set&& X) const{return operator()(X);}
	
	CalibrationForEquipment::CalibrationForEquipment(const id_set&eq_id,const DataItem&row,const field_set&field_names)
	:Calibration(row, field_names){for(const auto&item:eq_id)m_eq_id.push_back(item);}
	CalibrationForEquipment::CalibrationForEquipment(const id_set&eq_id,const CalibrationType&type,const parameter_set&values)
	:Calibration(type, values){for(const auto&item:eq_id)m_eq_id.push_back(item);}
	CalibrationForEquipment::CalibrationForEquipment(const CalibrationForEquipment&source)
	:Calibration(source){for(const auto&item:source.m_eq_id)m_eq_id.push_back(item);}
	CalibrationForEquipment::~CalibrationForEquipment(){}
	const size_t CalibrationForEquipment::calibration_id()const{return m_cal_id;}
	const id_set& CalibrationForEquipment::equipment_ids()const{return m_eq_id;}
	const size_t CalibrationForEquipment::type_id()const{return m_type_id;}
	
	CalibrationForEquipmentAndRun::CalibrationForEquipmentAndRun(const id_set&eq_id,size_t run_id,const DataItem&row,const field_set&field_names)
	:Calibration(row, field_names),m_run_id(run_id){for(const auto&item:eq_id)m_eq_id.push_back(item);}
	CalibrationForEquipmentAndRun::CalibrationForEquipmentAndRun(const CalibrationForEquipmentAndRun& source)
	:Calibration(source),m_run_id(source.m_run_id){for(const auto&item:source.m_eq_id)m_eq_id.push_back(item);}
	CalibrationForEquipmentAndRun::~CalibrationForEquipmentAndRun(){}
	const id_set&CalibrationForEquipmentAndRun::equipment_ids()const{return m_eq_id;}
	const size_t CalibrationForEquipmentAndRun::run_id()const{return m_run_id;}
	
};