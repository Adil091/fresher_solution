#include <bits/stdc++.h>

using namespace std;

// employee class
class Employee {
public:
    string name;
    float salary;

    Employee() {}
    Employee(string name, float salary) : name(name), salary(salary) {}

    // serialize employee data to string
    string serialize() const {
        ostringstream oss;
        oss << name << '\n' << salary << '\n';
        return oss.str();
    }

    // deserialize employee data from string
    void deserialize(istream &is) {
        getline(is, name);
        is >> salary;
        is.ignore();
    }
};

// customer class
class Customer {
public:
    string name;
    string address;

    Customer() {}
    Customer(string name, string address) : name(name), address(address) {}

    // serialize customer data to string
    string serialize() const {
        ostringstream oss;
        oss << name << '\n' << address << '\n';
        return oss.str();
    }

    // deserialize customer data from string
    void deserialize(istream &is) {
        getline(is, name);
        getline(is, address);
    }
};

// sale class
class Sale {
public:
    Customer *customer;
    Employee *salerep;
    float amount;
    string date;

    Sale() : customer(nullptr), salerep(nullptr) {}
    Sale(Customer *customer, Employee *salerep, float amount, string date)
        : customer(customer), salerep(salerep), amount(amount), date(date) {}

    // serialize sale data to string
    string serialize() const {
        ostringstream oss;
        oss << customer->name << '\n' << salerep->name << '\n' << amount << '\n' << date << '\n';
        return oss.str();
    }

    // deserialize sale data from string
    void deserialize(istream &is, const vector<Customer> &customers, const vector<Employee> &employees) {
        string customerName, employeeName;
        getline(is, customerName);
        getline(is, employeeName);
        is >> amount;
        is.ignore();
        getline(is, date);

        for (const auto &c : customers) {
            if (c.name == customerName) {
                customer = const_cast<Customer*>(&c);
                break;
            }
        }

        for (const auto &e : employees) {
            if (e.name == employeeName) {
                salerep = const_cast<Employee*>(&e);
                break;
            }
        }
    }
};

vector<Employee> employees;
vector<Customer> customers;
vector<Sale> sales;

void addEmployee();
void addCustomer();
void addSale();
void serializeToFile();
void deserializeFromFile();
string serialize();
void deserialize(const string &data);
void loadFile();
void writeFile();

int main() {
    loadFile();

    int choice;
    do {
        cout << "1. add employee\n2. add customer\n3. add sale\n4. save and exit\nenter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                addEmployee();
                break;
            case 2:
                addCustomer();
                break;
            case 3:
                addSale();
                break;
            case 4:
                writeFile();
                cout << "data saved. exiting...\n";
                break;
            default:
                cout << "invalid choice. try again.\n";
        }
    } while (choice != 4);

    return 0;
}

void addEmployee() {
    string name;
    float salary;
    cout << "enter employee name: ";
    getline(cin, name);
    cout << "enter employee salary: ";
    cin >> salary;
    cin.ignore();

    employees.emplace_back(name, salary);
    cout << "employee added.\n";
}

void addCustomer() {
    string name, address;
    cout << "enter customer name: ";
    getline(cin, name);
    cout << "enter customer address: ";
    getline(cin, address);

    customers.emplace_back(name, address);
    cout << "customer added.\n";
}

void addSale() {
    string customerName, employeeName, date;
    float amount;

    cout << "enter customer name: ";
    getline(cin, customerName);
    cout << "enter employee name: ";
    getline(cin, employeeName);
    cout << "enter sale amount: ";
    cin >> amount;
    cin.ignore();
    cout << "enter sale date: ";
    getline(cin, date);

    Customer* customer = nullptr;
    Employee* salerep = nullptr;

    for (auto &c : customers) {
        if (c.name == customerName) {
            customer = &c;
            break;
        }
    }

    for (auto &e : employees) {
        if (e.name == employeeName) {
            salerep = &e;
            break;
        }
    }

    if (customer && salerep) {
        sales.emplace_back(customer, salerep, amount, date);
        cout << "sale added.\n";
    } else {
        cout << "customer or employee not found.\n";
    }
}

// serialize all data to string
string serialize() {
    ostringstream oss;
    oss << employees.size() << '\n';
    for (const auto &e : employees) {
        oss << e.serialize();
    }
    oss << customers.size() << '\n';
    for (const auto &c : customers) {
        oss << c.serialize();
    }
    oss << sales.size() << '\n';
    for (const auto &s : sales) {
        oss << s.serialize();
    }
    return oss.str();
}

// deserialize all data from string
void deserialize(const string &data) {
    istringstream iss(data);
    size_t size;

    iss >> size;
    iss.ignore();
    employees.resize(size);
    for (auto &e : employees) {
        e.deserialize(iss);
    }

    iss >> size;
    iss.ignore();
    customers.resize(size);
    for (auto &c : customers) {
        c.deserialize(iss);
    }

    iss >> size;
    iss.ignore();
    sales.resize(size);
    for (auto &s : sales) {
        s.deserialize(iss, customers, employees);
    }
}

// load data from file if exists
void loadFile() {
    ifstream file("Data.bin", ios::binary);
    if (file) {
        ostringstream oss;
        oss << file.rdbuf();
        deserialize(oss.str());
        cout << "data loaded from file.\n";
    } else {
        cout << "no data file found.\n";
    }
}

// save data to file
void writeFile() {
    ofstream file("Data.bin", ios::binary);
    file << serialize();
    cout << "data saved to file.\n";
}
