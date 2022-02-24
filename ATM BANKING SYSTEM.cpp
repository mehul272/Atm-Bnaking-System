/*
README:
The following is a C++ implementation of an ATM system that supports creating new accounts, logging in users, withdrawing and depositing, balance inquiries and account modifications.
Authors:
		Mehul Pansari
		Atharva Pansare
*/

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <string.h>
#include <unistd.h>
#include <cmath>
#include <windows.h>

using namespace std;

class ATM
{

private:
    long int acc_num;
    char username[20];
    int pin;
    double balance, amount;
    int age;

    long int acc_num1;
    char username1[20];
    int pin1;
    double balance1;

public:
    //abstract classes to handle different exceptions
    class ageException
    {
    };
    class usernameException
    {
    };
    class withdrawalLimitException
    {
    };
    class transferException
    {
    };
    class invalidBalanceException
    {
    };
    class pinException
    {
    };
    class verificationException
    {
    };
    class transferBalanceException
    {
    };
    void create_account()
    {
        ifstream is("ATM.txt", ios::in);
        ofstream fs("ATM.txt", ios::app);

        cout << "Enter age for verification: ";
        cin >> age;

        if (!(age >= 18))
            throw ageException();

        cout << "Enter username: ";
        char usern[20];
        cin >> usern;

        //search file for matching username
        while (is >> acc_num >> username >> pin >> balance)
        {
            if ((strcmp(usern, this->username)) == 0)
                throw usernameException();
        }

        cout << "Enter 4 digit PIN number: ";
        cin >> pin;

        //check if pin is 4 digits long
        if (!((floor(log10(pin) + 1) == 4)))
            throw pinException();

        cout << "Enter account number: ";
        cin >> acc_num;
        balance = 0;

        // Save credentials in a file
        fs << acc_num << " " << usern << " " << pin << " " << balance << " ";
        cout << "\nAccount has been created!\n";
        fs.close();
        is.close();
    }

    int login(char un[], int pw)
    {
        ifstream fs("ATM.txt", ios::in);
        while (fs >> acc_num >> username >> pin >> balance)
        {
            // Check if login credentials are valid
            if (strcmp(un, this->username) == 0 && pw == pin)
            {
                cout << "\nLogin Successful!!\n";
                fs.close();
                return 1; // Return True if login successful and can proceed to withdraw/deposit
            }
        }
        fs.close();
        cout << "\nInvalid credentials\n";
        return 0; // Return False if login unsuccessful
    }

    void display_account()
    {

        cout << "\n\n\t------------------------------------------------------------------------------\n";
        cout << "\t|" << setw(18) << "ACC NUMBER |" << setw(18) << "NAME |" << setw(18) << "PIN |" << setw(18) << "BALANCE |";
        cout << "\n\n\t------------------------------------------------------------------------------\n";
        cout << "\t|" << setw(18) << " |" << setw(18) << " |" << setw(18) << " |" << setw(18) << "|" << endl;

        cout << "\t|" << setw(15) << acc_num << "  |" << setw(15) << username << "  |" << setw(15) << pin << "  |" << setw(15) << balance << "  |";
        cout << endl;

        //cout << "\t\t|" << setw(18) << " |" << setw(18) << " |" << setw(18) << " |" << setw(18) << "|" << endl;
        cout << "\n\t------------------------------------------------------------------------------\n";
    }

    void withdraw_deposit()
    {
        int choice;
        cout << "\nPress 1 to withdraw \nPress 2 to deposit\n";
        cin >> choice;

        if (choice == 1)
        {
            cout << "\nCurrent balance: " << balance << endl;
            cout << "\nEnter the amount to be withdrawn: ";
            cin >> amount;
            if (amount > 20000)
                throw withdrawalLimitException();

            if (balance < amount)
                throw invalidBalanceException();
            balance = balance - amount;
            cout << "\nCurrent balance: " << balance << endl;
        }
        else
        {
            cout << "\nCurrent balance: " << balance << endl;
            cout << "\nEnter the amount to be deposited: ";
            cin >> amount;
            balance = balance + amount;
            cout << "\nCurrent balance: " << balance << endl;
        }

        ifstream file1("ATM.txt", ios::in);
        ofstream file2("temp.txt", ios::app);

        while (file1 >> acc_num1 >> username1 >> pin1 >> balance1)
        {
            if (acc_num1 != acc_num)
                file2 << acc_num1 << " " << username1 << " " << pin1 << " " << balance1 << " ";
        }
        file1.close();
        file2.close();
        remove("ATM.txt");
        rename("temp.txt", "ATM.txt");

        ofstream fs("ATM.txt", ios::app);
        fs << acc_num << " " << username << " " << pin << " " << balance << " ";
        fs.close();
    }

    void modify_account()
    {
        cout << "\nEnter new Username: ";
        cin >> username;

        cout << "\nEnter new PIN: ";
        cin >> pin;

        ifstream file1("ATM.txt:", ios::in);
        ofstream file2("temp.txt", ios::app);

        while (file1 >> acc_num1 >> username1 >> pin1 >> balance1)
        {
            if (acc_num1 != acc_num)
                file2 << acc_num1 << username1 << pin1 << balance1;
        }
        file1.close();
        file2.close();
        remove("ATM.txt");
        rename("temp.txt", "ATM.txt");

        ofstream fs("ATM.txt", ios::app);
        fs << acc_num << " " << username << " " << pin << " " << balance << " ";
        fs.close();
    }

    void transfer_amount()
    {
        char payee[50];
        int flag = 0, payee_pin;
        double payee_bal, transfer_amt;
        long int payee_acc_no;
        ifstream is("ATM.txt", ios::in);
        cout << "\nEnter username of payee: ";
        cin >> payee;
        cout << "\nEnter account number of payee: ";
        cin >> payee_acc_no;
        while (is >> acc_num1 >> username1 >> pin1 >> balance1)
        {
            if (strcmp(payee, this->username1) == 0 && payee_acc_no == acc_num1)
            {
                cout << "\nVerification complete! Enter amount to transfer: ";
                flag = 1;
            }
        }
        if (flag == 0)
            throw verificationException();
        is.close();
        cin >> transfer_amt;
        if (balance < transfer_amt)
            throw transferBalanceException();
        ifstream file1("ATM.txt", ios::in);
        ofstream file2("temp.txt", ios::app);

        while (file1 >> acc_num1 >> username1 >> pin1 >> balance1)
        {
            if (acc_num1 != acc_num && acc_num1 != payee_acc_no)
                file2 << acc_num1 << username1 << pin1 << balance1;
            if (acc_num1 == payee_acc_no)
            {
                payee_pin = pin1;
                payee_bal = balance1 + transfer_amt;
            }
        }
        file1.close();
        file2.close();
        remove("ATM.txt");
        rename("temp.txt", "ATM.txt");

        ofstream fs("ATM.txt", ios::app);
        fs << acc_num << " " << username << " " << pin << " " << (balance - transfer_amt) << " ";
        fs << payee_acc_no << " " << payee << " " << payee_pin << " " << payee_bal << " ";
        fs.close();
        cout << "\nTransfer completed successfully!\n";
    }
};

void loading()
{
    system("cls");
    system("COLOR 8f");
    int i = 0;
    system("COLOR 70");
    cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
    cout << setw(50);
    cout << "Loading...\n";
    cout << setw(40);
    for (i = 0; i < 10; i++)
    {
        if (i < 5)
        {
            cout << "|";
            Sleep(1000);
        }
        else
        {
            cout << "|";
            Sleep(150);
        }
    }
    system("cls");
    system("COLOR 8f");
    cout << "\t\t\t\t\t\t\tATM Banking System\n\n\n";
    cout << "\n";
    system("pause");
}

int main()
{
    loading();
    ATM user;
    while (1)
    {
        char username[50];
        int pin, ch, logged_in = 0;
        cout << "\t\t**  Welcome to ATM **";
        cout << "\n1. Login \n2. Create a New Account\n3. Exit" << endl;
        cout << "Enter choice: ";
        cin >> ch;
        switch (ch)
        {
        case 1:
            cout << " Username : ";
            cin >> username;
            cout << " 4 digit PIN: ";
            cin >> pin;
            logged_in = user.login(username, pin);
            break;
        case 2:
            try
            {
                user.create_account();
            }
            catch (ATM::usernameException)
            {
                int choice = 1;
                cout << "Username already taken. Enter 1 to retry, or 2 to exit\n";
                cin >> choice;
                if (choice == 2)
                    exit(1);
            }

            catch (ATM::ageException)
            {
                int choice = 1;
                cout << "Entered age is below required minimum. Enter 1 to retry, or 2 to exit\n";
                cin >> choice;
                if (choice == 2)
                    exit(1);
            }

            catch (ATM::pinException)
            {
                int choice = 1;
                cout << "Pin entered is invalid. Please make sure pin is a 4 digit number. Enter 1 to retry, or 2 to exit\n";
                cin >> choice;
                if (choice == 2)
                    exit(1);
            }
            break;
        case 3:
            exit(1);
        }

        if (logged_in)
        {
            cout << " \n------------ Menu ------------ \n";
            cout << "1. Display Account Info\n";
            cout << "2. Modify Account\n";
            cout << "3. Deposit/Withdraw cash\n";
            cout << "4. Transfer money to another account\n";
            cout << "5. Logout\n";
            cout << "\nEnter your choice from menu: ";
            cin >> ch;

            switch (ch)
            {
            case 1:
                user.display_account();
                break;
            case 2:
                user.modify_account();
                break;
            case 3:
                try
                {
                    user.withdraw_deposit();
                }
                catch (ATM::withdrawalLimitException)
                {
                    int choice = 1;
                    cout << "Withdrawal amount is above maximum allowed amount. Enter 1 to retry, or 2 to exit\n";
                    cin >> choice;
                    if (choice == 2)
                        exit(1);
                }
                catch (ATM::invalidBalanceException)
                {
                    int choice = 1;
                    cout << "Balance is not enough to cover withdrawal amount. Enter 1 to retry, or 2 to exit\n";
                    cin >> choice;
                    if (choice == 2)
                        exit(1);
                }
                break;
            case 4:
                try
                {
                    user.transfer_amount();
                }
                catch (ATM::verificationException)
                {
                    int choice = 1;
                    cout << "Username or account number entered incorrectly or does not exist. Enter 1 to retry, or 2 to exit\n";
                    cin >> choice;
                    if (choice == 2)
                        exit(1);
                }
                catch (ATM::transferBalanceException)
                {
                    int choice = 1;
                    cout << "Balance is too low to carry out transfer. Please try again with lower amount. Enter 1 to retry, or 2 to exit\n";
                    cin >> choice;
                    if (choice == 2)
                        exit(1);
                }
                break;
            case 5:
                logged_in = 0;
            }
        }
    }
    return 0;
}