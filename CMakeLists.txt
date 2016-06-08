cmake_minimum_required(VERSION 2.6)
project(HV-adjust)
set(CMAKE_CXX_FLAGS "-pthread -std=gnu++11")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)


add_subdirectory(math_h)
include_directories(math_h/include)
include_directories(.)

file(GLOB JPETDATA JPetData/*.cpp)
add_library(jpetdata ${JPETDATA})
target_link_libraries(jpetdata)

link_directories($ENV{HVLIBDIR}/libsy4527)
include_directories($ENV{HVLIBDIR}/libsy4527)
link_directories($ENV{HVLIBDIR}/libhv)
include_directories($ENV{HVLIBDIR}/libhv)
file(GLOB HV HV/*.cpp)
add_library(hvadjust ${HV})
target_link_libraries(hvadjust jpetdata hv CAENComm CAENVME caenhvwrapper dl)

option(tests "Compile tests." OFF) 
if(tests)
	file(GLOB TESTS tests/*.cpp)
	find_package(GTest REQUIRED)
	include_directories(${GTEST_INCLUDE_DIRS})
	add_executable(test-all-libs-here.exe ${TESTS})
	target_link_libraries(test-all-libs-here.exe jpetdata hvadjust ${GTEST_BOTH_LIBRARIES})
endif()

find_package(PQXX REQUIRED)
include_directories(${PQXX_INCLUDE_DIRS})
add_definitions(${PQXX_DEFINITIONS})

file(GLOB PGInterface Postgres/*.cpp)
add_library(pg_interface ${PGInterface})
target_link_libraries(pg_interface jpetdata ${PQXX_LIBRARIES})

file(GLOB EXAMPLES example/*.cpp)
foreach(example ${EXAMPLES})
	get_filename_component(ex_app ${example} NAME_WE)
	add_executable(${ex_app} ${example})
	target_link_libraries(${ex_app} hvadjust jpetdata pg_interface ${PQXX_LIBRARIES})
endforeach()
