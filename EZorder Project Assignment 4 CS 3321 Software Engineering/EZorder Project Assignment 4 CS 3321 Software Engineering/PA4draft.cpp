#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <sstream>

using namespace std;

// Employee class for managing individual employee details
class Employee {
public:
    string name;
    string position;
    int pin;
    double hoursWorked;
    double wage;
    double tips;

    // Default Constructor
    Employee() : name("Unknown"), position("Unknown"), pin(0), wage(0.0), hoursWorked(0.0), tips(0.0) {}

    // Parameter Constructor
    Employee(string name, string position, int pin, double wage)
        : name(name), position(position), pin(pin), wage(wage), hoursWorked(0), tips(0) {}

    void addHours(double hours) {
        hoursWorked += hours;
    }

    void changePosition(string newPosition) {
        position = newPosition;
    }

    void display() const {
        cout << "Name: " << name << ", Position: " << position
            << ", Hours Worked: " << hoursWorked << ", Wage: $" << wage
            << ", Tips: $" << tips << endl;
    }
};

// Floor class for managing the floor layout
class Table {
public:
    int tableNumber;
    string server;
    double orderTotal;
    int partySize;
    bool isOccupied;

    Table(int number) : tableNumber(number), isOccupied(false), orderTotal(0), partySize(0), server("") {}

    void occupyTable(string serverName, int size) {
        isOccupied = true;
        server = serverName;
        partySize = size;
    }

    void vacateTable() {
        isOccupied = false;
        server = "";
        orderTotal = 0;
        partySize = 0;
    }

    void updateOrderTotal(double total) {
        orderTotal = total;
    }

    void display() const {
        cout << "Table " << tableNumber << ": "
            << (isOccupied ? "Occupied by " + server : "Unoccupied")
            << ", Order Total: $" << orderTotal << ", Party Size: " << partySize << endl;
    }
};

// Order management
class Order {
public:
    vector<string> items;
    double totalCost;

    Order() : totalCost(0) {}

    void addItem(string item, double price) {
        items.push_back(item);
        totalCost += price;
    }

    void displayOrder() const {
        cout << "Order: ";
        for (const auto& item : items) {
            cout << item << " ";
        }
        cout << "Total: $" << totalCost << endl;
    }
};

// Payment Window class
class PaymentWindow {
public:
    void displayCheck(const Order& order) {
        cout << "Displaying Check: ";
        order.displayOrder();
    }

    void splitCheck() {
        cout << "Check has been split.\n";
    }

    void printCheck() {
        cout << "Printing check...\n";
    }

    void addPaymentMethod() {
        cout << "Payment method added.\n";
    }

    void addTips(double tips) {
        cout << "Tips added: $" << tips << endl;
    }

    void voidOrDiscountItem() {
        cout << "Item voided/discounted.\n";
    }
};

// Database Window class for managing employees, sales, and payroll
class DatabaseWindow {
private:
    map<string, Employee> employees;
    vector<Table> tables;
    double dailySales;

public:
    DatabaseWindow() : dailySales(0) {
        for (int i = 1; i <= 10; ++i) {
            tables.emplace_back(i); // Initialize 10 tables for the restaurant
        }
    }

    void addEmployeeToCSV(const string& name, const string& position, int pin, double wage) {
        const string filename = "employees.csv";
        ofstream outputFile("employees.csv", ios::app);

        // Check if the file opened successfully
        if (outputFile.is_open()) {
            ifstream inFile("employees.csv", ios::ate);
            if (inFile.tellg() == 0) { // File size is 0, so it is empty
                outputFile << "Name,Position,Pin,Wage\n"; // Write header
            }
            inFile.close(); // Close the input stream
        }

        // Check if the file opened successfully
        if (!outputFile.is_open()) {
            cerr << "Error opening file!";
        }

        /* Check if the file is empty and write header if it is
        if (outputFile.tellp() == 0) {
            outputFile << "Name,Position,Pin,Wage\n";
        }*/

        // Append the employee's data in CSV format
        outputFile << name << "," << position << "," << pin << "," << wage << endl;

        // Close the file
        outputFile.close();
    }
    



    // Add employee (admin only)
    void addEmployee(string name, string position, int pin, double wage) {
        if (employees.find(name) == employees.end()) { // Store employee info. into CSV file
            employees[name] = Employee(name, position, pin, wage);


            cout << "Employee added: " << name << endl;
            addEmployeeToCSV(name, position, pin, wage);
        }
        else {
            cout << "Employee already exists." << endl;
        }
    }

    // Remove employee (admin only)
    void removeEmployee(string name) {
        if (employees.erase(name)) {
            cout << "Employee removed: " << name << endl;
        }
        else {
            cout << "Employee not found." << endl;
        }
    }

    // Track time clock (admin can add hours worked for employees)
    void trackTimeClock(string name, double hours) {
        if (employees.find(name) != employees.end()) {
            employees[name].addHours(hours);
            cout << "Hours added to " << name << endl;
        }
        else {
            cout << "Employee not found." << endl;
        }
    }

    // Change position (admin only)
    void changePosition(string name, string newPosition) {
        if (employees.find(name) != employees.end()) {
            employees[name].changePosition(newPosition);
            cout << "Position changed for " << name << " to " << newPosition << endl;
        }
        else {
            cout << "Employee not found." << endl;
        }
    }

    // Display all employees
    void displayEmployees() {
        for (const auto& pair : employees) {
            pair.second.display();
        }
    }

    // Access current day sales report
    void displaySalesReport() {
        cout << "Daily Sales: $" << dailySales << endl;
    }

    // Calculate wages and tips (admin only)
    void calculateWages() {
        cout << "Wages and Tips Report:" << endl;
        for (const auto& pair : employees) {
            double wages = pair.second.hoursWorked * pair.second.wage;
            cout << pair.first << ": $" << wages << " + $" << pair.second.tips << " (tips)" << endl;
        }
    }

    // Simulate adding to daily sales (e.g., orders from customers)
    void addToDailySales(double amount) {
        dailySales += amount;
    }

    // Floor Layout - display table information
    void displayFloorLayout() {
        cout << "Floor Layout:\n";
        for (const auto& table : tables) {
            table.display();
        }
    }

    // Occupy a table
    void occupyTable(int tableNumber, string serverName, int partySize) {
        if (tableNumber > 0 && tableNumber <= tables.size()) {
            tables[tableNumber - 1].occupyTable(serverName, partySize);
        }
        else {
            cout << "Invalid table number.\n";
        }
    }

    // Vacate a table
    void vacateTable(int tableNumber) {
        if (tableNumber > 0 && tableNumber <= tables.size()) {
            tables[tableNumber - 1].vacateTable();
        }
        else {
            cout << "Invalid table number.\n";
        }
    }
};

// Login Window class
class LoginWindow {
private:
    DatabaseWindow& dbWindow;
    map<int, string> loggedInUsers;

public:
    LoginWindow(DatabaseWindow& db) : dbWindow(db) {}

    int login(int pin) {
        if (verifyPin(pin)) {
            cout << "Login successful.\n";
            return true;
        }
        else {
            cout << "Invalid credentials.\n";
            return false;
        }
    }

    int modCode(int pin) {
        string modPc;

        cout << "Enter Passcode: ";
        cin >> modPc;

        if (modPc == "tj93mVvzbghM9wR")
        { // The admin will be taken to the addEmployee function and adds the Managers Information like name, pin, wage, position then closes the system.

            string name;
            string position;
            int pin;
            double wage;

            cout << "What is the name of the employee?" << endl;
            cin >> name;

            cout << "What will be the position?" << endl;
            cin >> position;

            cout << "What is the employees pin number?" << endl;
            cin >> pin;

            cout << "What is their starting wage?" << endl;
            cin >> wage;

            dbWindow.addEmployee(name, position, pin, wage);

            return 0;
        }
    }

    bool verifyPin(int pin) {
        ifstream file("employees.csv");

        // Ensure file is open
        if (!file.is_open()) {
            cerr << "Error: Could not open the file." << endl;
            return false;
        }

        string line, token;
        bool isHeader = true;

        // Read through the CSV file line by line
        while (getline(file, line)) {
            if (isHeader) {
                isHeader = false; //Skip the header row
                continue;
            }

            stringstream ss(line);
            string name, position, pinStr, wageStr;

            // Extarct values from the CSV line
            getline(ss, name, ',');
            getline(ss, position, ',');
            getline(ss, pinStr, ',');
            getline(ss, wageStr, ',');

            if (stoi(pinStr) == pin) {
                cout << "Access granted for employee:" << name << endl;
                return true; // Pin found
            }
        }
        file.close();
        return false; // Pin not found
    }

    void clockIn() {
        cout << "Clocking in...\n";
    }

    void clockOut() {
        cout << "Clocking out...\n";
    }
};

// Main function to simulate the restaurant management system
int main() {
    DatabaseWindow dbWindow;
    LoginWindow loginWindow(dbWindow);
    //PaymentWindow paymentWindow;

    ////////////////////////////////////////////////////////////////////// Start of Login
    int userPin;

    // Prompt user to enter a 4-digit PIN
    cout << "Enter your 4-digit PIN: ";
    cin >> userPin;

    // Ensure the input is a 4-digit number
    if (userPin >= 1000 && userPin <= 9999) {
        if (loginWindow.verifyPin(userPin)) {
            loginWindow.login(userPin);
        }
        else {
            loginWindow.login(userPin);
        }
    }
    else if (userPin == 0000) {
        loginWindow.modCode(userPin);
    }
    else {
        cout << "Invalid input" << endl;
        return 0;
    }
    ////////////////////////////////////////////////////////////////////// End of Login


    // Simulating system operations
    //dbWindow.addEmployee("John", "Server", 1234, 15.50);
    //dbWindow.addEmployee("Alice", "Chef", 5678, 20.00);

    /*if (loginWindow.login(1234)) {
        dbWindow.occupyTable(1, "John", 4);
        dbWindow.displayFloorLayout();

        // Process an order
        Order order;
        order.addItem("Appetizer: Salad", 5.99);
        order.addItem("Entree: Steak", 20.99);
        order.displayOrder();

        // Payment
        paymentWindow.displayCheck(order);
        paymentWindow.addTips(5.00);
        paymentWindow.printCheck();
    }*/

    dbWindow.displayEmployees();
    dbWindow.displaySalesReport();
    dbWindow.calculateWages();

    return 0;
}
