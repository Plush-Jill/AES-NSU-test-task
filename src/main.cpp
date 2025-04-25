#include <iostream>
#include <memory>
#include <string>
#include "commands/factory/command-factory.hpp"
#include <QFile>

#include "client/abstract-client.hpp"
// #include "client/abstract_client.hpp"


int main() {

    /*
    std::cout << "Available commands:\n";
    for (QStringList commands = CommandFactory::instance().get_available_commands(); const auto& command : commands) {
        std::cout << command.toStdString() << std::endl;
    }

    std::cout << std::endl;
    AbstractClient client;
    client.execute_command("asdas");
    auto cmd = CommandFactory::instance().create("get_data_size");
    if (cmd) cmd->execute();

    cmd = CommandFactory::instance().create("get_data_size");
    if (cmd) cmd->execute();


    cmd = CommandFactory::instance().create("something_else");
    if (!cmd) std::cout << "Unknown command\n";

    cmd = CommandFactory::instance().create("sdfdsgdfg");
    if (!cmd) std::cout << "Unknown command\n";
    */
}