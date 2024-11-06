#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib> // for rand() and srand() and system("cls")
#include <ctime>   // for time()
#include <set>     // for std::set to track unique account numbers
#include <string>

using namespace std;

class accountUpdate;

class accountCreation
{
    long int securityCode;
    long int deposite;
    string name;
    string witness;
    string mobileNumber;
    long long accountNumber;
    static set<long long> generatedAccountNumbers;
    friend class accountUpdate;
    friend class accounDeletion;
    friend class MoneyManagement;
    friend class loan;

public:
    accountCreation()
    {
        accountNumber = generateAccountNumber(12356280800000LL, 99999999999999LL);
    }

    long long generateAccountNumber(long long min, long long max)
    {
        long long accountNumber;
        do
        {
            accountNumber = rand() % (max - min + 1) + min;
        } while (generatedAccountNumbers.find(accountNumber) != generatedAccountNumbers.end());
        generatedAccountNumbers.insert(accountNumber);
        return accountNumber;
    }

    void code(string filename)
    {
        ifstream infile(filename);
        string line;
        bool found = false;

        while (getline(infile, line))
        {
            if (line.find("Security Code :") != string::npos)
            {
                size_t pos = line.find(':');
                if (pos != string::npos)
                {
                    string User_code = line.substr(pos + 1);
                    securityCode = stol(User_code);
                    found = true;
                }
            }
        }
    }

    void createAccount()
    {
        cout << "Enter the details carefully !!" << endl;
        cout << "ENTER YOUR NAME : ";
        cin >> name;
        cout << "ENTER WITNESS NAME : ";
        cin >> witness;
        bool validNum = false;
        while (!validNum)
        {
            cout << "ENTER MOBILE NUMBER : ";
            cin >> mobileNumber;

            if (mobileNumber.length() == 10)
            {
                validNum = true;
            }
            else
            {
                cout << "Invalid Input !!" << endl;
            }
        }
        bool validCode = false;
        while (!validCode)
        {
            cout << "ENTER 4-DIGIT SECURITY PIN : ";
            cin >> securityCode;

            if (to_string(securityCode).length() == 4)
            {
                validCode = true;
            }
            else
            {
                cout << "Pin must be 4-digit !!" << endl;
            }
        }

        cout << "ENTER THE AMOUNT : " << endl;
        cin >> deposite;

        saveDetails();
    }

    void showDetails()
    {
        string filepath;
        long int security;
        cout << "What is your name : ";
        cin >> filepath;
        cout << "Enter Your Security code : " << endl;
        cin >> security;
        cout << "\n";
        code(filepath + ".txt");
        if (security == securityCode)
        {
            ifstream in(filepath + ".txt");
            string st;
            if(in.is_open())
            {
            while (in.eof() == 0)
            {
                getline(in, st);
                cout << st << endl;
            }
            in.close();
            }
            else
            {
                cout<<"NO SUCH ACCOUNT WAS FOUND !!"<<endl;
            }
            
        }
        else
        {
            cout << "Incorrect code, Try again later !!";
        }
    }

    void saveDetails()
    {
        ofstream out(name + ".txt", ios::app);
        if (out.is_open())
        {
            out << "==============================" << endl;
            out << "     SHAMPI BANK OF INDIA     " << endl;
            out << "==============================\n"
                << endl;
            out << "-------------------------" << endl;
            out << "     ACCOUNT SUMMARY     " << endl;
            out << "-------------------------" << endl;
            out << "Account Number : " << accountNumber << endl;
            out << "Account Holder : " << name << endl;
            out << "Account Holder witness : " << witness << endl;
            out << "Status : Active" << endl
                << endl;
            out << "-------------------------" << endl;
            out << "         SECURITY        " << endl;
            out << "-------------------------" << endl;
            out << "Security Code : " << securityCode << endl;
            out << "Mobile Number : " << mobileNumber << endl
                << endl;
            out << "-------------------------" << endl;
            out << "         BALANCE         " << endl;
            out << "-------------------------" << endl;
            out << "Bank Balance : " << deposite << endl
                << endl;
            out << "-------------------------" << endl;
            out << "           EXIT          " << endl;
            out << "-------------------------" << endl;
            cout << "Details have been saved to " << name << ".txt" << endl;
            out.close();
        }
        else
        {
            cout << "Cannot open the file";
        }
    }
};

class accountUpdate : virtual public accountCreation
{
    string newMobile;
    string oldNumber;

public:
    void updateDetails()
    {
        string path;
        string response;
        cout << "You cannot change the name or account number" << endl;
        cout << "do you wish to change the mobile number ?" << endl;
        cin >> response;
        if (response == "yes")
        {
            cout << "Enter your name" << endl;
            cin >> path;
            cout << "Enter your old number : ";
            cin >> oldNumber;
            cout << "Enter your new number : ";
            cin >> newMobile;

            ifstream file(path + ".txt");
            string line;

            string newNumber;

            while (getline(file, line))
            {
                size_t pos = 0;
                while ((pos = line.find(oldNumber, pos)) != string::npos)
                {
                    line.replace(pos, oldNumber.length(), newMobile);
                    pos += newMobile.length();
                }

                newNumber += line + "\n";
            }
            file.close();

            ofstream outfile(path + ".txt");
            outfile << newNumber;
            outfile.close();
            cout << "Your number is updated";
        }
        else
        {
            cout << "No changes were made";
        }
    }
};

class accounDeletion : virtual public accountCreation
{
public:
    void deleteAccount()
    {
        string filepath;
        long int seccode;
        string response;
        cout << "What is your name : ";
        cin >> filepath;
        cout << "Enter your security code :";
        cin >> seccode;
        cout<<"ARE YOU SURE YOU WANT TO DELETE YOUR ACCOUNT ? (yes/no) : "<<endl;
        cin>>response;
        if (response == "yes")
        {
            code(filepath + ".txt");
            if (seccode == securityCode)
            {
                filepath += ".txt";
                if (remove(filepath.c_str()) == 0)
                {
                    cout << "ACCOUNT DELETED SUCCESSFULLY";
                }
                else
                {
                    cout << "Error deleting, TRY AGAIN LATER" << endl;
                }
            }
            else
            {
                cout << "WRONG PIN !!" << endl;
            }
        }
        else if (response == "no")
        {
            cout<<"OK SIR..."<<endl;
        }
        else
        {
            cout<<"INVALID RESPONSE !!"<<endl;
        } 
    }
};

class MoneyManagement : virtual public accountCreation
{
    long int deposite;
    long int balance;
    long int withdrawal;
    friend class accountCreation;
    friend class loan;

public:
    void generateLogs(string filepath, string situation, long int amount, long int balance)
    {
        bool isNewFile = false;
        ifstream checkfile(filepath + "Logs.txt");
        if (checkfile.is_open())
        {
            isNewFile = true;
        }

        ofstream writeLogs(filepath + "Logs.txt", ios::app);
        if (writeLogs.is_open())
        {
            if (isNewFile == false)
            {
                writeLogs << "=================================================" << endl;
                writeLogs << "               TRANSACTION HISTORY               " << endl;
                writeLogs << "=================================================\n"
                          << endl;
                writeLogs << "TRANSACTION        ";
                writeLogs << "AMOUNT             ";
                writeLogs << "BALANCE            " << endl;
                writeLogs << "-------------------------------------------------" << endl;
            }

            writeLogs << situation;
            writeLogs << "           " << amount;
            writeLogs << "               " << balance << endl;
        }
        else
        {
            cout << "LOGS ARE NOT MADE !!" << endl;
        }
    }

    void reciept(string name, long int accBalance, long int currBalance, string situation)
    {
        string recieptFile = name + "Reciept" + ".txt";

        // Append the new receipt to the file
        ofstream out(recieptFile, ios::app);
        if (out.is_open())
        {
            out << endl
                << "            SHAMPI BANK OF INDIA            " << endl;
            out << "Account Holder : " << name << endl;
            out << "Account number : 1235628XXXXXXX " << endl;
            out << "Money " << situation << " : " << accBalance << endl;
            out << "Current Balance : " << currBalance << endl;
            out.close();
            cout << endl
                 << "Your receipt has been saved !!" << endl;
        }
        else
        {
            cout << "Sorry, cannot fetch your receipt :( " << endl;
        }

        // Open the file to read only the last receipt
        ifstream in(recieptFile);
        if (in.is_open())
        {
            string st, lastReceipt = "";
            string delimiter = "            SHAMPI BANK OF INDIA            ";

            // Read through the file and capture the last receipt
            while (getline(in, st))
            {
                if (st.find(delimiter) != string::npos)
                {
                    lastReceipt = ""; // Start capturing from the last delimiter
                }
                lastReceipt += st + "\n"; // Accumulate the lines of the last receipt
            }
            in.close();

            // Display the last receipt
            cout << "\n--------------RECEIPT--------------" << endl;
            cout << lastReceipt;
            cout << "--------------RECEIPT--------------" << endl;
        }
    }

    void BalanceCalc(string filename)
    {
        ifstream infile(filename);
        string line;
        bool found = false;

        while (getline(infile, line))
        {
            if (line.find("Bank Balance :") != string::npos)
            {
                size_t pos = line.find(':');
                if (pos != string::npos)
                {
                    string User_balance = line.substr(pos + 1);
                    balance = stol(User_balance);
                    found = true;
                }
            }
        }
    }
    
    void replace(string filename, long int newNum, long int oldNum)
    {
        ifstream in(filename + ".txt");
        string fileContent = "";
        string line;

        while (getline(in, line))
        {
            size_t pos = 0;
            while ((pos = line.find(to_string(oldNum), pos)) != string::npos)
            {
                line.replace(pos, to_string(oldNum).length(), to_string(newNum));
                pos += to_string(newNum).length();
            }

            fileContent += line + "\n";
        }
        in.close();

        ofstream out(filename + ".txt");
        out << fileContent;
        out.close();
    }

    void moneyDeposite()
    {
        string filepath;
        long int secCode;
        cout << "What is your name : ";
        cin >> filepath;
        cout << "Enter the security code : " << endl;
        cin >> secCode;
        code(filepath + ".txt");
        if (secCode == securityCode)
        {
            cout << "Enter the amount : ";
            cin >> deposite;
            if (deposite < 30000)
            {
                BalanceCalc(filepath + ".txt");

                long int newBalance = balance + deposite;
                replace(filepath, newBalance, balance);
                generateLogs(filepath, "Deposited", deposite, newBalance);
                reciept(filepath, deposite, newBalance, "deposited");
                cout << "\nMONEY DEPOSITED !!";
            }
            else
            {
                cout << "Amount must be below 30k" << endl;
            }
        }
        else
        {
            cout << "Wrong pin try again later !!" << endl;
        }
    }

    void withdrawMoney()
    {
        string filepath;
        long int secCode;
        cout << "What is your name : ";
        cin >> filepath;
        cout << "Enter the security Code :" << endl;
        cin >> secCode;
        code(filepath + ".txt");
        if (secCode == securityCode)
        {
            BalanceCalc(filepath + ".txt");
            cout << "Enter the withdraw amount : ";
            cin >> withdrawal;
            if (withdrawal > balance)
            {
                cout << "Insufficient Balance !!" << endl;
            }
            else
            {
                long int newBalance = balance - withdrawal;
                replace(filepath, newBalance, balance);
                generateLogs(filepath, "Withdrawn", withdrawal, newBalance);
                reciept(filepath, withdrawal, newBalance, "withdrawn");
                cout << "\nMONEY WITHDRAWN !!"<<endl;
            }
        }
        else
        {
            cout << "Wrong PIN try again later !!";
        }
    }

    void showLogs()
    {
        string path;
        cout << endl
             << "Enter your name : ";
        cin >> path;
        string filepath = path + "Logs" + ".txt";
        ifstream in(filepath);
        string st;
        if (in.is_open())
        {

            while (in.eof() == 0)
            {
                getline(in, st);
                cout << st << endl;
            }
            in.close();
        }
        else
        {
            cout << "Enter the correct name" << endl;
        }
    }

    void transaction()
    {
        string sendersPath;
        string recieversPath;
        long int secCode;
        long int transaction;
        cout << "Enter the sender's name : ";
        cin >> sendersPath;
        cout << "Enter the reciever's name : ";
        cin >> recieversPath;
        BalanceCalc(sendersPath + ".txt");
        cout << "Enter your Security Code :" << endl;
        cin >> secCode;
        code(sendersPath + ".txt");
        bool possibleTrans = false;
        if (secCode == securityCode)
        {
            while (!possibleTrans)
            {
                cout << "Enter the amount you wish to tranfer : ";
                cin >> transaction;
                if (transaction > balance)
                {
                    cout << "INSUFFICIENT BALANCE !!" << endl;
                    break;
                }
                else
                {
                    long int newBalance = balance - transaction;
                    replace(sendersPath, newBalance, balance);
                    generateLogs(sendersPath, "to " + recieversPath, transaction, newBalance);
                    reciept(sendersPath, transaction, newBalance, "Withdrawn");
                }
                BalanceCalc(recieversPath + ".txt");
                long int recieverNewBalance = balance + transaction;
                replace(recieversPath, recieverNewBalance, balance);
                generateLogs(recieversPath, "from " + sendersPath, transaction, recieverNewBalance);

                possibleTrans = true;
            }
        }
        else
        {
            cout << "WRONG PIN, NO TRANSACTION WAS MADE !!" << endl;
        }
    }

};

class loan : virtual public MoneyManagement, virtual public accountCreation
{
    string applier;
    long long loanID;
    string choice;
    int years;
    long int principal;
    float interest = float(4) / 100;
    long int return_value;

public:
    void applyLoan()
    {
        long int secCode;
        loanID = generateAccountNumber(149000, 149999);
        cout << "Enter your name : ";
        cin >> applier;
        cout << "Enter your security code :" << endl;
        cin >> secCode;
        code(applier + ".txt");
        if (secCode == securityCode)
        {
            cout << "Enter your loan amount :";
            cin >> principal;
            BalanceCalc(applier + ".txt");
            ifstream in(applier + "Loans.txt");
            string line;
            bool wordIn = false;
            while (getline(in, line))
            {
                if (line.find("LOAN IS CURRENTLY ACTIVE") != string::npos)
                {
                    in.close();
                    wordIn = true;
                }
            }
            in.close();

            if (wordIn == true)
            {
                cout << "Loan cannot be approved, Please repay the existing loan";
            }
            else if (balance < 10000 | principal >= 5 * balance)
            {
                cout << "Loan of " << principal << " cannot be approved for bank balance of " << balance << endl;
            }
            else
            {
                return_value = principal;
                cout << "Interest rate offered : 4% " << endl;
                cout << "Loan term duration : ";
                cin >> years;
                for (int i = 0; i < years; i++)
                {
                    return_value = return_value * (1 + interest);
                }

                cout << "Amount to be returned : " << return_value << endl;
                cout << "Are you interested ? :" << endl;
                cin >> choice;
                if (choice == "yes")
                {
                    ofstream out(applier + "Loans.txt", ios::app);
                    if (out.is_open())
                    {
                        out << "\nLoan ID : " << loanID << endl;
                        out << "Loan Amount : " << principal << endl;
                        out << "Interest Rate : 4%" << endl;
                        out << "Repayment Peroiod (in years): " << years << endl;
                        out << "Amount Due : " << return_value << endl;
                        out << "LOAN IS CURRENTLY ACTIVE " << endl;
                    }

                    long int newBalance = balance + principal;
                    replace(applier, newBalance, balance);
                    cout << "LOAN APPROVED !!" << endl;
                    cout << "Your bank balance is updated " << endl;
                    generateLogs(applier, "Deposited", return_value, newBalance);
                }
                else
                {
                    cout << "OK SIR..";
                }
            }
        }
        else
        {
            cout << "WRONG PIN. CANNOT PROCEED !!" << endl;
        }
    }

    void repayLoan()
    {
        long long inputLoanID;
        long int repayAmount;
        string filepath;
        long int amountDue;
        cout << "Enter your name : ";
        cin >> filepath;
        ifstream in(filepath + "Loans.txt");
        if (in.is_open())
        {
            string st, lastReceipt = "";
            string delimiter = "------------------------------------------";

            while (getline(in, st))
            {
                if (st.find(delimiter) != string::npos)
                {
                    lastReceipt = "";
                }
                lastReceipt += st + "\n";
            }
            in.close();

            cout << "\n-------------YOUR LOANS------------" << endl;
            cout << lastReceipt << endl;
        }

        cout << "\nEnter 6-digit loan ID : " << endl;
        cin >> inputLoanID;

        ifstream infile(filepath + "Loans.txt");
        string line;
        bool found = false;

        while (getline(infile, line))
        {
            if (line.find("Loan ID :") != string::npos)
            {
                size_t pos = line.find(':');
                if (pos != string::npos)
                {
                    string UserLoanCode = line.substr(pos + 1);
                    loanID = stoll(UserLoanCode);
                    found = true;
                }
            }
        }

        if (inputLoanID == loanID)
        {
            cout << "Enter the amount : ";
            cin >> repayAmount;

            ifstream repay(filepath + "Loans.txt");
            string repayLine;
            bool dueFound = false;

            while (getline(repay, repayLine))
            {
                if (repayLine.find("Amount Due :") != string::npos)
                {
                    size_t rpos = repayLine.find(":");
                    if (rpos != string::npos)
                    {
                        char *end;
                        string amount = repayLine.substr(rpos + 1);
                        amountDue = stol(amount);
                        dueFound = true;
                    }
                }
            }
            long int newAmountDue = amountDue - repayAmount;
            replace(filepath + "Loans", newAmountDue, amountDue);
            cout << "AMOUNT WAS PAID !!" << endl;

            BalanceCalc(filepath + ".txt");
            long int newDueBalance = balance - repayAmount;
            replace(filepath, newDueBalance, balance);
            generateLogs(filepath, "Withdrawn", repayAmount, newDueBalance);
            reciept(filepath, balance, newDueBalance, "Withdrawn");

            if (newAmountDue <= 0)
            {
                string oldStatus = "LOAN IS CURRENTLY ACTIVE";
                string newStatus = "LOAN IS PAID";
                ifstream replaceWord(filepath + "Loans.txt");
                string fileContent;
                string stReplace;

                while (getline(replaceWord, stReplace))
                {
                    size_t dpos = 0;
                    while (dpos = stReplace.find(oldStatus, dpos) != string::npos)
                    {
                        stReplace.replace(dpos, oldStatus.length(), newStatus);
                        dpos += newStatus.length();
                    }
                    fileContent += stReplace + "\n";
                }
                replaceWord.close();

                ofstream coutWord(filepath + "Loans.txt");
                coutWord << fileContent;
                coutWord.close();
                cout << "LOAN IS PAID. THANK YOU " << endl;
            }
        }
        else
        {
            cout << "INVALID LOAN ID !!" << endl;
        }
    }
};

class accountManagement : public accountUpdate, public accounDeletion, public loan
{
};

set<long long> accountCreation::generatedAccountNumbers;

void closeMenu(int var){
    var = 0;
    cout<<"\nPress 1 to close "<<endl;
    while (var != 1)
    {
        cin>>var;
        if (var == 1)
        {
            break;
        }
        else
        {
            cout<<"Invalid input"<<endl;
        } 
    }
}

int main()
{
    // Seed the random number generator with the current time
    srand(static_cast<unsigned>(time(0)));
    // Seed the random number generator with the current time

    accountManagement p1, p2;
    int choice = 0;
    while (choice != 4)
    {
        system("cls");
        cout << "\n|============================================|" << endl;
        cout << "|            SHAMPI BANK OF INDIA            |" << endl;
        cout << "|============================================|" << endl;
        cout << "| 1.|  Account Management                    |" << endl;
        cout << "| 2.|  Money Management                      |" << endl;
        cout << "| 3.|  Loan Check                            |" << endl;
        cout << "| 4.|  Exit                                  |" << endl;
        cout << "|============================================|" << endl;
        cout << "| Enter your choice : ";
        cin >> choice;
        cout << "|============================================|" << endl;

        if (choice == 1)
        {
            int accPage;
            int accManage = 0;
            int back = 0;
            while (accManage != 5)
            {
                system("cls");
                cout << "|===========================|" << endl;
                cout << "|     ACCOUNT MANAGEMENT    |" << endl;
                cout << "|===========================|" << endl;
                cout << "|1.| Create Account         |" << endl;
                cout << "|2.| Show Details           |" << endl;
                cout << "|3.| Update Details         |" << endl;
                cout << "|4.| Delete Account         |" << endl;
                cout << "|5.| Home                   |" << endl;
                cout << "|===========================|"<<endl;
                cout << "|Enter your choice : ";
                cin >> accManage;
                cout << "\n";
                switch (accManage)
                {
                case 1:
                    system("cls");
                    cout<<"=========================="<<endl;
                    cout<<"     ACCOUNT CREATION     "<<endl;
                    cout<<"==========================\n"<<endl;
                    p1.createAccount();
                    closeMenu(accPage);
                    break;
                case 2:
                    system("cls");
                    cout<<"======================"<<endl;
                    cout<<"     SHOW DETAILS     "<<endl;
                    cout<<"======================\n"<<endl;
                    p1.showDetails();
                    closeMenu(accPage);
                    break;
                case 3:
                    system("cls");
                    cout<<"========================"<<endl;
                    cout<<"     UPDATE DETAILS     "<<endl;
                    cout<<"========================\n"<<endl;
                    p1.updateDetails();
                    closeMenu(accPage);
                    break;
                case 4:
                    system("cls");
                    cout<<"========================"<<endl;
                    cout<<"     DELETE ACCOUNT     "<<endl;
                    cout<<"========================\n"<<endl;
                    p1.deleteAccount();
                    closeMenu(accPage);
                    break;
                case 5:
                    break;

                default:
                    cout << "Invalid option !!" << endl;
                }
            }
        }
        else if (choice == 2)
        {
            int moneyPage;
            int moneyManage = 0;
            while (moneyManage != 5)
            {
                system("cls");
                cout << "|==========================|" <<endl;
                cout << "|     MONEY MANAGEMENT     |" << endl;
                cout << "|==========================|" <<endl;
                cout << "|1.| Deposite              |" << endl;
                cout << "|2.| Withdraw              |" << endl;
                cout << "|3.| Transfer              |" << endl;
                cout << "|4.| Show Logs             |" << endl;
                cout << "|5.| Home                  |" << endl;
                cout << "|==========================|"<<endl;
                cout << "|Enter your choice : ";
                cin >> moneyManage;

                switch (moneyManage)
                {
                case 1:
                    system("cls");
                    cout<<"================="<<endl;
                    cout<<"     DEPOSIT     "<<endl;
                    cout<<"=================\n"<<endl;
                    p1.moneyDeposite();
                    closeMenu(moneyPage);
                    break;

                case 2:
                    system("cls");
                    cout<<"=================="<<endl;
                    cout<<"     WITHDRAW     "<<endl;
                    cout<<"==================\n"<<endl;
                    p1.withdrawMoney();
                    closeMenu(moneyPage);
                    break;

                case 3:
                    system("cls");
                    cout<<"====================="<<endl;
                    cout<<"     TRANSACTION     "<<endl;
                    cout<<"=====================\n"<<endl;
                    p1.transaction();
                    closeMenu(moneyPage);
                    break;

                case 4:
                    system("cls");
                    cout<<"=============="<<endl;
                    cout<<"     LOGS     "<<endl;
                    cout<<"==============\n"<<endl;
                    p1.showLogs();
                    closeMenu(moneyPage);
                    break;

                case 5:
                    break;

                default:
                    cout << "Invalid Option !" << endl;
                }
            }
        }
        else if (choice == 3)
        {
            int loanPage;
            int loanCheck = 0;
            while (loanCheck != 3)
            {
                system("cls");
                cout << "|========================|" <<endl;
                cout << "|       LOAN CHECK       |" << endl;
                cout << "|========================|" <<endl;
                cout << "|1.| Apply for loan      |" << endl;
                cout << "|2.| Repay Loan          |" << endl;
                cout << "|3.| Home                |" << endl;
                cout << "|========================|"<<endl;
                cout << "|Enter your choice : ";
                cin >> loanCheck;
                switch (loanCheck)
                {
                case 1:
                    system("cls");
                    cout<<"====================="<<endl;
                    cout<<"     APPLICATION     "<<endl;
                    cout<<"=====================\n"<<endl;
                    p1.applyLoan();
                    closeMenu(loanPage);
                    break;

                case 2:
                    system("cls");
                    cout<<"==================="<<endl;
                    cout<<"     REPAYMENT     "<<endl;
                    cout<<"===================\n"<<endl;
                    p1.repayLoan();
                    closeMenu(loanPage);
                    break;

                case 3:
                    break;

                default:
                    cout << "invalid option !" << endl;
                }
            }
        }
        else if (choice == 4)
        {
            cout << "|============================================|" << endl;
            cout << "|       THANK YOU FOR BANKING WITH US        |" << endl;
            cout << "|============================================|" << endl;
        }
        else
        {
            cout << "Option Invalid !" << endl;
        }
    }
    return 0;
}