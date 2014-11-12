########################################################
#  
#  This is a CMake configuration file.
#  To use it you need CMake which can be 
#  downloaded from here: 
#    http://www.cmake.org/cmake/resources/software.html
#
#########################################################

cmake_minimum_required( VERSION 2.8 )

PROJECT(xlsx)

# Source code

find_package(Qt5Core REQUIRED)



set(CMAKE_AUTOMOC ON)

SET(XLSX_HEADERS xlsxdocument.h)

SET(XLSX_SOURCES
  xlsxabstractooxmlfile.cpp
  xlsxabstractsheet.cpp
  xlsxcell.cpp
  xlsxcellformula.cpp
  xlsxcellrange.cpp
  xlsxcellreference.cpp
  xlsxchart.cpp
  xlsxchartsheet.cpp
  xlsxcolor.cpp
  xlsxconditionalformatting.cpp
  xlsxcontenttypes.cpp
  xlsxdatavalidation.cpp
  xlsxdocpropsapp.cpp
  xlsxdocpropscore.cpp
  xlsxdocument.cpp
  xlsxdrawing.cpp
  xlsxdrawinganchor.cpp
  xlsxformat.cpp
  xlsxmediafile.cpp
  xlsxnumformatparser.cpp
  xlsxrelationships.cpp
  xlsxrichstring.cpp
  xlsxsharedstrings.cpp
  xlsxsimpleooxmlfile.cpp
  xlsxstyles.cpp
  xlsxtheme.cpp
  xlsxutility.cpp
  xlsxworkbook.cpp
  xlsxworksheet.cpp
  xlsxzipreader.cpp
  xlsxzipwriter.cpp
)

# Build setup

ADD_DEFINITIONS(-DHAVE_CONFIG_H)

IF(MSVC)
        ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)
ENDIF(MSVC)

SET(CMAKE_INCLUDE_CURRENT_DIR 1)
# needed to make sure to not link debug libs
# against release libs and vice versa
IF(WIN32)
  SET(CMAKE_DEBUG_POSTFIX "d")
ENDIF(WIN32)

ADD_LIBRARY(${PROJECT_NAME} ${XLSX_HEADERS} ${XLSX_SOURCES} )

# Special compiler and linker flags for MSVC
if( MSVC )
	add_definitions( /DUNICODE /D_UNICODE /Zc:wchar_t- /DXLSX_NO_LIB )
	set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")
	set( CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /Oi /GL" )
	set_target_properties( ${PROJECT_NAME} PROPERTIES STATIC_LIBRARY_FLAGS "/LTCG" ) 
endif()
qt5_use_modules(${PROJECT_NAME} Core Gui OpenGL Widgets)
