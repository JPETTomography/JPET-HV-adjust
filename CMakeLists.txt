cmake_minimum_required(VERSION 2.6)
project(JPET-data)
if(DEFINED MATH_H_INC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=gnu++14")
    list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
    include_directories(${MATH_H_INC})
    include_directories(${CMAKE_CURRENT_SOURCE_DIR})
    file(GLOB JPETDATA JPetData/*.cpp)
    add_library(jpetdata ${JPETDATA})
    option(tests "Compile jpetdata unit-tests." OFF) 
    if(tests)
	file(GLOB TESTS JPetData-tests/*.cpp)
	find_package(GTest REQUIRED)
	include_directories(${GTEST_INCLUDE_DIRS})
	add_executable(test-jpetdata.exe ${TESTS})
	target_link_libraries(test-jpetdata.exe jpetdata ${GTEST_BOTH_LIBRARIES})
    endif()

    find_package(PQXX REQUIRED)
    include_directories(${PQXX_INCLUDE_DIRS})
    add_definitions(${PQXX_DEFINITIONS})
    file(GLOB PGInterface Postgres/*.cpp)
    add_library(pg_interface ${PGInterface})
    target_link_libraries(pg_interface jpetdata ${PQXX_LIBRARIES})

    if(NOT ${HVLIBDIR} STREQUAL "")
	link_directories(${HVLIBDIR}/libsy4527)
	include_directories(${HVLIBDIR}/libhv)
	include_directories(${HVLIBDIR}/libsy4527)
	file(GLOB SM_HV ${HVLIBDIR}/libhv/*.cpp)
	file(GLOB HV HV/*.cpp)
	add_library(hvadjust ${HV} ${SM_HV})
	target_link_libraries(hvadjust jpetdata CAENComm CAENVME caenhvwrapper dl)
    endif()

    get_directory_property(has_parent PARENT_DIRECTORY)
    if(has_parent)
	set(JPET_DATA_INC 
		${CMAKE_CURRENT_SOURCE_DIR}
		${MATH_H_INC}
		${HVLIBDIR}/libhv
		PARENT_SCOPE)
	set(JPET_DATA_LIBDIR
		${HVLIBDIR}/libhv
		${HVLIBDIR}/libsy4527
		PARENT_SCOPE)
	set(JPET_DATA_LIBS
		jpetdata pg_interface  hvadjust ${PQXX_LIBRARIES}
		CAENComm CAENVME caenhvwrapper dl
		PARENT_SCOPE)
    endif()

    file(GLOB TOOLS *.cpp)
    foreach(tool ${TOOLS})
	get_filename_component(tool_app ${tool} NAME_WE)
	add_executable(${tool_app} ${tool} config-read.cc)
	target_link_libraries(${tool_app} jpetdata pg_interface  hvadjust ${PQXX_LIBRARIES})
    endforeach()
    file(GLOB EXAMPLES example/*.cpp)
    foreach(example ${EXAMPLES})
	get_filename_component(ex_app ${example} NAME_WE)
	add_executable(example-${ex_app} ${example})
	target_link_libraries(example-${ex_app} jpetdata pg_interface  hvadjust ${PQXX_LIBRARIES})
    endforeach()
else()
    message(FATAL_ERROR "The package requires math_h subproject to add. View README")
endif()
