// this file is distributed under 
// MIT license
#ifndef ________TEXTFILE_H_________
#	define ________TEXTFILE_H_________
#include <iostream>
#include "HVAdjuster.h"
namespace HVAdjust{
	std::istream&operator>>(std::istream&str,PhmParameters&val);
	std::ostream&operator<<(std::ostream&str,const PhmParameters&val);
};
#endif