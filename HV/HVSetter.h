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
	class IHVSetter{
	public:
		virtual ~IHVSetter(){}
		virtual double GetHV(size_t channel_no)const=0;
		virtual double GetCurent(size_t channel_no)const=0;
		virtual bool SetHV(size_t channel_no,double hv)=0;
	};
	class HVTable{
	public:
		HVTable(
			const JPetSetup::HVconfig&config,
			const JPetSetup::Setup&setup,const JPetSetup::Frame&frame,
			const JPetSetup::HighVoltage&hv_hardware,
			const std::shared_ptr<DataAccess::IDataSource> src,
			const std::shared_ptr<IHVSetter> hardware
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
		const std::vector<double>&HardwareHV()const;
		const std::vector<double>&HardwareCurent()const;
		bool SetHV(const size_t index,const double hv);
		void SynchroHardwarewithDB();
		void read_hardware();
		void clear();
	private:
		void read();
		void update();
		JPetSetup::HVconfig f_config;
		JPetSetup::Setup f_setup;
		JPetSetup::Frame f_frame;
		JPetSetup::HighVoltage f_hv_hardware;
		JPetSetup::HVPMConnections f_pmhv_conn;
		JPetSetup::Photomultipliers f_photomultipliers;
		MathTemplates::SortedChain<Item> f_items;
		std::vector<JPetSetup::HVconfigEntry> f_hv_values;
		std::vector<double> f_hv_from_hw;
		std::vector<double> f_i_from_hw;
		std::shared_ptr<IHVSetter> f_hardware;
	};
}
#endif