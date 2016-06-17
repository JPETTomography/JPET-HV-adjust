// this file is distributed under 
// MIT license
#ifndef ____________HV___CAEN_H__________
#	define ____________HV___CAEN_H__________
#include <vector>
#include <JPetData/HVSetter.h>
class libhv;
namespace Hardware{
	class CAEN:public HVAdjust::IHVSetter{
	public:
		CAEN(const std::string connstr);
		virtual ~CAEN();
		const size_t size()const;
		virtual double GetHV(size_t channel_no)const override;
		virtual double GetCurent(size_t channel_no)const override;
		virtual bool SetHV(size_t channel_no,double hv)override;
	private:
		libhv* f_handle;
		size_t f_count;
	};
}
#endif