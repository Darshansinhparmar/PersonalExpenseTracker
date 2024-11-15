#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <ctime>
#include <sstream>
#include <string>
#include <algorithm>  // For std::sort

using namespace std;

struct Expense {
    string category;
    string description;
    double amount;
    time_t date;

    Expense(string cat, string desc, double amt)
        : category(cat), description(desc), amount(amt) {
        time(&date);
    }

    string getDate() const {
        char buffer[80];
        struct tm* timeinfo = localtime(&date);
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
        return string(buffer);
    }
};

struct Budget {
    double budgetAmount;
    double currentSpending;

    // Default constructor
    Budget() : budgetAmount(0.0), currentSpending(0.0) {}

    // Constructor that accepts a budget amount
    Budget(double budgetAmt) : budgetAmount(budgetAmt), currentSpending(0.0) {}
};

class ExpenseTracker {
public:
    unordered_map<string, Budget> categoryBudgets;
    vector<Expense> expenses;

    void setBudget(const string& category, double amount) {
        categoryBudgets[category] = Budget(amount);
    }

    void addExpense(const string& category, const string& description, double amount) {
        expenses.push_back(Expense(category, description, amount));
        categoryBudgets[category].currentSpending += amount;
    }

    void showBudgetStatus() {
        for (const auto& budgetEntry : categoryBudgets) {
            const auto& category = budgetEntry.first;
            const auto& budget = budgetEntry.second;
            cout << "Category: " << category
                 << ", Budget: $" << budget.budgetAmount
                 << ", Spent: $" << budget.currentSpending
                 << ", Remaining: $" << (budget.budgetAmount - budget.currentSpending)
                 << endl;

            if (budget.currentSpending > budget.budgetAmount) {
                cout << "Warning: You have overspent in the " << category << " category!" << endl;
            }
        }
    }

    void sortByAmount() {
        sort(expenses.begin(), expenses.end(), [](const Expense& a, const Expense& b) {
            return a.amount < b.amount;
        });
    }

    void sortByCategory() {
        sort(expenses.begin(), expenses.end(), [](const Expense& a, const Expense& b) {
            return a.category < b.category;
        });
    }

    void sortByDate() {
        sort(expenses.begin(), expenses.end(), [](const Expense& a, const Expense& b) {
            return a.date < b.date;
        });
    }

    void saveExpensesToFile(const string& filename) {
        ofstream outFile(filename);
        for (const auto& expense : expenses) {
            outFile << expense.category << ", " << expense.description << ", " 
                    << expense.amount << ", " << expense.getDate() << endl;
        }
        outFile.close();
    }

    void loadExpensesFromFile(const string& filename) {
        ifstream inFile(filename);
        if (!inFile) return;

        string category, description;
        double amount;
        string date;

        while (getline(inFile, category, ',') && getline(inFile, description, ',') &&
               inFile >> amount && getline(inFile, date)) {
            expenses.push_back(Expense(category, description, amount));
        }
        inFile.close();
    }

    void displayExpenses() const {
        for (const auto& expense : expenses) {
            cout << "Category: " << expense.category
                 << ", Description: " << expense.description
                 << ", Amount: $" << expense.amount
                 << ", Date: " << expense.getDate() << endl;
        }
    }
};

int main() {
    ExpenseTracker tracker;

    while (true) {
        cout << "\n1. Add Expense\n2. Show Budget Status\n3. Sort Expenses\n4. Display Expenses\n5. Save Expenses\n6. Load Expenses\n7. Exit\n";
        cout << "Choose an option: ";
        int choice;
        cin >> choice;

        if (choice == 1) {
            cout << "Enter category: ";
            string category;
            cin >> category;
            cout << "Enter description: ";
            string description;
            cin.ignore();  // Ignore newline character
            getline(cin, description);
            cout << "Enter amount: ";
            double amount;
            cin >> amount;

            tracker.addExpense(category, description, amount);
            cout << "Expense added.\n";

        } else if (choice == 2) {
            tracker.showBudgetStatus();

        } else if (choice == 3) {
            cout << "\n1. Sort by Amount\n2. Sort by Category\n3. Sort by Date\n";
            cout << "Choose sort option: ";
            int sortChoice;
            cin >> sortChoice;

            if (sortChoice == 1) {
                tracker.sortByAmount();
                cout << "Expenses sorted by amount.\n";
            } else if (sortChoice == 2) {
                tracker.sortByCategory();
                cout << "Expenses sorted by category.\n";
            } else if (sortChoice == 3) {
                tracker.sortByDate();
                cout << "Expenses sorted by date.\n";
            }

        } else if (choice == 4) {
            tracker.displayExpenses();

        } else if (choice == 5) {
            cout << "Enter file name to save expenses: ";
            string filename;
            cin >> filename;
            tracker.saveExpensesToFile(filename);
            cout << "Expenses saved to file.\n";

        } else if (choice == 6) {
            cout << "Enter file name to load expenses: ";
            string filename;
            cin >> filename;
            tracker.loadExpensesFromFile(filename);
            cout << "Expenses loaded from file.\n";

        } else if (choice == 7) {
            cout << "Exiting program...\n";
            break;
        } else {
            cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}
