#include <iostream>
#include<fstream>
#include<sstream>
#include<cstdio>
#include <cstdlib>  // for rand() and srand()
#include <ctime>    // for time()
#include <set>      // for std::set to track unique account numbers
#include <string>

using namespace std;

class accountUpdate;

class accountCreation {
    long int securityCode;
    long int deposite;
    string name;
    string witness;
    string mobileNumber;
    long long accountNumber; // Store the unique account number
    static set<long long> generatedAccountNumbers; // Static set to track unique account numbers
    friend class accountUpdate;
    friend class accounDeletion;
    friend class MoneyManagement;
    friend class loan;

public:
    // Constructor to initialize and generate account number
    accountCreation() {
        accountNumber = generateAccountNumber(12356280800000LL, 99999999999999LL);
    }

    // Method to generate a random account number
    long long generateAccountNumber(long long min, long long max) {
        long long accountNumber;
        do {
            accountNumber = rand() % (max - min + 1) + min; // Generate a random number in the range
        } while (generatedAccountNumbers.find(accountNumber) != generatedAccountNumbers.end()); // Check for uniqueness
        generatedAccountNumbers.insert(accountNumber); // Add to set to track uniqueness
        return accountNumber;
    }

        void code(string filename){
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

    void createAccount() {
        cout<<"Enter the details carefully !!"<<endl;
        cout << "Enter your name: ";
        cin>>name;
        cout << "Enter witness name: ";
        cin>>witness;
        bool validNum = false;
        while (!validNum)
        {
            cout<<"Enter mobile number :";
            cin>>mobileNumber;

            if (mobileNumber.length() == 10)
            {
                validNum = true;
            }
            else
            {
                cout<<"Invalid Input !!"<<endl;
            } 
        }
        bool validCode = false;
        while (!validCode)
        {
            cout<<"Enter a 4-digit security code :";
            cin>>securityCode;

            if (to_string(securityCode).length()==4)
            {
                validCode = true;
            }
            else
            {
                cout<<"Pin must be 4-digit !!"<<endl;
            }  
        }
        

        cout<<"Enter the amount to be added : "<<endl;
        cin>>deposite;

        saveDetails();
    }

    void showDetails() {
        string filepath;
        long int security;
        cout<<"What is your name : ";
        cin>>filepath;
        cout<<"Enter Your Security code : "<<endl;
        cin>>security;
        code(filepath + ".txt");
        if (security == securityCode)
        {
            ifstream in(filepath + ".txt");
            string st;
            while (in.eof() == 0)
            {
                getline(in, st);
                cout<<st<<endl;
            }
            in.close();
        }
        else
        {
            cout<<"Incorrect code, Try again later !!";
        }
        

    }

    void saveDetails(){
        ofstream out(name + ".txt", ios::app);
        if (out.is_open())
        {
            out<<"Account Number : "<<accountNumber<<endl;
            out<<"Account Holder : "<<name<<endl;
            out<<"Security Code : "<<securityCode<<endl;
            out<<"Account Holder witness : "<<witness<<endl;
            out<<"Mobile Number : "<<mobileNumber<<endl;
            out<<"Bank Balance : "<<deposite<<endl;
            cout<<"Details have been saved to "<<name<<".txt"<<endl;
            out.close();
        }
        else
        {
            cout<<"Cannot open the file";
        }  
    }
};

class accountUpdate : virtual public accountCreation{
    string newMobile;
    string oldNumber;

    public:
        void updateDetails(){
            string path;
            string response;
            cout<<"You cannot change the name or account number"<<endl;
            cout<<"do you wish to change the mobile number ? \n Enter your security code"<<endl;
            cin>>response;
            if (response == "yes")
            {
                cout<<"Enter your name"<<endl;
                cin>>path;
                cout<<"Enter your old number : ";
                cin>>oldNumber;
                cout<<"Enter your new number : ";
                cin>>newMobile;

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
                outfile<<newNumber;
                outfile.close();
                cout<<"Your number is updated";
                
            }
            else
            {
                cout<<"No changes were made";
            }
        }
};

class accounDeletion : virtual public accountCreation{
    public: 
        void deleteAccount(){
            string filepath;
            long int seccode;
            cout<<"What is your name : ";
            cin>>filepath;
            cout<<"Enter your security code :";
            cin>>seccode;
            code(filepath + ".txt");
            if (seccode == securityCode)
            {
            filepath += ".txt";
            if (remove(filepath.c_str()) == 0)
            {
                cout<<"Account deleted successfully";
            }
            else
            {
                cout<<"Error deleting, TRY AGAIN LATER"<<endl;
            }
            }
            else
            {
                cout<<"WRONG PIN !!"<<endl;
            }
            
        }
};

class MoneyManagement : virtual public accountCreation{
    long int deposite;
    long int balance;
    long int withdrawal;
    friend class accountCreation;
    friend class loan;

    public:
void reciept(string name, long int accBalance, long int currBalance, string situation) {
    string recieptFile = name + "Reciept" + ".txt";

    // Append the new receipt to the file
    ofstream out(recieptFile, ios::app);
    if (out.is_open()) {
        out << endl << "            SHAMPI BANK OF INDIA            " << endl;
        out << "Account Holder : " << name << endl;
        out << "Account number : 1235628XXXXXXX " << endl;
        out << "Money " << situation << " : " << accBalance << endl;
        out << "Current Balance : " << currBalance << endl;
        out.close();
        cout << endl << "Your receipt has been saved !!" << endl;
    } else {
        cout << "Sorry, cannot fetch your receipt :( " << endl;
    }

    // Open the file to read only the last receipt
    ifstream in(recieptFile);
    if (in.is_open()) {
        string st, lastReceipt = "";
        string delimiter = "            SHAMPI BANK OF INDIA            ";

        // Read through the file and capture the last receipt
        while (getline(in, st)) {
            if (st.find(delimiter) != string::npos) {
                lastReceipt = "";  // Start capturing from the last delimiter
            }
            lastReceipt += st + "\n";  // Accumulate the lines of the last receipt
        }
        in.close();

        // Display the last receipt
        cout << "\n--------------Your New Receipt--------------" << endl;
        cout << lastReceipt << endl;
    }
}

        void moneyDeposite(){
            string filepath;
            long int secCode;
            cout<<"What is your name : ";
            cin>>filepath;
            cout<<"Enter the security code : "<<endl;
            cin>>secCode;
            code(filepath + ".txt");
            if(secCode == securityCode)
            {
                cout<<"Enter the amount : ";
                cin>>deposite;
                if (deposite < 30000)
                {
                BalanceCalc(filepath + ".txt");

                long int newBalance = balance + deposite;
                ifstream in(filepath + ".txt");
                string fileContent = "";
                string line;

                while (getline(in, line))
                    {
                        size_t pos = 0;
                        while ((pos = line.find(to_string(balance), pos)) != string::npos)
                        {
                            line.replace(pos, to_string(balance).length(), to_string(newBalance));
                            pos += to_string(newBalance).length();
                        }

                        fileContent += line + "\n";
                    }
                    in.close();

                ofstream out(filepath + ".txt");
                out<<fileContent;
                out.close();
                cout<<"Money deposited";

                string logsFilePath = filepath + "Logs.txt";

                ofstream logs(logsFilePath, ios::app);
                if (logs.is_open())
                {
                    logs<<"Money deposited : "<<deposite<<endl;
                    out.close();
                }
                else
                {
                    cout<<"Logs weren't made !!";
                }

                reciept(filepath, deposite, newBalance, "deposited");
                }
                else
                {
                    cout<<"Amount must be below 30k"<<endl;
                }
                
            }
            else
            {
                cout<<"Wrong pin try again later !!"<<endl;
            }
        }

        void withdrawMoney(){
            string filepath;
            long int secCode;
            cout<<"What is your name : ";
            cin>>filepath;
            cout<<"Enter the security Code :"<<endl;
            cin>>secCode;
            code(filepath + ".txt");
            if(secCode == securityCode)
            {
                BalanceCalc(filepath + ".txt");
                cout<<"Enter the withdraw amount : ";
                cin>>withdrawal;
                if (withdrawal > balance)
                {
                    cout<<"Insufficient Balance !!"<<endl;
                }
                else
                {
                    long int newBalance = balance - withdrawal;

                    ifstream in(filepath + ".txt");
                    string fileContent = "";
                    string line;

                    while (getline(in, line))
                        {
                            size_t pos = 0;
                            while ((pos = line.find(to_string(balance), pos)) != string::npos)
                            {
                                line.replace(pos, to_string(balance).length(), to_string(newBalance));
                                pos += to_string(newBalance).length();
                            }

                            fileContent += line + "\n";
                        }
                        in.close();

                    ofstream out(filepath + ".txt");
                    out<<fileContent;
                    out.close();
                    cout<<"Money Withdrawn, Balance updated !!";

                    string logsFilePath = filepath + "Logs.txt";

                    ofstream logs(logsFilePath, ios::app);
                    if (logs.is_open())
                    {
                        logs<<"Money withdrawn : "<<withdrawal<<endl;
                        out.close();
                    }
                    else
                    {
                        cout<<"Logs weren't made !!";
                    }
                    reciept(filepath, withdrawal, newBalance, "withdrawn");
                }
            }
            else
            {
                cout<<"Wrong PIN try again later !!";
            }
            
        }

        void showLogs(){
            string path;
            cout<<endl<<"Enter your name : ";
            cin>>path;
            string filepath = path + "Logs" + ".txt";
            ifstream in(filepath);
            string st;
            if (in.is_open())
            {
            cout<<endl<<"---------------TRANSACTION HISTORY---------------"<<endl;
            while (in.eof() == 0)
            {
                getline(in, st);
                cout<<st<<endl;
            }
            in.close();
            }
            else
            {
                cout<<"Enter the correct name"<<endl;
            }
            
            
        }

        void transaction(){
            string sendersPath;
            string recieversPath;
            long int secCode;
            long int transaction;
            cout<<endl<<"Enter the sender's name : ";
            cin>>sendersPath;
            cout<<"Enter the reciever's name : ";
            cin>>recieversPath;
            BalanceCalc(sendersPath + ".txt");
            cout<<"Enter your Security Code :"<<endl;
            cin>>secCode;
            code(sendersPath + ".txt");
            bool possibleTrans = false;
            if (secCode == securityCode)
            {
            while (!possibleTrans)
            {
            cout<<"Enter the amount you wish to tranfer : ";
            cin>>transaction;   
                if (transaction > balance)
                {
                    cout<<"INSUFFICIENT BALANCE !!"<<endl;
                    break;
                }
                else
                {
                    long int newBalance = balance - transaction;

                    ifstream in(sendersPath + ".txt");
                    string fileContent = "";
                    string line;

                    while (getline(in, line))
                        {
                            size_t pos = 0;
                            while ((pos = line.find(to_string(balance), pos)) != string::npos)
                            {
                                line.replace(pos, to_string(balance).length(), to_string(newBalance));
                                pos += to_string(newBalance).length();
                            }

                            fileContent += line + "\n";
                        }
                        in.close();

                    ofstream out(sendersPath + ".txt");
                    out<<fileContent;
                    out.close();

                    ofstream logs(sendersPath + "Logs.txt", ios::app);
                    if (logs.is_open())
                    {
                        logs<<"Money transfered : "<<transaction<<"  | From "<<sendersPath<<" to "<<recieversPath<<"  |"<<endl;
                        out.close();
                    }
                    else
                    {
                        cout<<"Logs weren't made !!";
                    }
                    reciept(sendersPath, transaction, newBalance, "Withdrawn"); 
                }
                BalanceCalc(recieversPath + ".txt");

                long int recieverNewBalance = balance + transaction;
                ifstream reciever(recieversPath + ".txt");
                string recieverContent = "";
                string rVar;

                while (getline(reciever, rVar))
                {
                    size_t rpos = 0;
                    while ((rpos = rVar.find(to_string(balance), rpos)) != string::npos)
                    {
                        rVar.replace(rpos, to_string(balance).length(), to_string(recieverNewBalance));
                        rpos += to_string(recieverNewBalance).length();
                    }
                    recieverContent += rVar + "\n";
                }
                reciever.close();

                ofstream rOut(recieversPath + ".txt");
                rOut<<recieverContent;
                rOut.close();

                ofstream rLogs(recieversPath + "Logs.txt", ios::app);
                if (rLogs.is_open())
                {
                    rLogs<<"Money Recieved : "<<transaction<<"   |  From "<<sendersPath<<" to "<<recieversPath<<"  |"<<endl;
                }
                else
                {
                    cout<<"Logs weren't made !!";
                }
                reciept(recieversPath, transaction, recieverNewBalance, "Deposited");

                possibleTrans = true;

        }
            }
            else
            {
                cout<<"WRONG PIN, NO TRANSACTION WAS MADE !!"<<endl;
            }        
    }

        void BalanceCalc(string filename){
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
};

class loan : virtual public MoneyManagement{
    string applier;
    string choice;
    int years;
    long int principal;
    float interest = float(4)/100;
    float return_value;

    public:
    void applyLoan(){
        cout<<"Enter your name : ";
        cin>>applier;
        cout<<"Enter your loan amount :";
        cin>>principal;
        BalanceCalc(applier + ".txt");
        if (balance < 10000 || principal == 5*balance)
        {
            cout<<"Loan of "<<principal<<" cannot be approved for bank balance of"<<balance<<endl;
        }
        else
        {
            return_value = principal;
            cout<<"Interest rate offered : 4% "<<endl;
            cout<<"Loan term duration : ";
            cin>>years;
            for (int i = 0; i < years; i++)
            {
                return_value = return_value * (1+interest);
            }
            
            cout<<"Amount to be returned : "<<return_value<<endl;
            cout<<"Are you interested ? :"<<endl;
            cin>>choice;
            if (choice == "yes")
            {
                ofstream out(applier + ".txt", ios::app);
                if (out.is_open())
                {
                    out<<"\nLoan Amount : "<<principal<<endl;
                    out<<"Return amount : "<<return_value<<endl;
                }

                long int newBalance = balance + principal;
                ifstream in(applier + ".txt");
                string fileContent = "";
                string line;

                while (getline(in, line))
                    {
                        size_t pos = 0;
                        while ((pos = line.find(to_string(balance), pos)) != string::npos)
                        {
                            line.replace(pos, to_string(balance).length(), to_string(newBalance));
                            pos += to_string(newBalance).length();
                        }

                        fileContent += line + "\n";
                    }
                    in.close();

                ofstream lout(applier + ".txt");
                lout<<fileContent;
                lout.close();
                cout<<"Your bank balance is updated "<<endl;
            }
            else{
                cout<<"OK SIR..";
            }   
        }
    }

};


class accountManagement : public accountUpdate, public accounDeletion, public loan{};

set<long long> accountCreation::generatedAccountNumbers;

int main() {
    // Seed the random number generator with the current time
    srand(static_cast<unsigned>(time(0)));
    // Seed the random number generator with the current time
    
    accountManagement p1, p2;
    int choice = 0;
    while (choice != 3)
    {
        cout<<"\n|------WELCOME TO SHAMPOO BANK OF INDIA-----|"<<endl;
        cout<<"| 1. Account Management                      |"<<endl;
        cout<<"| 2. Money Management                        |"<<endl;
        cout<<"| 3. Loan Check                              |"<<endl;
        cout<<"| 4. Exit                                    |"<<endl;
        cout<<"| Enter your choice : ";
        cin>>choice;
        cout<<"|--------------------------------------------|"<<endl;

        if (choice == 1)
        {   
            int accManage = 0;
            while (accManage != 5)
            {
                cout<<"\n--- ACCOUNT MANAGEMENT ---"<<endl;
                cout<<"1. Create Account"<<endl;
                cout<<"2. Show Details"<<endl;
                cout<<"3. Update Details"<<endl;
                cout<<"4. Delete Account"<<endl;
                cout<<"5. Exit"<<endl;
                cout<<"Enter your choice :"<<endl;
                cin>>accManage;
                cout<<"\n";
                switch (accManage)
                {
                case 1:
                    p1.createAccount();
                    break;
                
                case 2:
                    p1.showDetails();
                    break;
                
                case 3:
                    p1.updateDetails();
                    break;

                case 4:
                    p1.deleteAccount();
                    break;
                
                case 5:
                    break;

                default :
                    cout<<"Invalid option !!"<<endl;
                }
            }
        }
        else if (choice == 2)
        {
            int moneyManage = 0;
            while (moneyManage != 5)
            {
                cout<<"\n--- MONEY MANAGEMENT ---"<<endl;
                cout<<"1. Deposite"<<endl;
                cout<<"2. Withdraw"<<endl;
                cout<<"3. Transfer"<<endl;
                cout<<"4. Show Logs"<<endl;
                cout<<"5. Exit"<<endl;
                cout<<"Enter your choice :"<<endl;
                cin>>moneyManage;

                switch (moneyManage)
                {
                case 1:
                    p1.moneyDeposite();
                    break;

                case 2:
                    p1.withdrawMoney();
                    break;
                
                case 3:
                    p1.transaction();
                    break;

                case 4:
                    p1.showLogs();
                    break;

                case 5:
                    break;
                
                default:
                    cout<<"Invalid Option !"<<endl;
                }
            }
        }
        else if (choice == 3)
        {
            int loanCheck = 0;
            while (loanCheck != 3)
            {
                cout<<"\n--- LOAN CHECK ---"<<endl;
                cout<<"1. Apply for loan"<<endl;
                cout<<"2. Interest Calculation"<<endl;
                cout<<"3. Exit"<<endl;
                cout<<"Enter your choice :"<<endl;
                cin>>loanCheck;
                switch (loanCheck)
                {
                case 1:
                    p1.applyLoan();
                    break;

                case 3:
                    break;

                
                default:
                    break;
                }
            }
        }
        
        else if (choice == 4)
        {
            cout<<"Exiting...."<<endl;
        }
        else
        {
            cout<<"Option Invalid !"<<endl;
        }
    }
    return 0;
}
