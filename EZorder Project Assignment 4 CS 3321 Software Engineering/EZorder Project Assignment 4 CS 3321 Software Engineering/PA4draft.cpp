#include <iostream>
#include <vector>
#include <map>
#include <string>

using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////////// Employee class for managing individual employee details
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// End of Employee Class

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Floor class for managing the floor layout
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// End of Floor Class

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Order management
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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// End of Order Class

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Payment Window class
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// End of Payment Class

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Database Window class for managing employees, sales, and payroll
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

    // Add employee (admin only)
    void addEmployee(string name, string position, int pin, double wage) {
        if (employees.find(name) == employees.end()) {
            employees[name] = Employee(name, position, pin, wage);
            cout << "Employee added: " << name << endl;
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// End of Database Window Class

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Login Window class
class LoginWindow {
private:
    DatabaseWindow& dbWindow;
    map<int, string> loggedInUsers;

public:
    LoginWindow(DatabaseWindow& db) : dbWindow(db) {}

    bool login(int pin) {
        if (verifyPin(pin)) {
            cout << "Login successful.\n";
            return true;
        }
        else {
            cout << "Invalid credentials.\n";
            return false;
        }
    }

    bool verifyPin(int pin) {
        // For simplicity, pin verification is mocked here
        return pin == 1234;  // Assume 1234 is the valid pin for now
    }

    void clockIn() {
        cout << "Clocking in...\n";
    }

    void clockOut() {
        cout << "Clocking out...\n";
    }
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// End of Login Window Class

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Main function to simulate the restaurant management system
int main() {
    DatabaseWindow dbWindow;
    LoginWindow loginWindow(dbWindow);
    PaymentWindow paymentWindow;

    // Simulating system operations
    dbWindow.addEmployee("John", "Server", 1234, 15.50);
    dbWindow.addEmployee("Alice", "Chef", 5678, 20.00);

    if (loginWindow.login(1234)) {
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
    }

    dbWindow.displayEmployees();
    dbWindow.displaySalesReport();
    dbWindow.calculateWages();

    return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// End of Main Function
