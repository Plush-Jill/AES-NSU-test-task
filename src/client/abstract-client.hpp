//
// Created by plush-jill on 4/24/25.
//

#ifndef ABSTRACT_CLIENT_HPP
#define ABSTRACT_CLIENT_HPP
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QMutex>
#include "src/server-info.hpp"
#include "src/commands/factory/command-factory.hpp"

class AbstractClient {
private:
    QSharedPointer<QTcpSocket>  m_tcp_socket;
    ServerInfo                  m_last_server_info;
    mutable QMutex              m_mutex;
    const qint64                m_chunk_size                {1024 * 4};
    const int                   m_response_timeout_msec     {3000};

public:
                    AbstractClient                  (const QHostAddress& address, quint16 port);

    void            execute_command                 (const QString& command_name) noexcept(false);
    void            read_large_response_in_file     (const std::filesystem::path& response_file, quint64 data_size) const;
    QByteArray      read_response                   () const;
    static void     process_cli_input               ();
    void            send_command                    (const QString& command) const;

    [[nodiscard]] std::shared_ptr<ServerInfo> get_server_info_ptr();
};


#endif //ABSTRACT_CLIENT_HPP
