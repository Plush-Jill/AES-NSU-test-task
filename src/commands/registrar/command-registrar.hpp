//
// Created by plush-jill on 4/24/25.
//

#ifndef COMMAND_REGISTRAR_HPP
#define COMMAND_REGISTRAR_HPP
#include <algorithm>
#include "../factory/command-factory.hpp"


class CommandRegistrar {
public:
    CommandRegistrar(const QString& name_short, const QString& name_full, CommandFactory::Creator creator) {
        CommandFactory::instance().register_command(name_short, name_full, std::move(creator));
    }
};


#endif //COMMAND_REGISTRAR_HPP
