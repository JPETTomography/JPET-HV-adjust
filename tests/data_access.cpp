// this file is distributed under 
// MIT license
#include <random>
#include <gtest/gtest.h>
#include <math_h/error.h>
#include <DataGeneral/data_access.h>
#include "test_data_source.h"
using namespace std;
using namespace MathTemplates;
using namespace DataAccess;
TEST(DataSet,basetest){
	auto src=make_shared<test_data_source>(true);
	DataSet test(src,data_dummy,{});
	EXPECT_EQ(2,test.size());
	EXPECT_EQ(1,src->Count(DataAccess::data_obtain));
	EXPECT_EQ(0,src->Count(DataAccess::data_insert));
	EXPECT_EQ(0,src->Count(DataAccess::data_remove));
	EXPECT_EQ("value1",test[0]["field1"]);
	EXPECT_EQ("value2",test[0]["field2"]);
	EXPECT_EQ("value3",test[1]["field1"]);
	EXPECT_EQ("value4",test[1]["field2"]);
	EXPECT_EQ("",test[0]["field3"]);
	EXPECT_EQ("",test[1]["field3"]);
	EXPECT_THROW(test[2]["field1"],Exception<DataSet>);
	EXPECT_THROW(test[2]["field3"],Exception<DataSet>);
	test.Insert({});
	EXPECT_EQ(2,src->Count(DataAccess::data_obtain));
	EXPECT_EQ(1,src->Count(DataAccess::data_insert));
	EXPECT_EQ(0,src->Count(DataAccess::data_remove));
	test.Delete({});
	EXPECT_EQ(3,src->Count(DataAccess::data_obtain));
	EXPECT_EQ(1,src->Count(DataAccess::data_insert));
	EXPECT_EQ(1,src->Count(DataAccess::data_remove));
}