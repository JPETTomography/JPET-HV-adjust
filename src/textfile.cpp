// this file is distributed under 
// MIT license
#include <textfile.h>
using namespace std;
namespace HVAdjust {
	istream& operator>>(istream& str, PhmParameters& val){
		return str>>val.HV>>val.Gain;
	}
	ostream& operator<<(ostream& str, const PhmParameters& val){
		return str<<val.HV<<"\t"<<val.Gain<<endl;
	}
};