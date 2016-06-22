// this file is distributed under 
// MIT license
#include <random>
#include <gtest/gtest.h>
#include <math_h/error.h>
#include <JPetData/CalibrationGeneral.h>
#include <JPetData/Calibration.h>
#include "test_data_source.h"
using namespace std;
using namespace MathTemplates;
using namespace DataAccess;
using namespace Calibration;
TEST(CalibrationType,basetest){
	CalibrationTypes typetable(make_shared<test_data_source>(true));
	EXPECT_EQ(2,typetable.DataRowsCount());
	EXPECT_EQ(2,typetable.SelectAll().size());
	function<bool(const DataItem&)> condition;
	for(size_t id=1;id<=typetable.DataRowsCount();id++){
		condition=[id](const DataItem&row){return row.num_field<size_t>("type_id")==id;};
		EXPECT_EQ(typetable.Select(condition).size(),1);
	}
	condition=[](const DataItem&row){return row.num_field<size_t>("type_id")==0;};
	EXPECT_EQ(typetable.Select(condition).size(),0);
	for(size_t i=3;i<7;i++){
		condition=[i](const DataItem&row){return row.num_field<size_t>("type_id")==i;};
		EXPECT_EQ(typetable.Select(condition).size(),0);
	}
}
TEST(CalibrationType,inserting){
	auto src=make_shared<test_data_source>(true);
	CalibrationTypes typetable(src);
	EXPECT_EQ(2,typetable.DataRowsCount());
	EXPECT_EQ(true,typetable.Add(CalibrationType("new_element",1,"[0]+x")));
	EXPECT_EQ(1,src->Count(DataAccess::data_insert));
}
TEST(CalibrationType,deleting){
	auto src=make_shared<test_data_source>(true);
	CalibrationTypes typetable(src);
	EXPECT_EQ(2,typetable.DataRowsCount());
	EXPECT_EQ(2,typetable.SelectAll().size());
	EXPECT_EQ(1,src->Count(DataAccess::data_obtain));
	EXPECT_EQ(true,typetable.Delete(const_cast<CalibrationType&&>(typetable.SelectAll()[1])));
	EXPECT_EQ(1,src->Count(DataAccess::data_remove));
}
TEST(PhotomultiplierGains,basetest){
	auto src=make_shared<test_data_source>(true);
	PhotomultiplierGains Gains(src,1);
	EXPECT_EQ(2,Gains.DataRowsCount());
	EXPECT_EQ(2,Gains.SelectAll().size());
	EXPECT_EQ(1,src->Count(DataAccess::data_obtain));
}
TEST(PhotomultiplierGains4Run,basetest){
	auto src=make_shared<test_data_source>(true);
	PhotomultiplierGains4Run Gains(src,1);
	EXPECT_EQ(2,Gains.DataRowsCount());
	EXPECT_EQ(2,Gains.SelectAll().size());
	EXPECT_EQ(1,src->Count(DataAccess::data_obtain));
}
