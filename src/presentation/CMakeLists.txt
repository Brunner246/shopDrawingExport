project(presentation)

find_package(Qt${QT_VERSION} COMPONENTS
        Core
        Widgets
        REQUIRED)

add_library(${PROJECT_NAME}
        SHARED
        MyDialog.cc
        MyDialog.hh
)

target_compile_definitions(${PROJECT_NAME} PRIVATE PRESENTATION_EXPORTS)


target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt${QT_VERSION}::Core
        Qt${QT_VERSION}::Widgets
)


add_library(Test::Presentation ALIAS ${PROJECT_NAME})
