#include "mainwindow.h"
#include "AccountManager.h"
#include "registrationwindow.h"
#include "userdashboard.h"
#include <QVBoxLayout>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), userDashboard(nullptr) {
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    // Username field
    usernameField = new QLineEdit(this);
    usernameField->setPlaceholderText("Enter username");
    layout->addWidget(usernameField);

    // Password field
    passwordField = new QLineEdit(this);
    passwordField->setPlaceholderText("Enter password");
    passwordField->setEchoMode(QLineEdit::Password);
    layout->addWidget(passwordField);

    // Login button
    loginButton = new QPushButton("Login", this);
    layout->addWidget(loginButton);

    // Register button
    registerButton = new QPushButton("Register", this);
    layout->addWidget(registerButton);

    setCentralWidget(centralWidget);

    // Connect buttons
    connect(loginButton, &QPushButton::clicked, this, &MainWindow::handleLogin);
    connect(registerButton, &QPushButton::clicked, this, &MainWindow::handleRegister);
}

MainWindow::~MainWindow() {
    if (userDashboard) {
        delete userDashboard;
        userDashboard = nullptr;
    }
}

void MainWindow::handleLogin() {
    QString username = usernameField->text();
    QString password = passwordField->text();

    // Use AccountManager to validate login
    AccountManager *accountManager = AccountManager::getInstance();

    if (accountManager->validateLogin(username, password)) {
        QMessageBox::information(this, "Login Successful", "Welcome, " + username + "!");

        // Open the UserDashboard
        int userId = accountManager->getUserId(username);
        userDashboard = new UserDashboard(nullptr, userId);

        // Connect logout signal to return to login screen
        connect(userDashboard, &UserDashboard::logout, this, &MainWindow::handleLogout);

        userDashboard->show();

        this->hide(); // Hide the main login window
    } else {
        QMessageBox::critical(this, "Login Failed", "Invalid username or password.");
    }
}

void MainWindow::handleRegister() {
    RegistrationWindow *regWindow = new RegistrationWindow(this);
    regWindow->setModal(true);
    regWindow->exec();
}

void MainWindow::handleLogout() {
    // Confirm logout
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Confirm Logout", "Are you sure you want to log out?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (userDashboard) {
            userDashboard->close();
            delete userDashboard;
            userDashboard = nullptr;
        }

        // Clear the login fields
        usernameField->clear();
        passwordField->clear();

        // Show the login/registration screen
        this->show();
    }
}
