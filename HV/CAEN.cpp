// this file is distributed under 
// MIT license
#include <connectionstring.h>
#include <libhv_global.h>
#include <math_h/error.h>
#include "CAEN.h"
using namespace std;
using namespace HVAdjust;
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
		
	}
	CAEN::~CAEN(){
		delete f_handle;
	}
	double CAEN::GetCurent(size_t channel_no) const{
		if(channel_no>f_count)
			throw MathTemplates::Exception<CAEN>("Channel range check error");
		return LIBHV_getCurrent(f_handle,channel_no);
	}
	double CAEN::GetHV(size_t channel_no) const{
		if(channel_no>f_count)
			throw MathTemplates::Exception<CAEN>("Channel range check error");
		return LIBHV_getVoltage(f_handle,channel_no);
	}
	bool CAEN::SetHV(size_t channel_no, double hv){
		if(channel_no>f_count)
			throw MathTemplates::Exception<CAEN>("Channel range check error");
		//ToDo: chack maximum HV in the hardware
		return LIBHV_setVoltage(f_handle,channel_no,hv);
	}


}
