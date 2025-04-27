//
// Created by plush-jill on 4/27/25.
//

#include <iostream>
#include <QtCore>
#include <QTcpServer>
#include <QTcpSocket>

class ServerData {
private:
    bool m_running = true;

    uint32_t m_data_size = 0;
    std::vector<uint16_t> m_data = std::vector<uint16_t> (1000, 15);
    explicit ServerData(const u_int32_t data_size) : m_data_size(data_size) {
        m_data.reserve(data_size);
        for (uint32_t i = 0; i < data_size; ++i) {
            m_data.push_back(rand()%1000);
        }
    }

public:
    explicit ServerData() = default;
    static ServerData get_random_data() {
        return ServerData(rand()%1000);
    }

    [[nodiscard]] bool get_running() const {
        return m_running;
    }

    [[nodiscard]] uint32_t get_data_size() const {
        return m_data_size;
    }

    [[nodiscard]] std::vector<uint16_t> get_data() const {
        return m_data;
    }
};


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