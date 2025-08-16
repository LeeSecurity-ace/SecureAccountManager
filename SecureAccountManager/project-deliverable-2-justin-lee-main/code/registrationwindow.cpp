#include "registrationwindow.h"
#include "AccountManager.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDebug>

RegistrationWindow::RegistrationWindow(QWidget *parent) : QDialog(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Username field
    usernameField = new QLineEdit(this);
    usernameField->setPlaceholderText("Enter username");
    layout->addWidget(usernameField);

    // Password field
    passwordField = new QLineEdit(this);
    passwordField->setPlaceholderText("Enter password");
    passwordField->setEchoMode(QLineEdit::Password);
    layout->addWidget(passwordField);

    // Confirm password field
    confirmPasswordField = new QLineEdit(this);
    confirmPasswordField->setPlaceholderText("Confirm password");
    confirmPasswordField->setEchoMode(QLineEdit::Password);
    layout->addWidget(confirmPasswordField);

    // Register button
    registerButton = new QPushButton("Register", this);
    layout->addWidget(registerButton);

    // Connect register button signal to slot
    connect(registerButton, &QPushButton::clicked, this, &RegistrationWindow::handleRegistration);
}

RegistrationWindow::~RegistrationWindow() {}

void RegistrationWindow::handleRegistration() {
    QString username = usernameField->text();
    QString password = passwordField->text();
    QString confirmPassword = confirmPasswordField->text();

    if (username.isEmpty()) {
        QMessageBox::critical(this, "Registration Failed", "Username cannot be empty!");
        return;
    }

    if (password.isEmpty()) {
        QMessageBox::critical(this, "Registration Failed", "Password cannot be empty!");
        return;
    }

    if (password != confirmPassword) {
        QMessageBox::critical(this, "Registration Failed", "Passwords do not match!");
        return;
    }

    AccountManager *accountManager = AccountManager::getInstance();
    if (accountManager->registerUser(username, password)) {
        QMessageBox::information(this, "Registration Successful", "Account created successfully!");
        this->close();
    } else {
        QMessageBox::critical(this, "Registration Failed", "Registration failed! Username might already exist.");
    }
}
