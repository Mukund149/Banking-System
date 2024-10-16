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
        getline(cin, name);
        cout << "Enter witness name: ";
        getline(cin, witness);
        cout << "Enter mobile number: ";
        getline(cin, mobileNumber);
        cout<<"Enter a security code :";
        cin>>securityCode;
        cout<<"Enter the amount to be added : ";
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

class accountUpdate{
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

class accounDeletion{
    public:
        void deleteAccount(){
            string filepath;
            cout<<"What is your name : ";
            cin>>filepath;
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
};

class MoneyManagement{
    long int deposite;
    long int balance;
    long int withdrawal;
    friend class accountCreation;

    public:
        void reciept(string name, long int accBalance, long int currBalance, string situation){
            string recieptFile = name + "Reciept" + ".txt";
            ofstream out(recieptFile, ios::app);
            if (out.is_open())
            {
                out<<endl<<"            SHAMPI BANK OF INDIA            "<<endl;
                out<<"Account Holder : "<<name<<endl;
                out<<"Account number : 1235628XXXXXXX "<<endl;
                out<<"Money "<<situation<<" : "<<accBalance<<endl;
                out<<"Current Balance : "<<currBalance<<endl;
                out.close();
                cout<<endl<<"Your reciept has been saved !!"<<endl;
            }
            else
            {
                cout<<"Sorry cannot fetch your reciept :( "<<endl;
            }

            ifstream in(recieptFile);
            string st;
            cout<<"\n--------------Your New Reciept--------------"<<endl;
            while (getline(in, st))
            {
                cout<<st<<endl;
            }
            in.close();
            
        }
        void moneyDeposite(){
            string filepath;
            long int secCode;
            cout<<"What is your name : ";
            cin>>filepath;
            cout<<"Enter the amount : ";
            cin>>deposite;
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

        void withdrawMoney(){
            string filepath;
            cout<<"What is your name : ";
            cin>>filepath;
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

        void showLogs(){
            string path;
            cout<<endl<<"Enter your name : ";
            cin>>path;
            string filepath = path + "Logs" + ".txt";
            ifstream in(filepath);
            string st;
            cout<<endl<<"---------------TRANSACTION HISTORY---------------"<<endl;
            while (in.eof() == 0)
            {
                getline(in, st);
                cout<<st<<endl;
            }
            in.close();
            
        }

        void transaction(){
            string sendersPath;
            string recieversPath;
            long int transaction;
            cout<<endl<<"Enter the sender's name : ";
            cin>>sendersPath;
            cout<<"Enter the reciever's name : ";
            cin>>recieversPath;
            BalanceCalc(sendersPath + ".txt");
            cout<<"Enter the amount you wish to tranfer : ";
            cin>>transaction;
            
            if (transaction > balance)
            {
                cout<<"Insufficient Balance !!"<<endl;
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


class accountManagement : public accountCreation, public accountUpdate, public accounDeletion, public MoneyManagement{};

set<long long> accountCreation::generatedAccountNumbers;

int main() {
    // Seed the random number generator with the current time
    srand(static_cast<unsigned>(time(0)));
    // Seed the random number generator with the current time
    
    accountManagement p1, p2;
    
    p1.showDetails();



    return 0;
}
