//
// Created by plush-jill on 4/25/25.
//
#include "abstract-client.hpp"

#include <QFile>

#include "src/commands/factory/command-factory.hpp"


AbstractClient::AbstractClient(const QHostAddress &address, const quint16 port)
: m_tcp_socket(), m_mutex() {
    m_tcp_socket->connectToHost(address, port);
    if (!m_tcp_socket->waitForConnected(m_response_timeout_msec)) {
        throw std::runtime_error(QString("Failed to connect to %1:%2")
                                     .arg(address.toString(), QString::number(port))
                                     .toStdString());

    }
}

void AbstractClient::execute_command(const QString& command_name) noexcept(false) {
    m_mutex.lock();
    if (const auto command = CommandFactory::instance().create(command_name)) {
        command->execute(this);
    }
    m_mutex.unlock();
}

void AbstractClient::send_command(const QString &command) const {
    if (m_tcp_socket->state() == QAbstractSocket::ConnectedState) {
        m_tcp_socket->write(command.toUtf8());
        m_tcp_socket->waitForBytesWritten();
    } else {
        throw std::runtime_error("Socket is not connected");
    }
}

QByteArray AbstractClient::read_response() const {
    if (m_tcp_socket->state() == QAbstractSocket::ConnectedState) {
        if (m_tcp_socket->waitForReadyRead(m_response_timeout_msec)) {
            return QByteArray(m_tcp_socket->readAll());
        } else {
            throw std::runtime_error("Failed to read response");
        }
    } else {
        throw std::runtime_error("Socket is not connected");
    }
}

void AbstractClient::read_large_response_in_file(const std::filesystem::path& response_file, const quint64 data_size) const {
    // QMutexLocker locker(&m_mutex);

    if (!m_tcp_socket || !m_tcp_socket->isOpen()) {
        throw std::runtime_error("Socket is not open");
    }

    QFile file(QString::fromStdString(response_file.string()));
    if (!file.open(QIODevice::WriteOnly)) {
        throw std::runtime_error("Failed to open file for writing");
    }

    quint64 total_bytes_read = 0;

    while (total_bytes_read < data_size) {
        if (!m_tcp_socket->waitForReadyRead(m_response_timeout_msec)) {
            throw std::runtime_error("Timeout waiting for data from socket");
        }

        QByteArray chunk = m_tcp_socket->read(qMin(m_chunk_size, static_cast<qint64>(data_size - total_bytes_read)));

        if (chunk.isEmpty()) {
            throw std::runtime_error("Socket closed unexpectedly");
        }

        const qint64 written = file.write(chunk);
        if (written != chunk.size()) {
            throw std::runtime_error("Failed to write to file");
        }

        total_bytes_read += static_cast<quint64>(written);
    }

    file.close();
}

void AbstractClient::process_cli_input() {

}

std::shared_ptr<ServerInfo> AbstractClient::get_server_info_ptr() {
    return std::make_shared<ServerInfo>(m_last_server_info);
}
