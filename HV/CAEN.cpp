// this file is distributed under 
// MIT license
#include <connectionstring.h>
#include <libhv_global.h>
#include <math_h/error.h>
#include "CAEN.h"
using namespace std;
using namespace HVAdjust;
using namespace MathTemplates;
namespace Hardware{
	CAEN::CAEN(const string connstr){
		char buf[connstr.length()];
		for(size_t i=0; i<connstr.length();i++)buf[i]=connstr[i];
		LIBHV_createLibhvWithConnectionString(f_handle,buf,connstr.length());
		{
			int boardsCount=0;
			int* channelsCounts=nullptr;
			LIBHV_getBordCountAndChannel(f_handle,channelsCounts,boardsCount);
			channelsCounts=new int[boardsCount];
			LIBHV_getBordCountAndChannel(f_handle,channelsCounts,boardsCount);
			f_count=0;
			for(int i=0;i<boardsCount;i++)
				f_count+=channelsCounts[i];
			delete channelsCounts;
		}
		f_status_cache=new ChanelStatus*[f_count];
		for(size_t i=0;i<f_count;i++)
			f_status_cache[i]=new ChanelStatus();
		LIBHV_getStatusForAll(f_handle,f_status_cache,f_count);
	}
	CAEN::~CAEN(){
		for(size_t i=0;i<f_count;i++)
			delete f_status_cache[i];
		delete[] f_status_cache;
		delete f_handle;
	}
	const size_t CAEN::ChannelCount() const{return f_count;}
	ChanelStatus* CAEN::operator[](const size_t channel_no)const{
		if(channel_no>=f_count)
			throw Exception<CAEN>("Range check error");
		return f_status_cache[channel_no];
	}
	void CAEN::UpdateRequest(){
		LIBHV_getStatusForAll(f_handle,f_status_cache,f_count);
	}
	double CAEN::GetHV(size_t channel_no) const{
		if(channel_no>=f_count)
			throw Exception<CAEN>("Range check error");
		return f_status_cache[channel_no]->getVMon();
	}
	bool CAEN::IsOn(size_t channel_no) const{
		if(channel_no>=f_count)
			throw Exception<CAEN>("Range check error");
		return f_status_cache[channel_no]->getRamp();
	}
	bool CAEN::SetHV(size_t channel_no, double hv){}
	void CAEN::turnOn(size_t channel_no){}
	void CAEN::turnOff(size_t channel_no){}
}
