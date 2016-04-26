// this file is distributed under 
// MIT license
#ifndef _______POSTGRESS_DATA_H___________
#	define _______POSTGRESS_DATA_H___________
#include <pqxx/pqxx>
#include <data_access.h>
namespace DataAccess{
	struct DBConfigData{
		std::string db_name;
		std::string username;
		std::string password;
		std::string hostname;
		std::string port;
	};
	class PQData:public IDataSource{
	public:
		PQData(const DBConfigData&cfg);
		virtual ~PQData();
		virtual const bool Request(const RequestType request, const RequestParameters&params,std::vector<DataItem>&)override;
	private:
		std::shared_ptr<pqxx::connection> f_connection;
		std::shared_ptr<pqxx::work> f_work;
	};
};
#endif