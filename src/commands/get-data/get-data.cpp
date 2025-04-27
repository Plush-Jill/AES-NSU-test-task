//
// Created by plush-jill on 4/24/25.
//

#include "get-data.hpp"

#include <iostream>

QString GetData::m_command_name_short {"MEAS:DATA?"};
QString GetData::m_command_name_long {"MEASure:DATA?"};
CommandRegistrar GetData::registrar {
    m_command_name_short,
    m_command_name_long,
    [] { return std::make_unique<GetData>(); }
};

void GetData::execute(AbstractClient* client) {
    execute(client, nullptr);
}

void GetData::execute(AbstractClient* client, QString *result_message) {
    const std::shared_ptr<ServerInfo> server_info = client->get_server_info_ptr();
    const uint32_t data_size = server_info->get_data_size();
    client->send_command(m_command_name_short);
    const std::filesystem::path path {"data"};
    client->read_large_response_in_file(path, data_size * sizeof(uint16_t));
    server_info->set_data_path(path);
    if (result_message) {
        *result_message = QString("Result saved to: %1").arg(path.string());
    }
}

