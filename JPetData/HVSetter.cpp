// this file is distributed under 
// MIT license
#include <math.h>
#include <math_h/error.h>
#include <JPetData/HVSetter.h>
using namespace std;
using namespace DataAccess;
using namespace JPetSetup;
using namespace MathTemplates;
namespace HVAdjust{
	
	DummyHV::DummyHV(){}
	DummyHV::~DummyHV(){}
	double DummyHV::GetHV(size_t channel_no) const{return INFINITY;}
	bool DummyHV::IsOn(size_t channel_no)const{return false;}
	void DummyHV::UpdateRequest(){}
	void DummyHV::SetHV(size_t channel_no, double hv){}
	void DummyHV::turnOn(size_t channel_no){}
	void DummyHV::turnOff(size_t channel_no){}
	
	HVTable::Item::Item(const Layer&l, const Slot&s, const HVPMConnection&hvconn, const Photomultiplier&phm, const HVChannel&ch)
	:layer(l),slot(s),hvpm(hvconn),phm(phm),hvchannel(ch){}
	HVTable::Item::Item(const HVTable::Item& source)
	:layer(source.layer),slot(source.slot),hvpm(source.hvpm),phm(source.phm),hvchannel(source.hvchannel){}
	HVTable::Item& HVTable::Item::operator=(const HVTable::Item& source){
		layer=source.layer;
		slot=source.slot;
		hvpm=source.hvpm;
		phm=source.phm;
		hvchannel=source.hvchannel;
		return *this;
	}
	const bool HVTable::Item::operator<(const HVTable::Item& second) const{
		if(hvpm.side() <second.hvpm.side())return true;
		if(hvpm.side() >second.hvpm.side())return false;
		if(layer.name()<second.layer.name())return true;
		if(layer.name()>second.layer.name())return false;
		string s1=slot.name(),s2=second.slot.name();
		while(s1.length()<7)s1="0"+s1;
		while(s2.length()<7)s2="0"+s2;
		if(s1<s2)return true;
		return false;
	}
	const bool HVTable::Item::operator>(const HVTable::Item& second) const{
		if(hvpm.side() >second.hvpm.side())return true;
		if(hvpm.side() <second.hvpm.side())return false;
		if(layer.name()>second.layer.name())return true;
		if(layer.name()<second.layer.name())return false;
		string s1=slot.name(),s2=second.slot.name();
		while(s1.length()<7)s1="0"+s1;
		while(s2.length()<7)s2="0"+s2;
		if(s1>s2)return true;
		return false;
	}

	
	HVTable::HVTable(
		const HVconfig&config,const Setup& setup,const Frame& frame,
		const HighVoltage&hv_hardware,const shared_ptr<IDataSource>src,
		const shared_ptr<IHVSetter>hw
	):f_config(config),f_setup(setup),f_frame(frame),
	f_hv_hardware(hv_hardware),f_pmhv_conn(src),
	f_photomultipliers(src),f_hardware(hw){
		if(setup.frame_id()!=frame.id())throw Exception<HVTable>("Frame does not match Setup");
		if(config.setup_id()!=setup.id())throw Exception<HVTable>("HVconfig does not match Setup");
		if(setup.highvoltage_id()!=hv_hardware.id())throw Exception<HVTable>("Setup does not match HighVoltage");
		read();
	}
	HVTable::~HVTable(){}
	void HVTable::read(){
		auto hvchannels_cache = f_hv_hardware.CreateChannelsInterface().SelectAll();
		f_items.clear();
		for(const Layer& layer:f_frame.CreateLayersInterface().SelectAll())
			for(const Slot&slot:layer.CreateSlotsInterface().SelectAll())
				for(const HVPMConnection&conn:f_pmhv_conn.BySlotID(slot.id()))
					if(conn.setup_id()==f_setup.id())
						for(const HVChannel&channel:hvchannels_cache)
							if(channel.id()==conn.hvchannel_id())
								f_items<<Item(
									layer,slot,conn,
									f_photomultipliers.ByID(conn.photomultiplier_id()),
									channel
								);
		update();
		read_hardware();
	}
	void HVTable::update(){
		f_hv_values.clear();
		auto entries_cache=f_config.CreateEntriesInterface().SelectAll();
		for(const Item&item:f_items){
			HVconfigEntry Entry(item.hvpm.id(),INFINITY);
			for(const HVconfigEntry&entry:entries_cache)
				if(entry.HVPMConnection_id()==item.hvpm.id())
					Entry=entry;
			f_hv_values.push_back(Entry);
		}
	}
	void HVTable::read_hardware(){
		f_hardware->UpdateRequest();
		f_hv_from_hw.clear();
		for(const Item&item:SlotInfo()){
			f_hv_from_hw.push_back(f_hardware->GetHV(item.hvchannel.idx()));
		}
	}
	const SortedChain<HVTable::Item>& HVTable::SlotInfo() const{
		return f_items;
	}
	const vector<HVconfigEntry>& HVTable::HVConfigEntries() const{
		return f_hv_values;
	}
	const vector<double>& HVTable::HardwareHV() const{
		return f_hv_from_hw;
	}
	bool HVTable::SetHV(const size_t index, const double hv){
		if(index>=f_items.size())return false;
		if(hv*f_items[index].phm.max_hv()<0.0)return false;
		if(hv/f_items[index].phm.max_hv()>1.0)return false;
		bool res=false;
		{
			vector<HVconfigEntry> tmp;
			auto entries=f_config.CreateEntriesInterface();
			for(const HVconfigEntry&entry:entries.SelectAll())
				if(entry.HVPMConnection_id()==f_items[index].hvpm.id())
					tmp.push_back(entry);
			for(auto&item:tmp)
				if(!entries.Delete(item))
					return false;
			res=entries.Add(HVconfigEntry(f_items[index].hvpm.id(),hv));
		}
		update();
		return res;
	}
	void HVTable::SynchroHardwarewithDB(){
		for(size_t i=0; i<SlotInfo().size();i++)
			if(isfinite(HVConfigEntries()[i].HV())){
				f_hardware->SetHV(SlotInfo()[i].hvchannel.idx(),HVConfigEntries()[i].HV());
				f_hardware->turnOn(SlotInfo()[i].hvchannel.idx());
			}else{
				f_hardware->turnOff(SlotInfo()[i].hvchannel.idx());
			}
		read_hardware();
	}
	void HVTable::SwitchOffHardware(){
		for(size_t i=0; i<SlotInfo().size();i++)
			f_hardware->turnOff(SlotInfo()[i].hvchannel.idx());
		read_hardware();
	}
	void HVTable::clear(){
		{
			auto entries=f_config.CreateEntriesInterface();
			for(const HVconfigEntry&entry:entries.SelectAll())
				entries.Delete(entry);
		}
		read();
	}
}