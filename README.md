# Banking Management System (C++)

This is a personal C++ project that simulates the essential operations of a banking system via a Command Line Interface (CLI). The project focuses on **data security**, **rigorous input validation**, and **information persistence**.

##  Key Features

* **Account Management**: Create accounts with unique IDs and account holder names.
* **Advanced Security**: 
    * Password-protected access (supports letters, numbers, and special characters).
    * Secure account deletion using double-confirmation logic (Password + "YES" string).
* **Robust Validation (Anti-Crash)**:
    * Protection against string/text input in numerical fields using `cin.fail()` checks.
    * Real-time balance verification for withdrawal operations.
* **Data Persistence**: Automatic saving and loading of account information from a local `bank_data.txt` file.

---

##  Installation and Execution

To compile and run the project on macOS or Linux, ensure you are using the C++17 standard:

```bash
g++ -std=c++17 Sistem_Bancar.cpp -o Sistem_Bancar && ./Sistem_Bancar
