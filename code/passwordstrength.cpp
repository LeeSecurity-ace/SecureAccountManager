#include "passwordstrength.h"
#include <cctype>
#include <QSqlError>

std::string PasswordStrength::evaluate(const std::string &password) {
    bool hasUpper = false, hasLower = false, hasDigit = false, hasSpecial = false;

    if (password.length() < 6) {
        return "Weak";
    }

    for (char ch : password) {
        if (std::isupper(ch)) hasUpper = true;
        else if (std::islower(ch)) hasLower = true;
        else if (std::isdigit(ch)) hasDigit = true;
        else hasSpecial = true;
    }

    if (password.length() >= 8 && hasUpper && hasLower && hasDigit && hasSpecial) {
        return "Strong";
    }

    if (password.length() >= 6 && hasDigit && (hasUpper || hasLower)) {
        return "Medium";
    }

    return "Weak";
}
