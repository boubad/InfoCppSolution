cmake_minimum_required(VERSION 3.1)
project(infocppsolution)
include_directories(/usr/include/eigen3)
include_directories(./infohttp)
include_directories(./couchdb)
include_directories(./InfoBaseData)
include_directories(./infodomain)
#=============
FIND_PACKAGE ( FLTK )
IF ( FLTK_FOUND )
    INCLUDE_DIRECTORIES( ${FLTK_INCLUDE_DIR} )
    LINK_DIRECTORIES(${FLTK_LIBRARY_DIR} )
ENDIF ( FLTK_FOUND )
#=========================================
if (UNIX)
set (CMAKE_CXX_FLAGS "-std=c++1z -Wall")
set (CPPREST_LIBS cpprest boost_system ssl crypto pthread)
set (INFO_SYS_LIBS  pthread)
else()
set (CPPREST_LIBS cpprest)
set (CMAKE_CXX_FLAGS "/DWIN32 /D_WINDOWS /W3 /GR /EHsc /std:c++latest")
endif()
set (INFO_LIBS infodomain couchdb infohttp InfoBaseData)
set (TEST_LIBS gtest)
set (WORK_ALL_LIBS ${CPPREST_LIBS} )
add_subdirectory(InfoBaseData)
add_subdirectory(infohttp)
add_subdirectory(couchdb)
add_subdirectory(infodomain)
add_subdirectory(gtestinfodata)

