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
		virtual void UpdateRequest()override;
		virtual bool IsOn(size_t idx)const override;
		virtual double GetHV(size_t idx)const override;
		virtual void turnOn(size_t idx)override;
		virtual void turnOff(size_t idx)override;
		virtual void SetHV(size_t idx,double hv)override;

		const size_t ChannelCount()const;
		ChanelStatus*operator[](const size_t index)const;
		const size_t idx2index(const size_t idx)const;
		const size_t index2idx(const size_t index)const;
	private:
		libhv* f_handle;
		ChanelStatus** f_status_cache;
		size_t* f_idx_cache;
		size_t f_count;
	};
}
#endif