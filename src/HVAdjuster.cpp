// this file is distributed under 
// MIT license
#include <math_h/error.h>
#include <HVAdjuster.h>
namespace HVAdjust {
	using namespace std;
	using namespace MathTemplates;
	AbstractHVAdjuster::~AbstractHVAdjuster(){}
	AbstractHVAdjuster::AbstractHVAdjuster(const GainDerivativeByHV dGain_dHV):f_dGain_dHV(dGain_dHV){}
	AbstractHVAdjuster::AbstractHVAdjuster(const AbstractHVAdjuster& source):f_dGain_dHV(source.f_dGain_dHV){}
	const vector< PhmParameters > AbstractHVAdjuster::SetAllGain(const value< double >& newGain, const vector< PhmParameters >& data) const{
		vector<PhmParameters> res;
		for(const auto&phm:data){
			auto dGdV=f_dGain_dHV(phm);
			if(dGdV.min()<=0.0)throw Exception<AbstractHVAdjuster>("dG/dV cannot be negative or zero");
			res.push_back({.HV=phm.HV+((newGain-phm.Gain)/dGdV),.Gain=newGain});
		}
		return res;
	}

	HVAdjustToAverage::HVAdjustToAverage(const GainDerivativeByHV dGain_dHV): AbstractHVAdjuster(dGain_dHV){}
	HVAdjustToAverage::HVAdjustToAverage(const AbstractHVAdjuster& source):AbstractHVAdjuster(source){}
	HVAdjustToAverage::~HVAdjustToAverage(){}
	const vector<PhmParameters> HVAdjustToAverage::Adjust(const vector<PhmParameters>&data) const{
		if(data.size()<2)throw Exception<HVAdjustToAverage>("There must be at least two photomultipliers");
		WeightedAverage<double> AverageGain;
		for(const auto&phm:data)AverageGain<<phm.Gain;
		return SetAllGain(AverageGain(),data);
	}
	
	HVAdjustWithMaximum::HVAdjustWithMaximum(const GainDerivativeByHV dGain_dHV, const double maximumHV)
	:AbstractHVAdjuster(dGain_dHV),f_maximumHV(maximumHV){}
	HVAdjustWithMaximum::HVAdjustWithMaximum(const HVAdjustWithMaximum& source)
	:AbstractHVAdjuster(source),f_maximumHV(source.f_maximumHV){}
	HVAdjustWithMaximum::~HVAdjustWithMaximum(){}
	const vector<PhmParameters> HVAdjustWithMaximum::Adjust(const vector<PhmParameters>&data)const{
		if(data.size()<2)throw Exception<HVAdjustWithMaximum>("There must be at least two photomultipliers");
		value<double> gain;
		{
			WeightedAverage<double> AverageGain;
			for(const auto&phm:data)AverageGain<<phm.Gain;
			gain=AverageGain();
		}
		vector<PhmParameters> res;
		bool done=false;
		do{
			res=SetAllGain(gain,data);
			done=true;
			for(const auto&item:res)done&=(item.HV.max()<f_maximumHV);
			if(!done)gain*=0.9;
		}while(!done);
		return res;
	}


};