project(shopDrawingExport)

add_library(${PROJECT_NAME}
        SHARED
        library.cpp
)

message(STATUS "Current source dir: ${CMAKE_CURRENT_SOURCE_DIR}")

message(STATUS "Includes dir: ${CMAKE_CURRENT_SOURCE_DIR}/../../external/includes")

target_include_directories(${PROJECT_NAME}
        PRIVATE
#        ${CMAKE_CURRENT_SOURCE_DIR}/../presentation
        ${CMAKE_CURRENT_SOURCE_DIR}/../../external/includes
)


target_compile_definitions(${PROJECT_NAME} PRIVATE
        CWAPI3D_PLUGIN_NAME=L"Cadwork Test Plugin"
        CWAPI3D_AUTHOR_NAME=L"Your Name"
        CWAPI3D_AUTHOR_EMAIL=L"your_name@something.swiss"
)

