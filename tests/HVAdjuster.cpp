// this file is distributed under 
// MIT license
#include <random>
#include <gtest/gtest.h>
#include <math_h/error.h>
#include <HV/HVAdjuster.h>
using namespace std;
using namespace HVAdjust;
using namespace MathTemplates;
TEST(HVAdjustToAverage,throwing){
	HVAdjustToAverage test([](const PhmParameters&)->double{return 1.0;});
	EXPECT_THROW(test.Adjust({}),Exception<HVAdjustToAverage>);
	EXPECT_THROW(test.Adjust({{.HV=500.0,.Gain={1.0,0.1}}}),Exception<HVAdjustToAverage>);
	EXPECT_THROW(test.Adjust({{.HV=500.0,.Gain=1.0},{.HV=500.0,.Gain=1.0}}),Exception<WeightedAverage<double>>);
	EXPECT_NO_THROW(test.Adjust({{.HV=500.0,.Gain={1.0,0.1}},{.HV=500.0,.Gain={1.0,0.1}}}));
}
TEST(HVAdjustToAverage,similar_values){
	HVAdjustToAverage test([](const PhmParameters&)->double{return 1.0;});
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
TEST(HVAdjustToAverage, two_values_deriv_1){
	HVAdjustToAverage test([](const PhmParameters&)->double{return 1.0;});
	auto out=test.Adjust({{.HV=900.0,.Gain={900.0,1.0}},{.HV=1000.0,.Gain={1000.0,1.0}}});
	EXPECT_TRUE(value<double>(950,1).contains(out[0].Gain.val()));
	EXPECT_TRUE(value<double>(950,1).contains(out[1].Gain.val()));
	EXPECT_TRUE(value<double>(950,1).contains(out[0].HV.val()));
	EXPECT_TRUE(value<double>(950,1).contains(out[1].HV.val()));
}
TEST(HVAdjustToAverage, two_values_deriv_2){
	HVAdjustToAverage test([](const PhmParameters&)->double{return 2.0;});
	auto out=test.Adjust({{.HV=900.0,.Gain={900.0,1.0}},{.HV=1000.0,.Gain={1000.0,1.0}}});
	EXPECT_TRUE(value<double>(950,1).contains(out[0].Gain.val()));
	EXPECT_TRUE(value<double>(950,1).contains(out[1].Gain.val()));
	EXPECT_TRUE(value<double>(925,1).contains(out[0].HV.val()));
	EXPECT_TRUE(value<double>(975,1).contains(out[1].HV.val()));
}
TEST(HVAdjustToAverage, similar_dependence){
	HVAdjustToAverage test([](const PhmParameters&)->double{return 1.0;});
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
TEST(HVAdjustWithMaximum,throwing){
	HVAdjustWithMaximum test([](const PhmParameters&)->double{return 1.0;},1000.0);
	EXPECT_EQ(1000.0,test.MaximumHV());
	EXPECT_THROW(test.Adjust({}),Exception<HVAdjustWithMaximum>);
	EXPECT_THROW(test.Adjust({{.HV=500.0,.Gain={1.0,0.1}}}),Exception<HVAdjustWithMaximum>);
	EXPECT_THROW(test.Adjust({{.HV=500.0,.Gain=1.0},{.HV=500.0,.Gain=1.0}}),Exception<WeightedAverage<double>>);
	EXPECT_NO_THROW(test.Adjust({{.HV=500.0,.Gain={1.0,0.1}},{.HV=500.0,.Gain={1.0,0.1}}}));
}
TEST(HVAdjustWithMaximum,twoitemstest){
	HVAdjustWithMaximum test([](const PhmParameters&)->double{return 1.0;},950.0);
	EXPECT_EQ(950.0,test.MaximumHV());
	auto below=test.Adjust({{.HV=700.0,.Gain={700.0,1.0}},{.HV=900.0,.Gain={900.0,1.0}}});
	EXPECT_EQ(950.0,test.MaximumHV());
	EXPECT_TRUE(value<double>(800,1).contains(below[0].HV.val()));
	EXPECT_TRUE(value<double>(800,1).contains(below[1].HV.val()));
	auto above=test.Adjust({{.HV=1100.0,.Gain={1100.0,1.0}},{.HV=900.0,.Gain={900.0,1.0}}});
	EXPECT_EQ(950.0,test.MaximumHV());
	EXPECT_TRUE(value<double>(above[0].HV.val(),1).contains(above[1].HV.val()));
	EXPECT_TRUE(above[0].HV.max()<test.MaximumHV());
	EXPECT_TRUE(above[1].HV.max()<test.MaximumHV());
}