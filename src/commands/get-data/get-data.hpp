//
// Created by plush-jill on 4/24/25.
//

#ifndef GET_DATA_HPP
#define GET_DATA_HPP
#include "../registrar/command-registrar.hpp"
#include "src/client/abstract-client.hpp"


class GetData final : public AbstractCommand {
public:
    void execute(AbstractClient* client) override;
    void execute(AbstractClient* client, QString* result_message) override;
private:
    static QString m_command_name_short;
    static QString m_command_name_long;
    static CommandRegistrar registrar;
};

#endif //GET_DATA_HPP
