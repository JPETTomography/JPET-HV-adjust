// this file is distributed under 
// MIT license
#include <GainDerivative.h>
using namespace MathTemplates;
namespace HVAdjust {
	GainHVderivative::GainHVderivative(const double alfa, const double beta):f_alfa(alfa),f_beta(beta){}
	GainHVderivative::GainHVderivative(const GainHVderivative& source):f_alfa(source.f_alfa),f_beta(source.f_beta){}
	GainHVderivative::~GainHVderivative(){}
	const double GainHVderivative::alfa() const{return f_alfa;}
	const double GainHVderivative::beta() const{return f_beta;}
	const GainDerivativeByHV GainHVderivative::func() const{
		return [this](const PhmParameters&pp){
			return (pp.Gain/pp.HV)*f_beta;
		};
	}
};