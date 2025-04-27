//
// Created by plush-jill on 4/25/25.
//
#include <iostream>
#include <QFile>
#include "client.hpp"
#include "cli_thread.hpp"
#include "../commands/factory/command-factory.hpp"
#include "../commands/validator/command-validator.hpp"


Client::Client(const QHostAddress &address, const quint16 port)
: AbstractClient(address, port) {
    QObject::connect(this, &Client::command_execution_requested, this, &Client::handle_command, Qt::BlockingQueuedConnection);
}

void Client::execute_command(const QString& command_name) noexcept(false) {
    execute_command(command_name, nullptr);
}

void Client::execute_command(const QString &command_name, QString* const result_message) noexcept(false) {
    if (QThread::currentThread() != thread()) {
        emit command_execution_requested(command_name, result_message);
        return;
    }

    QMutexLocker locker(&m_mutex);
    if (!CommandValidator::is_SCPI_command(command_name.trimmed())) {
        throw std::runtime_error("Is not SCPI command");
    }
    if (const auto command = CommandFactory::instance().create(command_name)) {
        command->execute(this, result_message);
    } else {
        throw std::runtime_error("A not-yet-supported or invalid command");
    }
}

void Client::send_command(const QString &command) const {
    if (m_tcp_socket->state() == QAbstractSocket::ConnectedState) {
        m_tcp_socket->write(command.toStdString().c_str());
        m_tcp_socket->waitForBytesWritten();
    } else {
        throw std::runtime_error("Socket is not connected");
    }
}

QByteArray Client::read_response(const qint64 expected_size) const {
    if (m_tcp_socket->state() != QAbstractSocket::ConnectedState) {
        throw std::runtime_error("Socket is not connected");
    }

    QByteArray total_data;
    qint64 bytes_received = 0;

    while (bytes_received < expected_size) {
        if (m_tcp_socket->bytesAvailable() == 0) {
            if (!m_tcp_socket->waitForReadyRead(m_response_timeout_msec)) {
                throw std::runtime_error("Failed to read response: timeout");
            }
        }

        QByteArray chunk = m_tcp_socket->read(expected_size - bytes_received);
        if (chunk.isEmpty()) {
            if (m_tcp_socket->state() != QAbstractSocket::ConnectedState) {
                throw std::runtime_error("Connection closed before receiving complete data");
            }
            continue;
        }

        total_data.append(chunk);
        bytes_received += chunk.size();
    }

    return total_data;
}


void Client::read_large_response_in_file(const std::filesystem::path& response_file, const quint64 data_size) const {
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

void Client::stop_cli_input() noexcept(false) {
    if (m_cli_thread) {
        m_cli_thread->terminate();
        std::cin.putback('\n');
        m_cli_thread->wait();
        delete m_cli_thread;
        m_cli_thread = nullptr;
    }
}

void Client::process_cli_input() noexcept(false) {
    m_cli_thread = new CliThread(this);
    m_cli_thread->start();
}

Client::~Client() {
    stop_cli_input();
}


void Client::handle_command(const QString& command, QString* result) {
    try {
        execute_command(command, result);
    } catch (const std::exception& exception) {
        if (result) *result = QString("Error: %1").arg(exception.what());
    }
}