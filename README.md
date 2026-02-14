🏦 Mini ATM System (C++ & SQLite)

A console-based banking simulation system built with C++ and SQLite3.
This project demonstrates database integration, transaction logging, and basic security mechanisms in a real-time environment.

🚀 Features

Account Registration – Create and store new user accounts in a persistent SQLite database.

Encrypted Login System – Passwords are stored using a simple Caesar-style encryption technique.

Balance Management – Deposit and withdraw funds with instant balance updates.

Transaction Logging – Every operation is recorded with a timestamp.

Admin Panel – Special account (admin) can view all users and balances.

Input Validation – Prevents invalid or negative monetary input.

## 🗄 Database Structure

### USERS Table

| Column      | Type          |
|------------|--------------|
| ACCOUNT_NO | TEXT (UNIQUE) |
| PASSWORD   | TEXT          |
| BALANCE    | REAL          |

### TRANSACTIONS Table

| Column            | Type |
|------------------|------|
| ACCOUNT_NO       | TEXT |
| TRANSACTION_TYPE | TEXT |
| AMOUNT           | REAL |
| DATE             | TEXT |

---

🛠 Tech Stack

Language: C++

Database: SQLite3

IDE: Visual Studio 2022

Architecture: Procedural programming with modular helper functions


## How to Run

1. Download SQLite Amalgamation source files.
2. Add `sqlite3.c` and `sqlite3.h` to your Visual Studio project.
3. Build and run the project.
4. The database file (`atm_database.db`) will be created automatically.

🔐 Security Note

This project uses a basic character-shift encryption for educational purposes.
It is not intended for production-level security.

📚 Learning Objectives

-SQLite integration in C++

-CRUD database operations

-Input validation handling

-Basic encryption logic

-Role-based access (Admin vs User)
