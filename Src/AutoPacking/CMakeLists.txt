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
qt5_use_modules(${PROJECT_NAME} Widgets Sql)

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
if (MSVC)
    get_target_property(QMAKE_EXECUTABLE Qt5::qmake LOCATION)
    function(QUERY_QMAKE VAR RESULT)
        exec_program(${QMAKE_EXECUTABLE} ARGS "-query ${VAR}" RETURN_VALUE return_code OUTPUT_VARIABLE output )
        if(NOT return_code)
            file(TO_CMAKE_PATH "${output}" output)
            set(${RESULT} ${output} PARENT_SCOPE)
        endif(NOT return_code)
    endfunction(QUERY_QMAKE)

    query_qmake(QT_INSTALL_LIBS QT_LIBRARY_DIR)
    query_qmake(QT_INSTALL_BINS QT_INSTALL_BINS)
    query_qmake(QT_INSTALL_PLUGINS QT_PLUGINS_DIR)

    set( TEMP_PACKAGE_DIR ${CMAKE_BINARY_DIR}/temp_folder )
    set( MAIN_PACKAGE_DIR ${TEMP_PACKAGE_DIR}/AutopackingAndroid)
    set( OUTPUT_PACKAGE_DIR ${CMAKE_BINARY_DIR}/installer )
    
    # ISS conf file for the Inno Setup compiler
    # We first create a CMake configured version of the ISS file,
    # and then we copy it to the temp folder every time we need to build the installer.
    set( ISS_MAIN_LOCATION ${CMAKE_SOURCE_DIR}/installer/AutopackingAndroid.iss  )
    set( ISS_CONFIGURED_LOCATION ${CMAKE_BINARY_DIR}/AutopackingAndroid.iss )
    set( ISS_TEMP_LOCATION ${CMAKE_BINARY_DIR}/temp_folder/AutopackingAndroid.iss )

    # Specify platform var for Inno
    if ( 64_BIT_PLATFORM )
        # Used in the ISS CMake configuration
        set( ISS_ARCH "x64" )
        set( ISS_SETUP_FILENAME_PLATFORM "-x64" )
    endif()
    
    # Creates a copy of the ISS file in ${ISS_CONFIGURED_LOCATION} and then configures it 
    # Used in the ISS CMake configuration
    set( LICENSE_LOCATION ${CMAKE_SOURCE_DIR}/installer/win_installer_note.txt  )
    configure_file( ${ISS_MAIN_LOCATION} ${ISS_CONFIGURED_LOCATION} )

    # Run Inno Setup's iscc compiler (*AFTER* all the PRE_BUILD custom commands execute)
    add_custom_target(  makeinstaller 
        COMMAND cmake -E echo "For this to work, Inno Setup's iscc compiler needs to be installed and on the system path."
        COMMAND iscc ${ISS_TEMP_LOCATION} )
	if( MSVC_IDE )
    set( TARGET_FOR_COPY makeinstaller )
    # Copy ISS file to temp folder location
    add_custom_command( TARGET ${TARGET_FOR_COPY} PRE_BUILD 
                        COMMAND cmake -E copy ${ISS_CONFIGURED_LOCATION} ${ISS_TEMP_LOCATION} ) 
	endif()

	# Copy Qt runtime libs
    set( QT_LIBS Qt5Core Qt5Quick Qt5Sql Qt5Widgets )
    add_custom_command( TARGET ${TARGET_FOR_COPY} PRE_BUILD COMMAND cmake -E make_directory ${MAIN_PACKAGE_DIR}/ )    
    foreach( lib ${QT_LIBS} )
        set( location location-NOTFOUND )
        find_file( location ${lib}.dll PATHS ${QT_INSTALL_BINS} )
        add_custom_command( TARGET ${TARGET_FOR_COPY} PRE_BUILD COMMAND cmake -E copy ${location} ${MAIN_PACKAGE_DIR}/ )
    endforeach( lib )

	# Copy bin libs
    set( QT_BIN_LIBS D3DCompiler_43 )
    add_custom_command( TARGET ${TARGET_FOR_COPY} PRE_BUILD COMMAND cmake -E make_directory ${MAIN_PACKAGE_DIR}/ )    
    foreach( lib ${QT_BIN_LIBS} )
        set( location location-NOTFOUND )
        find_file( location ${lib}.dll PATHS ${QT_INSTALL_BINS} )
        add_custom_command( TARGET ${TARGET_FOR_COPY} PRE_BUILD COMMAND cmake -E copy ${location} ${MAIN_PACKAGE_DIR}/ )
    endforeach( lib )

    # Copy iconengines plugins
    set( QT_ICONENGINES qsvgicon )
    add_custom_command( TARGET ${TARGET_FOR_COPY} PRE_BUILD COMMAND cmake -E make_directory ${MAIN_PACKAGE_DIR}/Plugins/iconengines/ )
    foreach( lib ${QT_ICONENGINES} )
        set( location location-NOTFOUND )
        find_file( location ${lib}.dll PATHS ${QT_PLUGINS_DIR}/iconengines/ )
        add_custom_command( TARGET ${TARGET_FOR_COPY} PRE_BUILD COMMAND cmake -E copy ${location} ${MAIN_PACKAGE_DIR}/Plugins/iconengines/ )
    endforeach( lib ) 

    # Copy platform plugins
    set( QT_PLATFORMS qminimal qwindows)
    add_custom_command( TARGET ${TARGET_FOR_COPY} PRE_BUILD COMMAND cmake -E make_directory ${MAIN_PACKAGE_DIR}/Platforms/ )
    foreach( lib ${QT_PLATFORMS} )   
        set( location location-NOTFOUND )
        find_file( location ${lib}.dll PATHS ${QT_PLUGINS_DIR}/platforms/ )
        add_custom_command( TARGET ${TARGET_FOR_COPY} PRE_BUILD COMMAND cmake -E copy ${location} ${MAIN_PACKAGE_DIR}/Platforms/ )
    endforeach( lib )

   # Copy sqldrivers plugins
    set( QT_SQLDRIVERS qsqlite )
    add_custom_command( TARGET ${TARGET_FOR_COPY} PRE_BUILD COMMAND cmake -E make_directory ${MAIN_PACKAGE_DIR}/Plugins/sqldrivers/ )
    foreach( lib ${QT_SQLDRIVERS} )   
        set( location location-NOTFOUND )
        find_file( location ${lib}.dll PATHS ${QT_PLUGINS_DIR}/sqldrivers/ )
        add_custom_command( TARGET ${TARGET_FOR_COPY} PRE_BUILD COMMAND cmake -E copy ${location} ${MAIN_PACKAGE_DIR}/Plugins/sqldrivers/ )
    endforeach( lib )

   # Set the path of the application executable
    if( MSVC_IDE )
        set( EXE_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release/${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX} )
    endif()

    # Copy the application executable
    add_custom_command( TARGET ${TARGET_FOR_COPY} PRE_BUILD 
                        COMMAND cmake -E copy ${EXE_PATH} ${MAIN_PACKAGE_DIR} )

	file(COPY ${CMAKE_SOURCE_DIR}/installer/Resource/ DESTINATION ${MAIN_PACKAGE_DIR})

	    # Remove the temp directory used for building the installer 
    add_custom_command( TARGET makeinstaller POST_BUILD 
                        COMMAND cmake -E remove_directory ${TEMP_PACKAGE_DIR}
                        COMMENT "Removing temporary directory..." )
endif()