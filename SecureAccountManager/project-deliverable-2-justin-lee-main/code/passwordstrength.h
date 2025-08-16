#ifndef PASSWORDSTRENGTH_H
#define PASSWORDSTRENGTH_H

#include <string>
#include <QSqlError>

class PasswordStrength {
public:
    static std::string evaluate(const std::string &password);
};

#endif
