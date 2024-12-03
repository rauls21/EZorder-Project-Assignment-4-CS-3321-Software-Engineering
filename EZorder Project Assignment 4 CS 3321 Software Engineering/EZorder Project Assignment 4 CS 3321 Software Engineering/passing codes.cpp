 void calculateWages(const vector<Emp>& empVect) {
     const int WeeksPerYear = 52;
     const int HoursPerWeek = 40;
     for (const auto& emp : empVect) {
         double yearlyWage = emp.hourlyWage * HoursPerWeek * WeeksPerYear;
         cout << "Name: " << emp.name << ", Position: " << emp.position
             << ", Yearly Wage: $" << yearlyWage << endl;
     }
 }

 vector<Emp>readEmployeesFromFile() {
     const string filename = "employees.csv";
     vector<Emp>empVect;
     ifstream file(filename);
     if (!file.is_open()) {
         cerr << "Failed to open the file." << endl;
         return empVect;
     }

     string line;
     getline(file, line);  // Skip the header
     while (getline(file, line)) {
         stringstream stream(line);
         Emp empTemp;
         string wageStr, pinStr;

         getline(stream, empTemp.name, ',');
         getline(stream, empTemp.position, ',');
         getline(stream, pinStr, ',');
         getline(stream, wageStr, ',');

         empTemp.pin = stoi(pinStr);
         empTemp.hourlyWage = stod(wageStr);

         empVect.push_back(empTemp);
     }
     file.close();
     return empVect;
 }
