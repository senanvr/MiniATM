#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include<string>
#include<vector>
#include<ctime>
#include "sqlite3.h"

using namespace std;


void runQuery(sqlite3* DB, string sql) {
	char* error;
	sqlite3_exec(DB, sql.c_str(), NULL, 0, &error);
}

string encryptPassword(string s) {
	for (int i = 0; i < s.length(); i++) {
		s[i] += 5;
	}
		return s;
}

double getAmount() {
	double amount;
	while (!(cin >> amount) || amount < 0) {
		cout << "Invalid amount! Please enter a positive number: ";
		cin.clear();
		cin.ignore(1000, '\n');
	}
	return amount;
}

void logTransaction(sqlite3* DB, string accNo, string type, double amount) {
	time_t now = time(0);
	string date = ctime(&now);
	date.erase(date.find_last_not_of(" \n\r\t") + 1);

	string sql = "INSERT INTO TRANSACTIONS (ACCOUNT_NO, TRANSACTION_TYPE, AMOUNT, DATE) VALUES ('"
		+ accNo + "', '" + type + "', " + to_string(amount) + ", '" + date + "');";
	runQuery(DB, sql);
}




int main() {
	sqlite3* DB;

	if (sqlite3_open("atm_database.db", &DB) != SQLITE_OK) {
		cout << "Database error!" << endl;
		return 1;
	}

	runQuery(DB, "CREATE TABLE IF NOT EXISTS USERS(ACCOUNT_NO TEXT UNIQUE, PASSWORD TEXT, BALANCE REAL);");
	runQuery(DB, "CREATE TABLE IF NOT EXISTS TRANSACTIONS(ACCOUNT_NO TEXT, TRANSACTION_TYPE TEXT,AMOUNT REAL, DATE TEXT);");

	int choice =0;
	while (choice != 3) {
		cout << "\n========== MINI ATM SYSTEM ==========" << endl;
		cout << "1.Register (New Account)\n2.Login\n3.Exit\nChoice:";
		

		if (!(cin >> choice)) {
			cin.clear(); 
			cin.ignore(1000, '\n'); 
			continue;
		}

		if (choice == 1) {
			string acc, pass;
			cout << "New Account Number:";
			cin >> acc;
			cout << "Password:";
			cin >> pass;

			string sql = "INSERT INTO USERS VALUES('" + acc + "','" + encryptPassword(pass) + "', 0.0); ";

			if (sqlite3_exec(DB, sql.c_str(), NULL, 0, NULL) == SQLITE_OK) {
				cout << ">>> Registration successful! You can now login." << endl;
			}
			else {
				cout << "!!! Error: This account number already exists." << endl;
			}
		}

		else if (choice == 2) {

			string acc, pass;
			cout << "Account number:";
			cin >> acc;
			cout << "Password:";
			cin >> pass;

			sqlite3_stmt* stmt;
			string loginsql = "SELECT BALANCE FROM USERS WHERE ACCOUNT_NO = '" + acc + "' AND PASSWORD = '"+ encryptPassword(pass) +"';";

		if (sqlite3_prepare_v2(DB, loginsql.c_str(), -1, &stmt, NULL) == SQLITE_OK && sqlite3_step(stmt) == SQLITE_ROW) {
			double balance = sqlite3_column_double(stmt, 0);
			cout << "\n>>> LOGIN SUCCESSFUL!" << endl;

			if (acc == "admin") {
				cout << "\n--- ALL USERS (ADMIN PANEL) ---" << endl;
				sqlite3_stmt* listStmt;
				sqlite3_prepare_v2(DB, "SELECT * FROM USERS;", -1, &listStmt, NULL);
				cout << "ACCOUNT NO\tBALANCE" << endl;
				cout << "-----------------------------" << endl;

				while (sqlite3_step(listStmt) == SQLITE_ROW) {
					cout << sqlite3_column_text(listStmt, 0)
						<< "\t"
						<< sqlite3_column_double(listStmt, 2)
						<< " $" << endl;
				}
				sqlite3_finalize(listStmt);
			}
			else {
				int operation = 0;
				while (operation != 4) {
					cout << "\nYour Balance: " << balance << " $" << endl;
					cout << "1. Deposit\n2. Withdraw\n3. Transaction History\n4. Main Menu\nChoice: ";

					if (!(cin >> operation)) { cin.clear(); cin.ignore(1000, '\n'); continue; }

					if (operation == 1) {
						cout << "Enter amount to deposit: ";
						double m = getAmount();

						string updateSql = "UPDATE USERS SET BALANCE = BALANCE + "
							+ to_string(m) + " WHERE ACCOUNT_NO = '" + acc + "';";

						runQuery(DB, updateSql);
						logTransaction(DB, acc, "Deposit", m);
						balance += m;
						cout << "Transaction successful." << endl;


					}
					else if (operation == 2) {
						cout << "Enter amount to withdraw: ";
						double m = getAmount();

						if (m <= balance) {
							string updateSql = "UPDATE USERS SET BALANCE = BALANCE - "
								+ to_string(m) + " WHERE ACCOUNT_NO = '" + acc + "';";

							runQuery(DB, updateSql);
							logTransaction(DB, acc, "Withdrawal", m);

							balance -= m;
							cout << "Transaction successful." << endl;
						}
						else {
							cout << "Insufficient balance!" << endl;
						}

					}
					else if (operation == 3) {
						sqlite3_stmt* logStmt;
						string logSql = "SELECT * FROM TRANSACTIONS WHERE ACCOUNT_NO = '" + acc + "';";

						sqlite3_prepare_v2(DB, logSql.c_str(), -1, &logStmt, NULL);

						cout << "\n--- YOUR TRANSACTION HISTORY ---" << endl;

						while (sqlite3_step(logStmt) == SQLITE_ROW) {
							cout << sqlite3_column_text(logStmt, 3) << " | "
								<< sqlite3_column_text(logStmt, 1) << " | "
								<< sqlite3_column_double(logStmt, 2)
								<< " $" << endl;
						}
						sqlite3_finalize(logStmt);
					}
				}
			}
			sqlite3_finalize(stmt);
		}
		else {
		 cout << "!!! Invalid Account Number or Password!" << endl;
		}
     }
 }
				sqlite3_close(DB);
				return 0;
}
