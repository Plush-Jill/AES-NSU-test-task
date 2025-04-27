//
// Created by plush-jill on 4/25/25.
//

#include "get-data-size.hpp"

#include <iostream>

quint32 GetDataSize::m_response_size {4};
QString GetDataSize::m_command_name_short {"MEAS:POIN?"};
QString GetDataSize::m_command_name_long {"MEASure: POINts?"};
CommandRegistrar GetDataSize::registrar {
    m_command_name_short,
    m_command_name_long,
    [] { return std::make_unique<GetDataSize>(); }
};

void GetDataSize::execute(Client *client) {
    client->send_command(m_command_name_short);
    const QByteArray response = client->read_response(m_response_size);

    if (response.size() == static_cast<int>(m_response_size)) {
        // Преобразование из Big-Endian в uint32
        const uint32_t data_size = qFromBigEndian<uint32_t>(reinterpret_cast<const uchar*>(response.constData()));
        client->get_server_info_ptr()->set_data_size(data_size);
        std::cout << "Result: " << data_size << std::endl;
    } else {
        std::cerr << "Invalid response size: " << response.size() << std::endl;
    }
}

