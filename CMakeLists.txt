cmake_minimum_required(VERSION 3.28)
project(SAES_NSU_test_task)

set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_PREFIX_PATH "/home/plush-jill/All_Random/Qt/6.9.0/gcc_64/lib/cmake")


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 6.9 REQUIRED COMPONENTS
        Core
        Widgets
        Qml
        Quick
)

qt_policy(SET QTP0001 NEW)
qt_policy(SET QTP0004 NEW)

qt_add_executable(client-side src/main.cpp
        src/commands/abstract-command.hpp
        src/commands/factory/command-factory.cpp
        src/commands/factory/command-factory.hpp
        src/commands/command-registrar.hpp
        src/commands/get-state/get-state.cpp
        src/commands/get-state/get-state.hpp
        src/commands/get-data/get-data.cpp
        src/commands/get-data/get-data.hpp
        src/client/client.hpp
        src/commands/validator/command-validator.cpp
        src/commands/validator/command-validator.hpp
        src/commands/get-data-size/get-data-size.cpp
        src/commands/get-data-size/get-data-size.hpp
        src/client/client.cpp
        src/client/server-info.cpp
        src/client/server-info.hpp
        src/client/abstract-client.hpp
        src/client/abstract-client.cpp
        src/client/backend.cpp
        src/client/backend.hpp)

qt_add_qml_module(client-side
        URI client-side
        VERSION 1.0
        QML_FILES qml/main.qml
        OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/qml"
        RESOURCES qml/resources.qrc
)

target_link_libraries(client-side PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Qml
        Qt6::Quick
)

add_executable(server-side src/server/server.cpp)

target_link_libraries(server-side PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Qml
        Qt6::Quick
)
