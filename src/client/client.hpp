//
// Created by plush-jill on 4/24/25.
//

#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <thread>
#include "abstract-client.hpp"
#include "cli_thread.hpp"
#include "../entity/server-info.hpp"
#include "../commands/factory/command-factory.hpp"



class Client final : public QObject, public AbstractClient {
    Q_OBJECT
private:
    QThread* m_cli_thread {nullptr};
    std::atomic<bool> m_running;
public:
                    Client                          (const QHostAddress& address, quint16 port);

    void            execute_command                 (const QString& command_name) override;
    void            execute_command                 (const QString& command_name, QString* result_message) override;
    void            read_large_response_in_file     (const std::filesystem::path& response_file, quint64 data_size) const override;
    QByteArray      read_response                   (qint64 expected_size) const override;
    void            process_cli_input               () override;
    void            stop_cli_input                  () override;
    void            reconnect                       () override;

    ~Client() override;

    void            send_command                    (const QString& command) const override;

    signals:
        void command_execution_requested(const QString& command, QString* result);
        void reconnect_requested();

    private slots:
        void handle_command(const QString& command, QString* result);
        void handle_reconnect_request();
};



#endif //CLIENT_HPP
