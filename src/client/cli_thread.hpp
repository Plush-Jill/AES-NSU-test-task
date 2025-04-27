//
// Created by plush-jill on 4/27/25.
//

#ifndef CLI_THREAD_HPP
#define CLI_THREAD_HPP
#include <iostream>
#include "client.hpp"


class CliThread : public QThread {
    Q_OBJECT

private:
    Client* m_client;
    std::atomic<bool> m_running{true};
public:
    explicit CliThread(Client* client) : m_client(client) {}
    void stop() { m_running = false; }

protected:
    void run() override;
};


#endif //CLI_THREAD_HPP
