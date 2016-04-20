// this file is distributed under 
// MIT license
#include <Genetic/equation.h>
#include <HVAdjuster.h>
using namespace std;
using namespace Genetic;
HVAdjust::HVAdjuster::~HVAdjuster(){}
HVAdjust::HVAdjuster::HVAdjuster(const std::function< double > HV2Gain, const double hv_delta)
:f_HV2Gain(HV2Gain),f_hv_delta(hv_delta){}
HVAdjust::HVAdjuster::HVAdjuster(const HVAdjust::HVAdjuster& source)
:f_HV2Gain(source.f_HV2Gain),f_hv_delta(source.f_hv_delta){}
const std::vector< double > HVAdjust::HVAdjuster::Adjust(const std::vector<HVAdjust::MeasuredParameters>& data) const{
	const double HV_max=1300;
	
}


