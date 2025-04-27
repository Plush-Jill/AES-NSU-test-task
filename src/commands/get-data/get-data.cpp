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

void GetData::execute(Client *client) {
    client->send_command(m_command_name_short);
    const std::filesystem::path path {"data.txt"};
    const std::shared_ptr<ServerInfo> server_info = client->get_server_info_ptr();
    client->read_large_response_in_file(path, server_info->get_data_size());
    server_info->set_data_path(path);
    std::cout << std::format("Result saved to: {}", path.string()) << std::endl;
}

