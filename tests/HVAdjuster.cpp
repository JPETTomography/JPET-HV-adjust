// this file is distributed under 
// MIT license
#include <random>
#include <gtest/gtest.h>
#include <math_h/error.h>
#include <HVAdjuster.h>
using namespace std;
using namespace HVAdjust;
using namespace MathTemplates;
TEST(HVAdjuster,throwing){
	HVAdjuster test([](const PhmParameters&)->value<double>{return 1.0;});
	EXPECT_THROW(test.Adjust({}),Exception<HVAdjuster>);
	EXPECT_THROW(test.Adjust({{.HV=500.0,.Gain={1.0,0.1}}}),Exception<HVAdjuster>);
	EXPECT_THROW(test.Adjust({{.HV=500.0,.Gain=1.0},{.HV=500.0,.Gain=1.0}}),Exception<WeightedAverage<double>>);
	EXPECT_NO_THROW(test.Adjust({{.HV=500.0,.Gain={1.0,0.1}},{.HV=500.0,.Gain={1.0,0.1}}}));
}
TEST(HVAdjuster,similar_values){
	HVAdjuster test([](const PhmParameters&)->value<double>{return 1.0;});
	vector<PhmParameters> input={{.HV=500.0,.Gain={1.0,0.1}}};
	for(size_t i=0;i<10;i++){
		input.push_back({.HV=500.0,.Gain={1.0,0.1}});
		auto output=test.Adjust(input);
		EXPECT_EQ(input.size(),output.size());
		for(size_t j=0;j<input.size();j++){
			EXPECT_EQ(input[j].HV.val(),output[j].HV.val());
			EXPECT_EQ(input[j].Gain.val(),output[j].Gain.val());
		}
	}
}
