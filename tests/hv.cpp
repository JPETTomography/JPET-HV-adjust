// this file is distributed under 
// MIT license
#include <math.h>
#include <gtest/gtest.h>
#include <JPetData/Frames.h>
#include <JPetData/Detectors.h>
#include <JPetData/HVconfig.h>
#include <Postgres/postgres_data.h>
#include <JPetData/HVSetter.h>
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
	data_source->ResetCounters();
	HVTable table(config,setup,frame,hv_record,data_source,hv_source);
	EXPECT_EQ(8,data_source->Count(DataAccess::data_obtain));//8 because of two layers in the frame
	for(const double&hv:table.HardwareHV())
		EXPECT_EQ(INFINITY,hv);
	EXPECT_EQ(table.SlotInfo().size(),8);
	EXPECT_EQ(table.HVConfigEntries().size(),8);
	EXPECT_EQ(table.HardwareHV().size(),8);
	EXPECT_EQ(hv_source->UpdatesCount(),1);
	table.SynchroHardwarewithDB();
	EXPECT_EQ(hv_source->UpdatesCount(),2);
	size_t count=0;
	for(const double&hv:table.HardwareHV())
		if(isfinite(hv)){
			EXPECT_EQ(500,hv);
			count++;
		}
	EXPECT_EQ(5,count);
	table.SwitchOffHardware();
	EXPECT_EQ(hv_source->UpdatesCount(),3);
}