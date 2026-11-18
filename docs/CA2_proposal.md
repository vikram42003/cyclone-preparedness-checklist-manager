# Cyclone Preparedness Checklist Manager
## Project Report — Phase CA2: Persist & Search

---

| Field | Detail |
|---|---|
| **Student Name** | [Your Name] |
| **Roll Number** | [Your Roll Number] |
| **Course** | MCA — CAP455: Object-Oriented Programming using C++ |
| **Session** | 2026–27 |
| **Phase** | CA2 — Persist & Search |
| **Due Date** | 14 days from allocation |
| **Faculty** | [Faculty Name] |
| **Institution** | [Institution Name] |
| **Submission Date** | November 2026 |
| **File Name** | `[RollNo]_CAP455_CA2_CycloneChecklistManager` |

---

## AI Assistance Disclosure

> *[Complete this section accurately before submission.]*

---

## Table of Contents

1. [Abstract](#1-abstract)
2. [CA1 Recap and CA2 Build-On](#2-ca1-recap-and-ca2-build-on)
3. [CA2 Objectives](#3-ca2-objectives)
4. [Constructors and Destructor](#4-constructors-and-destructor)
5. [File Persistence Design](#5-file-persistence-design)
6. [Search and Update Features](#6-search-and-update-features)
7. [Updated Class Design and UML](#7-updated-class-design-and-uml)
8. [Application Flowchart — CA2 Operations](#8-application-flowchart--ca2-operations)
9. [Test Cases and Results](#9-test-cases-and-results)
10. [Phase Roadmap and References](#10-phase-roadmap-and-references)

---

## 1. Abstract

Phase CA2 (Persist & Search) extends the Cyclone Preparedness Checklist Manager built in CA1. The core addition is **binary file persistence using `fstream`**: checklist data now survives application restarts. CA2 also introduces all three constructors (default, parameterised, and copy) and an explicit destructor for the `ChecklistItem` class, satisfying Unit III requirements of CAP455.

Three new menu operations are added: Search by ID (which demonstrates `seekg` by reading directly from the binary file without loading the entire file), Search by Stage (which filters the in-memory array), and Update Preparation Status (which modifies an item and immediately saves the change to disk).

The application uses a fixed-size `FileRecord` Plain Old Data (POD) struct for binary I/O, since `std::string` cannot be written as a fixed-width block to a binary file. `ChecklistItem` converts to and from `FileRecord` using `toFileRecord()` and `fromFileRecord()` methods.

The application compiles with zero warnings under C++11 and passes all nine defined test cases including two persistence tests that verify data survives application restarts.

---

## 2. CA1 Recap and CA2 Build-On

### 2.1 What CA1 Delivered

CA1 implemented the core in-memory application:

- Two classes: `ChecklistItem` (entity) and `ChecklistManager` (collection manager).
- Static member (`totalItems`), inline getter (`getItemId`), friend function (`displayItem`).
- Fixed array of 50 `ChecklistItem` objects; pointer usage; `this` pointer in `setData()`.
- Two menu operations: Add Checklist Item, View All Checklist Items.
- Input validation for all fields; duplicate ID prevention.
- **Limitation:** all data was lost when the application exited.

### 2.2 What CA2 Adds

| CA1 | CA2 (new additions) |
|---|---|
| In-memory only | Binary file persistence — data survives restarts |
| No constructors defined | Default, parameterised, copy constructors + destructor |
| 2 menu options | 5 menu options (Add, View, Search by ID, Search by Stage, Update) |
| No file I/O | `fstream` binary read/write; `seekg` for direct record access |

All CA1 concepts are retained and unchanged.

---

## 3. CA2 Objectives

### 3.1 Academic Objectives (Unit III)

| Concept | Objective |
|---|---|
| Default constructor | Initialise a `ChecklistItem` to a safe empty state |
| Parameterised constructor | Create a fully initialised item in one statement |
| Copy constructor | Duplicate an existing item field-by-field |
| Destructor | Explicitly release resources (none in this case — body is empty) |
| `fstream` binary I/O | Save and load checklist records as fixed-size binary data |
| `seekg` | Jump to a specific record in the file by byte offset |

### 3.2 Functional Objectives

1. Data entered in one session must still be present in the next session.
2. The user can search for a checklist item by its ID, reading directly from the file.
3. The user can filter items by cyclone stage.
4. The user can update the preparation status of any item, with the change saved immediately.

### 3.3 Scope Objective

Implement only CA2 requirements. No deletion, inheritance, operator overloading, templates, or STL containers are used in this phase.

---

## 4. Constructors and Destructor

### 4.1 Default Constructor

```cpp
ChecklistItem() : itemId(0), itemName(""), stage(""), isPrepared(false) {
    // totalItems NOT incremented — an empty slot is not a real item
}
```

**Purpose:** When `ChecklistItem items[50]` is declared in `ChecklistManager`, the compiler calls this constructor 50 times — once per array element. Without a default constructor, the compiler-generated one would leave `int itemId` uninitialised (undefined behaviour). The explicit default constructor sets all fields to safe zero/empty values.

**Viva note:** The `totalItems` counter is **not** incremented in the default constructor because an empty slot in the array is not a real checklist entry. Only the parameterised constructor and the `addItem()` flow count real entries.

### 4.2 Parameterised Constructor

```cpp
ChecklistItem(int id, const string& name, const string& stg, bool prepared)
    : itemId(id), itemName(name), stage(stg), isPrepared(prepared) {
    totalItems++;
}
```

**Purpose:** Creates a fully initialised item in one statement. Increments `totalItems` because a real item is being created. This constructor is available for future phases (CA3 and CA4) where items may be created programmatically.

### 4.3 Copy Constructor

```cpp
ChecklistItem(const ChecklistItem& other)
    : itemId(other.itemId),
      itemName(other.itemName),
      stage(other.stage),
      isPrepared(other.isPrepared) {
    // Not incrementing totalItems: copy is not a new logical item
}
```

**Purpose:** Creates an exact copy of an existing `ChecklistItem`. The `std::string` members have their own copy constructors, so member-by-member copying is safe and correct. The counter is not incremented because a copy is not a new entry — it is the same item reproduced.

**Where it is used:** When `fromFileRecord()` populates a temporary `ChecklistItem tmp` and that object is then displayed (the copy constructor may be invoked when passing/returning objects by value).

### 4.4 Destructor

```cpp
~ChecklistItem() {
    // Nothing to free — body intentionally empty
}
```

**Purpose:** `ChecklistItem` does not use `new`/`delete` or any other resource that requires manual cleanup. The `std::string` members handle their own memory. The destructor is defined explicitly to satisfy the CA2 requirement and to demonstrate awareness of the concept. In CA3, when dynamic memory (`new`/`delete`) is introduced, the destructor will have real work to do.

---

## 5. File Persistence Design

### 5.1 Why Binary and Not Text?

A text file stores values as human-readable characters. The length of each line varies (e.g., "101\tStore drinking water\tBefore\ttrue\n" is different length from "9\tWater\tAfter\tfalse\n"). Variable-length records make it impossible to calculate the byte position of record `i` without reading all records before it — which defeats `seekg`.

A binary file with fixed-size records solves this: every record occupies exactly `sizeof(FileRecord)` bytes, so record `i` starts at byte `i × sizeof(FileRecord)`. `seekg` can jump there directly.

### 5.2 The `FileRecord` Struct

```cpp
struct FileRecord {
    int  itemId;        // 4 bytes
    char itemName[60];  // 60 bytes (59 chars + null terminator)
    char stage[10];     // 10 bytes ("Before\0", "During\0", "After\0")
    bool isPrepared;    // 1 byte
};
// Total: ~76 bytes (compiler may add 1 byte of padding)
```

`FileRecord` is a Plain Old Data (POD) struct — no constructors, no virtual functions, no member with dynamic allocation. This makes it safe to use with `fout.write(reinterpret_cast<const char*>(&rec), sizeof(FileRecord))`.

`ChecklistItem` retains `std::string` members for in-memory use. Two conversion methods bridge the two representations:

```cpp
FileRecord toFileRecord() const;    // ChecklistItem → FileRecord (for saving)
void fromFileRecord(const FileRecord& rec);  // FileRecord → ChecklistItem (for loading)
```

### 5.3 Writing to File (`saveAllToFile`)

```cpp
ofstream fout(dataFile.c_str(), ios::binary | ios::trunc);
for (int i = 0; i < count; i++) {
    FileRecord rec = items[i].toFileRecord();
    fout.write(reinterpret_cast<const char*>(&rec), sizeof(FileRecord));
}
fout.close();
```

- `ios::binary` — write raw bytes, no newline translation.
- `ios::trunc` — empty the file first, then rewrite all records from scratch. This avoids stale data beyond `count` lingering in the file.
- `reinterpret_cast<const char*>(&rec)` — tells `fout.write` to treat the struct as a raw byte sequence.
- Called after every Add and every Update.

### 5.4 Reading from File (`loadFromFile`) with `seekg`

```cpp
ifstream fin(dataFile.c_str(), ios::binary);

fin.seekg(0, ios::end);
long fileSize = (long)fin.tellg();
int  numRecords = (int)(fileSize / sizeof(FileRecord));

for (int i = 0; i < numRecords && count < CAPACITY; i++) {
    fin.seekg((long)(i * sizeof(FileRecord)), ios::beg);  // ← seekg
    FileRecord rec;
    fin.read(reinterpret_cast<char*>(&rec), sizeof(FileRecord));
    items[count].fromFileRecord(rec);
    ChecklistItem::incrementTotal();
    count++;
}
fin.close();
```

- First `seekg(0, ios::end)` measures the file size.
- Then `numRecords = fileSize / sizeof(FileRecord)` tells us how many records exist.
- Inside the loop, `seekg(i * sizeof(FileRecord), ios::beg)` jumps to exactly the right byte for record `i`.
- Called once at application startup from the `ChecklistManager` constructor.

### 5.5 Persistence Guarantee

Because `loadFromFile()` runs in the constructor and `saveAllToFile()` runs after every write operation, the file is always consistent with the in-memory array. The user never needs to manually save.

---

## 6. Search and Update Features

### 6.1 Search by ID — from File (`searchByIdFromFile`)

This is the primary demonstration of `seekg`. The application opens the binary file, iterates record-by-record using `seekg`, and compares each `rec.itemId` to the target. When found, it creates a temporary `ChecklistItem`, populates it via `fromFileRecord()`, and calls `displayItem()`.

```
Menu option 3 → Enter ID → seekg loop over file → found/not-found message
```

**Why read from the file and not just the array?** This explicitly demonstrates `seekg` for the assignment. The in-memory search (`findIndexById`) is used by Update for efficiency.

### 6.2 Search by Stage (`searchByStage`)

A simple linear scan of the in-memory array:

```
Menu option 4 → Select stage (1/2/3) → scan items[0..count-1] → display matches
```

If no items match the stage, a suitable message is displayed.

### 6.3 Update Preparation Status (`updateStatus`)

```
Menu option 5 → Enter ID → show current status → select new status → setIsPrepared() → saveAllToFile()
```

The update changes only the `isPrepared` boolean of one item in the array, then rewrites the entire file. The change is immediately persistent.

**What can be updated?** Only `isPrepared`. Item ID, name, and stage are identity fields — changing them would constitute a different item and is out of scope for CA2 (full update belongs to CA3).

---

## 7. Updated Class Design and UML

### 7.1 `ChecklistItem` — CA2 Additions

New members added in CA2 (shown in **bold**):

```
+--------------------------------------------------+
|                 <<class>>                        |
|              ChecklistItem                       |
+--------------------------------------------------+
| - itemId       : int                             |
| - itemName     : std::string                     |
| - stage        : std::string                     |
| - isPrepared   : bool                            |
| - totalItems   : int   {static}                  |
+--------------------------------------------------+
| + ChecklistItem()                   {default}    | ← NEW
| + ChecklistItem(id,name,stg,prep)   {param}      | ← NEW
| + ChecklistItem(other)              {copy}       | ← NEW
| + ~ChecklistItem()                  {destructor} | ← NEW
| + getItemId()    : int   {inline}                |
| + setData(...)   : void                          |
| + setIsPrepared(status) : void                   | ← NEW
| + getItemName()  : std::string                   | ← NEW
| + getStage()     : std::string                   |
| + getIsPrepared(): bool                          |
| + getTotalItems(): int   {static}                |
| + incrementTotal(): void  {static}               | ← NEW
| + toFileRecord() : FileRecord                    | ← NEW
| + fromFileRecord(rec) : void                     | ← NEW
+--------------------------------------------------+
| <<friend>> displayItem(item : ChecklistItem)     |
+--------------------------------------------------+

+--------------------------------------------------+
|                 <<struct>>                       |
|               FileRecord                         |
+--------------------------------------------------+
| + itemId     : int                               |
| + itemName   : char[60]                          |
| + stage      : char[10]                          |
| + isPrepared : bool                              |
+--------------------------------------------------+
  (used only for binary file I/O — not an OOP entity)

+--------------------------------------------------+
|                 <<class>>                        |
|              ChecklistManager                    |
+--------------------------------------------------+
| - CAPACITY  : int = 50   {static const}          |
| - items     : ChecklistItem[50]                  |
| - count     : int                                |
| - dataFile  : std::string                        | ← NEW
+--------------------------------------------------+
| + ChecklistManager(filename)                     | ← UPDATED
| + addItem()       : void                         |
| + viewAll()       : void   {const}               |
| + searchByIdFromFile() : void  {const}           | ← NEW
| + searchByStage()      : void  {const}           | ← NEW
| + updateStatus()       : void                    | ← NEW
| - loadFromFile()       : void                    | ← NEW
| - saveAllToFile()      : void  {const}           | ← NEW
| - isDuplicate(id)      : bool  {const}           |
| - displayByStage(label): void  {const}           |
| - findIndexById(id)    : int   {const}           | ← NEW
+--------------------------------------------------+
```

### 7.2 Relationships

| Relationship | Type |
|---|---|
| `ChecklistManager` **uses** `FileRecord` | Dependency — `saveAllToFile` and `loadFromFile` convert between `ChecklistItem` and `FileRecord` |
| `ChecklistItem` **converts to/from** `FileRecord` | Association via `toFileRecord()` and `fromFileRecord()` |
| `ChecklistManager` **owns** `ChecklistItem[50]` | Composition (unchanged from CA1) |

---

## 8. Application Flowchart — CA2 Operations

### 8.1 Application Startup (new in CA2)

```
START
  │
  ▼
Display banner
  │
  ▼
ChecklistManager constructor called
  │
  ▼
loadFromFile():
  ├─ File exists? ──NO──► (silent — first run)
  │
  YES ▼
  Measure file size with seekg(0, ios::end)
  Calculate numRecords = fileSize / sizeof(FileRecord)
  │
  for i = 0 to numRecords-1:
    seekg(i * sizeof(FileRecord), ios::beg)
    Read FileRecord
    items[count].fromFileRecord(rec)
    count++
  │
  ▼
  "[FILE] Loaded N record(s)" displayed
  │
  ▼
  MAIN MENU (1–6)
```

### 8.2 Add Item (CA2 — saves to file)

```
[ADD ITEM] (same validation as CA1, then:)
  │
  ▼
ptr->setData(id, name, stageStr, prepared)
ChecklistItem::incrementTotal()
count++
  │
  ▼
saveAllToFile()   ← new in CA2
  │
  ▼
"[OK] Item added and saved."
  │
  ▼
return to menu
```

### 8.3 Search by ID from File

```
[SEARCH BY ID — FILE]
  │
  ▼
Prompt: "Enter Item ID to search"
  │
  ▼
Open checklist.dat (ios::binary)
  ├─ File not open? ──► "[!] Data file not found" ──► return
  │
  ▼
seekg(0, ios::end) → measure file → calculate numRecords
  │
  for i = 0 to numRecords-1:
    seekg(i * sizeof(FileRecord), ios::beg)
    Read FileRecord rec
    rec.itemId == searchId?
      ├─ YES → fromFileRecord, displayItem → break
      └─ NO  → continue
  │
  ▼
Not found after all records?  "[!] ID not found in file."
  │
  ▼
return to menu
```

### 8.4 Update Status

```
[UPDATE STATUS]
  │
  ├─ count == 0? ──► "[!] Checklist is empty." ──► return
  │
  ▼
Prompt: "Enter ID to update"
findIndexById(id):
  ├─ -1 ──► "[!] Not found." ──► return
  │
  ▼
Show current status
Prompt: "Select new status (1/2)"
  │
  ▼
items[idx].setIsPrepared(newStatus)
saveAllToFile()
  │
  ▼
"[OK] Status updated. File saved."
  │
  ▼
return to menu
```

---

## 9. Test Cases and Results

### Compilation

```
g++ -std=c++11 -Wall -Wextra -o cyclone_manager.exe src/main.cpp
Exit code: 0   (zero warnings)
```

### Results

| ID | Description | Result |
|---|---|---|
| TC-01 | Fresh start, no data file — empty checklist shown | ✅ PASS |
| TC-02 | Add 3 items (Before/During/After) — all display correctly | ✅ PASS |
| TC-03 | Restart — 3 items loaded from file automatically | ✅ PASS |
| TC-04 | Search by ID 102 (seekg) — found at record #1 | ✅ PASS |
| TC-05 | Search by ID 999 — not found | ✅ PASS |
| TC-06 | Search by Stage "During" — ID 102 returned only | ✅ PASS |
| TC-07 | Update ID 103 (Not Prepared → Prepared) — "[OK] File saved." | ✅ PASS |
| TC-08 | Restart after update — ID 103 still shows Prepared | ✅ PASS |
| TC-09 | Duplicate ID rejected; "abc", 7, -1 at menu handled cleanly | ✅ PASS |

**9 of 9 test cases: ✅ PASS**

See [`tests/CA2_test_cases.md`](../tests/CA2_test_cases.md) for the full test log.

---

## 10. Phase Roadmap and References

### 10.1 Four-Phase Roadmap

| Phase | Status | Key Features |
|---|---|---|
| **CA1** | ✅ Done | In-memory Create & Read; entity class with static/inline/friend; array; pointers; `this`; validation |
| **CA2** | ✅ Done | Constructors & destructor; binary `fstream`; `seekg`; Search by ID and Stage; Update status; persistence |
| **CA3** | Upcoming | ≥2 derived classes; operator overloading (`<<`, `>>`, `==`, `<`); `new`/`delete`; virtual functions; virtual destructor; safe Delete with confirmation |
| **CA4** | Upcoming | Full CRUD; `try`/`catch`/`throw`; custom exception; function + class templates; STL `vector`/`list`; ≥1 advanced feature |

### 10.2 CA2 Marking Criteria Checklist

| # | Criterion | Status |
|---|---|---|
| 1 | Constructors & destructor — default, parameterised, copy, explicit destructor | ✅ |
| 2 | File persistence — `fstream` binary; fixed-size records; `ios::binary`, `ios::trunc` | ✅ |
| 3 | Reading records — `loadFromFile()` with `seekg`; data loads at startup | ✅ |
| 4 | Search & Update — Search by ID (file `seekg`), Search by Stage, Update status with save | ✅ |
| 5 | Docs & Code quality — proposal, updated UML, flowcharts, test cases, clean code | ✅ |

### 10.3 Repository Structure (CA2)

```
cyclone-preparedness-checklist-manager/
├── docs/
│   ├── CA1_summary.md        ← CA1 implementation summary + viva Q&A
│   ├── CA2_summary.md        ← CA2 implementation summary + viva Q&A
│   ├── CA2_proposal.md       ← This document
│   ├── data_dictionary.md    ← Updated for CA2
│   ├── flowchart.md          ← CA1 flowchart
│   ├── proposal.md           ← CA1 10-page report
│   ├── test_cases.md         ← CA1 test cases
│   └── uml.md                ← CA1 UML
├── src/
│   └── main.cpp              ← Complete C++ source (CA1 + CA2)
├── data/
│   ├── checklist.dat         ← Binary data file (auto-created)
│   └── README.md
├── tests/
│   ├── CA1_test_cases.md
│   └── CA2_test_cases.md     ← CA2 test log
├── cyclone_manager.exe
└── README.md
```

### 10.4 Compilation and Execution

```bash
g++ -std=c++11 -Wall -Wextra -o cyclone_manager.exe src/main.cpp
cyclone_manager.exe
```

The data file `data/checklist.dat` is created automatically on the first Add. Ensure the `data/` directory exists (it is in the repository).

### 10.5 References

1. Stroustrup, B. (2013). *The C++ Programming Language* (4th ed.). Addison-Wesley Professional.
2. Lippman, S. B., Lajoie, J., & Moo, B. E. (2012). *C++ Primer* (5th ed.). Addison-Wesley Professional.
3. Schildt, H. (2003). *C++: The Complete Reference* (4th ed.). McGraw-Hill.
4. cppreference.com — `fstream`, `seekg`, constructors. Retrieved November 2026 from https://en.cppreference.com
5. National Disaster Management Authority (NDMA), India. Retrieved from https://ndma.gov.in
6. India Meteorological Department. Retrieved from https://mausam.imd.gov.in

---

*End of Report — CA2: Persist & Search*  
*CAP455 — Object-Oriented Programming using C++ | Session 2026–27*
