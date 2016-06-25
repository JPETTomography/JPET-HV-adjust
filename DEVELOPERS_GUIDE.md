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
	
		ParticularDataRepresenter(const ParticularDataRepresenter&source);
		
		ParticularDataRepresenter&operator=(const ParticularDataRepresenter&source);
		
		virtual ~ParticularDataRepresenter();
		
		//ToDo: declare public methods for read accessing the properties
		
	protected:
	
		friend class DataAccess::DataTableInterface<ParticularDataRepresenter>;
		
		//value of enum 'datatype' meaning the request to data of this type
		
		enum{type=DataAccess::<request_type>};
		
		//Creating the instance from DataItem (by 'DataTableInterface')
		
		ParticularDataRepresenter(const DataAccess::DataItem&item,const std::shared_ptr<DataAccess::IDataSource>);
		
		// parameters transferred to data source for inserting new data item
		
		DataAccess::RequestParameters params_to_insert()const; 
		
		// parameters transferred to data source for deleting current data item
		
		DataAccess::RequestParameters params_to_delete()const;
		
	private:
		
		//.........
	};

