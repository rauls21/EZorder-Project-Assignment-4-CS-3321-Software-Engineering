#include<iostream>
#include <vector>
#include <string>
using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////// Start of Employee Class
enum Position { CHEF, HOST, SERVER, MANAGER}

class Employee {
private:
  string name;
  int pin;
  Position position;
  double hoursWorked;
  double wagePerHour;
  bool clockedIn;

public:
  Employee(string n, int p, Position pos, double wage)
    : name(n), pin(p), position(pos), wagePerHour(wage), hoursWorked(0), clockedIn(false) {}

  string getName() {return name; }
  int getPin() {return pin; }
  Position getPosition() {return position; }

  void changePosition(Position newPos) {
      position = newPos;
      cout << name << "'s position changed to " << position << endl;
  }

  void clockIn() {
    if (!clockedIn) {
      clockedIn = true;
      cout << name << " clcoked in." << endl;
    }
  }

  void clockOut(double hours) {
    if (clockedIn) {
      hoursWorked += hours;
      clockedIn = false;
      cout << name << " clocked out. Total hours: " << hoursWorked << endl;
    }
  }

  double calculateWage() {
    return hoursWorked * wagePerHour;
  }

  void displayInfo() {
    cout << "Name: " << name << ", Position: " << position << ", Hours Worked: " << hoursWorked << endl;
  }
};

//Helper function to display position enum as string
string positionToString(Position pos) {
  switch (pos) {
    case CHEF: return "Chef";
    case HOST: return "Host";
    case SERVER: return "Server";
    case MANAGER: return "Manager";
    default: return "Unknown";
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////// End of Employee CLass

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Start of Employee Management
class EmploymentManagement {
private:
  vector<Employee> employees;

public:
  void addEmployee(Employee emp) {
    employees.push_back(emp);
    cout << emp.getName() << " added to the system." << endl;
  }

  void removeEmplyee(int pin) {
    for (for it = employees.begin(); it != employees.end(); ++it) {
      if (it->getPin() == pin) {
      cout << it->getName() << " removed from the system." << endl;
      employees.erase(it);
      return;
      }
    }
    cout << "Employee with the pin " << pin << " not found." << endl;
  }

  Employee* findEmployee(int pin) {
    for (auto& emp : employees) {
      if (emp.getPin() == pin)
        return &emp;
    }
    return nullptr; // Not found
  }

  void displayAllEmployee() {
    for (const auto& emp : employees) {
      emp.displayInfo();
    }
  }
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////// End of Employee Management

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Start of Order Class
class Order {
private:
  int orderID;
  vector<string> items;
  double totalCost;

public:
  Order(int id) : orderID(id), totalCost(0) {}

  void addItem(string item, double price) {
    items.push_back(item);
    totalCost += price;
  }

  void removeItem(string item, double price) {
    auto it = find(items.begin(), items.end(), item);
    if (it != items.end()) {
      items.erase(it);
      totalCost -= price;
    }
  }

  double calculateTotal() {
    return totalCost;
  }

  void printOrder() {
    cout << "Order ID: " << orderID << endl;
    cout << "Items: " << endl;
    for (const auto& item : items) {
      cout << "- " << item << endl;
    }
    cout << "Total Cost: $" << totalCost << endl;
  }
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// End of Order Class

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Start of Menu Class
class Menu {
private:
    vector<pair<string, double>> menuItems;

public:
    void addItem(string name, double price) {
        menuItems.push_back(make_pair(name, price));
        cout << "Item added: " << name << " ($" << price << ")" << endl;
    }

    void removeItem(string name) {
        auto it = remove_if(menuItems.begin(), menuItems.end(),
            [&name](const pair<string, double>& item) { return item.first == name; });
        
        if (it != menuItems.end()) {
            cout << "Item removed: " << name << endl;
            menuItems.erase(it, menuItems.end());
        }
    }

    void displayMenu() {
        cout << "Menu:" << endl;
        for (const auto& item : menuItems) {
            cout << "- " << item.first << ": $" << item.second << endl;
        }
    }

    double getPrice(string name) {
        for (const auto& item : menuItems) {
            if (item.first == name) {
                return item.second;
            }
        }
        return 0;
    }
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// End of Menu Class

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Start of System Integration
int main() {
    EmployeeManagement employeeSystem;
    Menu restaurantMenu;

    // Add employees
    employeeSystem.addEmployee(Employee("John", 1234, CHEF, 15.0));
    employeeSystem.addEmployee(Employee("Sarah", 5678, SERVER, 12.0));

    // Add menu items
    restaurantMenu.addItem("Burger", 10.0);
    restaurantMenu.addItem("Pizza", 12.0);

    // Create order
    Order order1(1);
    order1.addItem("Burger", restaurantMenu.getPrice("Burger"));
    order1.addItem("Pizza", restaurantMenu.getPrice("Pizza"));

    // Print order details
    order1.printOrder();

    // Employee clock-in and clock-out
    Employee* emp = employeeSystem.findEmployee(1234);
    if (emp) {
        emp->clockIn();
        emp->clockOut(8.0);
        cout << emp->getName() << " earned $" << emp->calculateWage() << endl;
    }

    return 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// End of System Integration
