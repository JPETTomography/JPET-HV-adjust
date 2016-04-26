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
	map<operationtype,size_t> f_counters;
public:
	test_data_source(const bool valid):f_valid(valid){
		f_counters[data_obtain]=0;
		f_counters[data_insert]=0;
		f_counters[data_remove]=0;
	}
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
		f_counters[request.operation]++;
		return true;
	}
	const size_t Count(const operationtype type)const{return f_counters.find(type)->second;}
};

TEST(DataSet,basetest){
	auto src=make_shared<test_data_source>(true);
	DataSet test(src,{.data=dummy,.operation=data_obtain},{},{.data=dummy,.operation=data_insert},{.data=dummy,.operation=data_remove});
	EXPECT_EQ(2,test.size());
	EXPECT_EQ(1,src->Count(DataAccess::data_obtain));
	EXPECT_EQ(0,src->Count(DataAccess::data_insert));
	EXPECT_EQ(0,src->Count(DataAccess::data_remove));
	EXPECT_EQ("value1",test[0]["field1"]);
	EXPECT_EQ("value2",test[0]["field2"]);
	EXPECT_EQ("value3",test[1]["field1"]);
	EXPECT_EQ("value4",test[1]["field2"]);
	EXPECT_THROW(test[0]["field3"],Exception<DataItem>);
	EXPECT_THROW(test[1]["field3"],Exception<DataItem>);
	EXPECT_THROW(test[2]["field1"],Exception<DataSet>);
	test.Insert({});
	EXPECT_EQ(2,src->Count(DataAccess::data_obtain));
	EXPECT_EQ(1,src->Count(DataAccess::data_insert));
	EXPECT_EQ(0,src->Count(DataAccess::data_remove));
	test.Delete({});
	EXPECT_EQ(3,src->Count(DataAccess::data_obtain));
	EXPECT_EQ(1,src->Count(DataAccess::data_insert));
	EXPECT_EQ(1,src->Count(DataAccess::data_remove));
}