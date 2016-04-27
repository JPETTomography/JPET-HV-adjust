#ifndef ____TESTSOURCE_H__________
#	define ____TESTSOURCE_H__________
#include <DataGeneral/data_access.h>
class test_data_source:public DataAccess::IDataSource{
private:
	bool f_valid;
	std::map<DataAccess::operationtype,size_t> f_counters;
public:
	test_data_source(const bool valid);
	virtual ~test_data_source();
	virtual const bool Request(const DataAccess::RequestType request, const DataAccess::RequestParameters&params,std::vector<DataAccess::DataItem>&output)override;
	const size_t Count(const DataAccess::operationtype type)const;
};

#endif