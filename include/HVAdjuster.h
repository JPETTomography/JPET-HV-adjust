// this file is distributed under 
// MIT license
#ifndef ______HVADJUSTER____H______
#	define ______HVADJUSTER____H______
#include <functional>
#include <vector>
#include <Genetic/abstract.h>
namespace HVAdjust{
	const double HV_MAX=1300;
	const double HV_DELTA=50;
	
	struct MeasuredParameters{
		double HV;
		double Gain;
	};
	class HVAdjuster{
	public:
		HVAdjuster(const std::function<double(const MeasuredParameters&)> dGain_dHV);
		HVAdjuster(const HVAdjuster&source);
		virtual ~HVAdjuster();
		const Genetic::ParamSet Adjust(const std::vector<MeasuredParameters>&data,Genetic::RANDOM&engine)const;
	private:
		const std::function<double(const MeasuredParameters&)> f_dGain_dHV;
	};
};

#endif