// ============================================================
// CAP455 - Object-Oriented Programming using C++
// Project : Cyclone Preparedness Checklist Manager
// Phase   : CA2 - Persist & Search
// Author  : [Your Name] | [Roll Number]
// Date    : November 2026
// ============================================================
//
// CA2 adds on top of CA1:
//   - Default, parameterised, and copy constructors + destructor
//   - Binary file persistence with fstream (fixed-size records)
//   - Load all records on startup using seekg
//   - Search by ID and by Stage
//   - Basic Update (toggle isPrepared status)
//   - New menu options: Search by ID, Search by Stage, Update Status
// ============================================================

#include <iostream>
#include <string>
#include <limits>
#include <fstream>
#include <cstring>    // memset, strncpy

using namespace std;

// ============================================================
// HELPER: readInt
//   Reads one integer from stdin. Loops until a valid int
//   is entered; flushes the newline after a successful read.
// ============================================================
int readInt(const string& reprompt) {
    int value;
    while (!(cin >> value)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << reprompt;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return value;
}

// ============================================================
// HELPER: printSeparator
// ============================================================
void printSeparator() {
    cout << "\n  -----------------------------------------\n";
}

// ============================================================
// STRUCT: FileRecord
//   Fixed-size Plain Old Data (POD) structure used exclusively
//   for binary file I/O. Because std::string has variable size,
//   it cannot be written directly to a binary file as a fixed-
//   size block. FileRecord uses char arrays of fixed width so
//   every record occupies exactly the same number of bytes,
//   allowing seekg/seekp to jump to any record by index.
//
//   sizeof(FileRecord) = 4 + 60 + 10 + 1 + 1 (padding) = 76 bytes
// ============================================================
struct FileRecord {
    int  itemId;          // 4 bytes
    char itemName[60];    // 60 bytes  (59 chars + null terminator)
    char stage[10];       // 10 bytes  ("Before\0", "During\0", "After\0")
    bool isPrepared;      // 1 byte
};

// ============================================================
// CLASS: ChecklistItem
//   Represents one preparedness task.
//   CA2 adds: default constructor, parameterised constructor,
//             copy constructor, and destructor.
// ============================================================
class ChecklistItem {
private:
    int    itemId;
    string itemName;
    string stage;
    bool   isPrepared;

    static int totalItems;   // shared counter across all objects

public:
    // ---- Default constructor ----
    // Sets safe initial values so an uninitialised object is not
    // in an undefined state. Required when declaring arrays of objects.
    ChecklistItem() : itemId(0), itemName(""), stage(""), isPrepared(false) {
        // totalItems NOT incremented here — a "blank slot" is not a real item
    }

    // ---- Parameterised constructor ----
    // Creates a fully initialised ChecklistItem in one step.
    ChecklistItem(int id, const string& name, const string& stg, bool prepared)
        : itemId(id), itemName(name), stage(stg), isPrepared(prepared) {
        totalItems++;
    }

    // ---- Copy constructor ----
    // Creates a new ChecklistItem as an exact copy of another.
    // Used when passing/returning objects by value or when
    // duplicating records (e.g., from a file into the in-memory array).
    ChecklistItem(const ChecklistItem& other)
        : itemId(other.itemId),
          itemName(other.itemName),
          stage(other.stage),
          isPrepared(other.isPrepared) {
        // Not incrementing totalItems: copy is not a new logical item
    }

    // ---- Destructor ----
    // No dynamic memory is allocated, so no manual cleanup is needed.
    // The destructor is defined explicitly to satisfy the CA2 requirement.
    ~ChecklistItem() {
        // Nothing to free — destructor body intentionally empty
    }

    // ---- Inline getter ----
    inline int getItemId() const { return itemId; }

    // ---- setData: assigns all attributes using 'this' pointer ----
    //   Still used by CA1-style addItem flow. Does NOT increment
    //   totalItems (the parameterised constructor already does that
    //   when we create the object; here we just overwrite a slot).
    void setData(int id, const string& name, const string& stg, bool prepared) {
        this->itemId     = id;
        this->itemName   = name;
        this->stage      = stg;
        this->isPrepared = prepared;
    }

    // ---- Update: toggles (or sets) the isPrepared flag ----
    void setIsPrepared(bool status) {
        this->isPrepared = status;
    }

    // ---- Accessors ----
    string getItemName()   const { return itemName;   }
    string getStage()      const { return stage;      }
    bool   getIsPrepared() const { return isPrepared; }

    // ---- toFileRecord: converts this object into a FileRecord for saving ----
    FileRecord toFileRecord() const {
        FileRecord rec;
        memset(&rec, 0, sizeof(FileRecord));          // zero-fill for clean padding
        rec.itemId      = this->itemId;
        rec.isPrepared  = this->isPrepared;
        strncpy(rec.itemName, this->itemName.c_str(), sizeof(rec.itemName) - 1);
        strncpy(rec.stage,    this->stage.c_str(),    sizeof(rec.stage)    - 1);
        return rec;
    }

    // ---- fromFileRecord: populates this object from a FileRecord read from disk ----
    void fromFileRecord(const FileRecord& rec) {
        this->itemId     = rec.itemId;
        this->itemName   = string(rec.itemName);
        this->stage      = string(rec.stage);
        this->isPrepared = rec.isPrepared;
    }

    // ---- Static accessor ----
    static int getTotalItems() { return totalItems; }

    // ---- Static increment: used by ChecklistManager when restoring from file ----
    static void incrementTotal() { totalItems++; }

    // ---- Friend function declaration ----
    friend void displayItem(const ChecklistItem& item);
};

// Static member definition
int ChecklistItem::totalItems = 0;

// ============================================================
// FRIEND FUNCTION: displayItem
// ============================================================
void displayItem(const ChecklistItem& item) {
    string status = item.isPrepared ? "Prepared" : "Not Prepared";
    cout << "  [ID: " << item.itemId << "]  "
         << item.itemName
         << "  |  " << status << "\n";
}

// ============================================================
// CLASS: ChecklistManager
//   Manages the in-memory array AND binary file persistence.
// ============================================================
class ChecklistManager {
private:
    static const int CAPACITY = 50;
    ChecklistItem    items[CAPACITY];
    int              count;
    string           dataFile;    // path to the binary data file

    // ---- isDuplicate: check in-memory array ----
    bool isDuplicate(int id) const {
        for (int i = 0; i < count; i++) {
            if (items[i].getItemId() == id)
                return true;
        }
        return false;
    }

    // ---- displayByStage: print all items matching a stage ----
    void displayByStage(const string& stageLabel) const {
        bool found = false;
        for (int i = 0; i < count; i++) {
            if (items[i].getStage() == stageLabel) {
                displayItem(items[i]);
                found = true;
            }
        }
        if (!found)
            cout << "  No items for this stage.\n";
    }

    // ---- findIndexById: returns array index of item with given ID, or -1 ----
    int findIndexById(int id) const {
        for (int i = 0; i < count; i++) {
            if (items[i].getItemId() == id)
                return i;
        }
        return -1;
    }

public:
    // ---- Constructor: sets the data file path and loads existing records ----
    ChecklistManager(const string& filename) : count(0), dataFile(filename) {
        loadFromFile();
    }

    // ---- loadFromFile: reads all binary records from disk into the array ----
    //   Uses seekg to position the read head at the start of each record.
    //   This is called once at startup so data survives between runs.
    void loadFromFile() {
        ifstream fin(dataFile.c_str(), ios::binary);
        if (!fin.is_open()) {
            // File does not exist yet — first run, nothing to load
            return;
        }

        // Find out how many records are stored
        fin.seekg(0, ios::end);
        long fileSize = (long)fin.tellg();
        int  numRecords = (int)(fileSize / sizeof(FileRecord));

        count = 0;
        for (int i = 0; i < numRecords && count < CAPACITY; i++) {
            // seekg to the exact byte offset of record i
            fin.seekg((long)(i * sizeof(FileRecord)), ios::beg);

            FileRecord rec;
            fin.read(reinterpret_cast<char*>(&rec), sizeof(FileRecord));
            if (!fin) break;    // read failed — stop

            items[count].fromFileRecord(rec);
            items[count].setIsPrepared(rec.isPrepared);  // ensure flag is set
            ChecklistItem::incrementTotal();                  // count loaded items
            count++;
        }
        fin.close();
        if (count > 0)
            cout << "  [FILE] Loaded " << count << " record(s) from " << dataFile << ".\n";
    }

    // ---- saveAllToFile: rewrites the entire file from the in-memory array ----
    //   Called after every Add or Update so the file always reflects memory.
    void saveAllToFile() const {
        ofstream fout(dataFile.c_str(), ios::binary | ios::trunc);
        if (!fout.is_open()) {
            cout << "  [!] Could not open data file for writing.\n";
            return;
        }
        for (int i = 0; i < count; i++) {
            FileRecord rec = items[i].toFileRecord();
            fout.write(reinterpret_cast<const char*>(&rec), sizeof(FileRecord));
        }
        fout.close();
    }

    // ================================================================
    // MENU OPERATIONS
    // ================================================================

    // ---- addItem ----
    void addItem() {
        if (count >= CAPACITY) {
            cout << "\n  [!] Checklist is full (50 items). Cannot add more.\n";
            return;
        }

        int    id;
        string name;
        int    stageChoice;
        int    statusChoice;
        string stageStr;
        bool   prepared;

        // --- ID ---
        cout << "\n  Enter Item ID (positive integer): ";
        id = readInt("  [!] Invalid. Enter a positive integer: ");
        while (id <= 0) {
            cout << "  [!] ID must be greater than zero. Try again: ";
            id = readInt("  [!] Invalid. Enter a positive integer: ");
        }
        if (isDuplicate(id)) {
            cout << "  [!] ID " << id << " already exists. Duplicate rejected.\n";
            return;
        }

        // --- Name ---
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
        if ((int)name.size() > 59) {
            cout << "  [!] Name too long (max 59 characters). Trimming.\n";
            name = name.substr(0, 59);
        }

        // --- Stage ---
        cout << "  Select Cyclone Stage:\n";
        cout << "    1. Before the Cyclone\n";
        cout << "    2. During the Cyclone\n";
        cout << "    3. After the Cyclone\n";
        cout << "  Enter choice (1/2/3): ";
        stageChoice = readInt("  [!] Invalid. Enter 1, 2, or 3: ");
        while (stageChoice < 1 || stageChoice > 3) {
            cout << "  [!] Please enter 1, 2, or 3: ";
            stageChoice = readInt("  [!] Invalid. Enter 1, 2, or 3: ");
        }
        if      (stageChoice == 1) stageStr = "Before";
        else if (stageChoice == 2) stageStr = "During";
        else                       stageStr = "After";

        // --- Status ---
        cout << "  Select Preparation Status:\n";
        cout << "    1. Prepared\n";
        cout << "    2. Not Prepared\n";
        cout << "  Enter choice (1/2): ";
        statusChoice = readInt("  [!] Invalid. Enter 1 or 2: ");
        while (statusChoice < 1 || statusChoice > 2) {
            cout << "  [!] Please enter 1 or 2: ";
            statusChoice = readInt("  [!] Invalid. Enter 1 or 2: ");
        }
        prepared = (statusChoice == 1);

        // Store using parameterised constructor + pointer
        ChecklistItem* ptr = &items[count];
        ptr->setData(id, name, stageStr, prepared);
        ChecklistItem::incrementTotal();   // increment shared counter
        count++;

        // Persist immediately
        saveAllToFile();

        cout << "  [OK] Item added and saved. Total items: " << count << "\n";
    }

    // ---- viewAll ----
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
        cout << "  Total items: " << count << "\n";
        cout << "  =========================================\n";
    }

    // ---- searchById: finds and displays one item by its ID ----
    //   Demonstrates seekg concept: in a real binary-only search we
    //   would use seekg(i * sizeof(FileRecord)) to jump directly to
    //   a record. Here we show the equivalent in-memory version
    //   (the file version below in searchByIdFromFile shows seekg).
    void searchById() const {
        cout << "\n  Enter Item ID to search: ";
        int id = readInt("  [!] Invalid. Enter a positive integer: ");

        int idx = findIndexById(id);
        if (idx == -1) {
            cout << "  [!] No item found with ID " << id << ".\n";
            return;
        }
        cout << "\n  Record found:\n";
        displayItem(items[idx]);
    }

    // ---- searchByIdFromFile: demonstrates seekg by reading ONE record
    //      directly from the binary file without loading the whole file ----
    void searchByIdFromFile() const {
        cout << "\n  Enter Item ID to search (direct file read with seekg): ";
        int searchId = readInt("  [!] Invalid. Enter a positive integer: ");

        ifstream fin(dataFile.c_str(), ios::binary);
        if (!fin.is_open()) {
            cout << "  [!] Data file not found. Add and save items first.\n";
            return;
        }

        fin.seekg(0, ios::end);
        long fileSize = (long)fin.tellg();
        int  numRecords = (int)(fileSize / sizeof(FileRecord));

        bool found = false;
        for (int i = 0; i < numRecords; i++) {
            // seekg to exactly the byte where record i begins
            fin.seekg((long)(i * sizeof(FileRecord)), ios::beg);

            FileRecord rec;
            fin.read(reinterpret_cast<char*>(&rec), sizeof(FileRecord));
            if (!fin) break;

            if (rec.itemId == searchId) {
                ChecklistItem tmp;
                tmp.fromFileRecord(rec);
                cout << "\n  Record found in file at record #" << i << ":\n";
                displayItem(tmp);
                found = true;
                break;
            }
        }
        fin.close();

        if (!found)
            cout << "  [!] ID " << searchId << " not found in file.\n";
    }

    // ---- searchByStage: displays all items matching a chosen stage ----
    void searchByStage() const {
        cout << "\n  Select stage to filter:\n";
        cout << "    1. Before the Cyclone\n";
        cout << "    2. During the Cyclone\n";
        cout << "    3. After the Cyclone\n";
        cout << "  Enter choice (1/2/3): ";
        int ch = readInt("  [!] Invalid. Enter 1, 2, or 3: ");
        while (ch < 1 || ch > 3) {
            cout << "  [!] Please enter 1, 2, or 3: ";
            ch = readInt("  [!] Invalid. Enter 1, 2, or 3: ");
        }

        string stageStr;
        if      (ch == 1) stageStr = "Before";
        else if (ch == 2) stageStr = "During";
        else              stageStr = "After";

        cout << "\n  Items for stage [" << stageStr << " the Cyclone]:\n";
        bool found = false;
        for (int i = 0; i < count; i++) {
            if (items[i].getStage() == stageStr) {
                displayItem(items[i]);
                found = true;
            }
        }
        if (!found)
            cout << "  No items found for this stage.\n";
    }

    // ---- updateStatus: toggles the isPrepared flag for a given ID ----
    void updateStatus() {
        if (count == 0) {
            cout << "\n  [!] Checklist is empty. Nothing to update.\n";
            return;
        }

        cout << "\n  Enter Item ID to update: ";
        int id = readInt("  [!] Invalid. Enter a positive integer: ");

        int idx = findIndexById(id);
        if (idx == -1) {
            cout << "  [!] No item found with ID " << id << ".\n";
            return;
        }

        // Show current status
        cout << "  Current status: "
             << (items[idx].getIsPrepared() ? "Prepared" : "Not Prepared") << "\n";
        cout << "  Select new status:\n";
        cout << "    1. Prepared\n";
        cout << "    2. Not Prepared\n";
        cout << "  Enter choice (1/2): ";
        int ch = readInt("  [!] Invalid. Enter 1 or 2: ");
        while (ch < 1 || ch > 2) {
            cout << "  [!] Please enter 1 or 2: ";
            ch = readInt("  [!] Invalid. Enter 1 or 2: ");
        }

        bool newStatus = (ch == 1);
        items[idx].setIsPrepared(newStatus);

        // Persist the change
        saveAllToFile();

        cout << "  [OK] Status updated to ["
             << (newStatus ? "Prepared" : "Not Prepared")
             << "]. File saved.\n";
    }
};

// ============================================================
// MAIN
// ============================================================
int main() {
    cout << "\n  =========================================\n";
    cout << "   CYCLONE PREPAREDNESS CHECKLIST MANAGER\n";
    cout << "         CAP455 - CA2 | Phase 2\n";
    cout << "  =========================================\n";

    // Data file lives in the data/ subdirectory
    ChecklistManager manager("data/checklist.dat");

    int choice;

    do {
        printSeparator();
        cout << "  MAIN MENU\n";
        cout << "  1. Add Checklist Item\n";
        cout << "  2. View All Checklist Items\n";
        cout << "  3. Search by ID (file seekg)\n";
        cout << "  4. Search by Stage\n";
        cout << "  5. Update Preparation Status\n";
        cout << "  6. Exit\n";
        printSeparator();
        cout << "  Enter your choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  [!] Invalid input. Enter a number from 1 to 6.\n";
            choice = 0;
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: manager.addItem();             break;
            case 2: manager.viewAll();             break;
            case 3: manager.searchByIdFromFile();  break;
            case 4: manager.searchByStage();       break;
            case 5: manager.updateStatus();        break;
            case 6:
                cout << "\n  Exiting. Stay prepared and stay safe!\n\n";
                break;
            default:
                cout << "  [!] Invalid choice. Enter 1–6.\n";
        }

    } while (choice != 6);

    return 0;
}
