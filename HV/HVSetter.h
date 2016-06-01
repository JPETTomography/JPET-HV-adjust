// this file is distributed under 
// MIT license
#ifndef ___________________HV_____SETTER_H___________
#	define ___________________HV_____SETTER_H___________
#include <memory>
#include <math_h/chains.h>
#include <JPetData/HVconfig.h>
#include <JPetData/Frames.h>
#include <JPetData/Detectors.h>
namespace HVAdjust{
	class HVTable{
	public:
		HVTable(
			const JPetSetup::HVconfig&config,
			const JPetSetup::Setup&setup,const JPetSetup::Frame&frame,
			const JPetSetup::HighVoltage&hv_hardware,
			const std::shared_ptr<DataAccess::IDataSource> src
		);
		virtual ~HVTable();
		struct Item{
			JPetSetup::Layer layer;
			JPetSetup::Slot slot;
			JPetSetup::HVPMConnection hvpm;
			JPetSetup::Photomultiplier phm;
			JPetSetup::HVChannel hvchannel;
			Item(
				const JPetSetup::Layer&,const JPetSetup::Slot&,
				const JPetSetup::HVPMConnection&,const JPetSetup::Photomultiplier&,
				const JPetSetup::HVChannel&
    			);
			Item(const Item&source);
			Item&operator=(const Item&source);
			const bool operator>(const Item&second)const;
			const bool operator<(const Item&second)const;
		};
		const MathTemplates::SortedChain<Item>&SlotInfo()const;
		const std::vector<JPetSetup::HVconfigEntry>&HVConfigEntries()const;
		bool SetHV(const size_t index,const double hv);
		void clear();
	private:
		void update();
		JPetSetup::HVconfig f_config;
		JPetSetup::Setup f_setup;
		JPetSetup::Frame f_frame;
		JPetSetup::HighVoltage f_hv_hardware;
		JPetSetup::HVPMConnections f_pmhv_conn;
		JPetSetup::Photomultipliers f_photomultipliers;
		MathTemplates::SortedChain<Item> f_items;
		std::vector<JPetSetup::HVconfigEntry> f_hv_values;
	};
}
#endif