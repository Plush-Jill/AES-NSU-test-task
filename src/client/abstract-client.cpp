//
// Created by plush-jill on 4/26/25.
//
#include "abstract-client.hpp"

AbstractClient::AbstractClient(const QHostAddress &address, const quint16 port) noexcept(false)
: m_tcp_socket(std::make_shared<QTcpSocket>()), m_last_server_info(std::make_shared<ServerInfo>()) {
    m_tcp_socket->connectToHost(address, port);
    if (!m_tcp_socket->waitForConnected(m_response_timeout_msec)) {
        throw std::runtime_error(QString("Failed to connect to %1:%2")
            .arg(address.toString(), QString::number(port))
            .toStdString());
    }
}

AbstractClient::~AbstractClient() {
    if (m_tcp_socket) {
        m_tcp_socket->disconnectFromHost();
        m_tcp_socket->close();
    }
}

std::shared_ptr<ServerInfo> AbstractClient::get_server_info_ptr() {
    return m_last_server_info;
}
