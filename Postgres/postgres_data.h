// this file is distributed under 
// MIT license
#ifndef _______POSTGRESS_DATA_H___________
#	define _______POSTGRESS_DATA_H___________
#include <memory>
#include <pqxx/pqxx>
#include <pqxx/nontransaction>
#include <JPetData/data_access.h>
namespace DataAccess{
	class PQData:public IDataSource{
	public:
		PQData(const std::string&connstr);
		PQData(const std::string&&connstr):PQData(connstr){}
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