//
// Created by plush-jill on 4/24/25.
//

#include "command-factory.hpp"

CommandFactory & CommandFactory::instance() {
    static CommandFactory factory;
    return factory;
}

void CommandFactory::register_command(const QString &name, Creator creator) {
    m_creators[name] = std::move(creator);
    m_command_names << name;
}

std::unique_ptr<AbstractCommand> CommandFactory::create(const QString &name) {
    if (const auto it = m_creators.find(name); it != m_creators.end()) {
        return (it->second)();
    }
    return nullptr;
}

QStringList CommandFactory::get_available_commands() const {
    return m_command_names;
}

QJsonArray CommandFactory::get_available_commands_json() const {
    QJsonArray arr;
    for (const auto& name : m_command_names) {
        arr.append(name);
    }
    return arr;
}
