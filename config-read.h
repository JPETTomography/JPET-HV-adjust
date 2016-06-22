// this file is distributed under 
// MIT license
#ifndef ____________CONFIG_FOR_TOOLS__________
#	define ____________CONFIG_FOR_TOOLS__________
#include <string>
class connstr_config{
public:
	~connstr_config();
	static const std::string DB();
	static const std::string HV();	
private:
	connstr_config();
	static const connstr_config&instance();
	std::string f_db,f_hv;
};
#endif