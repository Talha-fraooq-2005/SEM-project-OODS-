#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <regex>
#include <algorithm>
#include <iomanip>
#include <queue>
#include <stack>
#include <list>
#include <thread>
#include <chrono>
#include <map>
#include <sstream>

using namespace std;

string Red() { return "\033[31m"; }
string Green() { return "\033[32m"; }
string Yellow() { return "\033[33m"; }
string Blue() { return "\033[34m"; }
string Magenta() { return "\033[35m"; }
string Cyan() { return "\033[36m"; }
string White() { return "\033[37m"; }
string Reset() { return "\033[0m"; }
string Bold() { return "\033[1m"; }

void clearScreen() {
   system("cls");
}

void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
    clearScreen();
}

void loadingAnimation(const string& message, int duration = 2) {
    cout << Bold() + Cyan() + message + Reset();
    for (int i = 0; i < duration * 4; i++) {
        cout << "." << flush;
        this_thread::sleep_for(chrono::milliseconds(250));
    }
    cout << endl;
}

void printHeader(const string& title) {
    clearScreen();
    cout << Cyan();
    cout << "+----------------------------------------------------------+" << endl;
    cout << "¦";
    int padding = (60 - title.length()) / 2;
    for (int i = 0; i < padding; i++) cout << " ";
    cout << Bold() << title << Reset();
    cout << Cyan();
    for (int i = 0; i < (60 - title.length() - padding); i++) cout << " ";
    cout << "¦" << endl;
    cout << "+----------------------------------------------------------+" << endl;
    cout << Reset();
}

void printMenu(const vector<string>& options) {
    cout << endl;
    for (int i = 0; i < options.size(); i++) {
        cout << "  " << i + 1 << ". " << options[i] << endl;
    }
    cout << endl;
}

void printSuccess(const string& message) {
    cout << Green() << "? " << message << Reset() << endl;
}

void printError(const string& message) {
    cout << Red() << "? " << message << Reset() << endl;
}

void printWarning(const string& message) {
    cout << Yellow() << "  " << message << Reset() << endl;
}

void printInfo(const string& message) {
    cout << Blue() << "  " << message << Reset() << endl;
}

class AdvancedEncryption {
private:
    static const int KEY = 7; 

public:
    static string encrypt(const string& data) {
        string encrypted = "";
        for (char c : data) {
            char encryptedChar = c;
            if (isalpha(c)) {
                char base = isupper(c) ? 'A' : 'a';
                encryptedChar = (c - base + KEY) % 26 + base;
            }
            else if (isdigit(c)) {
                encryptedChar = (c - '0' + KEY) % 10 + '0';
            }
            encryptedChar ^= KEY;
            encryptedChar += 3;
            encrypted += encryptedChar;
        }
        reverse(encrypted.begin(), encrypted.end());
        return encrypted;
    }

    static string decrypt(const string& data) {
        string decrypted = "";
        string reversed = data;
        reverse(reversed.begin(), reversed.end());

        for (char c : reversed) {
            char decryptedChar = c;
            decryptedChar -= 3;
            decryptedChar ^= KEY;
            if (isalpha(decryptedChar)) {
                char base = isupper(decryptedChar) ? 'A' : 'a';
                decryptedChar = (decryptedChar - base - KEY + 26) % 26 + base;
            }
            else if (isdigit(decryptedChar)) {
                decryptedChar = (decryptedChar - '0' - KEY + 10) % 10 + '0';
            }
            decrypted += decryptedChar;
        }
        return decrypted;
    }

    static string encryptForFile(const string& data) {
        return "[ENC]" + encrypt(data) + "[/ENC]";
    }

    static bool isEncrypted(const string& data) {
        return data.find("[ENC]") != string::npos && data.find("[/ENC]") != string::npos;
    }

    static string decryptFromFile(const string& data) {
        if (!isEncrypted(data)) return data;

        size_t start = data.find("[ENC]") + 5;
        size_t end = data.find("[/ENC]");
        string encryptedData = data.substr(start, end - start);
        return decrypt(encryptedData);
    }
};

class SecureFileHandler {
private:
    string filename;

public:
    SecureFileHandler(const string& fname) : filename(fname) {}

    bool writeEncryptedData(const vector<string>& data) {
        ofstream file(filename, ios::binary);
        if (!file.is_open()) {
            return false;
        }

        for (const string& line : data) {
            string encryptedLine = AdvancedEncryption::encryptForFile(line);
            file << encryptedLine << endl;
        }
        file.close();
        return true;
    }

    vector<string> readEncryptedData() {
        vector<string> data;
        ifstream file(filename, ios::binary);
        if (!file.is_open()) {
            return data;
        }

        string line;
        while (getline(file, line)) {
            if (!line.empty()) {
                string decryptedLine = AdvancedEncryption::decryptFromFile(line);
                data.push_back(decryptedLine);
            }
        }
        file.close();
        return data;
    }

    bool appendEncryptedData(const string& data) {
        ofstream file(filename, ios::binary | ios::app);
        if (!file.is_open()) {
            return false;
        }

        string encryptedLine = AdvancedEncryption::encryptForFile(data);
        file << encryptedLine << endl;
        file.close();
        return true;
    }

    bool fileExists() {
        ifstream file(filename);
        return file.good();
    }
};

class AccountNumberGenerator {
private:
    static long long nextAccountNumber;
    const string BANK_PREFIX = "TCBOI";

public:
    AccountNumberGenerator() {
        loadLastAccountNumber();
    }

    string generateAccountNumber() {
        nextAccountNumber++;
        saveLastAccountNumber();

        stringstream ss;
        ss << BANK_PREFIX << setw(10) << setfill('0') << nextAccountNumber;
        return ss.str();
    }

private:
    void loadLastAccountNumber() {
        SecureFileHandler file("account_counter.dat");
        if (file.fileExists()) {
            vector<string> data = file.readEncryptedData();
            if (!data.empty()) {
                try {
                    string lastNum = data[0];
                    if (lastNum.find(BANK_PREFIX) != string::npos) {
                        lastNum = lastNum.substr(BANK_PREFIX.length());
                    }
                    nextAccountNumber = stoll(lastNum);
                }
                catch (...) {
                    nextAccountNumber = 1000000000; 
                }
            }
            else {
                nextAccountNumber = 1000000000;
            }
        }
        else {
            nextAccountNumber = 1000000000;
        }
    }

    void saveLastAccountNumber() {
        vector<string> data;
        data.push_back(to_string(nextAccountNumber));
        SecureFileHandler file("account_counter.dat");
        file.writeEncryptedData(data);
    }
};

long long AccountNumberGenerator::nextAccountNumber = 0;

template<typename T>
class TransactionStack {
private:
    stack<T> transactions;
    const int MAX_SIZE = 20;

public:
    void push(const T& transaction) {
        if (transactions.size() >= MAX_SIZE) {
            stack<T> temp;
            while (transactions.size() > 1) {
                temp.push(transactions.top());
                transactions.pop();
            }
            transactions = stack<T>();
            while (!temp.empty()) {
                transactions.push(temp.top());
                temp.pop();
            }
        }
        transactions.push(transaction);
    }

    T pop() {
        if (!transactions.empty()) {
            T transaction = transactions.top();
            transactions.pop();
            return transaction;
        }
        throw out_of_range("Stack is empty");
    }

    T top() const {
        if (!transactions.empty()) {
            return transactions.top();
        }
        throw out_of_range("Stack is empty");
    }

    bool isEmpty() const {
        return transactions.empty();
    }

    size_t size() const {
        return transactions.size();
    }

    void displayRecent(int count = 5) const {
        if (transactions.empty()) {
            printWarning("No transactions available.");
            return;
        }

        stack<T> temp = transactions;
        vector<T> recentTransactions;

        while (!temp.empty()) {
            recentTransactions.push_back(temp.top());
            temp.pop();
        }

        cout << Bold() << "Recent Transactions (Last " << min(count, (int)recentTransactions.size()) << "):" << Reset() << endl;
        cout << "+------------------------------------------------------+" << endl;
        for (int i = 0; i < min(count, (int)recentTransactions.size()); i++) {
            cout << "¦ " << setw(2) << right << i + 1 << ". " << setw(50) << left << recentTransactions[i] << " ¦" << endl;
        }
        cout << "+------------------------------------------------------+" << endl;
    }

    vector<T> getAllTransactions() const {
        stack<T> temp = transactions;
        vector<T> allTransactions;
        while (!temp.empty()) {
            allTransactions.push_back(temp.top());
            temp.pop();
        }
        reverse(allTransactions.begin(), allTransactions.end());
        return allTransactions;
    }

    void loadFromVector(const vector<T>& transactionList) {
        transactions = stack<T>();
        for (const T& transaction : transactionList) {
            transactions.push(transaction);
        }
    }
};

template<typename T>
class ATMQueue {
private:
    queue<T> customers;
    const int MAX_QUEUE_SIZE = 10;

public:
    void enqueue(const T& customer) {
        if (customers.size() >= MAX_QUEUE_SIZE) {
            printError("ATM queue is full! Please try again later.");
            return;
        }
        customers.push(customer);
        printSuccess("Customer " + customer + " added to ATM queue. Position: " + to_string(customers.size()));
    }

    T dequeue() {
        if (!customers.empty()) {
            T customer = customers.front();
            customers.pop();
            return customer;
        }
        throw out_of_range("Queue is empty");
    }

    T front() const {
        if (!customers.empty()) {
            return customers.front();
        }
        throw out_of_range("Queue is empty");
    }

    bool isEmpty() const {
        return customers.empty();
    }

    size_t size() const {
        return customers.size();
    }

    void displayQueue() const {
        if (customers.empty()) {
            printInfo("ATM queue is empty.");
            return;
        }

        queue<T> temp = customers;
        cout << Bold() << "Current ATM Queue (" << customers.size() << " customers):" << Reset() << endl;
        cout << "+------------------------------------------------------+" << endl;
        int position = 1;
        while (!temp.empty()) {
            cout << "¦ " << setw(2) << right << position++ << ". " << setw(50) << left << temp.front() << " ¦" << endl;
            temp.pop();
        }
        cout << "+------------------------------------------------------+" << endl;
    }
};

class CustomerLinkedList {
private:
    struct Node {
        string accountNumber;
        string accountHolderName;
        double balance;
        string email;
        string phoneNumber;
        Node* next;

        Node(const string& accNum, const string& name, double bal, const string& mail, const string& phone)
            : accountNumber(accNum), accountHolderName(name), balance(bal), email(mail), phoneNumber(phone), next(nullptr) {
        }
    };

    Node* head;
    int size;

public:
    CustomerLinkedList() : head(nullptr), size(0) {}

    ~CustomerLinkedList() {
        clear();
    }

    void insert(const string& accNum, const string& name, double balance, const string& email, const string& phone) {
        Node* newNode = new Node(accNum, name, balance, email, phone);
        if (!head) {
            head = newNode;
        }
        else {
            Node* temp = head;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
        size++;
    }

    bool remove(const string& accNum) {
        if (!head) return false;

        if (head->accountNumber == accNum) {
            Node* temp = head;
            head = head->next;
            delete temp;
            size--;
            return true;
        }

        Node* current = head;
        while (current->next && current->next->accountNumber != accNum) {
            current = current->next;
        }

        if (current->next) {
            Node* temp = current->next;
            current->next = current->next->next;
            delete temp;
            size--;
            return true;
        }
        return false;
    }

    void displayAll() const {
        if (!head) {
            printWarning("No accounts in the system.");
            return;
        }

        cout << Bold() << "All Bank Accounts (" << size << " accounts):" << Reset() << endl;
        cout << "+----------------------------------------------------------------------------------+" << endl;
        cout << "¦ " << setw(4) << left << "No."
            << "¦ " << setw(16) << "Account Number"
            << "¦ " << setw(20) << "Account Holder"
            << "¦ " << setw(16) << "Email"
            << "¦ " << setw(12) << "Balance" << " ¦" << endl;
        cout << "+------+------------------+----------------------+------------------+--------------¦" << endl;

        Node* current = head;
        int count = 1;
        while (current) {
            cout << "¦ " << setw(4) << left << count++
                << "¦ " << setw(16) << current->accountNumber
                << "¦ " << setw(20) << (current->accountHolderName.length() > 18 ?
                    current->accountHolderName.substr(0, 15) + "..." :
                    current->accountHolderName)
                << "¦ " << setw(16) << (current->email.length() > 14 ?
                    current->email.substr(0, 11) + "..." :
                    current->email)
                << "¦ $" << setw(10) << fixed << setprecision(2) << current->balance << " ¦" << endl;
            current = current->next;
        }
        cout << "+----------------------------------------------------------------------------------+" << endl;
    }

    bool contains(const string& accNum) const {
        Node* current = head;
        while (current) {
            if (current->accountNumber == accNum) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    Node* find(const string& accNum) const {
        Node* current = head;
        while (current) {
            if (current->accountNumber == accNum) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    int getSize() const {
        return size;
    }

    void clear() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        size = 0;
    }

    vector<Node*> getAllNodes() const {
        vector<Node*> nodes;
        Node* current = head;
        while (current) {
            nodes.push_back(current);
            current = current->next;
        }
        return nodes;
    }
};

string passwordGenerator(int size, bool includeNumbers = true, bool includeSymbols = true) {
    if (size < 8) {
        printWarning("Password length should be at least 8 characters for security.");
        size = 8;
    }

    string resultPassword = "";
    string numbers = "1234567890";
    string alphabets = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";
    string symbols = "!@#$%^&*()_-+=[]{}|;:,.<>?";

    srand(time(0));

    if (includeNumbers) {
        resultPassword += numbers[rand() % numbers.size()];
        size--;
    }
    if (includeSymbols) {
        resultPassword += symbols[rand() % symbols.size()];
        size--;
    }
    // lowercase
    resultPassword += alphabets[rand() % (alphabets.size() / 2)];
    // uppercase
    resultPassword += alphabets[(rand() % (alphabets.size() / 2)) + (alphabets.size() / 2)];
    size -= 2;

    string allChars = alphabets;
    if (includeNumbers) allChars += numbers;
    if (includeSymbols) allChars += symbols;

    while (size--) {
        resultPassword += allChars[rand() % allChars.size()];
    }
    random_shuffle(resultPassword.begin(), resultPassword.end());
    return resultPassword;
}
string OTPGenerator(int size) {
    if (size != 4 && size != 6) {
        printWarning("OTP size must be 4 or 6 digits. Using 6 digits.");
        size = 6;
    }

    srand(time(0));
    string numbers = "1234567890";
    string result = "";
    for (int i = 0; i < size; i++) {
        result += numbers[rand() % 10];
    }
    return result;
}

string passwordStrengthChecker(const string& password) {
    bool hasLower = false, hasUpper = false, hasDigit = false, hasSpecial = false;
    bool goodLength = password.length() >= 12;
    bool excellentLength = password.length() >= 16;

    string specialChars = "!@#$%^&*()-+_=[]{}|;:,.<>?";

    for (char ch : password) {
        if (islower(ch)) hasLower = true;
        else if (isupper(ch)) hasUpper = true;
        else if (isdigit(ch)) hasDigit = true;
        else if (specialChars.find(ch) != string::npos) hasSpecial = true;
    }

    int score = hasLower + hasUpper + hasDigit + hasSpecial + goodLength + excellentLength;

    if (score >= 6) return "Excellent";
    if (score >= 4) return "Strong";
    if (score >= 3) return "Good";
    if (score >= 2) return "Weak";
    return "Very Weak";
}

void displayPasswordStrength(const string& password) {
    string strength = passwordStrengthChecker(password);

    if (strength == "Excellent") {
        cout << Green();
    }
    else if (strength == "Strong") {
        cout << Cyan();
    }
    else if (strength == "Good") {
        cout << Yellow();
    }
    else {
        cout << Red();
    }

    cout << "Password Strength: " << strength << " ";

    // Visual strength indicator
    int strengthLevel = 0;
    if (strength == "Very Weak") strengthLevel = 1;
    else if (strength == "Weak") strengthLevel = 2;
    else if (strength == "Good") strengthLevel = 3;
    else if (strength == "Strong") strengthLevel = 4;
    else if (strength == "Excellent") strengthLevel = 5;

    cout << "[";
    for (int i = 0; i < 5; i++) {
        if (i < strengthLevel) {
            if (strengthLevel <= 2) cout << Red();
            else if (strengthLevel <= 3) cout << Yellow();
            else cout << Green();
            cout << " ";
            cout << Reset();
        }
        else {
            cout << " ";
        }
    }
    cout << "]" << Reset() << endl;
}
CustomerLinkedList allAccounts;
AccountNumberGenerator accountGenerator;

class Admin {
private:
    string adminUsername = "admin";
    string adminPassword = "admin123"; 
    SecureFileHandler adminFile;

public:
    Admin() : adminFile("admin_config.dat") {
        loadAdminCredentials();
    }

    bool authenticate() {
        string username, password;
        printHeader("ADMIN LOGIN");
        cout << "Enter Admin Username: ";
        getline(cin, username);
        cout << "Enter Admin Password: ";
        getline(cin, password);

        if (username == adminUsername && password == adminPassword) {
            printSuccess("Admin authentication successful!");
            return true;
        }
        else {
            printError("Invalid admin credentials!");
            return false;
        }
    }

    void changePassword() {
        string currentPass, newPass;
        cout << "Enter current password: ";
        getline(cin, currentPass);

        if (currentPass != adminPassword) {
            printError("Incorrect current password!");
            return;
        }

        cout << "Enter new password: ";
        getline(cin, newPass);

        if (newPass.length() < 6) {
            printError("Password must be at least 6 characters long!");
            return;
        }

        adminPassword = newPass;
        saveAdminCredentials();
        printSuccess("Admin password changed successfully!");
    }

    void viewAllData() {
        printHeader("ALL BANK DATA - ADMIN VIEW");

        vector<string> accountFiles = getAllAccountFiles();

        if (accountFiles.empty()) {
            printInfo("No account files found.");
            return;
        }

        cout << "Found " << accountFiles.size() << " account files:" << endl;
        cout << "+--------------------------------------------------------------------+" << endl;

        for (const string& filename : accountFiles) {
            cout << "¦ File: " << setw(55) << left << filename << " ¦" << endl;

            SecureFileHandler fileHandler(filename);
            if (fileHandler.fileExists()) {
                vector<string> accountData = fileHandler.readEncryptedData();

                cout << "¦ +--------------------------------------------------------------+ ¦" << endl;
                for (const string& line : accountData) {
                    string displayLine = line.length() > 58 ? line.substr(0, 55) + "..." : line;
                    cout << "¦ ¦ " << setw(58) << left << displayLine << " ¦ ¦" << endl;
                }
                cout << "¦ +--------------------------------------------------------------+ ¦" << endl;
            }
            cout << "+--------------------------------------------------------------------¦" << endl;
        }
        cout << "+--------------------------------------------------------------------+" << endl;
    }

    void viewDecryptedData() {
        printHeader("DECRYPTED ACCOUNT DATA - ADMIN VIEW");

        vector<string> accountFiles = getAllAccountFiles();

        if (accountFiles.empty()) {
            printInfo("No account files found.");
            return;
        }

        for (const string& filename : accountFiles) {
            cout << "\nAccount File: " << filename << endl;
            cout << "+--------------------------------------------------------------------+" << endl;

            SecureFileHandler fileHandler(filename);
            if (fileHandler.fileExists()) {
                vector<string> accountData = fileHandler.readEncryptedData();

                for (const string& line : accountData) {
                    cout << "¦ " << setw(66) << left << line << " ¦" << endl;
                }
            }
            cout << "+--------------------------------------------------------------------+" << endl;
        }
    }

    void systemStatistics() {
        printHeader("SYSTEM STATISTICS");

        vector<string> accountFiles = getAllAccountFiles();
        double totalBalance = 0.0;
        int totalAccounts = accountFiles.size();

        for (const string& filename : accountFiles) {
            SecureFileHandler fileHandler(filename);
            if (fileHandler.fileExists()) {
                vector<string> accountData = fileHandler.readEncryptedData();
                if (accountData.size() >= 7) { 
                    try {
                        totalBalance += stod(accountData[6]);
                    }
                    catch (...) {
                    }
                }
            }
        }

        cout << "+--------------------------------------------------------------------+" << endl;
        cout << "¦ Total Accounts in System  : " << setw(42) << left << totalAccounts << " ¦" << endl;
        cout << "¦ Total Balance Across All Accounts : $" << setw(36) << left << fixed << setprecision(2) << totalBalance << " ¦" << endl;
        cout << "¦ Account Data Files        : " << setw(42) << left << accountFiles.size() << " ¦" << endl;
        cout << "¦ System Files Encrypted    : " << setw(42) << left << "Yes" << " ¦" << endl;
        cout << "¦ Encryption Level          : " << setw(42) << left << "Advanced Multi-layer" << " ¦" << endl;
        cout << "+--------------------------------------------------------------------+" << endl;
    }

private:
    void loadAdminCredentials() {
        if (adminFile.fileExists()) {
            vector<string> credentials = adminFile.readEncryptedData();
            if (credentials.size() >= 2) {
                adminUsername = credentials[0];
                adminPassword = credentials[1];
            }
        }
    }

    void saveAdminCredentials() {
        vector<string> credentials;
        credentials.push_back(adminUsername);
        credentials.push_back(adminPassword);
        adminFile.writeEncryptedData(credentials);
    }

    vector<string> getAllAccountFiles() {
        vector<string> accountFiles;

        vector<string> possibleFiles = {
            "account_counter.dat",
            "admin_config.dat"
        };
        for (int i = 0; i < 100; i++) {
            string possibleFile = "TCBOI" + string(10, '0').substr(0, 10 - to_string(1000000000 + i).length()) + to_string(1000000000 + i) + ".dat";
            ifstream testFile(possibleFile);
            if (testFile.good()) {
                accountFiles.push_back(possibleFile);
                testFile.close();
            }
        }

        return accountFiles;
    }
};

class bankAccount {
private:
    string bankName = "TCBOI";
    string accountHolderName;
    string accountNumber;
    string accountPassword;
    string addharNumber;
    string email;
    string phoneNumber;
    double balance;
    vector<string> transactionHistory;
    TransactionStack<string> transactionStack;
    SecureFileHandler accountFile;

public:
    bankAccount() : accountFile("") {} 

    bankAccount(const string& accNumber) : accountFile(accNumber + ".dat") {
        accountNumber = accNumber;
    }

    string getAccountNumber() { return accountNumber; }
    string getAccountPassword() { return accountPassword; }
    double getBalance() { return balance; }
    vector<string> getTransactionHistory() { return transactionHistory; }
    string getAccountHolderName() { return accountHolderName; }
    string getEmail() { return email; }
    string getPhoneNumber() { return phoneNumber; }

    void savetoFile();
    void loadFromFile();
    void mainMenu();
    void createAccount();
    void loginBankAccount();
    void depositMoney();
    void withdrawMoney();
    void checkBalance();
    void accountDetails();
    void allTransactionHistory();
    void undoLastTransaction();
    void transferMoney();
    void changePassword();

    friend class ATM;
};

void bankAccount::savetoFile() {
    vector<string> accountData;
    accountData.push_back(accountHolderName);
    accountData.push_back(accountNumber);
    accountData.push_back(accountPassword);
    accountData.push_back(addharNumber);
    accountData.push_back(email);
    accountData.push_back(phoneNumber);
    accountData.push_back(to_string(balance));

    for (const auto& transaction : transactionHistory) {
        accountData.push_back(transaction);
    }

    if (!accountFile.writeEncryptedData(accountData)) {
        printError("Error saving account data to encrypted file!");
    }
}

void bankAccount::loadFromFile() {
    vector<string> accountData = accountFile.readEncryptedData();

    if (accountData.size() >= 7) {
        accountHolderName = accountData[0];
        accountNumber = accountData[1];
        accountPassword = accountData[2];
        addharNumber = accountData[3];
        email = accountData[4];
        phoneNumber = accountData[5];
        try {
            balance = stod(accountData[6]);
        }
        catch (...) {
            balance = 0.0;
        }

        transactionHistory.clear();
        for (size_t i = 7; i < accountData.size(); i++) {
            transactionHistory.push_back(accountData[i]);
            transactionStack.push(accountData[i]);
        }
    }
    else {
        printError("Invalid account data format in file!");
    }
}

void bankAccount::undoLastTransaction() {
    try {
        string lastTransaction = transactionStack.pop();

        if (lastTransaction.find("Deposit") != string::npos) {
            size_t pos = lastTransaction.find(":");
            if (pos != string::npos) {
                string amountStr = lastTransaction.substr(pos + 1);
                double amount = stod(amountStr);
                balance -= amount;
                cout << "Undid deposit of $" << amount << endl;
            }
        }
        else if (lastTransaction.find("Withdraw") != string::npos) {
            size_t pos = lastTransaction.find(":");
            if (pos != string::npos) {
                string amountStr = lastTransaction.substr(pos + 1);
                double amount = stod(amountStr);
                balance += amount;
                cout << "Undid withdrawal of $" << amount << endl;
            }
        }
        else if (lastTransaction.find("Transfer") != string::npos) {
            printWarning("Transfer transactions cannot be automatically undone.");
            transactionStack.push(lastTransaction); 
            return;
        }

        auto it = find(transactionHistory.begin(), transactionHistory.end(), lastTransaction);
        if (it != transactionHistory.end()) {
            transactionHistory.erase(it);
        }

        printSuccess("Transaction undone successfully!");
        cout << "Current Balance: $" << balance << endl;
        savetoFile();
    }
    catch (const out_of_range& e) {
        printWarning("No transactions to undo!");
    }
}

void bankAccount::changePassword() {
    string currentPassword, newPassword, confirmPassword;

    cout << "Enter current password: ";
    getline(cin, currentPassword);

    if (currentPassword != accountPassword) {
        printError("Incorrect current password!");
        return;
    }

    cout << "Enter new password: ";
    getline(cin, newPassword);

    cout << "Confirm new password: ";
    getline(cin, confirmPassword);

    if (newPassword != confirmPassword) {
        printError("Passwords do not match!");
        return;
    }

    if (newPassword.length() < 8) {
        printError("Password must be at least 8 characters long!");
        return;
    }

    accountPassword = newPassword;
    savetoFile();
    printSuccess("Password changed successfully!");
}

void bankAccount::transferMoney() {
    string targetAccount;
    double amount;

    cout << "Enter target account number: ";
    getline(cin, targetAccount);

    if (targetAccount == accountNumber) {
        printError("Cannot transfer to your own account!");
        return;
    }

    bankAccount targetAcc(targetAccount);
    if (!targetAcc.accountFile.fileExists()) {
        printError("Target account not found!");
        return;
    }

    cout << "Enter amount to transfer: $";
    cin >> amount;
    cin.ignore();

    if (amount <= 0) {
        printError("Invalid amount!");
        return;
    }

    if (amount > balance) {
        printError("Insufficient balance!");
        return;
    }

    balance -= amount;
    string transaction = "Transfer to " + targetAccount + ": " + to_string(amount);
    transactionHistory.push_back(transaction);
    transactionStack.push(transaction);


    printSuccess("Transfer initiated successfully!");
    cout << "New Balance: $" << balance << endl;

    savetoFile();
}

void bankAccount::mainMenu() {
    int choice;

    do {
        printHeader("BANK ACCOUNT DASHBOARD");
        cout << "Welcome, " << accountHolderName << "!" << endl;
        cout << "Account: " << accountNumber << " | Balance: $" << balance << endl;
        cout << "------------------------------------------------------" << endl;

        vector<string> options = {
            "Deposit Money",
            "Withdraw Money",
            "Check Balance",
            "Account Details",
            "Transaction History",
            "Transfer Money",
            "Change Password",
            "Undo Last Transaction",
            "Logout"
        };
        printMenu(options);

        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();
        clearScreen();

        switch (choice) {
        case 1: depositMoney(); break;
        case 2: withdrawMoney(); break;
        case 3: checkBalance(); break;
        case 4: accountDetails(); break;
        case 5: allTransactionHistory(); break;
        case 6: transferMoney(); break;
        case 7: changePassword(); break;
        case 8: undoLastTransaction(); break;
        case 9:
            printSuccess("Logging out...!");
            savetoFile();
            break;
        default:
            printError("Invalid choice. Please try again.");
        }

        if (choice != 9) pauseScreen();
    } while (choice != 9);
}

void bankAccount::createAccount() {
    printHeader("CREATE NEW ACCOUNT");

    cout << "Enter Your Full Name   : ";
    getline(cin, accountHolderName);
    cout << "Enter Email Id         : ";
    getline(cin, email);
    cout << "Enter Phone Number     : ";
    getline(cin, phoneNumber);
    cout << "Enter Addhar Number    : ";
    getline(cin, addharNumber);

    string password;
    while (true) {
        cout << "Enter Account Password : ";
        getline(cin, password);

        displayPasswordStrength(password);

        if (passwordStrengthChecker(password) == "Very Weak" || passwordStrengthChecker(password) == "Weak") {
            printWarning("Please choose a stronger password!");
            cout << "Would you like to use a generated strong password? (y/n): ";
            char choice;
            cin >> choice;
            cin.ignore();
            if (choice == 'y' || choice == 'Y') {
                password = passwordGenerator(12, true, true);
                cout << "Generated Password: " << password << endl;
                break;
            }
        }
        else {
            break;
        }
    }
    accountPassword = password;

    balance = 500.0; 
    accountNumber = accountGenerator.generateAccountNumber();
    accountFile = SecureFileHandler(accountNumber + ".dat");

    string initialTransaction = "Account Created with deposit: " + to_string(balance);
    transactionHistory.push_back(initialTransaction);
    transactionStack.push(initialTransaction);

    allAccounts.insert(accountNumber, accountHolderName, balance, email, phoneNumber);

    savetoFile();

    printSuccess("Account Created Successfully!");
    cout << "+------------------------------------------------------+" << endl;
    cout << "¦  Account Number   : " << setw(35) << left << accountNumber << " ¦" << endl;
    cout << "¦  Account Holder   : " << setw(35) << left << accountHolderName << " ¦" << endl;
    cout << "¦  Initial Balance  : $" << setw(33) << left << fixed << setprecision(2) << balance << " ¦" << endl;
    cout << "+------------------------------------------------------+" << endl;

    pauseScreen();
}

void bankAccount::loginBankAccount() {
    printHeader("ACCOUNT LOGIN");

    string enteredAccountNumber, enteredPassword;

    cout << "Enter Account Number   : ";
    getline(cin, enteredAccountNumber);

    SecureFileHandler testFile(enteredAccountNumber + ".dat");
    if (!testFile.fileExists()) {
        printError("Account not found!");
        pauseScreen();
        return;
    }

    accountNumber = enteredAccountNumber;
    accountFile = SecureFileHandler(enteredAccountNumber + ".dat");
    loadFromFile();

    cout << "Enter Account Password : ";
    getline(cin, enteredPassword);

    if (accountPassword == enteredPassword) {
        loadingAnimation("Logging in");
        printSuccess("Login Successful!");
        mainMenu();
    }
    else {
        printError("Invalid Password!");
    }
    pauseScreen();
}

void bankAccount::depositMoney() {
    printHeader("DEPOSIT MONEY");

    double amount;
    cout << "Current Balance: $" << balance << endl;
    cout << "Enter Amount to Deposit : $";
    cin >> amount;
    cin.ignore();

    if (amount > 0) {
        balance += amount;
        string transaction = "Deposit: " + to_string(amount);
        transactionHistory.push_back(transaction);
        transactionStack.push(transaction);

        printSuccess("Deposit Successful!");
        cout << "New Balance: $" << balance << endl;
        savetoFile();
    }
    else {
        printError("Invalid Amount!");
    }
}

void bankAccount::withdrawMoney() {
    printHeader("WITHDRAW MONEY");

    double amount;
    cout << "Current Balance: $" << balance << endl;
    cout << "Enter Amount to Withdraw : $";
    cin >> amount;
    cin.ignore();

    if (amount > 0 && amount <= balance) {
        balance -= amount;
        string transaction = "Withdraw: " + to_string(amount);
        transactionHistory.push_back(transaction);
        transactionStack.push(transaction);

        printSuccess("Withdrawal Successful!");
        cout << "New Balance: $" << balance << endl;
        savetoFile();
    }
    else {
        printError(amount <= 0 ? "Invalid Amount!" : "Insufficient Balance!");
    }
}

void bankAccount::checkBalance() {
    printHeader("ACCOUNT BALANCE");

    cout << "Account Holder: " << accountHolderName << endl;
    cout << "Account Number: " << accountNumber << endl;
    cout << "------------------------------------------------------" << endl;
    cout << Green() << Bold() << "  Current Balance: $" << fixed << setprecision(2) << balance << Reset() << endl;
    cout << "------------------------------------------------------" << endl;
}

void bankAccount::accountDetails() {
    printHeader("ACCOUNT DETAILS");

    cout << "+------------------------------------------------------+" << endl;
    cout << "¦                   ACCOUNT INFORMATION                ¦" << endl;
    cout << "+------------------------------------------------------¦" << endl;
    cout << "¦  Bank Name          : " << setw(35) << left << bankName << " ¦" << endl;
    cout << "¦  Account Holder     : " << setw(35) << left << accountHolderName << " ¦" << endl;
    cout << "¦  Account Number     : " << setw(35) << left << accountNumber << " ¦" << endl;
    cout << "¦  Email Address      : " << setw(35) << left << email << " ¦" << endl;
    cout << "¦  Phone Number       : " << setw(35) << left << phoneNumber << " ¦" << endl;
    cout << "¦  Addhar Number      : " << setw(35) << left << addharNumber << " ¦" << endl;
    cout << "¦  Current Balance    : $" << setw(33) << left << fixed << setprecision(2) << balance << " ¦" << endl;
    cout << "+------------------------------------------------------+" << endl;
}

void bankAccount::allTransactionHistory() {
    printHeader("TRANSACTION HISTORY");

    cout << "Account Holder: " << accountHolderName << endl;
    cout << "Account Number: " << accountNumber << endl;
    cout << "------------------------------------------------------" << endl;

    if (transactionHistory.empty()) {
        printInfo("No transactions found.");
        return;
    }

    transactionStack.displayRecent(10);
}

class ATM : public bankAccount {
private:
    string atmCardNumber;
    string atmPin;
    string AccountNumber;
    static ATMQueue<string> atmQueue;

public:
    void MainMenuATM();
    void checkATMCard();
    void createATMCard();
    static void atmQueueSimulation();
    void quickCash();
};

ATMQueue<string> ATM::atmQueue;

void ATM::MainMenuATM() {
    int choice;
    loadFromFile();

    do {
        printHeader("ATM SERVICES");
        cout << "Card: " << atmCardNumber << " | Account: " << accountNumber << endl;
        cout << "------------------------------------------------------" << endl;

        vector<string> options = {
            "Check Balance",
            "Withdraw Money",
            "Deposit Money",
            "Quick Cash",
            "Transaction History",
            "Change PIN",
            "ATM Queue Simulation",
            "Logout"
        };
        printMenu(options);

        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();
        clearScreen();

        switch (choice) {
        case 1: checkBalance(); break;
        case 2: withdrawMoney(); break;
        case 3: depositMoney(); break;
        case 4: quickCash(); break;
        case 5: allTransactionHistory(); break;
        case 6: changePassword(); break;
        case 7: atmQueueSimulation(); break;
        case 8:
            printSuccess("Thank you for using ATM services!");
            savetoFile();
            break;
        default:
            printError("Invalid choice!");
        }

        if (choice != 8) pauseScreen();
    } while (choice != 8);
}

void ATM::quickCash() {
    printHeader("QUICK CASH");

    vector<double> amounts = { 100, 200, 500, 1000, 2000 };

    cout << "Select amount to withdraw:" << endl;
    for (int i = 0; i < amounts.size(); i++) {
        cout << "  " << i + 1 << ". $" << amounts[i] << endl;
    }
    cout << "  6. Custom Amount" << endl;

    int choice;
    cout << "Enter choice: ";
    cin >> choice;
    cin.ignore();

    double amount = 0;
    if (choice >= 1 && choice <= 5) {
        amount = amounts[choice - 1];
    }
    else if (choice == 6) {
        cout << "Enter custom amount: $";
        cin >> amount;
        cin.ignore();
    }
    else {
        printError("Invalid choice!");
        return;
    }

    if (amount > 0 && amount <= getBalance()) {
        balance -= amount;
        string transaction = "ATM Withdraw: " + to_string(amount);
        transactionHistory.push_back(transaction);
        transactionStack.push(transaction);

        printSuccess("Cash dispensed successfully!");
        cout << "Amount: $" << amount << endl;
        cout << "Remaining Balance: $" << balance << endl;
        savetoFile();
    }
    else {
        printError(amount <= 0 ? "Invalid amount!" : "Insufficient balance!");
    }
}

void ATM::atmQueueSimulation() {
    int choice;
    string customerName;

    do {
        printHeader("ATM QUEUE SIMULATION");
        atmQueue.displayQueue();
        cout << endl;

        vector<string> options = {
            "Join ATM Queue",
            "Serve Next Customer",
            "View Current Queue",
            "Back to ATM Menu"
        };
        printMenu(options);

        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();
        clearScreen();

        switch (choice) {
        case 1:
            cout << "Enter customer name: ";
            getline(cin, customerName);
            atmQueue.enqueue(customerName);
            break;
        case 2:
            try {
                customerName = atmQueue.dequeue();
                printSuccess("Now serving: " + customerName);
                loadingAnimation("Processing transaction for " + customerName);
                printSuccess("Transaction completed for " + customerName);
            }
            catch (const out_of_range& e) {
                printWarning("No customers in queue!");
            }
            break;
        case 3:
            atmQueue.displayQueue();
            break;
        case 4:
            printInfo("Returning to ATM menu...");
            break;
        default:
            printError("Invalid choice!");
        }

        if (choice != 4) pauseScreen();
    } while (choice != 4);
}

void ATM::checkATMCard() {
    printHeader("ATM CARD LOGIN");

    string cardNumber, pin;
    cout << "Enter ATM Card Number: ";
    getline(cin, cardNumber);
    cout << "Enter ATM Card PIN: ";
    getline(cin, pin);

    SecureFileHandler file(cardNumber + ".dat");
    if (file.fileExists()) {
        vector<string> cardData = file.readEncryptedData();
        if (cardData.size() >= 5) {
            atmCardNumber = cardData[0];
            AccountNumber = cardData[1];
            string storedPin = cardData[4];

            if (atmCardNumber == cardNumber && storedPin == pin) {
                accountNumber = AccountNumber;
                accountFile = SecureFileHandler(accountNumber + ".dat");
                loadingAnimation("Verifying card");
                printSuccess("Card verified successfully!");
                MainMenuATM();
            }
            else {
                printError("Invalid ATM Card Number or PIN!");
            }
        }
    }
    else {
        printError("Invalid ATM Card Number or PIN!");
    }
}

void ATM::createATMCard() {
    printHeader("CREATE ATM CARD");

    string accNumber;
    cout << "Enter your Account Number: ";
    getline(cin, accNumber);

    SecureFileHandler accFile(accNumber + ".dat");
    if (!accFile.fileExists()) {
        printError("Account not found! Please create a bank account first.");
        pauseScreen();
        return;
    }

    atmCardNumber = "ATMCARD" + OTPGenerator(8);
    atmPin = OTPGenerator(4);
    AccountNumber = accNumber;

    vector<string> cardData;
    cardData.push_back(atmCardNumber);
    cardData.push_back(AccountNumber);
    cardData.push_back("12/27"); 
    cardData.push_back(OTPGenerator(3)); 
    cardData.push_back(atmPin);

    SecureFileHandler atmFile(atmCardNumber + ".dat");
    if (atmFile.writeEncryptedData(cardData)) {
        printSuccess("ATM Card Created Successfully!");
        cout << "+------------------------------------------------------+" << endl;
        cout << "¦  ATM Card Number : " << setw(35) << left << atmCardNumber << " ¦" << endl;
        cout << "¦  PIN             : " << setw(35) << left << atmPin << " ¦" << endl;
        cout << "¦  Expiry Date     : " << setw(35) << left << "12/27" << " ¦" << endl;
        cout << "¦  CVV             : " << setw(35) << left << cardData[3] << " ¦" << endl;
        cout << "¦  Linked Account  : " << setw(35) << left << AccountNumber << " ¦" << endl;
        cout << "+------------------------------------------------------+" << endl;
        printWarning("Keep your PIN and CVV confidential!");
    }
    else {
        printError("Error creating ATM card file!");
    }
    pauseScreen();
}

class DataStructuresDemo {
public:
    static void stackDemo() {
        printHeader("STACK DEMO - TRANSACTION HISTORY");

        TransactionStack<string> demoStack;
        demoStack.push("Deposit: 1000.00");
        demoStack.push("Withdraw: 500.00");
        demoStack.push("Deposit: 2000.00");
        demoStack.push("Transfer: 300.00 to ACC123");
        demoStack.push("Withdraw: 150.00");

        cout << "Initial transaction stack:" << endl;
        demoStack.displayRecent();

        cout << "\nPopping last transaction..." << endl;
        try {
            string last = demoStack.pop();
            printSuccess("Removed: " + last);
        }
        catch (const out_of_range& e) {
            printError(e.what());
        }

        cout << "\nStack after pop operation:" << endl;
        demoStack.displayRecent();
    }

    static void queueDemo() {
        printHeader("QUEUE DEMO - ATM SERVICE");

        ATMQueue<string> demoQueue;
        demoQueue.enqueue("Customer: John Doe");
        demoQueue.enqueue("Customer: Jane Smith");
        demoQueue.enqueue("Customer: Bob Johnson");
        demoQueue.enqueue("Customer: Alice Brown");

        demoQueue.displayQueue();

        cout << "\nServing customers..." << endl;
        try {
            string customer = demoQueue.dequeue();
            printSuccess("Now serving: " + customer);

            customer = demoQueue.dequeue();
            printSuccess("Now serving: " + customer);
        }
        catch (const out_of_range& e) {
            printError(e.what());
        }

        cout << "\nQueue after serving 2 customers:" << endl;
        demoQueue.displayQueue();
    }

    static void linkedListDemo() {
        printHeader("LINKED LIST DEMO - CUSTOMER RECORDS");

        CustomerLinkedList demoList;
        demoList.insert("TCBOI1001", "Talha Farooq", 1500.75, "Talha@email.com", "123-456-7890");
        demoList.insert("TCBOI1002", "Ali", 2750.50, "Ali@email.com", "1234-567-890");
        demoList.insert("TCBOI1003", "Areeb Ahmed", 4200.25, "Areeb@email.com", "12345-678-90");
        demoList.insert("TCBOI1004", "zain salam", 1890.00, "zain.salam@email.com", "456-789-0123");

        demoList.displayAll();

        cout << "\nRemoving account TCBOI1002..." << endl;
        if (demoList.remove("TCBOI1002")) {
            printSuccess("Account removed successfully!");
        }
        else {
            printError("Account not found!");
        }

        cout << "\nUpdated customer list:" << endl;
        demoList.displayAll();
    }

    static void showAllDemos() {
        stackDemo();
        pauseScreen();
        queueDemo();
        pauseScreen();
        linkedListDemo();
    }
};
class SecurityTools {
public:
    static void showMenu() {
        int choice;
        do {
            printHeader("SECURITY TOOLS");
            vector<string> options = {
                "Password Generator",
                "Password Strength Checker",
                "OTP Generator",
                "Encryption/Decryption Demo",
                "Back to Main Menu"
            };
            printMenu(options);

            cout << "Enter your choice: ";
            cin >> choice;
            cin.ignore();
            clearScreen();

            switch (choice) {
            case 1: passwordGeneratorTool(); break;
            case 2: passwordStrengthTool(); break;
            case 3: otpGeneratorTool(); break;
            case 4: encryptionDemo(); break;
            case 5: printInfo("Returning to main menu..."); break;
            default: printError("Invalid choice!");
            }

            if (choice != 5) pauseScreen();
        } while (choice != 5);
    }

private:
    static void passwordGeneratorTool() {
        printHeader("PASSWORD GENERATOR");

        int length;
        bool includeNumbers, includeSymbols;

        cout << "Enter password length: ";
        cin >> length;
        cout << "Include numbers? (y/n): ";
        char choice;
        cin >> choice;
        includeNumbers = (choice == 'y' || choice == 'Y');
        cout << "Include symbols? (y/n): ";
        cin >> choice;
        includeSymbols = (choice == 'y' || choice == 'Y');
        cin.ignore();

        string password = passwordGenerator(length, includeNumbers, includeSymbols);

        cout << "\nGenerated Password: " << Green() << Bold() << password << Reset() << endl;

        displayPasswordStrength(password);

        string encrypted = AdvancedEncryption::encrypt(password);
        cout << "Encrypted version: " << Cyan() << encrypted << Reset() << endl;

        string decrypted = AdvancedEncryption::decrypt(encrypted);
        cout << "Decrypted version: " << Green() << decrypted << Reset() << endl;
    }

    static void passwordStrengthTool() {
        printHeader("PASSWORD STRENGTH CHECKER");

        string password;
        cout << "Enter password to analyze: ";
        getline(cin, password);

        displayPasswordStrength(password);

        cout << "\nDetailed Analysis:" << endl;
        cout << "• Length: " << password.length() << " characters" << endl;
        cout << "• Contains uppercase: " << (any_of(password.begin(), password.end(), ::isupper) ? "Yes" : "No") << endl;
        cout << "• Contains lowercase: " << (any_of(password.begin(), password.end(), ::islower) ? "Yes" : "No") << endl;
        cout << "• Contains digits: " << (any_of(password.begin(), password.end(), ::isdigit) ? "Yes" : "No") << endl;
        cout << "• Contains symbols: " << (any_of(password.begin(), password.end(), [](char c) {
            return !isalnum(c); }) ? "Yes" : "No") << endl;
    }

    static void otpGeneratorTool() {
        printHeader("OTP GENERATOR");

        int size;
        cout << "Enter OTP length (4 or 6): ";
        cin >> size;
        cin.ignore();

        if (size == 4 || size == 6) {
            string otp = OTPGenerator(size);
            cout << "Generated OTP: " << Green() << Bold() << otp << Reset() << endl;

            cout << "This OTP will expire in 5 minutes." << endl;
        }
        else {
            printError("OTP length must be 4 or 6 digits!");
        }
    }

    static void encryptionDemo() {
        printHeader("ENCRYPTION/DECRYPTION DEMO");

        string text;
        cout << "Enter text to encrypt: ";
        getline(cin, text);

        string encrypted = AdvancedEncryption::encrypt(text);
        string decrypted = AdvancedEncryption::decrypt(encrypted);

        cout << "\nOriginal Text: " << text << endl;
        cout << "Encrypted: " << encrypted << endl;
        cout << "Decrypted: " << decrypted << endl;

        cout << "\nFile Encryption Demo:" << endl;
        string fileEncrypted = AdvancedEncryption::encryptForFile(text);
        string fileDecrypted = AdvancedEncryption::decryptFromFile(fileEncrypted);

        cout << "File Encrypted: " << fileEncrypted << endl;
        cout << "File Decrypted: " << fileDecrypted << endl;
    }
};
class BankingSystem {
private:
    Admin admin;

public:
    void showMainMenu() {
        int choice;
        do {
            printHeader("ENHANCED BANKING SYSTEM");
            cout << "Welcome to  Bank of Talha Farooq (BOTF)" << endl;
            cout << "Your Trusted Banking Partner" << endl;
            cout << "------------------------------------------------------" << endl;

            vector<string> options = {
                "Bank Account Management",
                "ATM Services",
                "Password Manager",
                "Security Tools",
                "Data Structures Demo",
                "Admin Panel",
                "System Information",
                "Exit"
            };
            printMenu(options);

            cout << "Enter your choice: ";
            cin >> choice;
            cin.ignore();
            clearScreen();

            switch (choice) {
            case 1: bankAccountMenu(); break;
            case 2: atmMenu(); break;
            case 3: passwordManagerMenu(); break;
            case 4: securityToolsMenu(); break;
            case 5: dataStructuresDemoMenu(); break;
            case 6: adminPanel(); break;
            case 7: systemInfo(); break;
            case 8:
                printSuccess("Thank you for using Enhanced Banking System!");
                cout << "Have a great day! " << endl;
                break;
            default:
                printError("Invalid choice. Please try again.");
            }
        } while (choice != 8);
    }

private:
    void bankAccountMenu() {
        bankAccount account;
        int choice;
        do {
            printHeader("BANK ACCOUNT MANAGEMENT");
            cout << "Total Accounts in System: " << allAccounts.getSize() << endl;
            cout << "------------------------------------------------------" << endl;

            vector<string> options = {
                "Create New Account",
                "Login to Account",
                "Display All Accounts",
                "Back to Main Menu"
            };
            printMenu(options);

            cout << "Enter your choice: ";
            cin >> choice;
            cin.ignore();
            clearScreen();

            switch (choice) {
            case 1:
                account.createAccount();
                break;
            case 2:
                account.loginBankAccount();
                break;
            case 3:
                allAccounts.displayAll();
                pauseScreen();
                break;
            case 4:
                printInfo("Returning to main menu...");
                break;
            default:
                printError("Invalid choice.");
            }
        } while (choice != 4);
    }

    void atmMenu() {
        ATM atm;
        int choice;
        do {
            printHeader("ATM SERVICES");
            vector<string> options = {
                "Use ATM Card",
                "Create ATM Card",
                "Back to Main Menu"
            };
            printMenu(options);

            cout << "Enter your choice: ";
            cin >> choice;
            cin.ignore();
            clearScreen();

            switch (choice) {
            case 1: atm.checkATMCard(); break;
            case 2: atm.createATMCard(); break;
            case 3: printInfo("Returning to main menu..."); break;
            default: printError("Invalid choice.");
            }
        } while (choice != 3);
    }

    void passwordManagerMenu() {
        printHeader("PASSWORD MANAGER");
        printInfo("Password Manager feature is available in the enhanced version.");
        printInfo("It includes secure storage with encryption.");
        pauseScreen();
    }

    void securityToolsMenu() {
        SecurityTools::showMenu();
    }

    void dataStructuresDemoMenu() {
        int choice;
        do {
            printHeader("DATA STRUCTURES DEMO");
            vector<string> options = {
                "Stack Demo (Transaction History)",
                "Queue Demo (ATM Service)",
                "Linked List Demo (Customer Records)",
                "All Demos",
                "Back to Main Menu"
            };
            printMenu(options);

            cout << "Enter your choice: ";
            cin >> choice;
            cin.ignore();
            clearScreen();

            switch (choice) {
            case 1:
                DataStructuresDemo::stackDemo();
                break;
            case 2:
                DataStructuresDemo::queueDemo();
                break;
            case 3:
                DataStructuresDemo::linkedListDemo();
                break;
            case 4:
                DataStructuresDemo::showAllDemos();
                break;
            case 5:
                printInfo("Returning to main menu...");
                break;
            default:
                printError("Invalid choice.");
            }

            if (choice != 5) pauseScreen();
        } while (choice != 5);
    }

    void adminPanel() {
        if (!admin.authenticate()) {
            pauseScreen();
            return;
        }

        int choice;
        do {
            printHeader("ADMIN PANEL");
            vector<string> options = {
                "View All Encrypted Data",
                "View Decrypted Account Data",
                "System Statistics",
                "Change Admin Password",
                "Back to Main Menu"
            };
            printMenu(options);

            cout << "Enter your choice: ";
            cin >> choice;
            cin.ignore();
            clearScreen();

            switch (choice) {
            case 1:
                admin.viewAllData();
                break;
            case 2:
                admin.viewDecryptedData();
                break;
            case 3:
                admin.systemStatistics();
                break;
            case 4:
                admin.changePassword();
                break;
            case 5:
                printInfo("Returning to main menu...");
                break;
            default:
                printError("Invalid choice.");
            }

            if (choice != 5) pauseScreen();
        } while (choice != 5);
    }

    void systemInfo() {
        printHeader("SYSTEM INFORMATION");

        cout << "+------------------------------------------------------+" << endl;
        cout << "¦                 SYSTEM OVERVIEW                     ¦" << endl;
        cout << "+------------------------------------------------------¦" << endl;
        cout << "¦  Bank Name         :  Bank of Talha Farooq           ¦" << endl;
        cout << "¦  System Version    : 3.2 Secure File System        ¦" << endl;
        cout << "¦  Developed By      : Talha Codes                    ¦" << endl;
        cout << "¦  Total Accounts    : " << setw(30) << left << allAccounts.getSize() << " ¦" << endl;
        cout << "¦  File System       : Encrypted Data Storage        ¦" << endl;
        cout << "¦  Account Numbers   : Unique & Sequential           ¦" << endl;
        cout << "¦  Security Features : Multi-layer Encryption        ¦" << endl;
        cout << "¦  Admin Access      : Secure Authentication         ¦" << endl;
        cout << "+------------------------------------------------------+" << endl;

        cout << "\n+------------------------------------------------------+" << endl;
        cout << "¦                 SECURITY FEATURES                    ¦" << endl;
        cout << "+------------------------------------------------------¦" << endl;
        cout << "¦    Advanced Multi-layer Encryption                  ¦" << endl;
        cout << "¦    Secure File Handling                             ¦" << endl;
        cout << "¦    Unique Account Number Generation                 ¦" << endl;
        cout << "¦    Encrypted Transaction History                    ¦" << endl;
        cout << "¦    Admin-Only Data Access                           ¦" << endl;
        cout << "¦    Password Strength Enforcement                    ¦" << endl;
        cout << "¦    Secure ATM Card Management                       ¦" << endl;
        cout << "¦    Real-time Data Encryption/Decryption             ¦" << endl;
        cout << "+------------------------------------------------------+" << endl;

        pauseScreen();
    }
};

int main() {
    srand(time(0));

    clearScreen();
    cout << Cyan();
    cout << "+----------------------------------------------------------+" << endl;
    cout << "¦                                                          ¦" << endl;
    cout << "¦          WELCOME TO SECURE BANKING SYSTEM                ¦" << endl;
    cout << "¦                                                          ¦" << endl;
    cout << "¦         Advanced File Handling & Encryption              ¦" << endl;
    cout << "¦                                                          ¦" << endl;
    cout << "+----------------------------------------------------------+" << endl;
    cout << Reset();

    loadingAnimation("Initializing Secure System");

    BankingSystem bankingSystem;
    bankingSystem.showMainMenu();

    return 0;
}
