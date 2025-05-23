//
// Created by plush-jill on 4/27/25.
//

#ifndef BACKEND_HPP
#define BACKEND_HPP

#include <QFile>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "client/abstract-client.hpp"

/**
 * @brief класс-посредник между клиентом и GUI на QML.
 */
class Backend : public QObject {
    Q_OBJECT

private:
    std::shared_ptr<AbstractClient> m_client;
    const int m_max_history_size = 10;
    QStringList m_history;

public:
    explicit Backend(QObject* parent = nullptr) : QObject(parent) {}

    void set_client(const std::shared_ptr<AbstractClient> &client);

    Q_INVOKABLE void send_command(const QString& command);
    Q_INVOKABLE static QStringList get_commands();
    Q_INVOKABLE [[nodiscard]] QString get_history() const;
    Q_INVOKABLE void reconnect();

    signals:
        void historyChanged(const QString& history);
};



#endif //BACKEND_HPP
