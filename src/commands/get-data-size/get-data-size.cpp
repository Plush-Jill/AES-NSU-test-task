//
// Created by plush-jill on 4/25/25.
//

#include "get-data-size.hpp"

quint32 GetDataSize::m_response_size {4};
QString GetDataSize::m_command_name_short {"MEAS:POIN?"};
QString GetDataSize::m_command_name_long {"MEASure:POINts?"};
CommandRegistrar GetDataSize::registrar {
    m_command_name_short,
    m_command_name_long,
    [] { return std::make_unique<GetDataSize>(); }
};

void GetDataSize::execute(AbstractClient* client) {
    execute(client, nullptr);
}

void GetDataSize::execute(AbstractClient* client, QString* const result_message) {
    client->send_command(m_command_name_short);
    const QByteArray response = client->read_response(m_response_size);
    uint32_t data_size {};
    data_size = qFromBigEndian<uint32_t>(response.constData());
    client->get_server_info_ptr()->set_data_size(data_size);
    if (result_message != nullptr) {
        *result_message = QString("Result: %1").arg(std::to_string(data_size));
    }
}

