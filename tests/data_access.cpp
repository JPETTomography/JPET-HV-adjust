// this file is distributed under 
// MIT license
#include <random>
#include <gtest/gtest.h>
#include <math_h/error.h>
#include <data_access.h>
using namespace std;
using namespace MathTemplates;
using namespace DataAccess;
class test_data_source:public IDataSource{
private:
	bool f_valid;
public:
	test_data_source(const bool valid):f_valid(valid){}
	virtual ~test_data_source(){}
	virtual const bool Request(const RequestType request, const RequestParameters&params,vector<DataItem>&output)override{
		if(!f_valid)return false;
		map<string,string> item;
		item["field1"]="value1";
		item["field2"]="value2";
		output.push_back(DataItem(item));
		item["field1"]="value3";
		item["field2"]="value4";
		output.push_back(DataItem(item));
		return true;
	}
};

TEST(DataSet,basetest){
	DataSet test(make_shared<test_data_source>(true),{.data=dummy,.operation=data_obtain},{},{.data=dummy,.operation=data_insert},{.data=dummy,.operation=data_remove});
	EXPECT_EQ(2,test.size());
}