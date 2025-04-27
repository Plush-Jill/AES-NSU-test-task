//
// Created by plush-jill on 4/24/25.
//

#ifndef COMMAND_VALIDATOR_HPP
#define COMMAND_VALIDATOR_HPP
#include <QRegularExpression>


class CommandValidator {
private:
    // TODO: добавить выражение для команд апи
    static QRegularExpression m_pattern;
public:

    /**
     * @brief Проверяет совпадение двух команд
     * @param command_1 Первая команда
     * @param command_2 Вторая команда
     * @return True если команды совпадают, False если нет
     */
    [[nodiscard]] static bool is_commands_match(const QString &command_1, const QString &command_2);

    /**
     * @brief Проверяет валидность команды SCPI
     * @param command Произвольная строка
     * @return True если команда соответствует SCPI, False если нет
     */
    [[nodiscard]] static bool is_SCPI_command(const QString &command);
};



#endif //COMMAND_VALIDATOR_HPP
