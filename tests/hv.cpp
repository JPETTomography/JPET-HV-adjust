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
//ToDo: write tests for HVTable