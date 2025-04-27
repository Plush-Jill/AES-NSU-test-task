//
// Created by plush-jill on 4/26/25.
//

#ifndef ABSTRACT_CLIENT_HPP
#define ABSTRACT_CLIENT_HPP
#include <QMutex>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include "server-info.hpp"


class AbstractClient {
protected:
    std::shared_ptr<QTcpSocket> m_tcp_socket;
    std::shared_ptr<ServerInfo> m_last_server_info;
    mutable QMutex              m_mutex;
    const qint64                m_chunk_size                {1024 * 4};
    const int                   m_response_timeout_msec     {3000};


public:
                            AbstractClient                  (const QHostAddress& address, quint16 port) noexcept(false);
    virtual                 ~AbstractClient                 () noexcept;

    /**
     * @brief По command_name создаётся инстанс класса команды и вызывается его execute.
     * @param command_name Текст команды.
     * @throw std::runtime_error Если команда не формата SCPI или если не найден класс для команды.
     */
    virtual void            execute_command                 (const QString& command_name) noexcept(false) = 0;

    /**
     * @brief По command_name создаётся инстанс класса команды и вызывается его execute().
     * @param command_name Текст команды.
     * @param result_message Указатель на строку, в которую будет записано сообщение о выполнении команды.
     * @throw std::runtime_error Если команда не формата SCPI или если не найден класс для команды.
     */
    virtual void            execute_command                 (const QString& command_name, QString* result_message) noexcept(false) = 0;

    /**
     * @brief Читает ответ с сервера длины data_size и пишет в указанный файл.
     * @param response_file Файл, в который запишется весь ответ.
     * @param data_size_bytes Размер ответа.
     * @throw std::runtime_error Если сокет не подключен или не открыт, если возникли проблемы с записью в файл.
     */
    virtual void            read_large_response_in_file     (const std::filesystem::path& response_file, quint64 data_size_bytes) const noexcept(false) = 0;

    /**
     * @brief Читает ответ сервера размера expected_size и возвращает через массив байтов.
     * @return QByteArray из всего ответа.
     */
    virtual QByteArray      read_response                   (qint64 expected_size) const noexcept(false) = 0;

    /**
     * @brief Обработка терминального ввода. Создаёт отдельный detach поток.
     */
    virtual void            process_cli_input               () noexcept(false) = 0;
    virtual void            stop_cli_input                  () noexcept(false) = 0;

    /**
     * @brief Отправляет команду серверу.
     * @param command Текст команды.
     * @throw std::runtime_error Если сокет не подключен.
     */
    virtual void            send_command                    (const QString& command) const noexcept(false) = 0;

    /**
     * @return Указатель на наиболее актуальный хранящийся у клиента server_info.
     */
    [[nodiscard]] virtual std::shared_ptr<ServerInfo> get_server_info_ptr();
};


#endif //ABSTRACT_CLIENT_HPP
