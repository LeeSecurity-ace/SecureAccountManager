#ifndef REGISTRATIONWINDOW_H
#define REGISTRATIONWINDOW_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QSqlError>

class RegistrationWindow : public QDialog {
    Q_OBJECT

public:
    explicit RegistrationWindow(QWidget *parent = nullptr);
    ~RegistrationWindow();

private slots:
    void handleRegistration();

private:
    QLineEdit *usernameField;
    QLineEdit *passwordField;
    QLineEdit *confirmPasswordField;
    QPushButton *registerButton;
};

#endif
