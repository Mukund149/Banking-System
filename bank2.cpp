#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib> // for rand() and srand()
#include <ctime>   // for time()
#include <set>     // for std::set to track unique account numbers
#include <string>

using namespace std;

class accountUpdate;
class AccountCreation
{
    long int securityPin;
    long int initialDeposit;
    string accountHolderName;
    string referenceName;
    string contactNumber;
    long long uniqueAccountNumber;
    static set<long long> assignedAccountNumbers;
    friend class AccountUpdate;
    friend class AccountDeletion;
    friend class FinancialManagement;

public:
    AccountCreation()
    {
        uniqueAccountNumber = generateUniqueAccountNumber(12356280800000LL, 99999999999999LL);
    }

    long long generateUniqueAccountNumber(long long minVal, long long maxVal)
    {
        long long newAccountNumber;
        do
        {
            newAccountNumber = rand() % (maxVal - minVal + 1) + minVal;
        } while (assignedAccountNumbers.find(newAccountNumber) != assignedAccountNumbers.end());
        assignedAccountNumbers.insert(newAccountNumber);
        return newAccountNumber;
    }


    void createNewAccount()
    {
        cout << "Enter account details carefully !!" << endl;
        cout << "Enter your full name: ";
        cin >> accountHolderName;
        cout << "Enter reference name: ";
        cin >> referenceName;
        bool isValidNumber = false;
        while (!isValidNumber)
        {
            cout << "Enter contact number :";
            cin >> contactNumber;

            if (contactNumber.length() == 10)
            {
                isValidNumber = true;
            }
            else
            {
                cout << "Invalid Input !!" << endl;
            }
        }
        bool isValidPin = false;
        while (!isValidPin)
        {
            cout << "Enter a 4-digit security pin :";
            cin >> securityPin;

            if (to_string(securityPin).length() == 4)
            {
                isValidPin = true;
            }
            else
            {
                cout << "Pin must be 4 digits !!" << endl;
            }
        }

        cout << "Enter the amount to deposit : " << endl;
        cin >> initialDeposit;

        saveAccountDetails();
    }

    void displayAccountDetails()
    {
        string fileNameInput;
        long int enteredSecurityPin;
        cout << "Enter your name : ";
        cin >> fileNameInput;
        cout << "Enter Your Security Pin : " << endl;
        cin >> enteredSecurityPin;
        cout << "\n";
        fetchSecurityCode(fileNameInput + ".txt");
        if (enteredSecurityPin == securityPin)
        {
            ifstream inFile(fileNameInput + ".txt");
            string fileLine;
            while (!inFile.eof())
            {
                getline(inFile, fileLine);
                cout << fileLine << endl;
            }
            inFile.close();
        }
        else
        {
            cout << "Incorrect pin, try again later !!";
        }
    }

    void saveAccountDetails()
    {
        ofstream outFile(accountHolderName + ".txt", ios::app);
        if (outFile.is_open())
        {
            outFile << "Account Number : " << uniqueAccountNumber << endl;
            outFile << "Account Holder : " << accountHolderName << endl;
            outFile << "Security Code : " << securityPin << endl;
            outFile << "Reference Name : " << referenceName << endl;
            outFile << "Contact Number : " << contactNumber << endl;
            outFile << "Bank Balance : " << initialDeposit << endl;
            outFile << "------------------------------------------" << endl;
            cout << "Details have been saved to " << accountHolderName << ".txt" << endl;
            outFile.close();
        }
        else
        {
            cout << "Unable to open the file";
        }
    }

    void fetchSecurityCode(string fileName)
    {
        ifstream inputFile(fileName);
        string lineContent;
        bool codeFound = false;

        while (getline(inputFile, lineContent))
        {
            if (lineContent.find("Security Code :") != string::npos)
            {
                size_t pos = lineContent.find(':');
                if (pos != string::npos)
                {
                    string codeString = lineContent.substr(pos + 1);
                    securityPin = stol(codeString);
                    codeFound = true;
                }
            }
        }
    }
};

class AccountUpdate : virtual public AccountCreation
{
    string updatedContactNumber;
    string previousContactNumber;

public:
    void modifyAccountDetails()
    {
        string filePath;
        string userResponse;
        cout << "You cannot change the name or account number." << endl;
        cout << "Do you wish to update the contact number? (yes/no)" << endl;
        cin >> userResponse;

        if (userResponse == "yes")
        {
            cout << "Enter your name: ";
            cin >> filePath;
            cout << "Enter your old contact number: ";
            cin >> previousContactNumber;
            cout << "Enter your new contact number: ";
            cin >> updatedContactNumber;

            ifstream inputFile(filePath + ".txt");
            string fileLine;

            string modifiedContent;

            while (getline(inputFile, fileLine))
            {
                size_t pos = 0;
                while ((pos = fileLine.find(previousContactNumber, pos)) != string::npos)
                {
                    fileLine.replace(pos, previousContactNumber.length(), updatedContactNumber);
                    pos += updatedContactNumber.length();
                }

                modifiedContent += fileLine + "\n";
            }
            inputFile.close();

            ofstream outputFile(filePath + ".txt");
            outputFile << modifiedContent;
            outputFile.close();
            cout << "Your contact number has been updated successfully.";
        }
        else
        {
            cout << "No changes were made.";
        }
    }
};

class AccountDeletion : virtual public AccountCreation
{
public:
    void removeAccount()
    {
        string accountFileName;
        long int enteredSecurityPin;
        cout << "Enter your name: ";
        cin >> accountFileName;
        cout << "Enter your security pin: ";
        cin >> enteredSecurityPin;

        fetchSecurityCode(accountFileName + ".txt");

        if (enteredSecurityPin == securityPin)
        {
            accountFileName += ".txt";
            if (remove(accountFileName.c_str()) == 0)
            {
                cout << "Account deleted successfully.";
            }
            else
            {
                cout << "Error deleting the account, please try again later." << endl;
            }
        }
        else
        {
            cout << "Incorrect pin!" << endl;
        }
    }
};

class FinancialManagement : virtual public AccountCreation
{
    long int depositAmount;
    long int accountBalance;
    long int withdrawAmount;

public:
    void generateReceipt(string accountHolderName, long int transactionAmount, long int currentBalance, string transactionType)
    {
        string receiptFile = accountHolderName + "Receipt.txt";

        ofstream out(receiptFile);
        if (out.is_open())
        {
            out << endl
                << "            SHAMPI BANK OF INDIA            " << endl;
            out << "Account Holder: " << accountHolderName << endl;
            out << "Account Number: 1235628XXXXXXX" << endl;
            out << "Money " << transactionType << ": " << transactionAmount << endl;
            out << "Current Balance: " << currentBalance << endl;
            out.close();
            cout << endl
                 << "Your receipt has been saved!" << endl;
        }
        else
        {
            cout << "Sorry, cannot generate your receipt." << endl;
        }

        ifstream in(receiptFile);
        string line;
        if (in.is_open())
        {
            cout << "\n--------------Your New Receipt--------------" << endl;
            while (!in.eof())
            {
                getline(in, line);
                cout << line << endl;
            }
            in.close();
        }
    }

    void depositMoney()
    {
        string filePath;
        long int enteredPin;
        cout << "Enter your name: ";
        cin >> filePath;
        cout << "Enter the security pin: ";
        cin >> enteredPin;
        fetchSecurityCode(filePath + ".txt");

        if (enteredPin == securityPin)
        {
            cout << "Enter the deposit amount: ";
            cin >> depositAmount;

            if (depositAmount < 30000)
            {
                calculateBalance(filePath + ".txt");

                long int updatedBalance = accountBalance + depositAmount;
                ifstream in(filePath + ".txt");
                string fileContent = "";
                string line;

                while (getline(in, line))
                {
                    size_t pos = 0;
                    while ((pos = line.find(to_string(accountBalance), pos)) != string::npos)
                    {
                        line.replace(pos, to_string(accountBalance).length(), to_string(updatedBalance));
                        pos += to_string(updatedBalance).length();
                    }

                    fileContent += line + "\n";
                }
                in.close();

                ofstream out(filePath + ".txt");
                out << fileContent;
                out.close();
                cout << "Money deposited successfully." << endl;

                ofstream logs(filePath + "Logs.txt", ios::app);
                if (logs.is_open())
                {
                    logs << "Money deposited: " << depositAmount << endl;
                    logs.close();
                }
                else
                {
                    cout << "Transaction log could not be created!" << endl;
                }

                generateReceipt(filePath, depositAmount, updatedBalance, "deposited");
            }
            else
            {
                cout << "Amount must be below 30,000." << endl;
            }
        }
        else
        {
            cout << "Incorrect pin! Try again later." << endl;
        }
    }

    void withdrawMoney()
    {
        string filePath;
        long int enteredPin;
        cout << "Enter your name: ";
        cin >> filePath;
        cout << "Enter the security pin: ";
        cin >> enteredPin;
        fetchSecurityCode(filePath + ".txt");

        if (enteredPin == securityPin)
        {
            calculateBalance(filePath + ".txt");
            cout << "Enter the withdrawal amount: ";
            cin >> withdrawAmount;

            if (withdrawAmount > accountBalance)
            {
                cout << "Insufficient balance!" << endl;
            }
            else
            {
                long int updatedBalance = accountBalance - withdrawAmount;

                ifstream in(filePath + ".txt");
                string fileContent = "";
                string line;

                while (getline(in, line))
                {
                    size_t pos = 0;
                    while ((pos = line.find(to_string(accountBalance), pos)) != string::npos)
                    {
                        line.replace(pos, to_string(accountBalance).length(), to_string(updatedBalance));
                        pos += to_string(updatedBalance).length();
                    }

                    fileContent += line + "\n";
                }
                in.close();

                ofstream out(filePath + ".txt");
                out << fileContent;
                out.close();
                cout << "Money withdrawn, balance updated successfully." << endl;

                ofstream logs(filePath + "Logs.txt", ios::app);
                if (logs.is_open())
                {
                    logs << "Money withdrawn: " << withdrawAmount << endl;
                    logs.close();
                }
                else
                {
                    cout << "Transaction log could not be created!" << endl;
                }
                generateReceipt(filePath, withdrawAmount, updatedBalance, "withdrawn");
            }
        }
        else
        {
            cout << "Incorrect pin! Try again later." << endl;
        }
    }

    void viewTransactionHistory()
    {
        string accountHolderName;
        cout << "Enter your name: ";
        cin >> accountHolderName;
        string logFilePath = accountHolderName + "Logs.txt";

        ifstream in(logFilePath);
        string line;
        if (in.is_open())
        {
            cout << "\n---------------TRANSACTION HISTORY---------------" << endl;
            while (!in.eof())
            {
                getline(in, line);
                cout << line << endl;
            }
            in.close();
        }
        else
        {
            cout << "No transaction history found. Please enter the correct name." << endl;
        }
    }

    void transferMoney()
    {
        string senderName, receiverName;
        long int enteredPin, transferAmount;
        cout << "Enter the sender's name: ";
        cin >> senderName;
        cout << "Enter the receiver's name: ";
        cin >> receiverName;
        calculateBalance(senderName + ".txt");
        cout << "Enter your security pin: ";
        cin >> enteredPin;
        fetchSecurityCode(senderName + ".txt");

        if (enteredPin == securityPin)
        {
            cout << "Enter the transfer amount: ";
            cin >> transferAmount;

            if (transferAmount > accountBalance)
            {
                cout << "Insufficient balance!" << endl;
            }
            else
            {
                long int senderNewBalance = accountBalance - transferAmount;

                ifstream in(senderName + ".txt");
                string fileContent = "";
                string line;

                while (getline(in, line))
                {
                    size_t pos = 0;
                    while ((pos = line.find(to_string(accountBalance), pos)) != string::npos)
                    {
                        line.replace(pos, to_string(accountBalance).length(), to_string(senderNewBalance));
                        pos += to_string(senderNewBalance).length();
                    }

                    fileContent += line + "\n";
                }
                in.close();

                ofstream out(senderName + ".txt");
                out << fileContent;
                out.close();

                ofstream logs(senderName + "Logs.txt", ios::app);
                if (logs.is_open())
                {
                    logs << "Money transferred: " << transferAmount << " | From " << senderName << " to " << receiverName << " |" << endl;
                    logs.close();
                }
                else
                {
                    cout << "Transaction log could not be created!" << endl;
                }
                generateReceipt(senderName, transferAmount, senderNewBalance, "transferred");

                calculateBalance(receiverName + ".txt");
                long int receiverNewBalance = accountBalance + transferAmount;

                ifstream recieverIn(receiverName + ".txt");
                string receiverContent = "";
                while (getline(recieverIn, line))
                {
                    size_t pos = 0;
                    while ((pos = line.find(to_string(accountBalance), pos)) != string::npos)
                    {
                        line.replace(pos, to_string(accountBalance).length(), to_string(receiverNewBalance));
                        pos += to_string(receiverNewBalance).length();
                    }

                    receiverContent += line + "\n";
                }
                recieverIn.close();

                ofstream recieverOut(receiverName + ".txt");
                recieverOut << receiverContent;
                recieverOut.close();

                ofstream rLogs(receiverName + "Logs.txt", ios::app);
                if (rLogs.is_open())
                {
                    rLogs << "Money received: " << transferAmount << " | From " << senderName << " to " << receiverName << " |" << endl;
                    rLogs.close();
                }
                else
                {
                    cout << "Transaction log could not be created!" << endl;
                }
            }
        }
        else
        {
            cout << "Incorrect pin! No transaction was made." << endl;
        }
    }

    void calculateBalance(string fileName)
    {
        ifstream infile(fileName);
        string line;

        while (getline(infile, line))
        {
            if (line.find("Bank Balance :") != string::npos)
            {
                size_t pos = line.find(':');
                if (pos != string::npos)
                {
                    string balanceStr = line.substr(pos + 1);
                    accountBalance = stol(balanceStr);
                }
            }
        }
    }
};

class accountManagement : public AccountUpdate, public AccountDeletion, public FinancialManagement
{
};

set<long long> AccountCreation::assignedAccountNumbers;

int main()
{
    srand(static_cast<unsigned>(time(0))); // Seed the random number generator

    accountManagement account; // Updated instance name

    int mainChoice = 0;
    while (mainChoice != 3)
    {
        cout << "\n|------WELCOME TO SHAMPOO BANK OF INDIA-----|" << endl;
        cout << "| 1. Account Services                       |" << endl;
        cout << "| 2. Financial Services                     |" << endl;
        cout << "| 3. Exit                                   |" << endl;
        cout << "| Enter your choice: ";
        cin >> mainChoice;
        cout << "|--------------------------------------------|" << endl;

        if (mainChoice == 1)
        {
            int accountChoice = 0;
            while (accountChoice != 5)
            {
                cout << "\n--- ACCOUNT SERVICES ---" << endl;
                cout << "1. Open New Account" << endl;
                cout << "2. View Account Details" << endl;
                cout << "3. Modify Account Details" << endl;
                cout << "4. Remove Account" << endl;
                cout << "5. Return to Main Menu" << endl;
                cout << "Enter your choice: ";
                cin >> accountChoice;
                cout << "\n";

                switch (accountChoice)
                {
                case 1:
                    account.createNewAccount(); // Updated function name
                    break;
                case 2:
                    account.displayAccountDetails(); // Updated function name
                    break;
                case 3:
                    account.modifyAccountDetails(); // Updated function name
                    break;
                case 4:
                    account.removeAccount(); // Updated function name
                    break;
                case 5:
                    break;
                default:
                    cout << "Invalid option!" << endl;
                }
            }
        }
        else if (mainChoice == 2)
        {
            int financeChoice = 0;
            while (financeChoice != 5)
            {
                cout << "\n--- FINANCIAL SERVICES ---" << endl;
                cout << "1. Deposit Funds" << endl;
                cout << "2. Withdraw Funds" << endl;
                cout << "3. Transfer Funds" << endl;
                cout << "4. View Transaction Logs" << endl;
                cout << "5. Return to Main Menu" << endl;
                cout << "Enter your choice: ";
                cin >> financeChoice;

                switch (financeChoice)
                {
                case 1:
                    account.depositMoney(); // Updated function name
                    break;
                case 2:
                    account.withdrawMoney(); // Updated function name
                    break;
                case 3:
                    account.transferMoney(); // Updated function name
                    break;
                case 4:
                    account.viewTransactionHistory(); // Updated function name
                    break;
                case 5:
                    break;
                default:
                    cout << "Invalid option!" << endl;
                }
            }
        }
        else if (mainChoice == 3)
        {
            cout << "Exiting...." << endl;
        }
        else
        {
            cout << "Invalid option!" << endl;
        }
    }
    return 0;
}