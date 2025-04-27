//
// Created by plush-jill on 4/27/25.
//

#ifndef BACKEND_HPP
#define BACKEND_HPP

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QStringListModel>

#include "client/abstract-client.hpp"
#include "commands/factory/command-factory.hpp"

class Backend : public QObject {
    Q_OBJECT

private:
    std::shared_ptr<AbstractClient> m_client;
    const int m_max_history_size = 10;
    QStringList m_history;

public:
    explicit Backend(QObject* parent = nullptr) : QObject(parent) {}

    void set_client(const std::shared_ptr<AbstractClient> &client) {
        m_client = client;
    }

    Q_INVOKABLE void send_command(const QString& command) {
        try {
            QString result;
            m_client->execute_command(command, &result);
            m_history.prepend(QString("%1: %2").arg(command, result));
        } catch (const std::exception& exception) {
            m_history.prepend(QString("%1: %2").arg(command, exception.what()));
        }
        if (m_history.size() > m_max_history_size) {
            m_history.removeLast();
        }
        emit historyChanged(m_history.join("\n"));
    }

    Q_INVOKABLE static QStringList get_commands() {
        return CommandFactory::instance().get_available_commands();
    }

    Q_INVOKABLE QString get_history() const {
        // QStringList reversed = m_history;
        // std::ranges::reverse(reversed);
        // return reversed.join("\n");
        return m_history.join("\n");
    }

    signals:
        void historyChanged(const QString& history);
};



#endif //BACKEND_HPP
