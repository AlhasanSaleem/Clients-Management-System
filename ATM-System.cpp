#include<iostream>
#include<string>
#include<vector>
#include<iomanip>
#include<fstream>
#include<cmath>

using namespace std;

enum eATMMainMenueOptions
{
    QuickWithdraw       = 1,
    NormalWithdraw      = 2,
    Deposite            = 3,
    CheckBalance        = 4,
    ShowInformations    = 5,
    ChangeInformations  = 6,
    Logout              = 7
};

struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};

sClient CurrentClient;
const string ClientsFileName = "Clients.txt";

void ShowATMMainMenue();
void ShowQuickWithdrawScreen();
void Login();
void ShowNormalWithdrawScreen();

vector <string> SplitString(string Line, string Separator)
{
    vector <string> vStrings;
    int Postion = 0;
    string sWord = "";

    while ((Postion = Line.find(Separator)) != string::npos)
    {
        sWord = Line.substr(0, Postion);

        if (sWord != "")
        {
            vStrings.push_back(sWord);
        }

        Line.erase(0, Postion + Separator.length());
    }

    if (Line != "")
    {
        vStrings.push_back(Line);
    }
    
    return vStrings;
}

sClient ConvertClientLineToRecord(string ClientLine, string Separator = "#//#")
{
    sClient Client;
    vector <string> vStrings = SplitString(ClientLine, Separator);

    Client.AccountNumber = vStrings[0];
    Client.PinCode = vStrings[1];
    Client.Name = vStrings[2];
    Client.Phone = vStrings[3];
    Client.AccountBalance = stod(vStrings[4]);

    return Client;
}

string ConvertClientRecordToLine(sClient Client, string Separator = "#//#")
{
    string ClientLine = "";

    ClientLine += Client.AccountNumber + Separator;
    ClientLine += Client.PinCode + Separator;
    ClientLine += Client.Name + Separator;
    ClientLine += Client.Phone + Separator;
    ClientLine += to_string(Client.AccountBalance);

    return ClientLine;
}

vector <sClient> SaveClientDataToFile(vector <sClient> &vClients)
{
    fstream File;
    File.open(ClientsFileName, ios::out);

    if (File.is_open())
    {
        string ClientLine;

        for (sClient &Client : vClients)
        {
            ClientLine = ConvertClientRecordToLine(Client);
            File << ClientLine << endl;
        }
        
        File.close();
    }

    return vClients;
}

vector <sClient> LoadClientsDataFromFile(string FileName)
{
    vector <sClient> vClients;
    fstream File;
    File.open(FileName, ios::in);

    if (File.is_open())
    {
        string Line;
        sClient Client;

        while (getline(File, Line))
        {
            Client = ConvertClientLineToRecord(Line);
            vClients.push_back(Client);
        }
        
        File.close();
    }

    return vClients; 
}

bool FindClientByAccountNumber(string AccountNumber)
{
    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);

    for (sClient &Client : vClients)
    {
        if (AccountNumber == Client.AccountNumber)
            return true;
    }
   return false; 
}

bool FindClientByAccountNumberAndPinCode(string AccountNumber, string PinCode, sClient &Client)
{
    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);

    for (sClient &C : vClients)
    {
        if (AccountNumber == C.AccountNumber && PinCode == C.PinCode)
        {
            Client = C;
            return true;
        }
    }
    return false;
}

bool LoadClientInfo(string AccountNumber, string PinCode)
{
    if (FindClientByAccountNumberAndPinCode(AccountNumber, PinCode, CurrentClient))
        return true;
    else
        return false;    
}

short ReadATMMainMenueOption()
{
    short Choice = 0;
    cout << "Choose what do you want to do? [1 to 7]: ";
    cin  >> Choice;

    while (Choice < 1 || Choice > 7 || cin.fail())
    {
        cin.clear();
        cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid Choice, Enter a valid one : ";
        cin >> Choice;
    }

    return Choice;
}

short ReadQuickWithdrawOption()
{
    short Choice = 0;
    cout << "\nChoose what to do from [1] to [9]: ";
    cin  >> Choice;

    while (Choice < 1 || Choice > 9 || cin.fail())
    {
        cin.clear();
        cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid Choice, Enter a valid one: ";
        cin >> Choice;
    }

    return Choice;
}

int ReadWtihDrawAmount()
{
    int Amount = 0;
    cout << "\nEnter an amount multiple of 5's: ";
    cin  >> Amount;

    while (Amount % 5 != 0 || cin.fail())
    {
        cin.clear();
        cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid Choice, Enter an amount multiple of 5's: ";
        cin >> Amount;
    }

    return Amount;
}

double ReadDepositeAmount()
{
    double DepositeAmount = 0;
    cout << "Enter a positive Deposite Amount: ";
    cin  >> DepositeAmount;

    while (DepositeAmount <= 0 || cin.fail())
    {
        cin.clear();
        cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid Choice, Enter a valid one: ";
        cin >> DepositeAmount;
    }

    return DepositeAmount;
}

string ReadClientAccountNumber()
{
    string AccountNumber = "";
    cout << "Enter your Account Number: ";
    cin  >> AccountNumber;

    while (cin.fail())
    {
        cin.clear();
        cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid Choice, Enter a valid one: ";
        cin >> AccountNumber;
    }

    return AccountNumber;
}

void GoBackToATMMainMenue()
{
    cout << "\n\nPress any key to go back to ATM Main Menue ...";
    system("pause>0");
    ShowATMMainMenue();
}

short GetQuickWithdrawAmount(short QuickWithdrawOption)
{
    switch (QuickWithdrawOption)
    {
    case 1:
        return 20;

    case 2:
        return 50;
        
    case 3:
        return 100;

    case 4:
        return 200;

    case 5:
        return 400;

    case 6:
        return 600;

    case 7:
        return 800;

    case 8:
        return 1000;

    default:
        return 0;
    }
}

void DepositBalanceForClientByAccountNumber(double Amount, string AccountNumber, vector <sClient> &vClients)
{
    char Answer = 'n';
    cout << "\n\nAre you sure you want perform transactions y/n : ";
    cin  >> Answer;

    if (Answer == 'Y' || Answer == 'y')
    {
        for (sClient &Client : vClients)
        {
            if (AccountNumber == Client.AccountNumber)
            {
                Client.AccountBalance += Amount;
                SaveClientDataToFile(vClients);
                cout << "\n\nDone Successfuly, New Balance is: " << Client.AccountBalance;
            }
        }       
    }
}

void PerfromQuickWithdrawOptions(short QuickWithdrawOption)
{
    if (QuickWithdrawOption == 9) //exit.
        return;

    short WithdrawBalance = GetQuickWithdrawAmount(QuickWithdrawOption);
    if (WithdrawBalance > CurrentClient.AccountBalance)
    {
        cout << "\nThe amount exceeds your balance, make another choice.\n";
        cout << "Press Anykey to continue..."; 
        system("pause>0");
        ShowQuickWithdrawScreen();
        return;
    }

    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    DepositBalanceForClientByAccountNumber(QuickWithdrawOption * -1, CurrentClient.AccountNumber, vClients);
    CurrentClient.AccountBalance -= QuickWithdrawOption;
}

void ShowQuickWithdrawScreen()
{
    system("cls");
    cout << "===================================================\n";
    cout << "\t\tQucik Withdraw Screen\n";
    cout << "===================================================\n";
    cout << "\t[1] 20\t\t[2] 50\n";
    cout << "\t[3] 100\t\t[4] 200\n";
    cout << "\t[5] 400\t\t[6] 600\n";
    cout << "\t[7] 800\t\t[8] 1000\n";
    cout << "\t[9] Exit\n"; 
    cout << "===================================================\n";
    cout << "Your Balance is: " << CurrentClient.AccountBalance;
    PerfromQuickWithdrawOptions(ReadQuickWithdrawOption());
}

void PerfomNormalWtihdrawOption()
{
    int WithDrawBalance = ReadWtihDrawAmount();

    if (WithDrawBalance > CurrentClient.AccountBalance)
    {
        cout << "\nThe amount exceeds your balance, make another choice.\n";
        cout << "Press Anykey to continue..."; 
        system("pause>0");
        ShowNormalWithdrawScreen();
        return;
    }

    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    DepositBalanceForClientByAccountNumber(WithDrawBalance, CurrentClient.AccountNumber, vClients);
    CurrentClient.AccountBalance -= WithDrawBalance;
}

void ShowNormalWithdrawScreen()
{
    system("cls");
    cout << "===================================================\n";
    cout << "\t\tNormal Withdraw Screen\n";
    cout << "===================================================\n";
    PerfomNormalWtihdrawOption();
}

void PerformDepositeOption()
{
    double DepositeAmount = ReadDepositeAmount();
    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    DepositBalanceForClientByAccountNumber(DepositeAmount, CurrentClient.AccountNumber, vClients);
    CurrentClient.AccountBalance += DepositeAmount;
}

void ShowDepositeScreen()
{
    system("cls");
    cout << "==============================================\n";
    cout << "\t\tDeposite Screen\n";
    cout << "==============================================\n";
    PerformDepositeOption();
}

void ShowCheckBalanceScreen()
{
    system("cls");
    cout << "===================================================\n";
    cout << "\t\tCheck Balance Screen\n";
    cout << "===================================================n";
    cout << "Your Balance is: " << CurrentClient.AccountBalance << "\n";  
}

void PrintClientDetails(sClient Client)
{     
    cout << "\nThe following are the Client details:\n";
    cout << "-----------------------------------";
    cout << "\nAccount Number    : " << Client.AccountNumber; 
    cout << "\nPinCode           : " << Client.PinCode;
    cout << "\nName              : " << Client.Name;
    cout << "\nPhone             : " << Client.Phone;
    cout << "\nAccount Balance   : " << Client.AccountBalance;
    cout << "\n-----------------------------------\n";
}

void ShowClientInformations()
{
    PrintClientDetails(CurrentClient);
}

sClient ChangeClientInformations(string AccountNumber)
{
    sClient Client;

    Client.AccountNumber = AccountNumber;

    cout << "Enter new Pin Code : ";
    getline(cin >> ws, Client.PinCode);

    cout << "Enter new Name : ";
    getline(cin, Client.Name);

    cout << "Enter new Phone : ";
    getline(cin, Client.Phone);

    cout << "Enter new Account Balance : ";
    cin >> Client.AccountBalance;

    return Client; 
}

void ChangeClientIformations(string AccountNumber, vector <sClient> &vClients, sClient &Client)
{
    if (FindClientByAccountNumber(AccountNumber))
    {
        PrintClientDetails(CurrentClient);
        for (sClient &C : vClients)
        {
            C = ChangeClientInformations(AccountNumber);
            Client = C;
            break;
        }  

        SaveClientDataToFile(vClients);
        cout << "\nInformations Updated Successfuly by (" << Client.Name << ")\n";     
    }
    else
    {
        cout << "There is an error in this account number (" << AccountNumber << "), Go back and try again\n";
    }
}

void ShowChangeClientInformations()
{
    system("cls");
    cout << "========================================================\n";
    cout << "\t\tChange Client Informations Screen\n";
    cout << "========================================================\n";
    string AccountNumber = ReadClientAccountNumber();
    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    ChangeClientIformations(AccountNumber, vClients, CurrentClient);
}

void PerformATMMainMenueOptions(eATMMainMenueOptions ATMOption)
{
    switch (ATMOption)
    {
    case eATMMainMenueOptions::QuickWithdraw :
        ShowQuickWithdrawScreen();
        GoBackToATMMainMenue();
        break;

    case eATMMainMenueOptions::NormalWithdraw :
        ShowNormalWithdrawScreen();
        GoBackToATMMainMenue();
        break;

    case eATMMainMenueOptions::Deposite :
        ShowDepositeScreen();
        GoBackToATMMainMenue();
        break;

    case eATMMainMenueOptions::CheckBalance :
        ShowCheckBalanceScreen();
        GoBackToATMMainMenue();
        break;

    case eATMMainMenueOptions::ShowInformations :
        ShowClientInformations();
        GoBackToATMMainMenue();
        break;

    case eATMMainMenueOptions::ChangeInformations :
        ShowChangeClientInformations();
        GoBackToATMMainMenue();
        break;

    case eATMMainMenueOptions::Logout :
        system("cls");
        Login();
        break;
    }
}

void ShowATMMainMenue()
{
    system("cls");
    cout << "===================================================\n";
    cout << "\t\tATM Main Menue Screen\n";
    cout << "===================================================\n";
    cout << "\t[1] Quick Withdraw.\n";
    cout << "\t[2] Normal Withdraw.\n";
    cout << "\t[3] Deposite.\n";
    cout << "\t[4] Check Balance.\n";
    cout << "\t[5] Show Informations.\n";
    cout << "\t[6] Change Informations.\n";
    cout << "\t[7] Logout.\n";
    cout << "===========================================\n";

    PerformATMMainMenueOptions((eATMMainMenueOptions)ReadATMMainMenueOption());
}

void Login()
{
    bool LoginFail = false;
    string AccountNumber, PinCode;

    do
    {
        system("cls");

        cout << "\n--------------------------------\n";
        cout << "\tLogin Screen";
        cout << "\n--------------------------------\n";

        if (LoginFail)
        {
           cout << "Invalid Account Number or Pin Code!\n";
        }

        cout << "Enter Account Number: ";
        cin  >> AccountNumber;

        cout << "Enter PinCode: ";
        cin  >> PinCode;

        LoginFail = !LoadClientInfo(AccountNumber, PinCode);

    } while (LoginFail);
    
    ShowATMMainMenue();
}

int main()
{
    Login();
    return 0;
}