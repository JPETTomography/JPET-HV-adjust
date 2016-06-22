Set of libraries needed for HV management in J-PET
==================================================

Directories
===========

	JPetData - abstraction layer between data source and analysis layer

	HV - library for providing HV management in frames of previously described libraries

	Postgress - implementation of accessing data from postgress server in frames of previous library

Environment variables
=====================

	HVLIBDIR - HV directory from Silvermedia software source repository


Config file required for tools
==============================

	connstr.txt

Must contain two lines

	database connection string

	HV connection string


Tools provided in this repository
=================================

Tool for getting HV config from database and applying it into HV hardware

	hv-config-apply frame setup config


