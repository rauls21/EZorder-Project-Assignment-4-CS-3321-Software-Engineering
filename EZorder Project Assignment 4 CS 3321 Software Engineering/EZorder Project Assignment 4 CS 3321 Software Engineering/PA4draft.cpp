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
    default: return "Uknown";
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////// End of Employee CLass
