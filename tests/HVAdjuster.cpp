// this file is distributed under 
// MIT license
#include <random>
#include <gtest/gtest.h>
#include <math_h/error.h>
#include <math_h/sigma.h>
#include <HVAdjuster.h>
using namespace std;
using namespace HVAdjust;
using namespace MathTemplates;
TEST(HVAdjuster,one){
	HVAdjuster test([](const MeasuredParameters&){return 1.0;});
	Genetic::RANDOM engine;
	uniform_real_distribution<double> d(0,HV_MAX);
	for(size_t i=0;i<10;i++){
		double hv=d(engine);
		EXPECT_THROW(test.Adjust({{.HV=hv,.Gain=hv}},engine)[0],Exception<HVAdjuster>);
		EXPECT_NO_THROW(test.Adjust({{.HV=hv,.Gain=hv},{.HV=hv,.Gain=hv}},engine)[0]);
	}
}
TEST(HVAdjuster,size){
	HVAdjuster test([](const MeasuredParameters&){return 1.0;});
	Genetic::RANDOM engine;
	uniform_real_distribution<double> d(0,HV_MAX);
	for(size_t i=0;i<50;i++){
		double hv=d(engine);
		for(size_t j=2;j<5;j++){
			vector<MeasuredParameters> V;
			for(size_t k=0;k<j;k++)V.push_back({.HV=hv,.Gain=hv});
			auto new_hv=test.Adjust(V,engine);
			EXPECT_EQ(j,new_hv.size());
		}
	}
}
TEST(HVAdjuster,SimilarConditions){
	Genetic::RANDOM engine;
	HVAdjuster test([](const MeasuredParameters&){return 1.0;});
	uniform_real_distribution<double> d(0,HV_MAX);
	for(size_t i=0;i<50;i++){
		for(size_t j=2;j<5;j++){
			StandardDeviation<double> S;
			vector<MeasuredParameters> V;
			for(size_t k=0;k<j;k++){
				double hv=d(engine);
				V.push_back({.HV=hv,.Gain=hv});
				S << hv;
			}
			auto new_hv=test.Adjust(V,engine);
			EXPECT_EQ(j,new_hv.size());
			for(const double&v:new_hv)EXPECT_TRUE(S().contains(v));
		}
	}
}
