#include <iostream>
#include <memory>
#include <string>
#include "commands/factory/command-factory.hpp"
#include <QFile>
#include <QJsonObject>
#include <regex>

#include "client/client.hpp"
#include "commands/validator/command-validator.hpp"
// #include "client/abstract_client.hpp"
#include <QQmlApplicationEngine>
#include <QtCore>
#include <QGuiApplication>
#include "backend.hpp"

int main(int argc, char** argv) {
    if (argc != 2) {
        qFatal() << QString("Usage: ./client <server address>");
        return 1;
    }

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    Backend backend;


    const QStringList commands = backend.add_commands(CommandFactory::instance().get_available_commands());
    QStringListModel commandModel;
    commandModel.setStringList(commands);

    engine.rootContext()->setContextProperty("backend", &backend);
    engine.rootContext()->setContextProperty("commandModel", QVariant::fromValue(commands));

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();


    QFile file(argv[1]);
    if (!file.open(QIODevice::ReadOnly)) {
        qFatal() << QString("Cannot open file %1").arg(file.fileName());
        return 1;
    }
    const QByteArray raw_data = file.readAll();
    file.close();
    const QJsonDocument doc = QJsonDocument::fromJson(raw_data);
    if (!doc.isObject()) {
        std::cerr << "Invalid JSON";
        return 1;
    }
    QJsonObject json = doc.object();
    std::shared_ptr<AbstractClient> client;
    try {
        std::cout << "Trying to init client" << std::endl;
        client = std::make_shared<Client>(QHostAddress(json["address"].toString()), json["port"].toInt());
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << std::endl;
        return 1;
    }
    std::cout << std::format("Client initialized and connected to {}:{}\n",
        json["address"].toString().toStdString(),
        json["port"].toInt()
        )
    << std::endl;
    client->execute_command("SYSTem:STATe?");
    client->execute_command("MEAS:POIN?");
    client->process_cli_input();
    sleep(1000);
    // client->execute_command("MEAS:DATA?");
    return 0;

}