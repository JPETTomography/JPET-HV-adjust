// this file is distributed under 
// MIT license
#include <math_h/sigma.h>
#include <math_h/error.h>
#include <Genetic/equation.h>
#include <Genetic/initialconditions.h>
#include <HVAdjuster.h>
namespace HVAdjust {
	using namespace std;
	using namespace Genetic;
	using namespace MathTemplates;
	HVAdjuster::~HVAdjuster(){}
	HVAdjuster::HVAdjuster(const function<double(const MeasuredParameters&)> dGain_dHV):f_dGain_dHV(dGain_dHV){}
	HVAdjuster::HVAdjuster(const HVAdjuster& source):f_dGain_dHV(source.f_dGain_dHV){}
	const ParamSet HVAdjuster::Adjust(const vector<MeasuredParameters>&data,RANDOM&engine) const{
		if(data.size()<2)
			throw Exception<HVAdjuster>("There must be at least two photomultipliers");
		SearchMin<DifferentialMutations<>> adjust(
			[this,&data](const ParamSet&new_HV){
				StandardDeviation<double> AvrGain;
				for(size_t i=0,n=data.size();i<n;i++)
					AvrGain << data[i].Gain+(new_HV[i]-data[i].HV)*f_dGain_dHV(data[i]);
				return AvrGain().delta();
			}
		);
		adjust.SetFilter([](const ParamSet&new_HV){
			for(const double&hv:new_HV)
				if(hv>HV_MAX)return false;
			return true;
		});
		
		auto init=make_shared<GenerateByGauss>();
		for(const auto&item:data)init<< make_pair(item.HV,HV_DELTA);
		adjust.Init(data.size()*10,init,engine);
		auto final_disp=parEq(data.size(),1.0);
		while(!adjust.AbsoluteOptimalityExitCondition(0.001))
			adjust.Iterate(engine);
		return adjust.Parameters();
	}
};