DEVELOPERS GUIDE
================


library jpetdata
================

	database.h

This file contains the declaration of the abstraction level between analysis
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


