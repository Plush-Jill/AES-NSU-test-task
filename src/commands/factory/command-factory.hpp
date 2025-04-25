//
// Created by plush-jill on 4/24/25.
//

#ifndef COMMAND_FACTORY_HPP
#define COMMAND_FACTORY_HPP
#include <functional>
#include <memory>
#include <QJsonArray>

#include "../abstract-command.hpp"


class CommandFactory {
public:
    using Creator = std::function<std::unique_ptr<AbstractCommand>()>;

private:
    std::unordered_map<QString, Creator>    m_creators;
    QStringList                             m_command_names;

public:
                                            CommandFactory                  () = default;
    static CommandFactory&                  instance                        ();
    void                                    register_command                (const QString& name, Creator creator);
    std::unique_ptr<AbstractCommand>        create                          (const QString& name);
    QStringList                             get_available_commands          () const;
    QJsonArray                              get_available_commands_json     () const;
};



#endif //COMMAND_FACTORY_HPP
