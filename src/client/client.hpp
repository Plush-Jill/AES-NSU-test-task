//
// Created by plush-jill on 4/24/25.
//

#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <thread>
#include "abstract-client.hpp"
#include "server-info.hpp"
#include "../commands/factory/command-factory.hpp"

class Client final : public AbstractClient {
private:
    std::thread m_cli_thread;
    std::atomic<bool> m_running;
public:
                    Client                          (const QHostAddress& address, quint16 port) noexcept(false);

    void            execute_command                 (const QString& command_name) noexcept(false) override;
    void            execute_command                 (const QString& command_name, QString* result_message) noexcept(false) override;
    void            read_large_response_in_file     (const std::filesystem::path& response_file, quint64 data_size) const noexcept(false) override;
    QByteArray      read_response                   (qint64 expected_size) const noexcept(false) override;
    void            process_cli_input               () noexcept(false) override;
    void            stop_cli_input                  () noexcept(false) override;

    ~Client() override;

    void            send_command                    (const QString& command) const noexcept(false) override;
};


#endif //CLIENT_HPP
