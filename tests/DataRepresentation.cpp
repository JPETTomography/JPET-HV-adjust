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
class CalibrationTypes_moc:public CalibrationTypes{
public:
    CalibrationTypes_moc(const shared_ptr< IDataSource > src):CalibrationTypes(src){}
    virtual ~CalibrationTypes_moc(){}
    const vector<CalibrationType> ByID(const size_t id){return GetFieldEq("type_id",id);}
};
TEST(CalibrationType,basetest){
	CalibrationTypes_moc typetable(make_shared<test_data_source>(true));
	EXPECT_EQ(2,typetable.size());
	EXPECT_EQ(2,typetable.GetList().size());
	for(size_t id=1;id<=typetable.size();id++){
		EXPECT_EQ(typetable.ByID(id).size(),1);
	}
	EXPECT_EQ(typetable.ByID(0).size(),0);
	EXPECT_EQ(typetable.ByID(3).size(),0);
	EXPECT_EQ(typetable.ByID(4).size(),0);
	EXPECT_EQ(typetable.ByID(5).size(),0);
	EXPECT_EQ(typetable.ByID(6).size(),0);
}
TEST(CalibrationType,inserting){
	auto src=make_shared<test_data_source>(true);
	CalibrationTypes typetable(src);
	EXPECT_EQ(2,typetable.size());
	EXPECT_EQ(true,typetable.Add(CalibrationType("new_element",1,"[0]+x")));
	EXPECT_EQ(1,src->Count(DataAccess::data_insert));
}
TEST(CalibrationType,deleting){
	auto src=make_shared<test_data_source>(true);
	CalibrationTypes typetable(src);
	EXPECT_EQ(2,typetable.size());
	EXPECT_EQ(2,typetable.GetList().size());
	EXPECT_EQ(1,src->Count(DataAccess::data_obtain));
	EXPECT_EQ(true,typetable.Delete(const_cast<CalibrationType&&>(typetable.GetList()[1])));
	EXPECT_EQ(1,src->Count(DataAccess::data_remove));
}
TEST(PhotomultiplierGains,basetest){
	auto src=make_shared<test_data_source>(true);
	PhotomultiplierGains Gains(src,1);
	EXPECT_EQ(2,Gains.size());
	EXPECT_EQ(2,Gains.GetList().size());
	EXPECT_EQ(1,src->Count(DataAccess::data_obtain));
}
TEST(PhotomultiplierGains4Run,basetest){
	auto src=make_shared<test_data_source>(true);
	PhotomultiplierGains4Run Gains(src,1);
	EXPECT_EQ(2,Gains.size());
	EXPECT_EQ(2,Gains.GetList().size());
	EXPECT_EQ(1,src->Count(DataAccess::data_obtain));
}
