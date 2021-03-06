#ifndef ____TESTSOURCE_H__________
#	define ____TESTSOURCE_H__________
#include <JPetData/data_access.h>
#include <JPetData/HVSetter.h>
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
	std::map<size_t,std::pair<bool,double>> f_data;
	size_t f_update_counter;
public:
	test_hv_setter();
	virtual ~test_hv_setter();
	virtual void UpdateRequest()override;
	virtual bool IsOn(size_t channel_no)const override;
	virtual double GetHV(size_t channel_no)const override;
	virtual double GetHVMon(size_t channel_no)const override;
	virtual void turnOn(size_t channel_no) override;
	virtual void turnOff(size_t channel_no) override;
	virtual void SetHV(size_t channel_no,double hv) override;
	const size_t UpdatesCount()const;
};

#endif