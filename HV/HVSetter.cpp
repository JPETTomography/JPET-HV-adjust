// this file is distributed under 
// MIT license
#include <math.h>
#include <HV/HVSetter.h>
using namespace std;
using namespace DataAccess;
using namespace JPetSetup;
namespace HVAdjust{
	HVTable::Item::Item(const Layer&l, const Slot&s, const HVPMConnection&h, const Photomultiplier&p, const HVconfigEntry&e)
	:layer(l),slot(s),hvpm(h),phm(p),entry(e){}
	HVTable::Item::Item(const HVTable::Item& source)
	:layer(source.layer),slot(source.slot),hvpm(source.hvpm),phm(source.phm),entry(source.entry){}
	HVTable::Item& HVTable::Item::operator=(const HVTable::Item& source){
		layer=source.layer;
		slot=source.slot;
		hvpm=source.hvpm;
		phm=source.phm;
		entry=source.entry;
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

	
	HVTable::HVTable(const HVconfig&config,const Setup& setup,const Frame& frame,const HighVoltage&hv_hardware,const shared_ptr<IDataSource>src)
	:f_config(config),f_setup(setup),f_frame(frame),f_hv_hardware(hv_hardware),f_pmhv_conn(src),f_photomultipliers(src){
		
		update();
	}
	HVTable::~HVTable(){}
	void HVTable::update(){
		f_items.clear();
		auto f_entries_cache=f_config.CreateEntriesFactory().GetList();
		for(const Layer& layer:f_frame.CreateLayersFactory().GetList())
			for(const Slot&slot:layer.CreateSlotsFactory().GetList())
				for(const HVPMConnection&conn:f_pmhv_conn.BySlotID(slot.id()))
					if(conn.setup_id()==f_setup.id()){
						HVconfigEntry hventry(conn.id(),nan(""));
						for(const HVconfigEntry&item:f_entries_cache)
							if(item.HVPMConnection_id()==conn.id())
								hventry=item;
						f_items<<Item(layer,slot,conn,f_photomultipliers.ByID(conn.photomultiplier_id()),hventry);
					}
	}
	const size_t HVTable::size()const{return f_items.size();}
	HVTable::const_iterator HVTable::begin() const{return f_items.begin();}
	HVTable::const_iterator HVTable::cbegin() const{return f_items.cbegin();}
	HVTable::const_iterator HVTable::end() const{return f_items.end();}
	HVTable::const_iterator HVTable::cend() const{return f_items.cend();}
	const HVTable::Item& HVTable::operator[](const size_t index)const{return f_items[index];}
	bool HVTable::SetHV(const size_t index, const double hv){
		if(index>=size())return false;
		if(hv*f_items[index].phm.max_hv()<0.0)return false;
		if(hv/f_items[index].phm.max_hv()>1.0)return false;
		bool res=false;
		{
			vector<HVconfigEntry> tmp;
			auto entries=f_config.CreateEntriesFactory();
			for(const HVconfigEntry&entry:entries.GetList())
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
	void HVTable::clear(){
		{
			auto entries=f_config.CreateEntriesFactory();
			for(const HVconfigEntry&entry:entries.GetList())
				entries.Delete(entry);
		}
		update();
	}
}