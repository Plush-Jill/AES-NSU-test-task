//
// Created by plush-jill on 4/24/25.
//

#ifndef GET_DATA_HPP
#define GET_DATA_HPP
#include "../registrar/command-registrar.hpp"


class GetData final : public AbstractCommand {
public:
    void execute(Client* client) override;

private:
    static QString m_command_name_short;
    static QString m_command_name_long;
    static CommandRegistrar registrar;
};

#endif //GET_DATA_HPP
