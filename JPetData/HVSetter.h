// this file is distributed under 
// MIT license
#ifndef ___________________HV_____SETTER_H___________
#	define ___________________HV_____SETTER_H___________
#include <memory>
#include <fstream>
#include <math_h/tabledata.h>
#include <JPetData/HVconfig.h>
#include <JPetData/Frames.h>
#include <JPetData/Detectors.h>
namespace HVAdjust{
	class IHVSetter{
	public:
		virtual ~IHVSetter(){}
		virtual void UpdateRequest()=0;
		virtual bool IsOn(size_t channel_no)const=0;
		virtual double GetHV(size_t channel_no)const=0;
		virtual double GetHVMon(size_t channel_no)const=0;
		virtual void turnOn(size_t channel_no)=0;
		virtual void turnOff(size_t channel_no)=0;
		virtual void SetHV(size_t channel_no,double hv)=0;
	};
	class DummyHV:public IHVSetter{
	public:
		DummyHV();
		virtual ~DummyHV();
		virtual void UpdateRequest()override;
		virtual bool IsOn(size_t channel_no)const override;
		virtual double GetHV(size_t channel_no)const override;
		virtual double GetHVMon(size_t channel_no)const override;
		virtual void turnOn(size_t channel_no)override;
		virtual void turnOff(size_t channel_no)override;
		virtual void SetHV(size_t channel_no,double hv)override;
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
		const std::vector<double>&HVMon()const;
		bool SetHV(const size_t index,const double hv);
		void SynchroHardwarewithDB();
		void SwitchOffHardware();
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
		std::vector<double> f_hv_from_hw_mon;
		std::shared_ptr<IHVSetter> f_hardware;
	};
	void SaveHV(std::ofstream&stream,const HVTable&table);
	void ReadHV(std::ifstream&stream,      HVTable&table);
}
#endif
