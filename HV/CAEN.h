// this file is distributed under 
// MIT license
#ifndef ____________HV___CAEN_H__________
#	define ____________HV___CAEN_H__________
#include <vector>
#include <JPetData/HVSetter.h>
class libhv;
class ChanelStatus;
namespace Hardware{
	class CAEN:public HVAdjust::IHVSetter{
	public:
		CAEN(const std::string connstr);
		virtual ~CAEN();
		virtual void UpdateRequest()const override;
		//getters
		virtual const size_t ChannelCount()const override;
		virtual bool IsOn(size_t channel_no)const override;
		virtual double GetHV(size_t channel_no)const override;
		ChanelStatus*operator[](const size_t channel_no)const;
		//setters
		virtual void turnOn(size_t channel_no)override;
		virtual void turnOff(size_t channel_no)override;
		virtual bool SetHV(size_t channel_no,double hv)override;
	private:
		libhv* f_handle;
		ChanelStatus** f_status_cache;
		size_t f_count;
	};
}
#endif