//
// Created by plush-jill on 4/25/25.
//

#include "get-data-size.hpp"

void GetDataSize::execute(AbstractClient *client) {
    client->send_command(m_command_name_short);
    const QByteArray response = client->read_response();
    client->get_server_info_ptr()->set_data_size(response.toInt());
    std::cout << "GetDataSize executed\n";
}

QString GetDataSize::m_command_name_short {"MEAS: POIN?"};
QString GetDataSize::m_command_name_long {"SYSTem: STATe?"};
CommandRegistrar GetDataSize::registrar {
    m_command_name_long,
    [] { return std::make_unique<GetDataSize>(); }
};
