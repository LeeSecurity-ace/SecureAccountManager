#ifndef USERDASHBOARD_H
#define USERDASHBOARD_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>

class UserDashboard : public QWidget {
    Q_OBJECT

public:
    explicit UserDashboard(QWidget *parent = nullptr, int userId = -1);

signals:
    void logout(); // Signal to notify the parent of a logout action

private:
    int currentUserId;
    QTableWidget *tableWidget;
    QPushButton *togglePasswordButton;
    QPushButton *logoutButton; // New logout button
    bool passwordsVisible; // Tracks password visibility state

    void populateTable();
    void addNewEntry();
    void editEntry(int row);
    void deleteEntry(int row);
    QString calculatePasswordStrength(const QString &password);
    void togglePasswordVisibility();
};

#endif // USERDASHBOARD_H
