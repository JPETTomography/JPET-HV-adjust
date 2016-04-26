// this file is distributed under 
// MIT license
#ifndef ________GAINDERIVATIVE_H__________
#	define ________GAINDERIVATIVE_H__________
#include "HVAdjuster.h"
namespace HVAdjust{
	class GainHVderivative{
	public:
		GainHVderivative(const double alfa,const double beta);
		GainHVderivative(const GainHVderivative&source);
		virtual ~GainHVderivative();
		const GainDerivativeByHV func()const;
		const double alfa()const;
		const double beta()const;
	private:
		double f_alfa,f_beta;
	};
};
#endif