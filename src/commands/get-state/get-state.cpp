//
// Created by plush-jill on 4/24/25.
//

#include "get-state.hpp"

quint32 GetStateCommand::m_response_size {1};
QString GetStateCommand::m_command_name_short {"SYST:STAT?"};
QString GetStateCommand::m_command_name_long {"SYSTem:STATe?"};
CommandRegistrar GetStateCommand::registrar {
    m_command_name_short,
    m_command_name_long,
    [] { return std::make_unique<GetStateCommand>(); }
};

void GetStateCommand::execute(AbstractClient* client) {
    execute(client, nullptr);
}

void GetStateCommand::execute(AbstractClient* client, QString* const result_message) {
    client->send_command(m_command_name_short);
    QByteArray response = client->read_response(m_response_size);
    const bool state = response[0];
    client->get_server_info_ptr()->set_is_running(state);
    if (result_message != nullptr) {
        *result_message = QString("Result: %1").arg(state ? "1" : "0");
    }
}
