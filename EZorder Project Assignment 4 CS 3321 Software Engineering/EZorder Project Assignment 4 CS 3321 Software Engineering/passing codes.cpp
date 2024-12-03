void createOrderCSV(int orderNum, const string& orderType, const string& userName, const int& userPin) {
    const string menuFile = "menu.csv";
    map<string, double> menu = loadMenu(menuFile);

    if (menu.empty()) {
        cerr << "Error: Menu is empty or could not be loaded." << endl;
        return;
    }

    string menuItem;
    double price = 0.0;

    //Create file name with orderNum
    string orderFile = (orderType == "to-go" ? "ToGo_" : "DineIn_") + to_string(orderNum) + ".csv";

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
    else {
        cerr << "Error opening order file." << endl;
    }

    //Append new order to the "existingOrders.csv" file
    ofstream existingOrdersFile("existingorder.csv", ios::app);

    if (existingOrdersFile.is_open()) {
        //Append server's information and the order file they're assigned to
        existingOrdersFile << userName << "," << to_string(userPin) << "," << orderFile << endl;
        cout << "Order added to existingorders.csv\n";
    }
    else {
        cerr << "Error opening 'existingorders.csv'" << endl;
    }

}
