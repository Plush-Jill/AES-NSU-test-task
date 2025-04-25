//
// Created by plush-jill on 4/24/25.
//

#ifndef COMMAND_VALIDATOR_HPP
#define COMMAND_VALIDATOR_HPP
#include <QRegularExpression>


class CommandValidator {
private:
    // TODO: добавить выражение для команд апи
    static QRegularExpression m_regex;

public:
    /***
     * @brief Проверяет валидность команды
     * @param command Произвольная строка
     * @return True если команда валидна, то есть соответствует SCPI, False если нет
     */
    [[nodiscard]] static bool is_command_valid(QStringView command);
};



#endif //COMMAND_VALIDATOR_HPP
