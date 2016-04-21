// this file is distributed under 
// MIT license
#include <math_h/error.h>
#include <HVAdjuster.h>
namespace HVAdjust {
	using namespace std;
	using namespace MathTemplates;
	HVAdjuster::~HVAdjuster(){}
	HVAdjuster::HVAdjuster(const GainDerivativeByHV dGain_dHV):f_dGain_dHV(dGain_dHV){}
	HVAdjuster::HVAdjuster(const HVAdjuster& source):f_dGain_dHV(source.f_dGain_dHV){}
	const vector<PhmParameters> HVAdjuster::Adjust(const vector<PhmParameters>&data) const{
		if(data.size()<2)throw Exception<HVAdjuster>("There must be at least two photomultipliers");
		WeightedAverage<double> AverageGain;
		for(const auto&phm:data)AverageGain<<phm.Gain;
		vector<PhmParameters> res;
		for(const auto&phm:data)
			res.push_back({.HV=phm.HV+(AverageGain()-phm.Gain)/f_dGain_dHV(phm),.Gain=AverageGain()});
		return res;
	}
};