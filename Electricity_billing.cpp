#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

class Consumer {
protected:
    int consumerId;
    string name;
    int units;

public:
    Consumer(int id, string n, int u) {
        try {
            // Check Consumer ID
            if (id <= 0)
                throw invalid_argument("Consumer ID must be a positive number.");

            // Check Name (Only alphabets and spaces)
            for (char ch : n) {
                if (!isalpha(ch) && ch != ' ')
                    throw invalid_argument("Consumer name must contain only alphabets.");
            }

            // Check Units
            if (u < 0)
                throw invalid_argument("Units cannot be negative.");

            consumerId = id;
            name = n;
            units = u;
        }
        catch (const exception &e) {
            cout << "Constructor Error: " << e.what() << endl;
            throw;      // Re-throw so object is not created with invalid data
        }
    }

    virtual double calculateBill() = 0;

    virtual string getCategory() = 0;

    void displayBill() {
        cout << "\n------ Electricity Bill ------\n";
        cout << "Consumer ID : " << consumerId << endl;
        cout << "Name        : " << name << endl;
        cout << "Category    : " << getCategory() << endl;
        cout << "Units Used  : " << units << endl;
        cout << fixed << setprecision(2);
        cout << "Total Bill  : Rs. " << calculateBill() << endl;
    }

    void saveToFile() {
        ofstream file("electricity_bills.txt", ios::app);

        if (file.is_open()) {
            file << consumerId << "|"
                 << name << "|"
                 << getCategory() << "|"
                 << units << "|"
                 << fixed << setprecision(2)
                 << calculateBill() << "\n";
            file.close();
            cout << "Bill saved successfully!\n";
        } else {
            cout << "Error opening file!\n";
        }
    }

    virtual ~Consumer() {}
};

// Domestic Consumer
class Domestic : public Consumer {
public:
    Domestic(int id, string n, int u)
        : Consumer(id, n, u) {}

    double calculateBill() override {
        if (units <= 100)
            return units * 1.50;
        else if (units <= 300)
            return 100 * 1.50 + (units - 100) * 2.50;
        else
            return 100 * 1.50 + 200 * 2.50
                   + (units - 300) * 4.00;
    }

    string getCategory() override {
        return "Domestic";
    }
};

// Commercial Consumer
class Commercial : public Consumer {
public:
    Commercial(int id, string n, int u)
        : Consumer(id, n, u) {}

    double calculateBill() override {
        if (units <= 100)
            return units * 4.00;
        else if (units <= 300)
            return 100 * 4.00 + (units - 100) * 5.00;
        else
            return 100 * 4.00 + 200 * 5.00
                   + (units - 300) * 7.00;
    }

    string getCategory() override {
        return "Commercial";
    }
};

// Industrial Consumer
class Industrial : public Consumer {
public:
    Industrial(int id, string n, int u)
        : Consumer(id, n, u) {}

    double calculateBill() override {
        if (units <= 100)
            return units * 6.00;
        else if (units <= 300)
            return 100 * 6.00 + (units - 100) * 7.00;
        else
            return 100 * 6.00 + 200 * 7.00
                   + (units - 300) * 9.00;
    }

    string getCategory() override {
        return "Industrial";
    }
};

void viewRecords() {
    ifstream file("electricity_bills.txt");
    string line;

    cout << "\n------ Saved Electricity Bills ------\n";

    if (!file.is_open()) {
        cout << "No records found.\n";
        return;
    }

    while (getline(file, line)) {
        cout << line << endl;
    }

    file.close();
}

int main() {
    int choice;

    do {
        cout << "\n===== Electricity Billing System =====\n";
        cout << "1. Domestic Consumer\n";
        cout << "2. Commercial Consumer\n";
        cout << "3. Industrial Consumer\n";
        cout << "4. View Saved Bills\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice >= 1 && choice <= 3) {
            int id, units;
            string name;

            cout << "Enter Consumer ID: ";
            cin >> id;

            cout << "Enter Consumer Name: ";
            getline(cin >> ws, name);

            cout << "Enter Units Consumed: ";
            cin >> units;

            if (units < 0) {
                cout << "Units cannot be negative!\n";
                continue;
            }

            Consumer* consumer = nullptr;

            if (choice == 1)
                consumer = new Domestic(id, name, units);
            else if (choice == 2)
                consumer = new Commercial(id, name, units);
            else
                consumer = new Industrial(id, name, units);

            consumer->displayBill();
            consumer->saveToFile();

            delete consumer;
        }
        else if (choice == 4) {
            viewRecords();
        }
        else if (choice == 5) {
            cout << "Thank you for using the system!\n";
        }
        else {
            cout << "Invalid choice! Try again.\n";
        }

    } while (choice != 5);

    return 0;
}
