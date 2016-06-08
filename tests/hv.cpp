// this file is distributed under 
// MIT license
#include <gtest/gtest.h>
#include <JPetData/Frames.h>
#include <JPetData/Detectors.h>
#include <JPetData/HVconfig.h>
#include <Postgres/postgres_data.h>
#include <HV/HVSetter.h>
#include "test_data_source.h"
using namespace std;
using namespace DataAccess;
using namespace JPetSetup;
using namespace HVAdjust;
TEST(HVTable,base){
	auto data_source=make_shared<test_data_source>(true);
	auto hv_source=make_shared<test_hv_setter>();
	auto frame=Frames(data_source).ByID(2);
	auto setup=frame.CreateSetupFactory().SelectAll()[0];
	auto config=HVconfigTable(data_source,setup.id()).SelectAll()[0];
	auto hv_record=HighVoltageTable(data_source).ByID(setup.highvoltage_id());
	HVTable table(config,setup,frame,hv_record,data_source,hv_source);
	EXPECT_EQ(table.SlotInfo().size(),8);
	EXPECT_EQ(table.HVConfigEntries().size(),8);
	EXPECT_EQ(table.HardwareHV().size(),8);
}