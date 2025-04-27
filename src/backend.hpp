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

class Backend final : public QObject {
    Q_OBJECT
public:
    explicit Backend(QObject* parent = nullptr) : QObject(parent) {}

    Q_INVOKABLE void send_command(const QString& command) {
        // Здесь можно реализовать логику отправки команды и получения результата
        emit command_result(QString("Результат выполнения команды: %1").arg(command));
    }

    QStringList add_commands(QStringList commands) {
        return commands;
    }

    signals:
        void command_result(const QString& result);
};



#endif //BACKEND_HPP
