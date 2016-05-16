// this file is distributed under 
// MIT license
#ifndef ___________________HV_____SETTER_H___________
#	define ___________________HV_____SETTER_H___________
#include <memory>
#include <JPetData/HVconfig.h>
#include <JPetData/Frames.h>
#include <JPetData/Detectors.h>
namespace HVAdjust{
	class IHVSetter{
	public:
		virtual ~IHVSetter(){}
		virtual bool SetHV(const size_t channel,const double hv)=0;
	};
	class HVTable{
	public:
		HVTable(
			const JPetSetup::HVconfig&config,
			const JPetSetup::Setup&setup,const JPetSetup::Frame&frame,
			const std::shared_ptr<DataAccess::IDataSource> src
		);
		virtual ~HVTable();
		struct Item{
			JPetSetup::Layer layer;
			JPetSetup::Slot slot;
			JPetSetup::HVPMConnection hvpm;
			JPetSetup::Photomultiplier phm;
			JPetSetup::HVconfigEntry entry;
		};
		const size_t size()const;
		const Item&operator[](const size_t index)const;
		bool SetHV(const size_t index, const double hv);
	private:
		void update();
		JPetSetup::HVconfig f_config;
		JPetSetup::Setup f_setup;
		JPetSetup::Frame f_frame;
		JPetSetup::HVPMConnections f_pmhv_conn;
		JPetSetup::Photomultipliers f_photomultipliers;
		std::vector<Item> f_items;
	};
}
#endif