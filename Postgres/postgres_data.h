// this file is distributed under 
// MIT license
#ifndef _______POSTGRESS_DATA_H___________
#	define _______POSTGRESS_DATA_H___________
#include <memory>
#include <pqxx/pqxx>
#include <pqxx/nontransaction>
#include <JPetData/data_access.h>
namespace DataAccess{
	struct DBConfigData{
		std::string db_name;
		std::string username;
		std::string password;
		std::string hostname;
		std::string port;
		DBConfigData(const std::string&connstr);
		DBConfigData(const std::string&&connstr):DBConfigData(connstr){}
	};
	class PQData:public IDataSource{
	public:
		PQData(const DBConfigData&cfg);
		PQData(const DBConfigData&&cfg):PQData(cfg){}
		virtual ~PQData();
		virtual const bool Request(const RequestType request, const RequestParameters&params,std::vector<DataItem>&)override;
		const bool changed()const;
		void Commit();
	private:
		pqxx::connection f_connection;
		pqxx::nontransaction f_work;
		bool f_changed;
	};
};
#endif