#ifndef ____TESTSOURCE_H__________
#	define ____TESTSOURCE_H__________
#include <JPetData/data_access.h>
#include <HV/HVSetter.h>
class test_data_source:public DataAccess::IDataSource{
private:
	bool f_valid;
	std::map<DataAccess::operationtype,size_t> f_counters;
public:
	test_data_source(const bool valid);
	virtual ~test_data_source();
	virtual const bool Request(const DataAccess::RequestType request, const DataAccess::RequestParameters&params,std::vector<DataAccess::DataItem>&output)override;
	const size_t Count(const DataAccess::operationtype type)const;
	void ResetCounters();
};

class test_hv_setter:public HVAdjust::IHVSetter{
private:
	std::map<size_t,std::pair<size_t,double>> f_data;
public:
	test_hv_setter();
	virtual ~test_hv_setter();
	size_t GetCount(size_t channel_no)const;
	virtual double GetHV(size_t channel_no)const override;
	virtual double GetCurent(size_t channel_no)const override;
	virtual bool SetHV(size_t channel_no,double hv)override;
	
};

#endif