//
// Created by plush-jill on 4/24/25.
//

#include "get-state.hpp"


const QString GetStateCommand::m_command_name_short {"SYST: STAT?"};
const QString GetStateCommand::m_command_name_long {"SYSTem: STATe?"};
const CommandRegistrar GetStateCommand::registrar {
    m_command_name_long,
    [] { return std::make_unique<GetStateCommand>(); }
};

void GetStateCommand::execute(AbstractClient *client) {
    client->send_command(m_command_name_short);
    QByteArray response = client->read_response();
    const bool state = response[0];
    client->get_server_info_ptr()->set_is_running(state);
    std::cout << "GetState executed\n";
}
