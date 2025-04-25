//
// Created by plush-jill on 4/24/25.
//
#include "abstract-command.hpp"

void AbstractCommand::execute(AbstractClient* client) {
    if (m_handler) {
        m_handler(client);
    } else {
        throw std::runtime_error("Command handler not set");
    }
}
