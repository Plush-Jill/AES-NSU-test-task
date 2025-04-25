//
// Created by plush-jill on 4/24/25.
//

#ifndef GET_STATE_HPP
#define GET_STATE_HPP
#include <iostream>
#include <string>
#include "../registrar/command-registrar.hpp"


class GetStateCommand final : public AbstractCommand {
public:
    void execute(AbstractClient* client) override;
private:

    static const QString m_command_name_short;
    static const QString m_command_name_long;
    static const CommandRegistrar registrar;
};

#endif //GET_STATE_HPP
