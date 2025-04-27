//
// Created by plush-jill on 4/25/25.
//
#include "client.hpp"

#include <iostream>
#include <QFile>
#include <qthread.h>

#include "../commands/factory/command-factory.hpp"
#include "../commands/validator/command-validator.hpp"


Client::Client(const QHostAddress &address, const quint16 port)
: AbstractClient(address, port) {
}

void Client::execute_command(const QString& command_name) noexcept(false) {
    QMutexLocker locker(&m_mutex);
    if (!CommandValidator::is_SCPI_command(command_name.trimmed())) {
        throw std::runtime_error("Is not SCPI command");
    }
    if (const auto command = CommandFactory::instance().create(command_name)) {
        command->execute(this);
    } else {
        throw std::runtime_error("An unsupported command yet");
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

QByteArray Client::read_response(qint64 expected_size) const {
    if (m_tcp_socket->state() == QAbstractSocket::ConnectedState) {
        if (m_tcp_socket->waitForReadyRead(m_response_timeout_msec)) {
            return QByteArray(m_tcp_socket->readAll());
        }
        throw std::runtime_error("Failed to read response");
    }
    throw std::runtime_error("Socket is not connected");
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

void Client::process_cli_input() noexcept(false) {
    static std::atomic<bool> running{true};

    std::thread cliThread([this]() {
        const QString help_message = "Available commands: exit, help, <SCPI command>";
        std::cout << help_message.toStdString() << std::endl;
        while (running) {
            try {
                std::string input;
                std::getline(std::cin, input);

                if (input == "exit") {
                    running = false;
                    break;
                } else if (input == "help") {
                    std::cout << help_message.toStdString() << std::endl;
                    continue;
                }

                if (input.empty()) {
                    continue;
                }
                QString command = QString::fromStdString(input).trimmed();
                try{
                    execute_command(command);
                } catch (std::exception& exception) {
                    std::cerr << exception.what() << std::endl;
                }
            } catch (const std::exception& exception) {
                std::cerr << std::format("\nError: {}", exception.what()) << std::endl;
            }
        }
    });

    cliThread.detach();
}
