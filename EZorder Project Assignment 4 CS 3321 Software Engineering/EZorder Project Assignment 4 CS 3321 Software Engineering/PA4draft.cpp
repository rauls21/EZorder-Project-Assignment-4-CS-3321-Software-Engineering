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

    void positionPrivilege(string privilege) {
        position = privilege;
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
    bool removeEmployee(string name, int pin) {
        ifstream inputFile("employees.csv");
        ofstream tempFile("temp.csv");
        string line;

        // Ensure the input file is open
        if (!inputFile.is_open()) {
            cerr << "Error: Could not open the file for reading." << endl;
            return false;
        }

        // Ensure the temporary file is open
        if (!tempFile.is_open()) {
            cerr << "Error: Could not open the temporary file for writing." << endl;
            return false;
        }

        bool found = false;

        // Read each line from the original file
        while (getline(inputFile, line)) {
            stringstream ss(line);
            string empName, position, pinStr, wageStr;

            // Extract values from the CSV line
            getline(ss, empName, ',');
            getline(ss, position, ',');
            getline(ss, pinStr, ',');
            getline(ss, wageStr, ',');

            // If the name does not match the employee to be removed, write the line to the temp file
            if (empName != name) {
                tempFile << line << endl;  // Write the original line to the temp file
            }
            else {
                found = true;  // Mark that we found the employee
            }
        }

        // Close the files
        inputFile.close();
        tempFile.close();

        // If the employee was found and removed, replace the original file with the temp file
        if (found) {
            remove("employees.csv");         // Delete the original file
            rename("temp.csv", "employees.csv");  // Rename temp file to original file
            cout << "Employee " << name << " removed successfully." << endl;
            return true;
        }
        else {
            remove("temp.csv");  // No employee found, so remove temp file
            cout << "Employee " << name << " not found." << endl;
            return false;
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
    
    void AddToMenuCSV() {
        vector<string> menuItems;
        vector<double> prices;
        string item;
        double price;
        char choice;

            do {
                cout << "Enter Menu Item: ";
                cin.ignore();
                getline(cin, item);

                cout << "Enter price: ";
                cin >> price;

                menuItems.push_back(item);
                prices.push_back(price);

                cout << "Do you want to add another item? (y/n): ";
                cin >> choice;
            } while (choice == 'y' || choice == 'Y');

            if (menuItems.size() != prices.size()) {
                cerr << "Error: Menu items and prices must have the same number of entries." << endl;
                return;
            }

            ofstream menuFile("menu.csv", ios::app);

            if (menuFile.is_open()) {
                ifstream inFile("menu.csv", ios::ate);
                if (inFile.tellg() == 0) {
                    menuFile << "Menu Item, Price\n";
                }
                inFile.close();
            }

            if (!menuFile.is_open()) {
                cerr << "Error: Could not open file." << endl;
                return;
            }

            for (size_t i = 0; i < menuItems.size(); i++) {
                menuFile << menuItems[i] << "," << prices[i] << "\n";
            }

            menuFile.close();
            cout << "Menu Updated" << endl;
            return;
        }
};

// Login Window class
class LoginWindow {
private:
    DatabaseWindow& dbWindow;
    map<int, string> loggedInUsers;
    Employee user;

public:
    LoginWindow(DatabaseWindow& db) : dbWindow(db) {}

    int login(int pin) {

        if (verifyPin(pin)) {
            cout << "Login successful.\n";
            clockInOutDisplay();
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

            user.positionPrivilege(position);
            if (stoi(pinStr) == pin) {
                cout << "Access granted for employee:" << name << endl;
                return true;
            }

        }
        file.close();
        return false; // Pin not found
    }

    void clockInOutDisplay() {
        int navigate;
        cout << "1. Clock in\n";
        cout << "2. Clock out\n";
        cin >> navigate;
        if (navigate == 1) {
            if (user.position == "Manager") {
                cout << "Clocking manager in...\n";
                managerInterface();
            }
            else if (user.position == "Server") {
                cout << "Clocking Server in...\n";
                serverInterface();
            }
            else {
                cout << "Clocking in...\n";
                return;
            }
        }
        else {
            cout << "Clocking out...\n";
        }
        return;
    }

    void toGoOrder() {
        int navigateToGo;
        cout << "List of To Go Orders: ";
    }

    void orderView() {
        int navigateOrderView;
        cout << "1. Modify Order\n";
        cout << "2. Back\n";
        cin >> navigateOrderView;

        switch (navigateOrderView) {
        case 1:
            cout << "Modifying Order";
            break;
        case 2:
            cout << "Directing back to Table View...\n";
            serverInterface();
            break;
        default:
            cout << "Invalid Input\n";
            orderView();
            break;
        }
    }

    void newOrder() {
        int navigateNewOrder;
        cout << "1. New To-Go Order\n";
        cout << "2. New Table Order\n";
        cout << "3. Back\n";
        cin >> navigateNewOrder;

        switch (navigateNewOrder) {
        case 1:
            cout << "To-Go Order";
            break;
        case 2:
            cout << "Table Order";
            break;
        case 3:
            cout << "Directing back to Table View...\n";
            serverInterface();
            break;
        default:
            cout << "Invalid Input\n";
            newOrder();
            break;
        }
    }

    void serverInterface() {
        //Automatically display table view
        int navigateUserInterface;
        cout << "1. New Order\n";
        cout << "2. View Current Order\n";
        cout << "3. View To-Go Orders\n";
        cin >> navigateUserInterface;

        switch (navigateUserInterface) {
        case 1:
            cout << "Directing to New Order...\n";
            newOrder();
            break;
        case 2:
            cout << "Directing to Order View...\n";
            orderView();
            break;
        case 3:
            cout << "Directing to To-Go Orders...\n";
            toGoOrder();
            break;
        default:
            cout << "Invalid Input\n";
            serverInterface();
            break;
        }
    }

    void editEmpInterface() {
        int editEmpNav;
        string name;
        string position;
        int pin;
        double wage;

        cout << "1. Edit Employee Information\n";
        cout << "2. Add Employee\n";
        cout << "3. Remove Employee\n";
        cout << "4. Back\n";
        cin >> editEmpNav;

        switch (editEmpNav) {
        case 1:
            cout << "Directing to Edit Employee Information...\n";
            break;
        case 2:
            cout << "Directing to Add Employee...\n";

            cout << "What is the name of the employee?" << endl;
            cin >> name;

            cout << "What will be the position?" << endl;
            cin >> position;

            cout << "What is the employees pin number?" << endl;
            cin >> pin;

            cout << "What is their starting wage?" << endl;
            cin >> wage;

            dbWindow.addEmployee(name, position, pin, wage);

            cout << "Directing to Table View...\n";
            managerInterface();
            break;
        case 3:
            cout << "Directing to Remove Employee...\n";

            cout << "Enter the name of the employee to remove: " << endl;
            cin >> name;

            cout << "Enter the pin number of that employee: " << endl;
            cin >> pin;

            dbWindow.removeEmployee(name, pin);

            cout << "Directing to Table View...\n";
            managerInterface();
            break;
        case 4:
            cout << "Directing to Table View...\n";
            managerInterface();
            break;
        default:
            cout << "Invalid Input\n";
            editEmpInterface();
            break;
        }
    }

    void managerInterface() {
        int navigate;
        cout << "1. Edit Menu\n";
        cout << "2. Edit Employee\n";
        cout << "3. View Current Orders\n";
        cout << "4. Start New Order\n";
        cout << "5. View To-Go Orders\n";
        cout << "6. Transfer Table\n";
        cout << "7. Calculate Wage\n";
        cout << "8. Logout\n";
        cin >> navigate;

        switch (navigate) {
        case 1:            //Table
            cout << "Directing to Edit Menu...\n";
            dbWindow.AddToMenuCSV();

            cout << "Directing back to Table View...\n";
            managerInterface();
            break;
        case 2://Orders
            cout << "Directing to Edit Employee...\n";
            editEmpInterface();
            break;
        case 3://Transfer
            cout << "3";
            break;
        case 4:
            cout << "4";
            break;
        case 5:
            cout << "5";
            break;
        case 6:            //Admin edit
            cout << "6";
            break;
        case 7:
            cout << "7";
            break;
        case 8:
            cout << "8";
            return;
            break;
        default:
            cout << "Invalid Input\n";
            managerInterface();
            break;
        }
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
        loginWindow.login(userPin);
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
    //dbWindow.displaySalesReport();
    //dbWindow.calculateWages();

    return 0;
}
