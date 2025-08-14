🏦 Banking Simulation — Realistic Console Banking System in C++

A fully functional, console-based banking system simulation built with C++, implementing real-world banking operations like account management, transactions, loans, and secure access.

This isn’t just a menu-driven toy project — it’s a modular backend simulation with file-based persistence, OOP architecture, and real transaction logic, designed to model how core banking workflows operate.

⚡ Key Features

Account Management — Create, view, update, and delete accounts with persistent storage.

Secure Access — All accounts protected by 4-digit PIN verification.

Money Transactions — Deposit, withdraw, and transfer money between accounts.

Transaction History — Logs of all deposits, withdrawals, and transfers.

Loan Management — Apply for loans, track balances, and make repayments.

Receipts — Auto-generate transaction receipts for records.

🧠 System Architecture

Object-Oriented Design — Separate classes for accounts, money handling, and loans.

File-Based Storage — Uses C++ file I/O to persist data between sessions.

STL for Data Integrity — set ensures unique account numbers.

Basic Windows API — Simple loading animations for better user experience.

Error Handling & Validation — Protects against invalid input and unauthorized access.

🛠 Tech Stack
Layer	Tech / Tools
Language	C++
Paradigm	Object-Oriented Programming
Storage	File Handling (Persistent Data)
Libraries	Standard Template Library (STL)
UI Layer	Console + Windows API animations
🚀 How to Run

Clone the repository

git clone https://github.com/your-username/banking-simulation.git


Compile the program

g++ main.cpp -o banking-system


Run the executable

./banking-system


🎯 Project Goals

Simulate the core logic of a real banking backend in a console environment.

Reinforce OOP, file I/O, and modular program design principles.

Demonstrate secure data handling in a non-GUI application.

🧠 Concepts Practiced

OOP with inheritance (multiple + virtual)

File persistence and I/O handling

STL containers for unique record management

Data validation and error handling

Real-world simulation thinking

📜 License

MIT — free for educational use and modification.
