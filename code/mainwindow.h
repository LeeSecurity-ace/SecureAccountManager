#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include "userdashboard.h"
#include "registrationwindow.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleLogin();
    void handleRegister();
    void handleLogout(); // Add this declaration

private:
    QLineEdit *usernameField;
    QLineEdit *passwordField;
    QPushButton *loginButton;
    QPushButton *registerButton;
    RegistrationWindow *registrationWindow; // Login/Registration screen
    UserDashboard *userDashboard;          // User dashboard
};

#endif // MAINWINDOW_H
