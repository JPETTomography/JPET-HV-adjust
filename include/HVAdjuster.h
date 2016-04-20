// this file is distributed under 
// MIT license
#ifndef ______HVADJUSTER____H______
#	define ______HVADJUSTER____H______
#include <functional>
#include <vector>
namespace HVAdjust{
	struct MeasuredParameters{
		double HV;
		double Gain;
	};
	class HVAdjuster{
	public:
		HVAdjuster(const std::function<double(double)> HV2Gain, const double hv_delta);
		HVAdjuster(const HVAdjuster&source);
		virtual ~HVAdjuster();
		const std::vector<double> Adjust(const std::vector<MeasuredParameters>&data)const;
	private:
		const std::function<double(double)> f_HV2Gain;
		double f_hv_delta;
	};
};

#endif