#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <iomanip>

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

    void createOrderCSV(int orderNum) {
        const string menuFile = "menu.csv";


        map<string, double> menu = loadMenu(menuFile);
        if (menu.empty()) {
            cerr << "Error: Menu is empty or could not be loaded." << endl;
            return;
        }

        string menuItem;
        double price = 0.0;

        //Create file name with orderNum
        string orderFile = "Order_" + to_string(orderNum) + ".csv";

        //Create & Open To-Go File
        ofstream csvFile(orderFile, ios::app);

        //Check if file opened successfully
        if (csvFile.is_open()) {
            ifstream inFile(orderFile, ios::ate);
            if (inFile.tellg() == 0) {
                //Write header and initial details
                csvFile << "Menu Item, Price\n";
            }
            inFile.close();
        }

        //If file did not open
        if (!csvFile.is_open()) {
            cerr << "Error opening file!";
        }

        cin.ignore();
        while (true) {
            cout << "Enter Menu Item (type 'done' to exit): ";
            getline(cin, menuItem);

            if (menuItem == "done") {
                break;
            }

            //Find the price of the menu item
            auto it = menu.find(menuItem);
            if (it != menu.end()) {
                price = it->second;
                
                //Append the data to the file
                csvFile << menuItem << "," << price << endl;
                cout << menuItem << " addded to order.\n";
            }
            else {
                cerr << "Error: Menu item not found." << endl;
            }
        }

        //Close File
        csvFile.close();

    }

    map<string, double> loadMenu(const string& menuFile) {
        map<string, double> menu;
        ifstream menuStream(menuFile);

        if (!menuStream.is_open()) {
            cerr << "Error: Could not open menu.csv" << endl;
            return menu;
        }

        string line;
        bool isFirstLine = true;
        while (getline(menuStream, line)) {
            if (isFirstLine) {
                isFirstLine = false;
                continue; // Skip header
            }

            size_t commaPos = line.find(',');
            if (commaPos != string::npos) {
                string menuItem = line.substr(0, commaPos);
                double price = stod(line.substr(commaPos + 1));
                menu[menuItem] = price;
            }
        }

        menuStream.close();
        return menu;
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

    void displayMenu() {
        ifstream menu("menu.csv");
        if (!menu.is_open()) {
            cerr << "Error: Could not open menu file";
            return;
        }

        string line;
        cout << left << setw(15) << "Menu Item" << setw(10) << "Price" << endl;
        cout << "----------------------" << endl;

        //skip header
        bool isFirstLine = true;
        while (getline(menu, line)) {
            if (isFirstLine) {
                isFirstLine = false;
                continue;
            }

            size_t commaPos = line.find(',');
            if (commaPos != string::npos) {
                string menuItem = line.substr(0, commaPos);
                string price = line.substr(commaPos + 1);
                cout << left << setw(15) << menuItem << setw(10) << price << endl;
            }
        }
        
        menu.close();
        cout << endl;

    }

    void appendToMenuCSV(const string& filename, const vector<string>& menuItems, const vector<double>& prices) {
        if (menuItems.size() != prices.size()) {
            cerr << "Error: Menu items and prices must have the same number of entries." << endl;
            return;
        }

        // Check if the file exists and has content
        ifstream checkFile(filename);
        bool fileIsNew = !checkFile.is_open() || checkFile.peek() == ifstream::traits_type::eof();
        checkFile.close();

        // Open the file in append mode
        ofstream file(filename, ios::app);
        if (!file.is_open()) {
            cerr << "Error: Could not open file." << endl;
            return;
        }

        // Write the header if the file is new or empty
        if (fileIsNew) {
            file << "Menu Item,Price\n";
        }

        // Write each menu item and its price
        for (size_t i = 0; i < menuItems.size(); ++i) {
            file << menuItems[i] << "," << prices[i] << "\n";
        }

        file.close();
        cout << "Menu items added to the CSV file successfully!" << endl;
    }

    bool removeMenuItem(string name) {
        ifstream inputFile("menu.csv");
        ofstream tempFile("tempfile.csv");
        string line;

        //Ensure the file is open
        if (!inputFile.is_open()) {
            cerr << "Error: Could not open the file for reading." << endl;
            return false;
        }


        //Ensure the temporary file is open
        if (!tempFile.is_open()) {
            cerr << "Error: Could not open the temporary file for writing." << endl;
            return false;
        }

        bool found = false;

        //Read each line from the original file
        while (getline(inputFile, line)) {
            stringstream ss(line);
            string item, price;

            //Extract values from CSV line

            getline(ss, item, ',');
            getline(ss, price, ',');


            //If the name does not match the item to be removed, write the line into temp file
            if (item != name) {
                tempFile << line << endl; //Write the original line to the temp file
            }
            else {
                found = true; //Mark that we found the item
            }
        }

        //Close files
        inputFile.close();
        tempFile.close();

        //If the item was found and removed, replace the original file with temp file
        if (found) {
            remove("menu.csv"); //delete original file
            rename("tempfile.csv", "menu.csv"); //rename temp file to original file
            cout << "Menu item " << name << " removed successfully." << endl;
            return true;
        }
        else {
            remove("tempfile.csv"); //no menu item found, so remove temp file
            cout << "Menu item " << name << " not found." << endl;
            return false;
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

    bool editMenu(string item) {
        ifstream inputFile("menu.csv");
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
        double newPrice;

        // Read each line from the original file
        while (getline(inputFile, line)) {
            stringstream ss(line);
            string itemF, priceStr;

            // Extract values from the CSV line
            getline(ss, itemF, ',');
            getline(ss, priceStr, ',');

            // If the name and pin match, prompt for new values
            if (itemF == item) {
                found = true;

                cout << "1. Change Price\n";
                cout << "2. Back\n";
                int editNav;
                cin >> editNav;

                switch (editNav) {
                case 1:
                    // Get new position
                    cout << "Enter new price for " << item << ": ";
                    cin >> newPrice;
                    tempFile << itemF << "," << newPrice << endl;
                    break;
                case 2:
                    cout << "Returning to Edit Menu...\n";
                    tempFile << line << endl;  // Write the original line if no valid choice is made
                    break;
                default:
                    cout << "Invalid choice. No changes made." << endl;
                    tempFile << line << endl;  // Write the original line if no valid choice is made
                    break;
                }
            }
            else {
                // Write the original data to the temp file
                tempFile << line << endl;
            }
        }

        // Close the files
        inputFile.close();
        tempFile.close();

        // If the item was found and updated, replace the original file with the temp file
        if (found) {
            remove("menu.csv");         // Delete the original file
            rename("temp.csv", "menu.csv");  // Rename temp file to original file
            cout << "Price on " << item << " has been saved." << endl;
            return true;
        }
        else {
            remove("temp.csv");  // No item found, so remove temp file
            cout << "Item " << item << " not found or pin did not match." << endl;
            return false;
        }
    }

    // Change position (admin only)
    bool editEmployee(string name, int pin) {
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
        string newPosition;
        double newWage;

        // Read each line from the original file
        while (getline(inputFile, line)) {
            stringstream ss(line);
            string editName, position, pinStr, wageStr;

            // Extract values from the CSV line
            getline(ss, editName, ',');
            getline(ss, position, ',');
            getline(ss, pinStr, ',');
            getline(ss, wageStr, ',');

            // If the name and pin match, prompt for new values
            if (editName == name && stoi(pinStr) == pin) {
                found = true;

                cout << "1. Change Position\n";
                cout << "2. Change Wage\n";
                cout << "3. Back\n";
                int editNav;
                cin >> editNav;

                switch (editNav) {
                case 1:
                    // Get new position
                    cout << "Enter new position for " << name << ": ";
                    cin >> newPosition;
                    tempFile << editName << "," << newPosition << "," << pinStr << "," << wageStr << endl;
                    break;
                case 2:
                    // Get new wage
                    cout << "Enter new wage for " << name << ": ";
                    cin >> newWage;
                    tempFile << editName << "," << position << "," << pinStr << "," << newWage << endl;
                    cin.ignore();  // To ignore the newline left in the input buffer
                    break;
                case 3:
                    cout << "Returning to Edit Employee...\n";
                    tempFile << line << endl;  // Write the original line if no valid choice is made
                    break;
                default:
                    cout << "Invalid choice. No changes made." << endl;
                    tempFile << line << endl;  // Write the original line if no valid choice is made
                    break;
                }
            }
            else {
                // Write the original data to the temp file
                tempFile << line << endl;
            }
        }

        // Close the files
        inputFile.close();
        tempFile.close();

        // If the employee was found and updated, replace the original file with the temp file
        if (found) {
            remove("employees.csv");         // Delete the original file
            rename("temp.csv", "employees.csv");  // Rename temp file to original file
            cout << "Employee " << name << " updated successfully." << endl;
            return true;
        }
        else {
            remove("temp.csv");  // No employee found, so remove temp file
            cout << "Employee " << name << " not found or pin did not match." << endl;
            return false;
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
    Employee user;
    Order createOrder;

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
        cout << "1. View To-Go Orders\n";
        cout << "2. Close Out\n";
        cout << "3. Back\n";
        cin >> navigateToGo;

        switch (navigateToGo) {
        case 1:
            cout << "print list of to-go orders\n";
            break;
        case 2:
            cout << "Directing to Close out...\n";
            break;
        case 3:
            cout << "Directing back to Table View...\n";
            serverInterface();
            break;
        default:
            cout << "Invalid Input\n";
            toGoOrder();
            break;
        }
       
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
        int orderNum;
        cout << "1. New To-Go Order\n";
        cout << "2. New Table Order\n";
        cout << "3. Back\n";
        cin >> navigateNewOrder;

        switch (navigateNewOrder) {
        case 1:
            cout << "Enter Order Number: ";
            cin >> orderNum;

            dbWindow.displayMenu();

            createOrder.createOrderCSV(orderNum);

            if (user.position == "Manager") {
                cout << "Directing to Table View...\n";
                managerInterface();
            }

            else {
                cout << "Directing to table View...\n";
                serverInterface();
            }
            break;
        case 2:
            cout << "Enter Order Number: ";
            cin >> orderNum;

            dbWindow.displayMenu();

            createOrder.createOrderCSV(orderNum);
            
            if (user.position == "Manager") {
                cout << "Directing to Table View...\n";
                managerInterface();
            }

            else {
                cout << "Directing to table View...\n";
                serverInterface();
            }
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
        cout << "4. Logout\n";
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
        case 4:
            cout << "Logging out...\n";
            return;
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

            cout << "What is the name if the employee you want to edit?" << endl;
            cin >> name;

            cout << "Enter the pin number of that employee: " << endl;
            cin >> pin;

            dbWindow.editEmployee(name, pin);

            cout << "Directing to Table View...\n";
            managerInterface();
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

    void editMenuInterface() {
        int editMenuNav;
        vector<string> menuItems;
        vector<double> prices;

        string item;
        double price;
        char choice;


        cout << "1. Add New Item\n";
        cout << "2. Remove Item\n";
        cout << "3. Edit Price\n";
        cout << "4. Back\n";
        cin >> editMenuNav;

        switch (editMenuNav) {
        case 1:
            cout << "Current Menu:" << endl;
            dbWindow.displayMenu();

            cout << "Enter the menu item and price you wish to add. Type 'n' when you are done.\n";

            do {
                cout << "Enter menu item: ";
                cin.ignore(); // To clear any leftover input in the stream
                getline(cin, item);

                cout << "Enter price: ";
                cin >> price;

                // Add to vectors
                menuItems.push_back(item);
                prices.push_back(price);

                cout << "Do you want to add another item? (y/n): ";
                cin >> choice;
            } while (choice == 'y' || choice == 'Y');

            // Append to the CSV file
            dbWindow.appendToMenuCSV("menu.csv", menuItems, prices);

            cout << "Directing back to Table View...\n";
            managerInterface();
            break;
        case 2:
            cout << "Directing to Remove Item...\n";

            cout << "Current Menu:\n";
            
            dbWindow.displayMenu();

            cout << "Enter the name of the menu item to remove: " << endl;
            cin.ignore();
            getline(cin, item);

            dbWindow.removeMenuItem(item);

            cout << "Directing to Table View...\n";
            managerInterface();
            break;
        case 3:
            cout << "Directing to Edit Price...\n";

            cout << "Current Menu:\n";

            dbWindow.displayMenu();

            cout << "What is the name of the item you want to edit?" << endl;
            cin.ignore();
            getline(cin, item);

            dbWindow.editMenu(item);

            cout << "Directing to Table View...\n";
            managerInterface();
            break;
        case 4:
            cout << "Directing back to Table View...\n";
            managerInterface();
            break;
        default:
            cout << "Invalid Input\n";
            editMenuInterface();
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
        case 1:          
            cout << "Directing to Edit Menu...\n";
            editMenuInterface();
            break;
        case 2:
            cout << "Directing to Edit Employee...\n";
            editEmpInterface();
            break;
        case 3:
            cout << "3";
            break;
        case 4:
            cout << "Directing to Start New Order...\n";

            break;
        case 5:
            cout << "5";
            break;
        case 6:
            cout << "6";
            break;
        case 7:
            cout << "7";
            break;
        case 8:
            cout << "Logging out...\n";
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
