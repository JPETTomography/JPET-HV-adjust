Set of libraries needed for HV management in J-PET
==================================================

Compiling
=========

If you have your git repository with cmake project you can add needed repositories as submodules:

	git submodule add https://github.com/alexkernphysiker/math_h.git
	git submodule add https://github.com/alexkernphysiker/JPET-Data.git
	git submodule update --init --recursive

Then add to CMakeLists.txt the following lines

	add_definitions(--std=c++17) #the most recommended compiler mode
	set(GTEST ON) #for compiling unit-tests. Requires gtest
	add_subdirectory(math_h)
	include_directories(${MATH_H_INC})
	set(HVLIBDIR "/path/to/Silvermedia/LIBS/HV")
	add_subdirectory(JPET-Data)
	include_directories(${JPET_DATA_INC})
	link_directories(${JPET_DATA_LIBDIR})



Directories with source code of libraries
=========================================

	JPetData
abstraction layer between data source and analysis layer

	HV
library for providing HV management in frames of previously described libraries

	Postgress
implementation of accessing data from postgress server in frames of previous library


Config file required for tools
==============================
The tools look for file

	connstr.txt

in current directory.
It must contain two lines

	database connection string

	HV connection string


Tools provided in this repository
=================================

Tool for applying HV config from database into HV hardware

	hv-config-apply frame setup config

Tool for exporting HV config from database into text file

	hv-config-save frame setup config filename

Tool for import HV config from text file to database

	hv-config-load frame setup config filename



SQL scripts
===========

	DB_upgrade.sql
adding tables connected with newly implemented calibration
	
	HVConfigTables_patch.sql
fix structure of tables connected with HV configuration (!!!DELETES OLD DATA)
