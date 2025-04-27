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
    QMap<QPair<QString, QString>, Creator>    m_creators;
    QStringList                               m_command_names;

public:
    CommandFactory                  () = default;

    /**
     * @brief Создание команды по имени.
     * @param name Имя команды.
     * @return Указатель на созданную команду или nullptr, если команда не найдена.
     */
    [[nodiscard]] std::unique_ptr<AbstractCommand>  create                          (const QString& name) const;

    /**
     *
     * @return Инстанс синглтона.
     */
    static CommandFactory&                          instance                        ();

    /**
     * @brief Регистрация команды в фабрике.
     * @param name_short Короткое имя команды.
     * @param name_full Полное имя команды.
     * @param creator Функция создания команды.
     */
    void                                            register_command                (const QString& name_short, const QString &name_full, Creator creator);

    /**
     * @brief Получение списка доступных команд.
     * @return QStringList со всеми доступными командами.
     */
    [[nodiscard]] QStringList                       get_available_commands          () const;
};

#endif //COMMAND_FACTORY_HPP
