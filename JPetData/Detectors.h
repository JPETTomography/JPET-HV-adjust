// this file is distributed under 
// MIT license
#ifndef ______DETECTORS_H______
#	define ______DETECTORS_H______
#include <JPetData/data_access.h>
namespace JPetSetup{
	class Photomultiplier{
	public:
		Photomultiplier(const Photomultiplier&source);
		Photomultiplier&operator=(const Photomultiplier&source);
		virtual ~Photomultiplier();
		const size_t id()const;
		const std::string&name()const;
		const std::string&status()const;
		const std::string&description()const;
		const double max_hv()const;
	protected:
		friend class DataAccess::Factory<Photomultiplier>;
		enum{type=DataAccess::data_photomultiplier};
		Photomultiplier(const DataAccess::DataItem&item,const std::shared_ptr<DataAccess::IDataSource>);
		//DataAccess::RequestParameters params_to_insert()const; 
		//DataAccess::RequestParameters params_to_delete()const;
	private:
		size_t f_id;
		std::string f_name,f_status,f_description;
		double f_max_hv;
	};
	class Photomultipliers:public DataAccess::Factory<Photomultiplier>{
	public:
		Photomultipliers(const std::shared_ptr<DataAccess::IDataSource> src);
		virtual ~Photomultipliers();
		const Photomultiplier ByID(const size_t id)const;
	};
};
#endif