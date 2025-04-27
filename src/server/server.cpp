//
// Created by plush-jill on 4/27/25.
//

#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QByteArray>
#include <QHostAddress>
#include <iostream>
#include <QtEndian>
#include <QtCore>

class SCPI_Server final : public QTcpServer {

public:
    explicit SCPI_Server(QObject* parent = nullptr) : QTcpServer(parent) {
        if (!listen(QHostAddress::Any, 1337)) {
            std::cerr << "Failed to start server: " << errorString().toStdString() << std::endl;
            exit(1);
        }
        std::cout << "Server started on port 1337" << std::endl;
    }

protected:
    void incomingConnection(qintptr socketDescriptor) override {
        auto* socket = new QTcpSocket(this);
        if (!socket->setSocketDescriptor(socketDescriptor)) {
            delete socket;
            return;
        }
        connect(socket, &QTcpSocket::readyRead, this, [socket]() {
            const QByteArray data = socket->readAll();
            const QString command = QString::fromUtf8(data).trimmed();
            QByteArray response;

            if (command == "SYSTem:STATe?" || command == "SYST:STAT?") {
                response.append(static_cast<char>(0)); // bool (0 или 1)
            } else if (command == "MEASure:POINts?" || command == "MEAS:POIN?") {
                uint32_t points = 1000;
                points = qToBigEndian(points); // Преобразование в Big-Endian
                response.append(reinterpret_cast<const char*>(&points), sizeof(points));
            } else if (command == "MEASure:DATA?" || command == "MEAS:DATA?") {
                uint16_t data_[1000];
                for (uint16_t i = 0; i < 1000; ++i) {
                    data_[i] = i; // Преобразование в Little-Endian
                }
                for (int i = 0; i < 1000; ++i) {
                    data_[i] = qToBigEndian(static_cast<uint16_t>(i)); // Преобразование в Big-Endian
                }
                response.append(reinterpret_cast<const char*>(data_), sizeof(data_));
            } else {
                std::cout << "Unknown command: " << command.toStdString() << std::endl;
                response.append("ERROR: Unknown command\n");
            }
            std::cout << std::format(
                "Received command: {}\nSent response: {}",
                command.toStdString(),
                response.toStdString())
                << std::endl;
            socket->write(response);
            socket->flush();
        });

        connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    }
};

int main(int argc, char** argv) {
    QCoreApplication app(argc, argv);
    SCPI_Server server;
    return app.exec();
}

#include "server.moc"