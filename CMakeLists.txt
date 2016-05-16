cmake_minimum_required(VERSION 2.6)
project(HV-adjust)
set(CMAKE_CXX_FLAGS "-pthread -std=gnu++11")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
# foreach(mode QUIET REQUIRED)
#   find_package(ROOT 5 ${mode} COMPONENTS
#     Hist
#     Physics
#     RIO
#     Thread
#     Tree
#     )
#   if(ROOT_USE_FILE)
#     include(${ROOT_USE_FILE})
#   endif()
#   if(ROOT_FOUND)
#     break()
#   endif()
#   list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/fallback)
# endforeach()
# include_directories(${ROOT_INCLUDE_DIRS})
# add_definitions(${ROOT_DEFINITIONS})
# message(STATUS "ROOT version: ${ROOT_VERSION}")
# if(ROOT_FOUND AND NOT ROOT_VERSION VERSION_LESS "6.0")
#   message(FATAL_ERROR "ROOT 6.0 is not compatible")
# endif()

find_package(PQXX REQUIRED)
include_directories(${PQXX_INCLUDE_DIRS})
add_definitions(${PQXX_DEFINITIONS})

add_subdirectory(math_h)
include_directories(math_h/include)
include_directories(.)

file(GLOB DATA DataGeneral/*.cpp)
add_library(dataaccess ${DATA})

file(GLOB JPETDATA JPetData/*.cpp)
add_library(jpetdata ${JPETDATA})
target_link_libraries(jpetdata dataaccess)

file(GLOB HV HV/*.cpp)
add_library(hvadjust ${HV})
target_link_libraries(hvadjust jpetdata dataaccess)


file(GLOB PGInterface Postgres/*.cpp)
add_library(pg_interface ${PGInterface})
target_link_libraries(pg_interface ${PQXX_LIBRARIES})

file(GLOB LOCAL_R local_routines/*.cpp)
add_library(local_routines ${LOCAL_R})
target_link_libraries(local_routines hvadjust dataaccess jpetdata)


file(GLOB EXAMPLES example/*.cpp)
foreach(example ${EXAMPLES})
	get_filename_component(ex_app ${example} NAME_WE)
	add_executable(${ex_app} ${example})
	target_link_libraries(${ex_app} hvadjust dataaccess jpetdata pg_interface local_routines ${PQXX_LIBRARIES})
endforeach()

option(tests "Compile tests." OFF) 
if(tests)
	file(GLOB TESTS tests/*.cpp)
	find_package(GTest REQUIRED)
	include_directories(${GTEST_INCLUDE_DIRS})
	add_executable(test-all-libs-here.exe ${TESTS})
	target_link_libraries(test-all-libs-here.exe hvadjust dataaccess jpetdata ${GTEST_BOTH_LIBRARIES})
endif()