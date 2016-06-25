DEVELOPERS GUIDE
================


library jpetdata - general data access
=======================================

	database.h

This file contains the declaration of the abstraction layer between analysis
parametric objects and the access to the data source (f.eg. database).

The enums 'datatype' and 'operationtype' are used to encode the interaction
with data source wrapper implementing the interface 'IDataSource'.
'DataItem' is the internal readonly representation of abstract single data
item obtained from the source.
'DataSet' is the representation of a set of such data items with the same type.

The template class 'DataTableInterface' provides the access to particular
types of parametric data defined in J-PET project. 
The only parameter of this template is the name of class representing this
particular data type.
The class for particular data type can be implemented arbitarilly but must
contain the following declarations:

	class ParticularDataRepresenter{
	public:
		virtual ~ParticularDataRepresenter();
		
		//For copying
		ParticularDataRepresenter(const ParticularDataRepresenter&source);
		ParticularDataRepresenter&operator=(const ParticularDataRepresenter&source);
		
		//For creating new data item you must implement constructor with parameters
		//defined by you. It's called in your higher-level code
		ParticularDataRepresenter(......);
		
		//ToDo: declare public methods for read accessing the properties
		.............
	protected:
		//For DataTableInterface could call these methods
		//declared in this 'protected; section
		//but they stayed closed for your higher-level code
		friend class DataAccess::DataTableInterface<ParticularDataRepresenter>;
		
		//Here you insert value of enum 'datatype' 
		//meaning the request to data of this type
		enum{type=DataAccess::<request_type>};
		
		//Creating the instance from DataItem
		//This constructor will be called by DataTableInterface
		ParticularDataRepresenter(const DataAccess::DataItem&item,const std::shared_ptr<DataAccess::IDataSource>);
		
		// parameters transferred to data source for inserting new data item
		// if it's not declared calling Add method of
		// corresponding DataTableInterface won't compile
		DataAccess::RequestParameters params_to_insert()const; 
		
		// parameters transferred to data source for deleting current data item
		// if it's not declared calling Delete method of
		// corresponding DataTableInterface won't compile
		DataAccess::RequestParameters params_to_delete()const;
	private:
		............
	};

'DataTableInterface' performs the access to corresponding type of data from the data source.
It's constructor requires the data source and set of parameters transferred to it for every request.
(when sending 'add' or 'delete' requests, parameters obtained from corresponding representer's
methods are appended to the "common" list)

'DataTableInterface' caches the set of 'DataItem' instances and provides methods of creating 
the instances of higher-level representing classes for the particular data type.
For creating instances of representing higher-level class for set of obtained 'DataItem' 
instances there are SelectAll() and Select(...) methods.
The first one returns vector of higher-level representations for all data obtained.
The second one require a function or lambda providing the condition on 'DataItem' instances
we are interested in.
Both these methods return vector of representer instances.

Method Add(...) requires as a parameter the instance of representer created 
by the "higher-level" constructor and writes it into the data source.

Method Delete(...) requires as a parameter the instance of representer
obtained from current 'DataTableInterface' instance and removes this 
item from data source.

library jpetdata - implemented particular data types
====================================================

	Detectors.h

Class 'Photomultipliers' provides interface to the table of photomultipliers.
It's constructor requires the data source.
Single photomultiplier is represented as 'Photomultiplier' class instance.

	Frames.h

Class 'Frames' provides interface to the table of J-PET frames.
Single frame is represented by the class 'Frame'.
It has methods CreateLayersInterface() and CreateSetupInterface that created
interfaces for the tables of J-PET layers and setups respectively.
All these interfaces are looking only for the records linked to current frame.

class 'Layer' has also method CreateSlotsInterface() creating the interface
to slots table for looking for slots in curent layer.


	HVconfig.h

'HighVoltageTable' provides interface to the table of high voltage hardware units.
It's constructor requires the data source.
Single instance is represented by the class 'HighVoltage'.
It's method CreateChannelsInterface() creates interface to the table of HV chnnels
owned by curent HV hardware unit.
Single channel is represented by the class 'HVChannel'

'HVPMConnections' provides interface to the table of connections between 
photomultipliers slots and HV channels with linking to J-PET setup.
It has methods for selecting set of connection by ids of all these types of objects.
Single connection is represented by 'HVPMConnection'.

'HVconfigTable' provides interface to the table of HV configurations.
It's constructor requires data source and J-PET setup id.
Single configuration is represented by 'HVconfig' that has method
CreateEntriesInterface() that creates interface to the table of entries connecting
the configuration with HVPMConnections and store used HV values.
