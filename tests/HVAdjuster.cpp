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
	HVAdjuster test([](const PhmParameters&)->double{return 1.0;});
	EXPECT_THROW(test.Adjust({}),Exception<HVAdjuster>);
	EXPECT_THROW(test.Adjust({{.HV=500.0,.Gain={1.0,0.1}}}),Exception<HVAdjuster>);
	EXPECT_THROW(test.Adjust({{.HV=500.0,.Gain=1.0},{.HV=500.0,.Gain=1.0}}),Exception<WeightedAverage<double>>);
	EXPECT_NO_THROW(test.Adjust({{.HV=500.0,.Gain={1.0,0.1}},{.HV=500.0,.Gain={1.0,0.1}}}));
}
TEST(HVAdjuster,similar_values){
	HVAdjuster test([](const PhmParameters&)->double{return 1.0;});
	vector<PhmParameters> input={{.HV=500.0,.Gain={1.0,0.1}}};
	for(size_t i=0;i<10;i++){
		input.push_back({.HV=500.0,.Gain={1.0,0.1}});
		auto output=test.Adjust(input);
		EXPECT_EQ(input.size(),output.size());
		for(size_t j=0;j<input.size();j++){
			EXPECT_EQ(input[j].HV,output[j].HV);
			EXPECT_EQ(input[j].Gain,output[j].Gain);
		}
	}
}
TEST(HVAdjuster, two_values_deriv_1){
	HVAdjuster test([](const PhmParameters&)->double{return 1.0;});
	auto out=test.Adjust({{.HV=900.0,.Gain={900.0,1.0}},{.HV=1000.0,.Gain={1000.0,1.0}}});
	EXPECT_TRUE(value<double>(950,1).contains(out[0].Gain.val()));
	EXPECT_TRUE(value<double>(950,1).contains(out[1].Gain.val()));
	EXPECT_TRUE(value<double>(950,1).contains(out[0].HV.val()));
	EXPECT_TRUE(value<double>(950,1).contains(out[1].HV.val()));
}
TEST(HVAdjuster, two_values_deriv_2){
	HVAdjuster test([](const PhmParameters&)->double{return 2.0;});
	auto out=test.Adjust({{.HV=900.0,.Gain={900.0,1.0}},{.HV=1000.0,.Gain={1000.0,1.0}}});
	EXPECT_TRUE(value<double>(950,1).contains(out[0].Gain.val()));
	EXPECT_TRUE(value<double>(950,1).contains(out[1].Gain.val()));
	EXPECT_TRUE(value<double>(925,1).contains(out[0].HV.val()));
	EXPECT_TRUE(value<double>(975,1).contains(out[1].HV.val()));
}
TEST(HVAdjuster, similar_dependence){
	HVAdjuster test([](const PhmParameters&)->double{return 1.0;});
	mt19937 engine;
	normal_distribution<double> v_gen(1000,50);
	StandardDeviation<double> V;
	vector<PhmParameters> input;
	for(size_t i=0;i<10;i++){
		double hv=v_gen(engine);
		input.push_back({.HV=hv,.Gain={hv,hv*0.01}});
		V<<hv;
		if(i>=1){
			auto output=test.Adjust(input);
			EXPECT_EQ(input.size(),output.size());
			for(size_t j=0;j<input.size();j++){
				EXPECT_TRUE(output[j].Gain.contains(V().val()));
				EXPECT_TRUE(value<double>(output[j].HV.val(),output[j].Gain.delta()).contains(V().val()));
			}
		}
	}
}
