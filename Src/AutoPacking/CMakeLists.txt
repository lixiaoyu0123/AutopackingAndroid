########################################################
#  
#  This is a CMake configuration file.
#  To use it you need CMake which can be 
#  downloaded from here: 
#    http://www.cmake.org/cmake/resources/software.html
#
#########################################################

cmake_minimum_required( VERSION 2.8.9 )

#############################################################################

# Print a message and fail for people who don't
# read the build instructions and then complain
# when the build process fails for them.
if ( ${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR} )
    message( FATAL_ERROR "You are trying to run CMake from the <top_folder>/src/iEditor directory, "
                         "instead of just from the <top_folder> directory.\nDO NOT DO THIS.\n"
                         "The correct way looks like this:\n"
                         "  cmake -G '<generator_name>' /path/to/topmost/folder/in/source/package\n"
                         "You will probably now need to first clean your build directory." )
endif()

#############################################################################

# We use the lower case name
# on UNIX systems other than Mac OS X

project( Autopacking )

#############################################################################

# Qt5 packages
find_package(Qt5Core REQUIRED)
find_package(Qt5Network)
find_package(Qt5Svg)
find_package(Qt5WebKit)
find_package(Qt5WebKitWidgets)
find_package(Qt5Widgets)
find_package(Qt5Xml)
find_package(Qt5XmlPatterns)
find_package(Qt5Concurrent)
find_package(Qt5PrintSupport)
find_package(Qt5LinguistTools)
find_package(Qt5Sensors)
find_package(Qt5Sql)
set(CMAKE_AUTOMOC ON)

set( MAIN_FILES
    Main/main.cpp
    )
	
set( MAINUI_WINDOW
	MainWindow/mainwindow.cpp
	MainWindow/mainwindow.h
	MainWindow/BjToolBar.cpp
	MainWindow/BjToolBar.h
	MainWindow/BjCentralFram.cpp
	MainWindow/BjCentralFram.h
	MainWindow/BjStatusBar.cpp
	MainWindow/BjStatusBar.h
	MainWindow/BjTableView.cpp
	MainWindow/BjTableView.h
	)
	
set( UI_FILES
	MainUi/mainwindow.ui
	MainUi/About.ui
	MainUi/ImportData.ui
	MainUi/ExportData.ui
	MainUi/SetKey.ui
	MainUi/SetJdkPath.ui
	MainUi/SourcePackSetting.ui
	MainUi/DecPackSetting.ui
	MainUi/LogDialog.ui
	MainUi/EditTable.ui
	MainUi/ThreadConfigDialog.ui
	MainUi/SetVersionDialog.ui
	)

set( MODEL_FILES
	Model/DatabaseManager.h
	Model/DatabaseManager.cpp
	Model/PathManager.cpp
	Model/PathManager.h
	Model/BjTableDelegate.cpp
	Model/BjTableDelegate.h
	Model/BjIdDelegate.cpp
	Model/BjIdDelegate.h
	Model/BjEditDelegate.cpp
	Model/BjEditDelegate.h
	Model/BjTableModel.cpp
	Model/BjTableModel.h
	)
	
set( DIALOG_FILES
	Dialogs/About.cpp
	Dialogs/About.h
	Dialogs/BjMessageBox.cpp
	Dialogs/BjMessageBox.h
	Dialogs/ImportData.cpp
	Dialogs/ImportData.h
	Dialogs/ExportData.cpp
	Dialogs/ExportData.h
	Dialogs/SetJdkPath.cpp
	Dialogs/SetJdkPath.h
	Dialogs/SetKey.cpp
	Dialogs/SetKey.h
	Dialogs/SourcePackSetting.cpp
	Dialogs/SourcePackSetting.h
	Dialogs/DecPackSetting.cpp
	Dialogs/DecPackSetting.h
	Dialogs/LogDialog.cpp
	Dialogs/LogDialog.h
	Dialogs/EditTable.cpp
	Dialogs/EditTable.h
	Dialogs/ThreadConfigDialog.cpp
	Dialogs/ThreadConfigDialog.h
	Dialogs/SetVersionDialog.cpp
	Dialogs/SetVersionDialog.h
	)
set( QRC_FILES  
    Resource/About/About.qrc
	Resource/Icon/main.qrc
    )
set( WORKS_FILES  
	Works/Pack.cpp
	Works/Pack.h
    Works/DecPack.cpp
	Works/DecPack.h
	Works/SrcPack.cpp
	Works/SrcPack.h
    )
set( INTERFACE_FILES  
    Interface/ITableContral.h
    )
set( ADAPTER_FILES  
	Adapter/TableContralAdapter.cpp
	Adapter/TableContralAdapter.h
	Adapter/TableContralAdapterStr.cpp
	Adapter/TableContralAdapterStr.h
	Adapter/TableContralAdapterRes.cpp
	Adapter/TableContralAdapterRes.h
	Adapter/TableContralAdapterPak.cpp
	Adapter/TableContralAdapterPak.h
    )
set( TABLE_FILES 
	Table/ReplaceStrTable.cpp
	Table/ReplaceStrTable.h
	Table/ReplaceResTable.cpp
	Table/ReplaceResTable.h
	Table/ReplacePakTable.cpp
	Table/ReplacePakTable.h
	)
set( RAW_SOURCES ${MAIN_FILES} ${MAINUI_WINDOW} ${MODEL_FILES} ${DIALOG_FILES} ${WORKS_FILES} ${INTERFACE_FILES} ${ADAPTER_FILES} ${TABLE_FILES})

#############################################################################

# Runs UIC on specified files
qt5_wrap_ui( UI_FILES_H ${UI_FILES} )
qt5_add_resources( QRC_FILES_CPP ${QRC_FILES} )
set_source_files_properties( Dialogs/About.cpp PROPERTIES COMPILE_DEFINITIONS TOOLS_FULL_VERSION="${TOOLS_FULL_VERSION}" )

#############################################################################

# Adds folders for Visual Studio solution explorer (and for Xcode explorer)
source_group( "MainUi"           FILES ${UI_FILES} )
source_group( "MainWindow"       FILES ${MAINUI_WINDOW} )
source_group( "Main"             FILES ${MAIN_FILES} )
source_group( "Model"            FILES ${MODEL_FILES} )
source_group( "Dialogs"          FILES ${DIALOG_FILES} )
source_group( "Works"            FILES ${WORKS_FILES} )
source_group( "Interface"        FILES ${INTERFACE_FILES} )
source_group( "Adapter"          FILES ${ADAPTER_FILES} )
source_group( "Table"			 FILES ${TABLE_FILES} )

#############################################################################

set( ALL_SOURCES ${RAW_SOURCES} ${UI_FILES_H} ${QRC_FILES_CPP} ${WORKS_FILES})

# Adding resource (RC) files for Windows

if ( WIN32 )
    configure_file( Resource/Windows/version.rc.in ${PROJECT_BINARY_DIR}/version.rc )
set( WINDOWS_RC_FILES
    Resource/Icon/icon.rc
    ${PROJECT_BINARY_DIR}/version.rc
    )
list( APPEND ALL_SOURCES ${WINDOWS_RC_FILES} )
source_group( "Resources" FILES {$WINDOWS_RC_FILES} )
endif()

#############################################################################
#############################################################################

# We need to pick up the ui*.h files,
# and the headers for the linked-to libraries.
# The directories are in reverse order because we're using before to
# put our include dirs before any system ones.
#############################################################################
include_directories( BEFORE
                     ${CMAKE_CURRENT_SOURCE_DIR}
                     ${CMAKE_CURRENT_BINARY_DIR} )
					 
add_executable( ${PROJECT_NAME} WIN32 ${ALL_SOURCES} )

if (MSVC)
    set(QT_MAIN Qt5::WinMain)
endif()

target_link_libraries( ${PROJECT_NAME} ${QT_MAIN} )
qt5_use_modules(${PROJECT_NAME} Widgets Xml XmlPatterns PrintSupport Svg WebKit WebKitWidgets Network Concurrent Sql)

#############################################################################


#############################################################################

# "Link time code generation" flags for MSVC
if( MSVC )
    add_definitions( /DUNICODE /D_UNICODE )
    # The /Zc:wchar_t- flag can't go into add_definitions
    # because the RC compiler picks it up too and it provokes a name clash
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:wchar_t- /MP")
    set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Oi /GL" ) 
    set( CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /LTCG" )

# "Print all warnings" flag for GCC
elseif( CMAKE_COMPILER_IS_GNUCXX )
    add_definitions( -Wall )
endif()
#############################################################################
