#include<iostream>
#include<fstream>
#include<iomanip>
using namespace std;

//Bank Management System
//using file handling

class Account
{
    int accNo;
    char name[50];
    char password[20];
    float balance;

    public:

    void createAccount()
    {
        //taking input from user
        cout<<"\nEnter Account Number : ";
        cin>>accNo;
        cout<<"Enter Name : ";
        cin.ignore();
        cin.getline(name,50);
        cout<<"Set a Password : ";
        cin>>password;
        cout<<"Enter Initial Deposit Amount : ";
        cin>>balance;

        cout<<"\nAccount Created Successfully!\n";
    }

    void showAccount()
    {
        //display details
        cout<<"\nAccount Number : "<<accNo;
        cout<<"\nAccount Holder Name : "<<name;
        cout<<"\nBalance : "<<fixed<<setprecision(2)<<balance;
        cout<<endl;
    }

    //these two functions update balance
    void deposit(float amt)
    {
        balance=balance+amt;
    }

    void withdraw(float amt)
    {
        balance=balance-amt;
    }

    int getAccNo()
    {
        return accNo;
    }

    char* getPassword()
    {
        return password;
    }

    float getBalance()
    {
        return balance;
    }

    void modify()
    {
        cout<<"\nModify Account No "<<accNo;
        cout<<"\nEnter Name : ";
        cin.ignore();
        cin.getline(name,50);
        cout<<"Enter Password : ";
        cin>>password;
        cout<<"Enter Balance : ";
        cin>>balance;
    }
};


//---------------------------------------
//function to create a new account
void writeAccount()
{
    Account acc;
    ofstream outFile;

    outFile.open("accounts.dat",ios::binary|ios::app);

    if(!outFile)
    {
        cout<<"\nError opening file!";
        return;
    }

    acc.createAccount();

    outFile.write((char*)&acc,sizeof(Account));
    outFile.close();
}


//function to show all accounts
void displayAll()
{
    Account acc;
    ifstream inFile;

    inFile.open("accounts.dat",ios::binary);

    if(!inFile)
    {
        cout<<"\nFile could not be opened!";
        return;
    }

    cout<<"\n----- All Account Holders -----\n";

    while(inFile.read((char*)&acc,sizeof(Account)))
    {
        acc.showAccount();
        cout<<"-------------------------\n";
    }

    inFile.close();
}


//function to search for an account by acc no
int searchAccount(int n)
{
    Account acc;
    ifstream inFile;

    inFile.open("accounts.dat",ios::binary);

    if(!inFile)
    {
        cout<<"\nFile could not be opened!";
        return -1;
    }

    while(inFile.read((char*)&acc,sizeof(Account)))
    {
        if(acc.getAccNo()==n)
        {
            inFile.close();
            acc.showAccount();
            return 1;
        }
    }

    inFile.close();
    return -1;
}


//function for depositing money
void depositAmount()
{
    int n;
    float amt;
    Account acc;
    int found=0;

    cout<<"\nEnter Account Number : ";
    cin>>n;

    fstream file;
    file.open("accounts.dat",ios::binary|ios::in|ios::out);

    if(!file)
    {
        cout<<"\nFile could not be opened!";
        return;
    }

    while(file.read((char*)&acc,sizeof(Account)))
    {
        if(acc.getAccNo()==n)
        {
            cout<<"Enter Amount to Deposit : ";
            cin>>amt;
            acc.deposit(amt);

            //going back to overwrite this record
            int pos=(-1)*static_cast<int>(sizeof(Account));
            file.seekp(pos,ios::cur);
            file.write((char*)&acc,sizeof(Account));

            cout<<"\nAmount Deposited Successfully!\n";
            acc.showAccount();
            found=1;
            break;
        }
    }

    if(found==0)
        cout<<"\nAccount Not Found!";

    file.close();
}


//function for withdrawing money
void withdrawAmount()
{
    int n;
    float amt;
    Account acc;
    int found=0;

    cout<<"\nEnter Account Number : ";
    cin>>n;

    fstream file;
    file.open("accounts.dat",ios::binary|ios::in|ios::out);

    if(!file)
    {
        cout<<"\nFile could not be opened!";
        return;
    }

    while(file.read((char*)&acc,sizeof(Account)))
    {
        if(acc.getAccNo()==n)
        {
            cout<<"Enter Amount to Withdraw : ";
            cin>>amt;

            if(amt>acc.getBalance())
            {
                cout<<"\nInsufficient Balance!";
            }
            else
            {
                acc.withdraw(amt);

                int pos=(-1)*static_cast<int>(sizeof(Account));
                file.seekp(pos,ios::cur);
                file.write((char*)&acc,sizeof(Account));

                cout<<"\nAmount Withdrawn Successfully!\n";
                acc.showAccount();
            }

            found=1;
            break;
        }
    }

    if(found==0)
        cout<<"\nAccount Not Found!";

    file.close();
}


//function to modify an existing account
void modifyAccount()
{
    int n;
    Account acc;
    int found=0;

    cout<<"\nEnter Account Number to Modify : ";
    cin>>n;

    fstream file;
    file.open("accounts.dat",ios::binary|ios::in|ios::out);

    if(!file)
    {
        cout<<"\nFile could not be opened!";
        return;
    }

    while(file.read((char*)&acc,sizeof(Account)))
    {
        if(acc.getAccNo()==n)
        {
            acc.modify();

            int pos=(-1)*static_cast<int>(sizeof(Account));
            file.seekp(pos,ios::cur);
            file.write((char*)&acc,sizeof(Account));

            cout<<"\nAccount Modified Successfully!\n";
            found=1;
            break;
        }
    }

    if(found==0)
        cout<<"\nAccount Not Found!";

    file.close();
}


//function to delete an account
//we make a temp file, copy all except the one to delete, then rename it
void deleteAccount()
{
    int n;
    Account acc;
    int found=0;

    cout<<"\nEnter Account Number to Delete : ";
    cin>>n;

    ifstream inFile;
    ofstream outFile;

    inFile.open("accounts.dat",ios::binary);
    outFile.open("temp.dat",ios::binary);

    if(!inFile||!outFile)
    {
        cout<<"\nError opening file!";
        return;
    }

    while(inFile.read((char*)&acc,sizeof(Account)))
    {
        if(acc.getAccNo()!=n)
        {
            outFile.write((char*)&acc,sizeof(Account));
        }
        else
        {
            found=1;
        }
    }

    inFile.close();
    outFile.close();

    remove("accounts.dat");
    rename("temp.dat","accounts.dat");

    if(found==1)
        cout<<"\nAccount Deleted Successfully!";
    else
        cout<<"\nAccount Not Found!";
}


int main()
{
    int choice,num;

    //menu driven program
    do
    {
        cout<<"\n========================================";
        cout<<"\n        BANK MANAGEMENT SYSTEM          ";
        cout<<"\n========================================";
        cout<<"\n1. Open New Account";
        cout<<"\n2. Deposit Amount";
        cout<<"\n3. Withdraw Amount";
        cout<<"\n4. Balance Enquiry";
        cout<<"\n5. Show All Accounts";
        cout<<"\n6. Modify Account";
        cout<<"\n7. Delete Account";
        cout<<"\n8. Exit";
        cout<<"\n========================================";
        cout<<"\nEnter your choice : ";
        cin>>choice;

        switch(choice)
        {
            case 1:
                writeAccount();
                break;

            case 2:
                depositAmount();
                break;

            case 3:
                withdrawAmount();
                break;

            case 4:
                cout<<"\nEnter Account Number : ";
                cin>>num;
                if(searchAccount(num)==-1)
                    cout<<"\nAccount Not Found!";
                break;

            case 5:
                displayAll();
                break;

            case 6:
                modifyAccount();
                break;

            case 7:
                deleteAccount();
                break;

            case 8:
                cout<<"\nThank you for using our Bank Management System!\n";
                cout<<"Program Ended.\n";
                break;

            default:
                cout<<"\nInvalid Choice! Please try again.";
        }

    }while(choice!=8);

    return 0;
}
