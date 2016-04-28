// this file is distributed under 
// MIT license
#ifndef _______F_R_A_M_E_S____H_________
#	define _______F_R_A_M_E_S____H_________
#include <DataGeneral/data_access.h>
namespace JPetSetup{
	class Slot{
	public:
		Slot(const Slot&source);
		virtual ~Slot();
		const size_t id()const;
		const size_t layer_id()const;
		const std::string&name()const;
		const bool active()const;
	protected:
		friend class DataAccess::Factory<Slot>;
		enum{type=DataAccess::data_slot};
		Slot(const DataAccess::DataItem&item,const std::shared_ptr<DataAccess::IDataSource>);
		//DataAccess::RequestParameters params_to_insert()const; 
		//DataAccess::RequestParameters params_to_delete()const;
	private:
		size_t f_id,f_layer_id;
		std::string f_name;
		bool f_active;
	};
	class Layer{
	public:
		Layer(const Layer&source);
		virtual ~Layer();
		const size_t id()const;
		const size_t frame_id()const;
		const std::string&name()const;
		const double radius()const;
		const bool active()const;
		DataAccess::Factory<Slot> CreateSlotsFactory()const;
	protected:
		friend class DataAccess::Factory<Layer>;
		enum{type=DataAccess::data_layer};
		Layer(const DataAccess::DataItem&item,const std::shared_ptr<DataAccess::IDataSource>src);
		//DataAccess::RequestParameters params_to_insert()const; 
		//DataAccess::RequestParameters params_to_delete()const;
	private:
		size_t f_id,f_frame_id;
		std::string f_name;
		double f_radius;
		bool f_active;
		const std::shared_ptr<DataAccess::IDataSource> f_data_source;
	};
	class Frame{
	public:
		Frame(const Frame&source);
		virtual ~Frame();
		const size_t id()const;
		const size_t version()const;
		const std::string&description()const;
		const std::string&status()const;
		const bool active()const;
		DataAccess::Factory<Layer> CreateLayersFactory()const;
	protected:
		friend class DataAccess::Factory<Frame>;
		enum{type=DataAccess::data_frame};
		Frame(const DataAccess::DataItem&item,const std::shared_ptr<DataAccess::IDataSource>src);
		//DataAccess::RequestParameters params_to_insert()const; 
		//DataAccess::RequestParameters params_to_delete()const;
	private:
		size_t f_id,f_version;
		std::string f_dercription,f_status;
		bool f_active;
		const std::shared_ptr<DataAccess::IDataSource> f_data_source;
	};
	
};
#endif