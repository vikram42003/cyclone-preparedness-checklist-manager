// ============================================================
// CAP455 - Object-Oriented Programming using C++
// Project : Cyclone Preparedness Checklist Manager
// Phase   : CA1 - Design & Create
// Author  : [Your Name] | [Roll Number]
// Date    : November 2026
// ============================================================

#include <iostream>
#include <string>
#include <limits>

using namespace std;

// ============================================================
// HELPER: readInt
//   Reads one integer from stdin, flushing the rest of the line.
//   Keeps prompting with 'reprompt' until a valid integer is read.
// ============================================================
int readInt(const string& reprompt) {
    int value;
    while (!(cin >> value)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << reprompt;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');  // flush trailing newline
    return value;
}

// ============================================================
// CLASS: ChecklistItem
//   Represents one preparedness task in the household checklist.
// ============================================================
class ChecklistItem {
private:
    int    itemId;      // Unique identifier
    string itemName;    // Name of the task
    string stage;       // "Before", "During", or "After"
    bool   isPrepared;  // true = Prepared, false = Not Prepared

    // Static member: shared counter across all ChecklistItem objects
    static int totalItems;

public:
    // ---- Inline getter: returns the item's ID ----
    // Defined inside the class body → automatically inline
    inline int getItemId() const {
        return itemId;
    }

    // ---- setData: assigns all four attributes using 'this' pointer ----
    void setData(int id, const string& name, const string& stg, bool prepared) {
        this->itemId     = id;
        this->itemName   = name;
        this->stage      = stg;
        this->isPrepared = prepared;
        totalItems++;
    }

    // ---- Accessors used by ChecklistManager ----
    string getStage()      const { return stage;     }
    bool   getIsPrepared() const { return isPrepared; }

    // ---- Static accessor: returns the shared item count ----
    static int getTotalItems() {
        return totalItems;
    }

    // ---- Friend function declaration ----
    // displayItem is a friend so it can read private members directly
    friend void displayItem(const ChecklistItem& item);
};

// Static member definition (must live outside the class body)
int ChecklistItem::totalItems = 0;

// ============================================================
// FRIEND FUNCTION: displayItem
//   Prints one checklist entry. Declared friend of ChecklistItem
//   so it accesses private members directly — no extra getters needed.
// ============================================================
void displayItem(const ChecklistItem& item) {
    string status = item.isPrepared ? "Prepared" : "Not Prepared";
    cout << "  [ID: " << item.itemId << "]  "
         << item.itemName
         << "  |  " << status << "\n";
}

// ============================================================
// CLASS: ChecklistManager
//   Owns the fixed-size array of ChecklistItem objects.
//   Handles adding items and displaying them by stage.
// ============================================================
class ChecklistManager {
private:
    static const int CAPACITY = 50;    // Maximum entries the array can hold
    ChecklistItem    items[CAPACITY];  // Array of objects
    int              count;            // Number of entries currently stored

    // Returns true if an item with the given ID already exists
    bool isDuplicate(int id) const {
        for (int i = 0; i < count; i++) {
            if (items[i].getItemId() == id) {
                return true;
            }
        }
        return false;
    }

    // Displays all entries whose stage matches stageLabel
    void displayByStage(const string& stageLabel) const {
        bool found = false;
        for (int i = 0; i < count; i++) {
            if (items[i].getStage() == stageLabel) {
                displayItem(items[i]);   // calls the friend function
                found = true;
            }
        }
        if (!found) {
            cout << "  No items added for this stage.\n";
        }
    }

public:
    ChecklistManager() : count(0) {}

    // ---- addItem: collects and validates input, then stores a new item ----
    void addItem() {
        if (count >= CAPACITY) {
            cout << "\n  [!] Checklist is full. Cannot add more items.\n";
            return;
        }

        int    id;
        string name;
        int    stageChoice;
        int    statusChoice;
        string stageStr;
        bool   prepared;

        // --- Item ID: must be a positive integer ---
        cout << "\n  Enter Item ID (positive integer): ";
        id = readInt("  [!] Invalid input. Please enter a positive integer: ");
        while (id <= 0) {
            cout << "  [!] ID must be greater than zero. Try again: ";
            id = readInt("  [!] Invalid input. Please enter a positive integer: ");
        }

        if (isDuplicate(id)) {
            cout << "  [!] An item with ID " << id
                 << " already exists. Duplicate rejected.\n";
            return;
        }

        // --- Item Name: must not be empty or only whitespace ---
        cout << "  Enter Item Name: ";
        getline(cin, name);

        bool allSpace = true;
        for (int i = 0; i < (int)name.size(); i++) {
            if (name[i] != ' ' && name[i] != '\t') { allSpace = false; break; }
        }
        if (name.empty() || allSpace) {
            cout << "  [!] Item name cannot be empty.\n";
            return;
        }

        // --- Cyclone Stage: numbered menu, 1–3 only ---
        cout << "  Select Cyclone Stage:\n";
        cout << "    1. Before the Cyclone\n";
        cout << "    2. During the Cyclone\n";
        cout << "    3. After the Cyclone\n";
        cout << "  Enter choice (1/2/3): ";
        stageChoice = readInt("  [!] Invalid input. Enter 1, 2, or 3: ");
        while (stageChoice < 1 || stageChoice > 3) {
            cout << "  [!] Please enter 1, 2, or 3: ";
            stageChoice = readInt("  [!] Invalid input. Enter 1, 2, or 3: ");
        }

        if      (stageChoice == 1) stageStr = "Before";
        else if (stageChoice == 2) stageStr = "During";
        else                       stageStr = "After";

        // --- Preparation Status: numbered menu, 1–2 only ---
        cout << "  Select Preparation Status:\n";
        cout << "    1. Prepared\n";
        cout << "    2. Not Prepared\n";
        cout << "  Enter choice (1/2): ";
        statusChoice = readInt("  [!] Invalid input. Enter 1 or 2: ");
        while (statusChoice < 1 || statusChoice > 2) {
            cout << "  [!] Please enter 1 or 2: ";
            statusChoice = readInt("  [!] Invalid input. Enter 1 or 2: ");
        }

        prepared = (statusChoice == 1);

        // --- Use a pointer to the next free slot and call setData ---
        ChecklistItem* ptr = &items[count];
        ptr->setData(id, name, stageStr, prepared);
        count++;

        cout << "  [OK] Item added successfully. "
             << "Total items in checklist: " << ChecklistItem::getTotalItems() << "\n";
    }

    // ---- viewAll: displays every item grouped by cyclone stage ----
    void viewAll() const {
        cout << "\n";
        if (count == 0) {
            cout << "  The checklist is currently empty. Add items using option 1.\n";
            return;
        }

        cout << "  =========================================\n";
        cout << "       CYCLONE PREPAREDNESS CHECKLIST\n";
        cout << "  =========================================\n\n";

        cout << "  --- BEFORE THE CYCLONE ---\n";
        displayByStage("Before");

        cout << "\n  --- DURING THE CYCLONE ---\n";
        displayByStage("During");

        cout << "\n  --- AFTER THE CYCLONE ---\n";
        displayByStage("After");

        cout << "\n  =========================================\n";
        cout << "  Total items: " << ChecklistItem::getTotalItems() << "\n";
        cout << "  =========================================\n";
    }
};

// ============================================================
// HELPER: printSeparator — visual divider between sections
// ============================================================
void printSeparator() {
    cout << "\n  -----------------------------------------\n";
}

// ============================================================
// MAIN
// ============================================================
int main() {
    ChecklistManager manager;
    int choice;

    cout << "\n  =========================================\n";
    cout << "   CYCLONE PREPAREDNESS CHECKLIST MANAGER\n";
    cout << "         CAP455 - CA1 | Phase 1\n";
    cout << "  =========================================\n";

    do {
        printSeparator();
        cout << "  MAIN MENU\n";
        cout << "  1. Add Checklist Item\n";
        cout << "  2. View All Checklist Items\n";
        cout << "  3. Exit\n";
        printSeparator();
        cout << "  Enter your choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  [!] Invalid input. Please enter 1, 2, or 3.\n";
            choice = 0;
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                manager.addItem();
                break;
            case 2:
                manager.viewAll();
                break;
            case 3:
                cout << "\n  Exiting. Stay prepared and stay safe!\n\n";
                break;
            default:
                cout << "  [!] Invalid choice. Please enter 1, 2, or 3.\n";
        }

    } while (choice != 3);

    return 0;
}
