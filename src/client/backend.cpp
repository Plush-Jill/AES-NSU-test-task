//
// Created by plush-jill on 4/27/25.
//

#include "backend.hpp"

void Backend::set_client(const std::shared_ptr<AbstractClient> &client) {
    m_client = client;
}


void Backend::send_command(const QString &command) {
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

QStringList Backend::get_commands() {
    return CommandFactory::instance().get_available_commands();
}

QString Backend::get_history() const {
    return m_history.join("\n");
}

void Backend::reconnect() {
    try {
        m_client->reconnect();
        m_history.prepend(QString("Reconnected"));
    } catch (std::exception& exception) {
        m_history.prepend(QString("Reconnect error: %1").arg(exception.what()));
    }
    emit historyChanged(m_history.join("\n"));
}
