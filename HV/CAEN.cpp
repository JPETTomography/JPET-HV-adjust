// this file is distributed under 
// MIT license
#include <CAENHVWrapper.h>
#include "CAEN.h"
using namespace std;
using namespace HVAdjust;
namespace Hardware{
	AbstractCAEN::AbstractCAEN(const int handle):f_handle(handle){}
	AbstractCAEN::~AbstractCAEN(){
		CAENHV_DeinitSystem(f_handle);
	}

}
