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
			JPetSetup::HVconfigEntry entry;
			Item(const JPetSetup::Layer&,const JPetSetup::Slot&,const JPetSetup::HVPMConnection&,const JPetSetup::Photomultiplier&,const JPetSetup::HVconfigEntry&);
			Item(const Item&source);
			Item&operator=(const Item&source);
			const bool operator>(const Item&second)const;
			const bool operator<(const Item&second)const;
		};
		const size_t size()const;
		typedef MathTemplates::SortedChain<Item>::const_iterator const_iterator;
		const_iterator begin()const;
		const_iterator cbegin()const;
		const_iterator end() const;
		const_iterator cend() const;
		const Item&operator[](const size_t index)const;
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
	};
}
#endif