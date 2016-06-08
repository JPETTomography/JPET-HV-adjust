// this file is distributed under 
// MIT license
#ifndef _______F_R_A_M_E_S____H_________
#	define _______F_R_A_M_E_S____H_________
#include <JPetData/data_access.h>
namespace JPetSetup{
	class Slot{
	public:
		Slot(const Slot&source);
		Slot&operator=(const Slot&source);
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
		Layer&operator=(const Layer&source);
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
		std::shared_ptr<DataAccess::IDataSource> f_data_source;
	};
	
	class Setup{
	public:
		Setup(const Setup&source);
		Setup&operator=(const Setup&source);
		virtual ~Setup();
		const size_t id()const;
		const size_t frame_id()const;
		const size_t highvoltage_id()const;
		const std::string&name()const;
		const std::string&description()const;
	protected:
		friend class DataAccess::Factory<Setup>;
		enum{type=DataAccess::data_setup};
		Setup(const DataAccess::DataItem&item,const std::shared_ptr<DataAccess::IDataSource>src);
		//DataAccess::RequestParameters params_to_insert()const; 
		//DataAccess::RequestParameters params_to_delete()const;
	private:
		size_t f_id,f_frame_id,f_highvoltage_id;
		std::string f_name,f_description;
	};
	class Frame{
	public:
		Frame(const Frame&source);
		Frame&operator=(const Frame&source);
		virtual ~Frame();
		const size_t id()const;
		const size_t version()const;
		const std::string&description()const;
		const std::string&status()const;
		const bool active()const;
		DataAccess::Factory<Layer> CreateLayersFactory()const;
		DataAccess::Factory<Setup> CreateSetupFactory()const;
		const std::shared_ptr<DataAccess::IDataSource> DataSource()const;
	protected:
		friend class DataAccess::Factory<Frame>;
		enum{type=DataAccess::data_frame};
		Frame(const DataAccess::DataItem&item,const std::shared_ptr<DataAccess::IDataSource>src);
		//DataAccess::RequestParameters params_to_insert()const; 
		//DataAccess::RequestParameters params_to_delete()const;
	private:
		size_t f_id,f_version;
		std::string f_description,f_status;
		bool f_active;
		std::shared_ptr<DataAccess::IDataSource> f_data_source;
	};
	class Frames:public DataAccess::Factory<Frame>{
	public:
		Frames(const std::shared_ptr<DataAccess::IDataSource> src);
		virtual ~Frames();
		const Frame ByID(const size_t id)const;
	};
};
#endif