// this file is distributed under 
// MIT license
#include <connectionstring.h>
#include <libhv_global.h>
#include "CAEN.h"
using namespace std;
using namespace HVAdjust;
namespace Hardware{
	CAEN::CAEN(const string connstr){
		char buf[connstr.length()];
		for(size_t i=0; i<connstr.length();i++)buf[i]=connstr[i];
		LIBHV_createLibhvWithConnectionString(f_handle,buf,connstr.length());
	}
	CAEN::~CAEN(){
		delete f_handle;
	}

}
