//
// Created by plush-jill on 4/27/25.
//

#include <iostream>
#include <QtCore>
#include <QTcpServer>
#include <QTcpSocket>



class SCPI_Server final : public QTcpServer {

public:
    explicit SCPI_Server(QObject* parent = nullptr, const uint16_t port = 1337) : QTcpServer(parent) {
        if (!listen(QHostAddress::Any, port)) {
            std::cerr << "Failed to start server: " << errorString().toStdString() << std::endl;
            exit(1);
        }
        std::cout << "Server started on port " << port << std::endl;
    }

protected:
    void incomingConnection(const qintptr socket_descriptor) override {
        auto* socket = new QTcpSocket(this);
        if (!socket->setSocketDescriptor(socket_descriptor)) {
            delete socket;
            return;
        }

        connect(socket, &QTcpSocket::readyRead, this, [socket]() {
            const QByteArray data = socket->readAll();
            const QString command = QString::fromUtf8(data).trimmed();
            QByteArray response;

            if (command == "SYSTem:STATe?" || command == "SYST:STAT?") {
                response.append(static_cast<char>(1)); // bool (0 или 1)
            } else if (command == "MEASure:POINts?" || command == "MEAS:POIN?") {
                uint32_t points = 1000;
                points = qToBigEndian(points); // Преобразование в Big-Endian
                response.append(reinterpret_cast<const char*>(&points), sizeof(points));
            } else if (command == "MEASure:DATA?" || command == "MEAS:DATA?") {
                uint16_t data_[1000];
                for (int i = 0; i < 1000; ++i) {
                    data_[i] = qToBigEndian(static_cast<uint16_t>(1)); // Преобразование в Big-Endian
                }
                response.append(reinterpret_cast<const char*>(data_), sizeof(data_));
            } else {
                std::cout << "Unknown command: " << command.toStdString() << std::endl;
                response.append("ERROR: Unknown command\n");
            }
            std::cout << std::format(
                "Received command: {}\nResponse sent.",
                command.toStdString())
                << std::endl;
            socket->write(response);
            socket->flush();
        });

        connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    }
};

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }
    QCoreApplication app(argc, argv);
    SCPI_Server server(nullptr, static_cast<uint16_t>(std::stoi(argv[1])));
    return QCoreApplication::exec();
}

#include "server.moc"