project(shopDrawingExport)

find_package(Qt${QT_VERSION} COMPONENTS
        Core
        Widgets
        REQUIRED)

add_library(${PROJECT_NAME}
        SHARED
        library.cpp
)

message(STATUS "Current source dir: ${CMAKE_CURRENT_SOURCE_DIR}")

message(STATUS "Includes dir: ${CMAKE_CURRENT_SOURCE_DIR}/../../external/includes")

target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../presentation
        ${CMAKE_CURRENT_SOURCE_DIR}/../../external/includes
)


target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt${QT_VERSION}::Core
        Qt${QT_VERSION}::Widgets
        Test::Presentation
)


target_compile_definitions(${PROJECT_NAME} PRIVATE
        CWAPI3D_PLUGIN_NAME=L"Cadwork Test Plugin"
        CWAPI3D_AUTHOR_NAME=L"Your Name"
        CWAPI3D_AUTHOR_EMAIL=L"your_name@something.swiss"
)

