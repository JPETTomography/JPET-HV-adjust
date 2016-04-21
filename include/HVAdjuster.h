// this file is distributed under 
// MIT license
#ifndef ______HVADJUSTER____H______
#	define ______HVADJUSTER____H______
#include <functional>
#include <vector>
#include <math_h/sigma.h>
namespace HVAdjust{
	const double HV_MAX=1300;
	const double HV_DELTA=50;
	
	struct PhmParameters{
		MathTemplates::value<double> HV;
		MathTemplates::value<double> Gain;
	};
	typedef std::function<MathTemplates::value<double>(const PhmParameters&)> GainDerivativeByHV;
	class HVAdjuster{
	public:
		HVAdjuster(const GainDerivativeByHV dGain_dHV);
		HVAdjuster(const HVAdjuster&source);
		virtual ~HVAdjuster();
		virtual const std::vector<PhmParameters> Adjust(const std::vector<PhmParameters>&data)const;
	private:
		const GainDerivativeByHV f_dGain_dHV;
	};
};

#endif