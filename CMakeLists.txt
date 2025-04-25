cmake_minimum_required(VERSION 3.30)
project(SAES_NSU_test_task)

set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_PREFIX_PATH "/home/plush-jill/All_Random/Qt/6.9.0/gcc_64/lib/cmake")


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 6.9 REQUIRED COMPONENTS
        Core
        Widgets
        3DCore
        3DRender
        3DExtras
        3DInput
        3DLogic
        Qml
        Quick
        3DQuickScene2D # Попробуем этот модуль
)

qt_policy(SET QTP0001 NEW)
qt_policy(SET QTP0004 NEW)

qt_add_executable(SAES_NSU_test_task src/main.cpp
        src/commands/abstract-command.hpp
        src/commands/factory/command-factory.cpp
        src/commands/factory/command-factory.hpp
        src/commands/registrar/command-registrar.hpp
        src/commands/get-state/get-state.cpp
        src/commands/get-state/get-state.hpp
        src/commands/get-data/get-data.cpp
        src/commands/get-data/get-data.hpp
        src/client/abstract-client.hpp
        src/commands/validator/command-validator.cpp
        src/commands/validator/command-validator.hpp
        src/commands/get-data-size/get-data-size.cpp
        src/commands/get-data-size/get-data-size.hpp
        src/client/abstract-client.cpp
        src/server-info.cpp
        src/server-info.hpp)

qt_add_qml_module(SAES_NSU_test_task
        URI SAES_NSU_test_task
        VERSION 1.0
        QML_FILES qml/main.qml
        OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/qml"
        RESOURCES qml/resources.qrc
)

target_link_libraries(SAES_NSU_test_task PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::3DCore
        Qt6::3DRender
        Qt6::3DExtras
        Qt6::3DInput
        Qt6::3DLogic
        Qt6::Qml
        Qt6::Quick
        Qt6::3DQuickScene2D # Добавляем линковку
)

#add_executable(SAES_NSU_test_task main.cpp)
