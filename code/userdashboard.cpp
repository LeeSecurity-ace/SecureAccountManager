#include "userdashboard.h"
#include "entrymanager.h"
#include "serviceentry.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QInputDialog>
#include <QDateTime>
#include <QHeaderView>

UserDashboard::UserDashboard(QWidget *parent, int userId)
    : QWidget(parent), currentUserId(userId), passwordsVisible(false) {
    // Set a bigger default window size
    resize(900, 600);

    QVBoxLayout *layout = new QVBoxLayout(this);

    // Setup Table
    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(6);
    tableWidget->setHorizontalHeaderLabels(
        {"Service Name", "Username", "Password", "Strength", "Last Changed", "Actions"});
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->horizontalHeader()->setStretchLastSection(true);
    tableWidget->setSelectionMode(QAbstractItemView::NoSelection);

    // Adjust column widths
    tableWidget->setColumnWidth(4, 200); // "Last Changed" column
    tableWidget->setColumnWidth(5, 150); // "Actions" column

    layout->addWidget(tableWidget);

    // Populate the table
    populateTable();

    // Add New Entry Button
    QPushButton *addButton = new QPushButton("Add New Entry", this);
    connect(addButton, &QPushButton::clicked, this, &UserDashboard::addNewEntry);
    layout->addWidget(addButton);

    // Toggle Password Visibility Button
    togglePasswordButton = new QPushButton("Show Passwords", this);
    connect(togglePasswordButton, &QPushButton::clicked, this, &UserDashboard::togglePasswordVisibility);
    layout->addWidget(togglePasswordButton);

    // Logout Button
    logoutButton = new QPushButton("Log Out", this); // New button
    connect(logoutButton, &QPushButton::clicked, this, &UserDashboard::logout);
    layout->addWidget(logoutButton); // Add to the layout

    setLayout(layout);
}

void UserDashboard::populateTable() {
    tableWidget->setRowCount(0); // Clear the table
    QList<ServiceEntry *> entries = EntryManager::getInstance()->loadEntries(currentUserId);

    for (ServiceEntry *entry : entries) {
        int row = tableWidget->rowCount();
        tableWidget->insertRow(row);

        tableWidget->setItem(row, 0, new QTableWidgetItem(entry->getServiceName()));
        tableWidget->setItem(row, 1, new QTableWidgetItem(entry->getUsername()));
        tableWidget->setItem(row, 2, new QTableWidgetItem("*****"));
        tableWidget->setItem(row, 3, new QTableWidgetItem(entry->getPasswordStrength()));

        // Ensure consistent date formatting and adjust to local timezone
        QString formattedDate = QDateTime::fromString(entry->getLastChanged(), Qt::ISODate)
                                    .toLocalTime()
                                    .toString("yyyy-MM-dd hh:mm:ss");
        tableWidget->setItem(row, 4, new QTableWidgetItem(formattedDate));

        // Store the ID as data in a hidden column
        QTableWidgetItem *idItem = new QTableWidgetItem();
        idItem->setData(Qt::UserRole, entry->getId());
        tableWidget->setItem(row, 5, idItem);

        // Add action buttons
        QWidget *actionsWidget = new QWidget();
        QHBoxLayout *actionsLayout = new QHBoxLayout(actionsWidget);
        actionsLayout->setContentsMargins(0, 0, 0, 0);

        QPushButton *editButton = new QPushButton("Edit");
        connect(editButton, &QPushButton::clicked, [this, row]() { editEntry(row); });
        actionsLayout->addWidget(editButton);

        QPushButton *deleteButton = new QPushButton("Delete");
        connect(deleteButton, &QPushButton::clicked, [this, row]() { deleteEntry(row); });
        actionsLayout->addWidget(deleteButton);

        actionsWidget->setLayout(actionsLayout);
        tableWidget->setCellWidget(row, 5, actionsWidget);
    }
}

void UserDashboard::togglePasswordVisibility() {
    passwordsVisible = !passwordsVisible; // Toggle state

    for (int row = 0; row < tableWidget->rowCount(); ++row) {
        int entryId = tableWidget->item(row, 5)->data(Qt::UserRole).toInt();
        ServiceEntry *entry = EntryManager::getInstance()->loadEntries(currentUserId).at(row);

        if (passwordsVisible) {
            tableWidget->item(row, 2)->setText(entry->getPassword()); // Show password
            togglePasswordButton->setText("Hide Passwords");
        } else {
            tableWidget->item(row, 2)->setText("*****"); // Mask password
            togglePasswordButton->setText("Show Passwords");
        }
    }
}

void UserDashboard::addNewEntry() {
    QString serviceName = QInputDialog::getText(this, "New Entry", "Service Name:");
    QString username = QInputDialog::getText(this, "New Entry", "Username:");
    QString password = QInputDialog::getText(this, "New Entry", "Password:", QLineEdit::Password);

    if (serviceName.isEmpty() || username.isEmpty() || password.isEmpty()) {
        QMessageBox::critical(this, "Error", "All fields must be filled!");
        return;
    }

    QString strength = calculatePasswordStrength(password);

    ServiceEntry *newEntry = new ServiceEntry(serviceName, username, password);
    newEntry->setPasswordStrength(strength);

    // Ensure the Last Changed field is stored in local timezone
    newEntry->setLastChanged(QDateTime::currentDateTime().toLocalTime().toString("yyyy-MM-dd hh:mm:ss"));

    EntryManager::getInstance()->addEntry(newEntry, currentUserId);
    populateTable();
    QMessageBox::information(this, "Success", "New entry added successfully!");
}

void UserDashboard::editEntry(int row) {
    if (row < 0 || row >= tableWidget->rowCount()) {
        QMessageBox::critical(this, "Error", "Invalid entry selection.");
        return;
    }

    int entryId = tableWidget->item(row, 5)->data(Qt::UserRole).toInt();

    QString currentServiceName = tableWidget->item(row, 0)->text();
    QString currentUsername = tableWidget->item(row, 1)->text();

    QString newServiceName = QInputDialog::getText(this, "Edit Entry", "Service Name:",
                                                   QLineEdit::Normal, currentServiceName);
    QString newUsername = QInputDialog::getText(this, "Edit Entry", "Username:",
                                                QLineEdit::Normal, currentUsername);
    QString newPassword = QInputDialog::getText(this, "Edit Entry", "Password:",
                                                QLineEdit::Password);

    if (newServiceName.isEmpty() || newUsername.isEmpty() || newPassword.isEmpty()) {
        QMessageBox::critical(this, "Error", "Fields cannot be empty!");
        return;
    }

    QString strength = calculatePasswordStrength(newPassword);

    // Ensure the Last Changed field is updated in local timezone
    QString lastChanged = QDateTime::currentDateTime().toLocalTime().toString("yyyy-MM-dd hh:mm:ss");

    tableWidget->item(row, 0)->setText(newServiceName);
    tableWidget->item(row, 1)->setText(newUsername);
    tableWidget->item(row, 2)->setText("*****");
    tableWidget->item(row, 3)->setText(strength);
    tableWidget->item(row, 4)->setText(lastChanged);

    EntryManager::getInstance()->editEntry(entryId, newServiceName, newUsername, newPassword);
    QMessageBox::information(this, "Success", "Entry updated successfully!");
}

void UserDashboard::deleteEntry(int row) {
    if (row < 0 || row >= tableWidget->rowCount()) {
        QMessageBox::critical(this, "Error", "Invalid entry selection.");
        return;
    }

    int entryId = tableWidget->item(row, 5)->data(Qt::UserRole).toInt();

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Delete Entry", "Are you sure you want to delete this entry?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        EntryManager::getInstance()->deleteEntry(entryId);
        populateTable(); // Refresh the table to ensure consistency
        QMessageBox::information(this, "Success", "Entry deleted successfully!");
    }
}

QString UserDashboard::calculatePasswordStrength(const QString &password) {
    if (password.length() < 6) return "Weak";

    bool hasUpper = false, hasDigit = false, hasSpecial = false;
    for (const QChar &c : password) {
        if (c.isUpper()) hasUpper = true;
        if (c.isDigit()) hasDigit = true;
        if (!c.isLetterOrNumber()) hasSpecial = true;
    }

    if (password.length() > 8 && hasUpper && hasDigit && hasSpecial) return "Strong";
    if (password.length() >= 6 && (hasUpper || hasDigit)) return "Medium";
    return "Weak";
}
