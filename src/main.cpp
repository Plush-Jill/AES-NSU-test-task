#include <iostream>
#include <memory>
#include <string>
#include <QtCore>
#include "client/client.hpp"
#include "client/backend.hpp"

int main(int argc, char** argv) {
    if (argc != 2) {
        qFatal("Usage: ./client <config.json>");
    }

    QFile file(argv[1]);
    if (!file.open(QIODevice::ReadOnly)) {
        qFatal("Cannot open config file");
    }

    const auto json = QJsonDocument::fromJson(file.readAll()).object();
    const auto address = QHostAddress(json["address"].toString());
    const uint16_t port = json["port"].toInt();
    file.close();

    const std::shared_ptr<AbstractClient> client = std::make_shared<Client>(address, port);

    client->process_cli_input();


    const QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    const auto backend = new Backend(&engine);
    backend->set_client(client);
    engine.rootContext()->setContextProperty("backend", backend);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    QObject::connect(&app, &QGuiApplication::aboutToQuit, [&]() {
        client->stop_cli_input();
    });

    return QGuiApplication::exec();
}