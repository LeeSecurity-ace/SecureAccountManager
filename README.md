# Account Information Manager

## Overview
The Account Information Manager is a secure and scalable system designed to manage user accounts and their associated service entries. It uses AES-128 encryption for sensitive data, ensuring user credentials and service passwords are safely stored.

---

## Features
- User Registration and Login: Securely register and log in with encrypted passwords.
- Service Entry Management: Add, edit, retrieve, and delete service entries tied to individual user accounts.
- Encryption: All passwords are encrypted using AES (Advanced Encryption Standard) to ensure data security.

---

## Requirements
- Qt Creator: Version 5.15 or higher.
- Database: SQLite (configured by default).
- (Optional) **DB Browser for SQLite**: Useful for easier database reading and management.

---

## Installation and Setup

1. Clone the Repository:

2. Open in Qt Creator:
   - Locate the `.pro` file in the repository.
   - Open it using Qt Creator.

3. Run the Application:
   - Simply press the `Run` button in Qt Creator.
   - The application will initialize the database and launch the GUI.

**Note:** The accounts.db database is saved by default in the code>build folder of the project.
---

## Usage

### Registration and Login
- Register a new user by providing a unique username and password.
- Log in with your credentials to access the user dashboard.

### Service Entry Management
- Add, edit, or delete service entries from the dashboard.
- Toggle password visibility for service entries.
- Logout at any time to switch accounts.

---

## Contributing
1. Fork the repository.
2. Make your changes in a feature branch.
3. Submit a pull request for review.

---

## License
This project is licensed under the MIT License. See `LICENSE.md` for details.

---

## Contact
For questions or issues, please contact [justin].
```
