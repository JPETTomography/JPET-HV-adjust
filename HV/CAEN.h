// this file is distributed under 
// MIT license
#ifndef ____________HV___CAEN_H__________
#	define ____________HV___CAEN_H__________
#include <functional>
#include <vector>
#include "HVSetter.h"
namespace Hardware{
	class AbstractCAEN:public HVAdjust::IHVSetter{
	protected:
		AbstractCAEN(const int handle);
	public:
		virtual ~AbstractCAEN();
	private:
		int f_handle;
	};
}
#endif