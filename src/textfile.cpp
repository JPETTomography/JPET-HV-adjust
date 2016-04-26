// this file is distributed under 
// MIT license
#include <textfile.h>
using namespace std;
namespace HVAdjust {
	istream& operator>>(istream& str, PhmParameters& val){
		double a,b,c,d;
		str>>a>>b>>c>>d;
		val={.HV={a,b},.Gain={c,d}};
		return str;
	}
	ostream& operator<<(ostream& str, const PhmParameters& val){
		return str<<val.HV<<"\t"<<val.Gain<<endl;
	}
};