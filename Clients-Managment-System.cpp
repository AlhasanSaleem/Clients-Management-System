#include<iostream>
#include<string>
#include<vector>
#include<iomanip>
#include<fstream>
#include<cmath>

using namespace std;

struct sUser
{
    string Username;
    string Password;
    int Permissions;
    bool MarkForDelete = false;
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

enum eMainMenueOptions {
    eListClients  = 1,
    eAddNewClient = 2,
    eDeleteClient = 3,
    eUpdateClient = 4,
    eFindClient   = 5,
    eTransactionsMenueOptions = 6,
    eManageUsers = 7,
    eExit = 8
};

enum eMainMenuePermissions {
    pAll          = -1,
    pListClients  = 1,
    pAddNewClient = 2,
    pDeleteClient = 4,
    pUpdateClient = 8,
    pFindClient   = 16,
    pTransactions = 32,
    pManageUsers  = 46,
};

enum eTransactionsMenue {
    eDeposite         = 1,
    eWtihdraw         = 2, 
    eShowTotalBalance = 3,
    eShowMainMenue    = 4
};

enum eManageUsersMenue {
    eListUsers   = 1,
    eAddNewUser  = 2,
    eDeleteUser  = 3,
    eUpdateUser  = 4,
    eFindUser    = 5,
    eMainMenue   = 6
};

const string ClientsFileName = "Clients.txt";
const string UsersFileName = "Users.txt";

sUser CurrentUser;

void Login();
void ShowMainMenue();
void ShowTransactionsMenue();
void ShowManageUsersMenue();

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

sClient ConvertClientLineToRecord(string Line, string Separator = "#//#")
{
    sClient Client;
    vector <string> vStrings = SplitString(Line, Separator);

    Client.AccountNumber = vStrings[0];
    Client.PinCode = vStrings[1];
    Client.Name = vStrings[2];
    Client.Phone = vStrings[3];
    Client.AccountBalance = stod(vStrings[4]);

    return Client;
}

sUser ConvertUserLineToRecord(string Line, string Separator = "#//#")
{
    sUser User;
    vector <string> vStrings = SplitString(Line, Separator);
    
    User.Username    = vStrings[0];
    User.Password    = vStrings[1];
    User.Permissions = stoi(vStrings[2]);
    
    return User;
}

bool CheckAccessPermision(eMainMenuePermissions Permision)
{
    if (CurrentUser.Permissions == eMainMenuePermissions::pAll)
        return true;

    if ((Permision & CurrentUser.Permissions) == Permision)  
        return true;
    else
        return false;   
}

void ShowAccessDeniedMessage()
{
    cout << "\n------------------------------------\n"; 
    cout << "Access Denied, \nYou dont Have Permission To Do this,\nPlease Conact Your Admin."; 
    cout << "\n------------------------------------\n";
}

string ConvertClientRecordToLine(sClient Client, string Separator = "#//#")
{
    string Line = "";

    Line += Client.AccountNumber + Separator;
    Line += Client.PinCode + Separator;
    Line += Client.Name + Separator;
    Line += Client.Phone + Separator;
    Line += to_string(Client.AccountBalance);

    return Line;
}

string ConvertUserRecordToLine(sUser User, string Separator = "#//#")
{
    string Line = "";

    Line += User.Username + Separator;
    Line += User.Password + Separator;
    Line += to_string(User.Permissions);

    return Line;
}

vector <sClient> LoadClientsDataFromFile(string FileName)
{
    fstream ClientsFile;
    vector <sClient> vClients;
    sClient Client;

    ClientsFile.open(FileName, ios::in);

    if (ClientsFile.is_open())
    {
        string Line;
        while (getline(ClientsFile, Line))
        {
            Client = ConvertClientLineToRecord(Line);
            vClients.push_back(Client);
        }
        
        ClientsFile.close();
    }
    return vClients;
}

vector <sUser> LoadUsersDataFromFile(string Filename)
{
    fstream UsersFile;
    sUser User;
    vector <sUser> vUsers;

    UsersFile.open(Filename, ios::in);
    if (UsersFile.is_open())
    {
        string Line;

        while (getline(UsersFile, Line))
        {
            User = ConvertUserLineToRecord(Line);
            vUsers.push_back(User);
        }
        
        UsersFile.close();
    }
    return vUsers;
}

bool FindClientByAccountNumber(string AccountNumber, vector <sClient> vClients, sClient &Client)
{
    vClients = LoadClientsDataFromFile(ClientsFileName);

    for (sClient &C : vClients)
    {
        if (AccountNumber == C.AccountNumber)
        {
            Client = C;
            return true;
        }
    }
    return false;
}

short ReadMainMenueOptions()
{
    cout << "Choose what do you want to do? [1 to 8] : ";
    short Choice = 0;
    cin >> Choice;

    while (Choice < 1 || Choice > 8 || cin.fail())
    {
        cin.clear();
        cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid Choice, Enter a valid one : ";
        cin >> Choice;
    }

    return Choice;
}

short ReadTransactionsMenueOptions()
{
    short Choice = 0;
    cout << "Choose what do you want to do? [1 to 4] : ";
    cin >> Choice;

    while (Choice < 1 || Choice > 4 || cin.fail())
    {
        cin.clear();
        cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid Choice, Enter a valid one : ";
        cin >> Choice;
    }

    return Choice;
}

short ReadManageUsersMenue()
{
    short Choice = 0;
    cout << "Choose what do you want to do? [1 to 6] : ";
    cin >> Choice;

    while (Choice < 1 || Choice > 6 || cin.fail())
    {
        cin.clear();
        cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid Choice, Enter a valid one : ";
        cin >> Choice;
    }

    return Choice;
}

void GoBackToMainMenue()
{
    cout << "\n\nPress any key to go back to Main Menue ...";
    system("pause>0");
    ShowMainMenue();
}

void GoBackToTransactionsMenue()
{
    cout << "\n\nPress any key to go back to Transactions Menue ...";
    system("pause>0");
    ShowTransactionsMenue();
}

void GoBackToManageUsersMenue()
{
    cout << "\n\nPress any key to go back to Manage Users Menue ...";
    system("pause>0");
    ShowManageUsersMenue();
}

void PrintClientRecord(sClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void PrintUserCard(sUser User)
{     
    cout << "\nThe following are the user details:\n";
    cout << "-----------------------------------";
    cout << "\nUsername    : " << User.Username; 
    cout << "\nPassword    : " << User.Password;
    cout << "\nPermissions : " << User.Permissions;
    cout << "\n-----------------------------------\n";
}

void ShowAllClientsScreen()
{
    if (!CheckAccessPermision(eMainMenuePermissions::pListClients))
    {
        ShowAccessDeniedMessage();
        return;
    }

    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\tClients List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vClients.size() == 0)
    {
        cout << "\t\t\tNo Clients Available In The System!";
    }
    else
    {
        for (sClient &Client : vClients)
        {
            PrintClientRecord(Client);
            cout << "\n";
        }
    }
    
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n"<< endl; 

}

bool ClientExistsByAccountNumber(string FileName, string AccountNumber)
{
    fstream ClientsFile;
    sClient Client;

    ClientsFile.open(FileName);
    if (ClientsFile.is_open())
    {
        string Line;
        while (getline(ClientsFile, Line))
        {
            Client = ConvertClientLineToRecord(Line);
            
            if (AccountNumber == Client.AccountNumber)
            {
                ClientsFile.close();
                return true;
            }
        }
        ClientsFile.close();
    }
    return false;
}

bool UserExistsByUsername(string FileName, string Username)
{
    fstream UsersFile;
    UsersFile.open(FileName, ios::in);

    if (UsersFile.is_open())
    {
        string Line = "";

        while (getline(UsersFile, Line))
        {
            sUser User = ConvertUserLineToRecord(Line);

            if (User.Username == Username)
            {
                UsersFile.close();
                return true;
            }
        }
        UsersFile.close();
    }
    return false;
}

sClient ReadNewClient()
{
    sClient Client;

    cout << "Enter Account Number : ";
    getline(cin >> ws, Client.AccountNumber);

    while (ClientExistsByAccountNumber(ClientsFileName, Client.AccountNumber))
    {
        cout <<"\nClient with [" << Client.AccountNumber << "] already exists, Enter another Account Number? ";
        getline(cin >> ws, Client.AccountNumber);
    }
    
    cout << "Enter Pin Code : ";
    getline(cin, Client.PinCode);

    cout << "Enter Name : ";
    getline(cin, Client.Name);

    cout << "Enter Phone : ";
    getline(cin, Client.Phone);

    cout << "Enter Account Balance : ";
    cin >> Client.AccountBalance;

    return Client;
}

int ReadPermissionsToSet()
{
    int Permissions = 0;
    char Answer = 'n';

    cout << "\nDo you want to give full access? y/n? ";
    cin  >> Answer;
    if (Answer == 'Y' || Answer == 'y')
    {
        return -1;
    }

    cout << "\nDo you want to give access to : \n ";

    cout << "\nShow Client List? y/n? "; 
    cin >> Answer;
    if (Answer == 'Y' || Answer == 'y')
    {
        Permissions += eMainMenuePermissions::pListClients;
    }

    cout << "\nAdd New Client? y/n? ";
    cin >> Answer;    
    if (Answer == 'y' || Answer == 'Y')
    {         
        Permissions += eMainMenuePermissions::pAddNewClient;     
    }

    cout << "\nDelete Client? y/n? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    { 
        Permissions += eMainMenuePermissions::pDeleteClient;
    }

    cout << "\nUpdate Client? y/n? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {     
        Permissions += eMainMenuePermissions::pUpdateClient;
    }

    cout << "\nFind Client? y/n? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {        
        Permissions += eMainMenuePermissions::pFindClient;
    }

    cout << "\nTransactions? y/n? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y') 
    {        
        Permissions += eMainMenuePermissions::pTransactions; 
    } 

    cout << "\nManage Users? y/n? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {         
        Permissions += eMainMenuePermissions::pManageUsers; 
    }

    return Permissions;
}

sUser ReadNewUser()
{
    sUser User;

    cout << "Enter Username: ";
    getline(cin >> ws, User.Username);

    if (UserExistsByUsername(UsersFileName, User.Username))
    {
        cout <<"\nUser with [" << User.Username << "] already exists, Enter another Username? ";
        getline(cin >> ws, User.Username);
    }

    cout << "Enter Password: ";
    getline(cin, User.Password);

    User.Permissions = ReadPermissionsToSet();

    return User;
}

string ReadClientAccountNumber()
{
    string AccountNumber = "";
    cout <<"\nPlease enter Account Number: ";
    cin >> AccountNumber;
    return AccountNumber;
}

string ReadUsername()
{
    string Username = "";
    cout <<"\nPlease enter Username: ";
    cin >> Username;
    return Username;
}

void AddDataToFile(string FileName, string Line)
{
    fstream File;
    File.open(FileName, ios::out | ios::app);

    if (File.is_open())
    {
        File << Line << endl;
        File.close();
    }
}

void AddNewClient()
{
    sClient Client = ReadNewClient();
    AddDataToFile(ClientsFileName, ConvertClientRecordToLine(Client));
}

void ShowEndScreen()
{     
    cout <<"\n-----------------------------------\n";
    cout <<"\tProgram Ends :-)";
    cout <<"\n-----------------------------------\n";
}

void AddNewClients()
{
    char Answer = 'Y';
    do
    {
        cout << "Adding New Clients : \n\n";
        AddNewClient();
        cout << "\nClient Added Successfully, do you want to add more clients? Y/N? ";
        cin >> Answer;

    } while (toupper(Answer) == 'Y');
}

void ShowAddNewClientsScreen()
{
    if (!CheckAccessPermision(eMainMenuePermissions::pAddNewClient))
    {
        ShowAccessDeniedMessage();
        return;
    }

    cout << "\n------------------------------------------\n";
    cout << "\tAdd New Clients Screen\n";
    cout << "--------------------------------------------\n";
    AddNewClients();
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <sClient> &vClients)
{
    for (sClient &Client : vClients)
    {
        if (AccountNumber == Client.AccountNumber)
        {
            Client.MarkForDelete = true;
            return true;
        }
    }
    return false;
}

bool MarkUserForDeleteByUsername(string Username, vector <sUser> &vUsers)
{
    for (sUser &User : vUsers)
    {
        if (Username == User.Username)
        {
            User.MarkForDelete = true;
            return true;
        }
    }
    return false;
}

vector <sClient> SaveClientDataToFile(vector <sClient> &vClients)
{
    fstream ClientsFile;
    ClientsFile.open(ClientsFileName, ios::out);
    if (ClientsFile.is_open())
    {
        for (sClient &Client : vClients)
        {
            if (Client.MarkForDelete == false)
            {
                string Line = ConvertClientRecordToLine(Client);
                ClientsFile << Line << endl;
            }
        }
        ClientsFile.close();      
    }
    return vClients;
}

vector <sUser> SaveUserDataToFile(string FileName, vector <sUser> &vUsers)
{
    fstream UsersFile;
    UsersFile.open(FileName, ios::out);

    if (UsersFile.is_open())
    {
        for (sUser &User : vUsers)
        {
            if (User.MarkForDelete == false)
            {
                string Line = ConvertUserRecordToLine(User);
                UsersFile << Line << endl;
            }
        } 
        UsersFile.close();
    }
    return vUsers;
}

void DeleteClientByAccountNumber(string AccountNumber, vector <sClient> &vClients)
{
    char Answer = 'n';
    sClient Client;
    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientRecord(Client);
        cout << "\nAre you want delete this client y/n? :";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveClientDataToFile(vClients);
            // Refresh Data
            vClients = LoadClientsDataFromFile(ClientsFileName);
            cout << "\nClient Deletesd Successfuly\n";
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
    }
}

sUser ChangeUserRecord(string Username)
{
    sUser User;

    User.Username = Username;

    cout << "Enter Password: ";
    getline(cin >> ws, User.Password);

    User.Permissions = ReadPermissionsToSet();

    return User;
}

bool FindUserByUsername(string Username, vector <sUser> vUsers, sUser &User)
{
    vUsers = LoadUsersDataFromFile(UsersFileName);

    for (sUser &U : vUsers)
    {
        if (Username == U.Username)
        {
            User = U;
            return true;
        }
    }
    return false;
}

bool FindUserByUsernameAndPassword(string Username, string Password, sUser &User)
{
    vector <sUser> vUsers = LoadUsersDataFromFile(UsersFileName);

    for (sUser &U : vUsers)
    {
        if (Username == U.Username && Password == U.Password)
        {
            User = U;
            return true;
        }
    }
    return false;
}

void DeleteUserByUsername(string Username, vector <sUser> &vUsers)
{
    if (Username == "admin")
    {
        cout << "\n\nYou cannot Delete This User.";
    }

    sUser User;
    char Answer = 'n';

    if (FindUserByUsername(Username, vUsers, User))
    {
        PrintUserCard(User);

        cout << "\n\nAre you sure you want delete this User? y/n ? ";
        cin >> Answer;
        if (Answer == 'Y' || Answer == 'y')
        {
            MarkUserForDeleteByUsername(Username, vUsers);
            SaveUserDataToFile(UsersFileName, vUsers);

            vUsers = LoadUsersDataFromFile(UsersFileName);
            cout << "\nUser Deletesd Successfuly\n";
        }
    }
     else
    {
        cout << "\nUser with Username (" << Username << ") is Not Found!";
    }
}

void UpdateUserByUsername(string Username, vector <sUser> &vUsers)
{
    sUser User;
    char Answer = 'n';

    if (FindUserByUsername(Username, vUsers, User)) 
    {
        PrintUserCard(User);

        cout << "\n\nAre you sure you want update this User? y/n ? ";
        cin >> Answer;
        if (Answer == 'Y' || Answer == 'y')
        {
            for (sUser &User : vUsers)
            {
                if (Username == User.Username)
                {
                    User = ChangeUserRecord(Username);
                    break;
                }
            }
            SaveUserDataToFile(UsersFileName, vUsers);
            cout << "\nUser Updated Successfuly\n";
        }
    }
    else
    {
        cout << "\nUser with Username (" << Username << ") is Not Found!";
    }
}

void ShowDeleteClientsScreen()
{
    if (!CheckAccessPermision(eMainMenuePermissions::pDeleteClient))
    {
        ShowAccessDeniedMessage();
        return;
    }

    cout << "\n------------------------------------------\n";
    cout << "\tDelete Clients Screen\n";
    cout << "--------------------------------------------\n";

    string AccountNumber = ReadClientAccountNumber();
    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    DeleteClientByAccountNumber(AccountNumber, vClients);
}

sClient ChangeClientRecord(string AccountNumber)
{
    sClient Client;

    Client.AccountNumber = AccountNumber;

    cout << "Enter Pin Code : ";
    getline(cin >> ws, Client.PinCode);

    cout << "Enter Name : ";
    getline(cin, Client.Name);

    cout << "Enter Phone : ";
    getline(cin, Client.Phone);

    cout << "Enter Account Balance : ";
    cin >> Client.AccountBalance;

    return Client; 
}

void UpdateClientByAccountNumber(string AccountNumber, vector <sClient> &vClients)
{
    char Answer = 'n';
    sClient Client;
    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientRecord(Client);
        cout << "\nAre you want update this client y/n? :";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            for (sClient &Client : vClients)
            {
                if (AccountNumber == Client.AccountNumber)
                {
                    Client = ChangeClientRecord(AccountNumber);
                    break;
                }
            }
            SaveClientDataToFile(vClients);
            cout << "\nClient Updated Successfuly by (" << CurrentUser.Username << ")\n";
        }    
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber<< ") is Not Found!";
    }
}

void ShowUpdateClientScreen()
{
    if (!CheckAccessPermision(eMainMenuePermissions::pUpdateClient))
    {
        ShowAccessDeniedMessage();
        return;
    }

    cout << "\n------------------------------------------\n";
    cout << "\tUpdate Clients Screen\n";
    cout << "--------------------------------------------\n";

    string AccountNumber = ReadClientAccountNumber();
    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    UpdateClientByAccountNumber(AccountNumber, vClients);
}

void FindClientsScreen()
{
    if (!CheckAccessPermision(eMainMenuePermissions::pFindClient))
    {
        ShowAccessDeniedMessage();
        return;
    }

    cout << "\n------------------------------------------\n";
    cout << "\tFind Clients Screen\n";
    cout << "--------------------------------------------\n";

    sClient Client;
    string AccountNumber = ReadClientAccountNumber();
    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientRecord(Client);
        cout << "\nClient Find Successfuly\n";
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber<< ") is Not Found!";
    }
}

void DepositeBalanceToClientByAccountNumber(double Amount, string AccountNumber, vector <sClient> &vClients)
{
    char Answer = 'n';
    cout << "\n\nAre you sure you want perform transactions y/n : ";
    cin >> Answer;
    if (Answer == 'Y' || Answer == 'y')
    {
        for (sClient &Client : vClients)
        {
            if (AccountNumber == Client.AccountNumber)
            {
                Client.AccountBalance += Amount;
                SaveClientDataToFile(vClients);
                cout << "\n\nDone Successfuly, New balance is : " << Client.AccountBalance;
            }
        }
    }
}

void ShowDepositeScreen()
{
    cout << "\n------------------------------------------\n";
    cout << "\tDeposite Screen\n";
    cout << "--------------------------------------------\n";

    string AccountNumber = ReadClientAccountNumber();
    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    sClient Client;

    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with Account Number (" << AccountNumber<< ") is Not exist!";
        AccountNumber = ReadClientAccountNumber();
    }

    PrintClientRecord(Client);

    double Amount = 0.0;
    cout << "\nPlease enter deposite amount : ";
    cin >> Amount;

    while (Amount < 0)
    {
        cout << "\nThis value cannot be deposited.\n";
        cout << "\nPlease enter deposite amount : ";
        cin >> Amount;
    }
    
    DepositeBalanceToClientByAccountNumber(Amount, AccountNumber, vClients);
}

void ShowWithdrawScreen()
{
    cout << "\n------------------------------------------\n";
    cout << "\tWithdraw Screen\n";
    cout << "--------------------------------------------\n";

    string AccountNumber      = ReadClientAccountNumber();
    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    sClient Client;

    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with Account Number (" << AccountNumber<< ") is Not exist!";
        AccountNumber = ReadClientAccountNumber();
    }

    PrintClientRecord(Client);

    double Amount = 0.0;
    cout << "\nPlease enter Withdraw amount : ";
    cin >> Amount;

    while (Amount > Client.AccountBalance)
    {
        cout << "\nAmount Exceeds the balance, you can draw up to : " << Client.AccountBalance << ".\n";
        cout << "\nPlease enter deposite amount : ";
        cin >> Amount;
    }
    
    DepositeBalanceToClientByAccountNumber(Amount * -1, AccountNumber, vClients);
}

void PrintClientsBalanceRecord(sClient Client)
{
    cout <<"| "<< setw(15) << left <<Client.AccountNumber;
    cout <<"| "<< setw(40) << left <<Client.Name;
    cout <<"| "<< setw(12) << left <<Client.AccountBalance << endl;
}

void ShowTotalBalance()
{
    vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);

    cout << "\n\t\tBalances List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "| "<< left << setw(15) << "Accout Number";
    cout << "| "<< left << setw(40) << "Client Name";
    cout << "| "<< left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    double TotalBalances = 0.0;

    if (vClients.size() == 0)
    {
       cout << "\t\tNo Clients Available In the System!";
    }
    else
    {
        for (sClient &Client : vClients)
        {
            PrintClientsBalanceRecord(Client);
            TotalBalances += Client.AccountBalance;
        }
        
    }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n"<< endl;
    cout << "\t\tTotal Balances = "<< TotalBalances;
} 

void ShowTotalBalanceScreen()
{
    ShowTotalBalance();
}

void PrintUserRecordLine(sUser User)
{
    cout << "| " <<  setw(15) <<  left  << User.Username;
    cout << "| " <<  setw(40) <<  left  << User.Password;
    cout << "| " <<  setw(12) <<  left  << User.Permissions;
}

void ShowAllUsersScreen()
{
    vector <sUser> vUsers = LoadUsersDataFromFile(UsersFileName);

    cout << "\n\t\tUsers List (" << vUsers.size() << ") User(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n"<< endl;
    cout << "| "<< left << setw(15) << "Username";
    cout << "| "<< left << setw(40) << "Password";
    cout << "| "<< left << setw(12) << "Permissions";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n"<< endl;

    if (vUsers.size() == 0)
    {
        cout << "\t\tNo Users Available In the System!";
    }
    else
    {
        for (sUser &User : vUsers)
        {
            PrintUserRecordLine(User);
            cout << endl;
        }
        
    }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}

void ShowListUsersScreen()
{
    ShowAllUsersScreen();
}

void AddNewUser()
{
    sUser User = ReadNewUser();
    string Line = ConvertUserRecordToLine(User);
    AddDataToFile(UsersFileName, Line);
}

void AddNewUsers()
{
    char AddNewUsers = 'n';

    do
    {
        cout << "Adding New Users : \n\n";

        AddNewUser();

        cout << "\nUser Added Successfully, do you want to add more users? Y/N? ";

        cin >> AddNewUsers;

    } while (toupper(AddNewUsers) == 'Y');
    
}

void ShowAddNewUsersScreen()
{
    cout << "\n------------------------------------------\n";
    cout << "\tAdd New User Screen\n";
    cout << "--------------------------------------------\n";

    AddNewUsers();
}

void ShowDeleteUsersScreen()
{
    cout << "\n------------------------------------------\n";
    cout << "\tDelete User Screen\n";
    cout << "--------------------------------------------\n";

    string Username = ReadUsername();
    vector <sUser> vUsers = LoadUsersDataFromFile(UsersFileName);
    DeleteUserByUsername(Username, vUsers);
}

void ShowUpdateUsersScreen()
{
    cout << "\n------------------------------------------\n";
    cout << "\tUpdate Users Screen\n";
    cout << "--------------------------------------------\n";

    string Username = ReadUsername();
    vector <sUser> vUsers = LoadUsersDataFromFile(UsersFileName);
    UpdateUserByUsername(Username, vUsers);
}

void ShowFindUsersScreen()
{
    cout << "\n------------------------------------------\n";
    cout << "\tFind Users Screen\n";
    cout << "--------------------------------------------\n";

    sUser User;
    string Username = ReadUsername();
    vector <sUser> vUsers = LoadUsersDataFromFile(UsersFileName);
    if (FindUserByUsername(Username, vUsers, User))
    {
        PrintUserCard(User);
        cout << "\nUser Find Successfuly\n";
    }
    else
    {
        cout << "\nUser with Username (" << Username << ") is Not Found!";
    }
}

void PerformTransactionsMenueOptions(eTransactionsMenue TransactionsMenue)
{
    switch (TransactionsMenue)
    {
    case eTransactionsMenue::eDeposite :
        system("cls");
        ShowDepositeScreen();
        GoBackToTransactionsMenue();
        break;

    case eTransactionsMenue::eWtihdraw :
        system("cls");
        ShowAddNewUsersScreen();
        ShowWithdrawScreen();
        break;

    case eTransactionsMenue::eShowTotalBalance :
        system("cls");
        ShowTotalBalanceScreen();
        GoBackToTransactionsMenue();
        break;

    case eTransactionsMenue::eShowMainMenue :
        system("cls");
        ShowMainMenue();
        break;
    }
}

void ShowTransactionsMenue()
{
    if (!CheckAccessPermision(eMainMenuePermissions::pTransactions))
    {
        ShowAccessDeniedMessage();
        return;
    }

    system("cls");
    cout << "===========================================\n";
    cout << "\t\tTransactions Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Deposite.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Show Total Balance.\n";
    cout << "\t[4] Show Main Menue.\n";
    cout << "===========================================\n";
    PerformTransactionsMenueOptions((eTransactionsMenue)ReadTransactionsMenueOptions());
}

void PerformManageUsersMenueOptions(eManageUsersMenue UsersMenue)
{
    switch (UsersMenue)
    {
    case eManageUsersMenue::eListUsers :
        system("cls");
        ShowListUsersScreen();
        GoBackToManageUsersMenue();
        break;

    case eManageUsersMenue::eAddNewUser :
        system("cls");
        ShowAddNewUsersScreen();
        GoBackToManageUsersMenue();
        break;

    case eManageUsersMenue::eDeleteUser :
        system("cls");
        ShowDeleteUsersScreen();
        GoBackToManageUsersMenue();
        break;

    case eManageUsersMenue::eUpdateUser :
        system("cls");
        ShowUpdateUsersScreen();
        GoBackToManageUsersMenue();
        break;

    case eManageUsersMenue::eFindUser :
        system("cls");
        ShowFindUsersScreen();
        GoBackToManageUsersMenue();
        break;

    case eManageUsersMenue::eMainMenue :
        system("cls");
        GoBackToMainMenue();
        break;
    }
}

void ShowManageUsersMenue()
{
    if (!CheckAccessPermision(eMainMenuePermissions::pManageUsers))
    {
        ShowAccessDeniedMessage();
        return;
    }

    system("cls");
    cout << "===========================================\n";
    cout << "\t\tManage Users Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] List Users.\n";
    cout << "\t[2] Add New User.\n";
    cout << "\t[3] Delete User.\n";
    cout << "\t[4] Update User.\n";
    cout << "\t[5] Find User.\n";
    cout << "\t[6] Main Menue.\n";
    cout << "===========================================\n";

    short ReadUsersOptions = ReadManageUsersMenue();
    PerformManageUsersMenueOptions((eManageUsersMenue)ReadUsersOptions);
}

void PerformMainMenueOptions(eMainMenueOptions MainMenueOptions)
{
    switch (MainMenueOptions)
    {
    case eMainMenueOptions::eListClients :
        system("cls");
        ShowAllClientsScreen();
        GoBackToMainMenue();
        break;

    case eMainMenueOptions::eAddNewClient :
        system("cls");
        ShowAddNewClientsScreen();
        GoBackToMainMenue();
        break;

    case eMainMenueOptions::eDeleteClient :
        system("cls");
        ShowDeleteClientsScreen();
        GoBackToMainMenue();
        break;

    case eMainMenueOptions::eUpdateClient :
        system("cls");
        ShowUpdateClientScreen();
        GoBackToMainMenue();
        break;

    case eMainMenueOptions::eFindClient :
        system("cls");
        FindClientsScreen();
        GoBackToMainMenue();
        break;

    case eMainMenueOptions::eTransactionsMenueOptions :
        system("cls");
        ShowTransactionsMenue();
        GoBackToMainMenue();
        break;

    case eMainMenueOptions::eManageUsers :
        system("cls");
        ShowManageUsersMenue();
        GoBackToMainMenue();
        break;

    case eMainMenueOptions::eExit :
        system("cls");
        Login();
        // ShowEndScreen();
        break;                    
    }        
}

void ShowMainMenue()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tMain Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Show Clients List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] Manage Users.\n";
    cout << "\t[8] Logout.\n";
    cout << "===========================================\n";
    PerformMainMenueOptions((eMainMenueOptions)ReadMainMenueOptions());
}

bool LoadUserInfo(string Username, string Password)
{
    if (FindUserByUsernameAndPassword(Username, Password, CurrentUser))
        return true;
    else
        return false;    
}

void Login()
{
    bool LoginFaild = false;
    string Username, Password;

    do
    {
        system("cls");

        cout << "\n--------------------------------\n";
        cout << "\tLogin Screen";
        cout << "\n--------------------------------\n";

        if (LoginFaild)
        {
            cout << "Invalid Username or Password!\n";
        }

        cout << "Enter Username: ";
        cin  >> Username;

        cout << "Enter Password: ";
        cin  >> Password;

        LoginFaild = !LoadUserInfo(Username, Password);

    } while (LoginFaild);

    ShowMainMenue();
}

int main()
{
    Login();
    return 0;
}