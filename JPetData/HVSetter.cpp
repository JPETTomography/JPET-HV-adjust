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
		if(slot.name() <second.slot.name())return true;
		return false;
	}
	const bool HVTable::Item::operator>(const HVTable::Item& second) const{
		if(hvpm.side() >second.hvpm.side())return true;
		if(hvpm.side() <second.hvpm.side())return false;
		if(layer.name()>second.layer.name())return true;
		if(layer.name()<second.layer.name())return false;
		if(slot.name() >second.slot.name())return true;
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
		auto hvchannels_cache = f_hv_hardware.CreateChannelsFactory().SelectAll();
		f_items.clear();
		for(const Layer& layer:f_frame.CreateLayersFactory().SelectAll())
			for(const Slot&slot:layer.CreateSlotsFactory().SelectAll())
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
		auto entries_cache=f_config.CreateEntriesFactory().SelectAll();
		for(const Item&item:f_items){
			HVconfigEntry Entry(item.hvpm.id(),INFINITY);
			for(const HVconfigEntry&entry:entries_cache)
				if(entry.HVPMConnection_id()==item.hvpm.id())
					Entry=entry;
			f_hv_values.push_back(Entry);
		}
	}
	void HVTable::read_hardware(){
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
			auto entries=f_config.CreateEntriesFactory();
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
			if(isfinite(HVConfigEntries()[i].HV()))
				f_hardware->SetHV(SlotInfo()[i].hvchannel.idx(),HVConfigEntries()[i].HV());
		read_hardware();
	}
	void HVTable::clear(){
		{
			auto entries=f_config.CreateEntriesFactory();
			for(const HVconfigEntry&entry:entries.SelectAll())
				entries.Delete(entry);
		}
		read();
	}
}