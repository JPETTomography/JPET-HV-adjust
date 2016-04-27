// this file is distributed under 
// MIT license
#include <random>
#include <gtest/gtest.h>
#include <math_h/error.h>
#include <data_access.h>
#include <CalibrationGeneral.h>
#include "test_data_source.h"
using namespace std;
using namespace MathTemplates;
using namespace DataAccess;
using namespace Calibration;
TEST(CalibrationType,basetest){
	CalibrationTypes typetable(make_shared<test_data_source>(true));
	EXPECT_EQ(2,typetable.size());
	for(size_t id=1;id<=typetable.size();id++){
		auto item=typetable.Get(id);
		EXPECT_EQ(item.id(),id);
	}
	EXPECT_EQ(typetable.Get(0).id(),0);
	EXPECT_EQ(typetable.Get(3).id(),0);
	EXPECT_EQ(typetable.Get(4).id(),0);
	EXPECT_EQ(typetable.Get(5).id(),0);
	EXPECT_EQ(typetable.Get(6).id(),0);
}
TEST(CalibrationType,inserting){
	auto src=make_shared<test_data_source>(true);
	CalibrationTypes typetable(src);
	EXPECT_EQ(2,typetable.size());
	EXPECT_EQ(false,typetable.Add(typetable.Get(1)));
	EXPECT_EQ(0,src->Count(DataAccess::data_insert));
	EXPECT_EQ(true,typetable.Add(CalibrationType("new_element",1,"[0]+x")));
	EXPECT_EQ(1,src->Count(DataAccess::data_insert));
}
TEST(CalibrationType,deleting){
	auto src=make_shared<test_data_source>(true);
	CalibrationTypes typetable(src);
	EXPECT_EQ(2,typetable.size());
	EXPECT_EQ(true,typetable.Delete(typetable.Get(1)));
	EXPECT_EQ(1,src->Count(DataAccess::data_remove));
	EXPECT_EQ(false,typetable.Delete(CalibrationType("new_element",1,"[0]+x")));
	EXPECT_EQ(1,src->Count(DataAccess::data_remove));
}