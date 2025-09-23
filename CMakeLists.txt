cmake_minimum_required(VERSION 4.0)
project(shopDrawingExport)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

SET(CADLIB_DIR "C:/source/cadlib")

set(CADWORK_VERSION 33)
set(QT_VERSION 6)

set(CMAKE_PREFIX_PATH "${CADLIB_DIR}\\3rdParty\\v_${CADWORK_VERSION}.0\\Qt\\x64\\lib\\cmake\\Qt${QT_VERSION}")

add_subdirectory(src/app)
add_subdirectory(src/presentation)

#find_package(Qt${QT_VERSION} COMPONENTS
#        Core
#        Widgets
#        REQUIRED)
#
#
##include(FetchContent)
##FetchContent_Declare(
##        cwapi3dcpp
##        GIT_REPOSITORY https://github.com/cwapi3d/cwapi3dcpp.git
##        GIT_TAG main
##)
##
##FetchContent_MakeAvailable(cwapi3dcpp)
##FetchContent_GetProperties(cwapi3dcpp SOURCE_DIR cwapi3dcpp_source_dir)
#
#add_library(shopDrawingExport
#        SHARED
#        library.cpp
#        MyDialog.cc
#        MyDialog.hh
#)
#
#target_link_libraries(${PROJECT_NAME} PRIVATE
#        Qt${QT_VERSION}::Core
#        Qt${QT_VERSION}::Widgets
#)
#
#target_include_directories(${PROJECT_NAME}
#        PRIVATE
##        ${cwapi3dcpp_source_dir}/include
#        ${CMAKE_CURRENT_SOURCE_DIR}/external/includes
#
#)
#
#target_compile_definitions(${PROJECT_NAME} PRIVATE
#        CWAPI3D_PLUGIN_NAME=L"Cadwork Test Plugin"
#        CWAPI3D_AUTHOR_NAME=L"Your Name"
#        CWAPI3D_AUTHOR_EMAIL=L"your_name@something.swiss"
#)
#
