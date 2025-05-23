//
// Created by plush-jill on 4/27/25.
//

#include "cli_thread.hpp"

#include <csignal>

CliThread::CliThread(AbstractClient* client) : m_client(client) {}

void CliThread::run() {
    const QString help_message {"Available commands: exit, help, reconnect, <SCPI command>"};
    std::cout << help_message.toStdString() << std::endl;
    while (m_running) {
        std::cout << "> " << std::flush;

        if (std::string input; std::getline(std::cin, input)) {
            if (input == "exit") {
                m_running = false;
                break;
            }

            if (input == "help") {
                std::cout << help_message.toStdString() << std::endl;
                continue;
            }

            if (input == "reconnect") {
                try {
                    m_client->reconnect();
                } catch (const std::exception& exception) {
                    std::cerr << std::format("Reconnect error: {}", exception.what()) << std::endl;
                }
                if (m_client->is_connected()) {
                    std::cout << "Reconnected successfully" << std::endl;
                }
                continue;
            }

            if (!input.empty()) {
                try {
                    QString result_message;
                    m_client->execute_command(QString::fromStdString(input).trimmed(), &result_message);
                    std::cout << result_message.toStdString() << std::endl;
                } catch (const std::exception& exception) {
                    std::cerr << std::format("Error: {}", exception.what()) << std::endl;
                }
            }
        } else {
            std::cin.clear();
        }
    }
}
