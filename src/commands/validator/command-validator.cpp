//
// Created by plush-jill on 4/24/25.
//

#include "command-validator.hpp"
// QRegularExpression CommandValidator::pattern = QRegularExpression(
// R"(^[A-Za-z\*]+(:[A-Za-z0-9]+)*\??$)",
//     QRegularExpression::CaseInsensitiveOption
// );

QRegularExpression CommandValidator::m_pattern(R"(^[A-Za-z\*]+(:[A-Za-z0-9]+)*\??$)", QRegularExpression::CaseInsensitiveOption);

bool CommandValidator::is_commands_match(const QString &command_1, const QString &command_2) {
    return command_1.compare(command_2, Qt::CaseInsensitive) == 0;

}

bool CommandValidator::is_SCPI_command(const QString &command) {
    return m_pattern.matchView(command).hasMatch();
}
