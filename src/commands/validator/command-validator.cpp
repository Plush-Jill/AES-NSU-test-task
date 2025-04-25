//
// Created by plush-jill on 4/24/25.
//

#include "command-validator.hpp"
QRegularExpression CommandValidator::m_regex = QRegularExpression(R"((\w+(?::\w+)*\?))", QRegularExpression::CaseInsensitiveOption);
bool CommandValidator::is_command_valid(const QStringView command) {
    return m_regex.matchView(command).hasMatch();
}
