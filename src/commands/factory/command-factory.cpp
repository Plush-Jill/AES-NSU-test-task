//
// Created by plush-jill on 4/24/25.
//

#include "command-factory.hpp"
#include "../validator/command-validator.hpp"

CommandFactory & CommandFactory::instance() {
    static CommandFactory factory;
    return factory;
}

void CommandFactory::register_command(const QString &name_short, const QString &name_full, Creator creator) {
    m_creators[QPair(name_short, name_full)] = std::move(creator);
    m_command_names << name_short;
}

std::unique_ptr<AbstractCommand> CommandFactory::create(const QString &name) const {
    for (auto it = m_creators.cbegin(); it != m_creators.cend(); ++it) {
        const auto &[fst, snd] = it.key();
        const auto &creator_function = it.value();

        if (CommandValidator::is_commands_match(fst, name) ||
            CommandValidator::is_commands_match(snd, name)) {
            return creator_function();
            }
    }
    return nullptr;
}
//     if (const auto it = m_creators.find(name); it != m_creators.end()) {
//         return (it->second)();
//     }
//     return nullptr;
// }

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
