//
// Created by plush-jill on 4/25/25.
//

#ifndef GET_DATA_SIZE_HPP
#define GET_DATA_SIZE_HPP
#include "../registrar/command-registrar.hpp"
#include "src/client/abstract-client.hpp"


class GetDataSize final : public AbstractCommand {
public:
    void execute(AbstractClient* client) override;
    void execute(AbstractClient* client, QString* result_message) override;
private:
    static quint32 m_response_size;
    static QString m_command_name_short;
    static QString m_command_name_long;
    static CommandRegistrar registrar;
};

#endif //GET_DATA_SIZE_HPP
