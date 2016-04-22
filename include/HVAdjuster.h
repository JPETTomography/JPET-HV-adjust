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
	class AbstractHVAdjuster{
	public:
		AbstractHVAdjuster(const GainDerivativeByHV dGain_dHV);
		AbstractHVAdjuster(const AbstractHVAdjuster&source);
		virtual ~AbstractHVAdjuster();
		virtual const std::vector<PhmParameters> Adjust(const std::vector<PhmParameters>&data)const=0;
	protected:
		const std::vector<PhmParameters> SetAllGain(const MathTemplates::value<double>&newGain,const std::vector<PhmParameters>&data)const;
	private:
		const GainDerivativeByHV f_dGain_dHV;
	};
	class HVAdjustToAverage:public AbstractHVAdjuster{
	public:
		HVAdjustToAverage(const GainDerivativeByHV dGain_dHV);
		HVAdjustToAverage(const AbstractHVAdjuster&source);
		virtual ~HVAdjustToAverage();
		virtual const std::vector<PhmParameters> Adjust(const std::vector<PhmParameters>&data)const override;
	};
	class HVAdjustWithMaximum:public AbstractHVAdjuster{
	public:
		HVAdjustWithMaximum(const GainDerivativeByHV dGain_dHV,const double maximumHV);
		HVAdjustWithMaximum(const HVAdjustWithMaximum&source);
		virtual ~HVAdjustWithMaximum();
		const double&MaximumHV()const;
		virtual const std::vector<PhmParameters> Adjust(const std::vector<PhmParameters>&data)const override;
	private:
		double f_maximumHV;
	};
};

#endif