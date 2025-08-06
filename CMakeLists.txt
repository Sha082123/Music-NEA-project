cmake_minimum_required(VERSION 3.16)

project(Music_NEA_MSVC VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Svg)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Xml)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)




qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appMusic_NEA_MSVC
    main.cpp
)

qt_add_qml_module(appMusic_NEA_MSVC
    URI Music_NEA_MSVC
    VERSION 1.0
    QML_FILES
        Main.qml
        SOURCES file_opening/file_open.h file_opening/file_open.cpp
        QML_FILES
        QML_FILES styles/FileButton.qml
        SOURCES render/render_file.h render/render_file.cpp
        QML_FILES UI/Display_window.qml
        SOURCES
        SOURCES file_opening/scan_existing.h file_opening/scan_existing.cpp
        QML_FILES UI/C_MenuBar.qml
        SOURCES render/image_provider.h render/image_provider.cpp
        SOURCES globals.h globals.cpp
        QML_FILES
        QML_FILES styles/File_menu.qml
        QML_FILES styles/PreviousDirectoryButton.qml
        QML_FILES
        QML_FILES File_view.qml
        SOURCES render/verovio_loader.h render/verovio_loader.cpp
        SOURCES render/resvg_loader.h render/resvg_loader.cpp
        SOURCES
        SOURCES
        SOURCES
        SOURCES
        SOURCES data_parsing/xml_parser.h data_parsing/xml_parser.cpp
        SOURCES data_parsing/mei_parser.h data_parsing/mei_parser.cpp
        SOURCES data_parsing/parser_data.h data_parsing/parser_data.cpp
        QML_FILES UI/Break_manager.qml
        SOURCES
        SOURCES part_objects/part_object.h part_objects/part_object.cpp
        SOURCES part_objects/part_manager.h part_objects/part_manager.cpp
        QML_FILES UI/Music_scroll_view.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appMusic_NEA_MSVC PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appMusic_NEA_MSVC
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appMusic_NEA_MSVC
    PRIVATE Qt6::Quick
)
target_link_libraries(appMusic_NEA_MSVC PRIVATE Qt6::Core PRIVATE Qt6::Widgets PRIVATE Qt6::Svg PRIVATE Qt6::Xml)

include(GNUInstallDirs)
install(TARGETS appMusic_NEA_MSVC
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)


#LINKING VEROVIO

set(VEROVIO_DIR "external/verovio")
include_directories(
    ${VEROVIO_DIR}/include/vrv
    ${VEROVIO_DIR}/include/crc
    ${VEROVIO_DIR}/include/midi
    ${VEROVIO_DIR}/include/hum
    ${VEROVIO_DIR}/include/json
    ${VEROVIO_DIR}/include/pugi
    ${VEROVIO_DIR}/include/zip
    ${VEROVIO_DIR}/include/vrv
    ${VEROVIO_DIR}/libmei/dist
    ${VEROVIO_DIR}/libmei/addons
    ${VEROVIO_DIR}/fonts/Leipzig
)


target_link_libraries(appMusic_NEA_MSVC PRIVATE Qt6::Core)
link_directories(${VEROVIO_DIR}/tools/build-library/Release)
target_link_directories(appMusic_NEA_MSVC PRIVATE "external/verovio/cmake/build/Release/")
target_link_libraries(appMusic_NEA_MSVC PRIVATE verovio)



#LINKING LUNASVG
set(plutovg_DIR "D:/lunasvg/install/lib/cmake/plutovg")
find_package(plutovg REQUIRED)

set(LUNASVG_DIR "D:/lunasvg/install")
find_package(lunasvg REQUIRED)
target_include_directories(appMusic_NEA_MSVC PRIVATE ${LUNASVG_DIR}/include/lunasvg/)
target_link_directories(appMusic_NEA_MSVC PRIVATE ${LUNASVG_DIR}/lib/lunasvg.lib)
target_link_libraries(appMusic_NEA_MSVC PRIVATE lunasvg::lunasvg)
target_link_libraries(appMusic_NEA_MSVC PRIVATE Qt6::Core)



#LINKING SVG++

include_directories(external/svgpp/include/)
include_directories(external/svgpp/third_party/)


#LINKING BOOST (SVG++ requires boost as a header only library)

include_directories(external/boost)



#LINKING RESVG

include_directories(external/resvg/crates/c-api)
link_directories(external/resvg/target/release)
target_link_directories(appMusic_NEA_MSVC PRIVATE external/resvg/target/release)
target_link_libraries(appMusic_NEA_MSVC PRIVATE resvg ntdll)
target_link_libraries(appMusic_NEA_MSVC PRIVATE Qt6::Core)
target_link_libraries(appMusic_NEA_MSVC PRIVATE Qt6::Core)


#LINKING MX

target_link_directories(appMusic_NEA_MSVC PRIVATE external/mx/build/Release)
target_link_libraries(appMusic_NEA_MSVC PRIVATE mx)
target_link_libraries(appMusic_NEA_MSVC PRIVATE Qt6::Core)
target_link_libraries(appMusic_NEA_MSVC PRIVATE Qt6::Core)
target_link_libraries(appMusic_NEA_MSVC PRIVATE Qt6::Core)
target_link_libraries(appMusic_NEA_MSVC PRIVATE Qt6::Core)
target_link_libraries(appMusic_NEA_MSVC PRIVATE Qt6::Core)
target_link_libraries(appMusic_NEA_MSVC PRIVATE Qt6::Core)
target_link_libraries(appMusic_NEA_MSVC PRIVATE Qt6::Core)
target_link_libraries(appMusic_NEA_MSVC PRIVATE Qt6::Core)
target_include_directories(appMusic_NEA_MSVC PRIVATE external/mx/Sourcecode/include)
