# Cyclone Preparedness Checklist Manager
## Project Report — Phase CA1: Design & Create

---

| Field | Detail |
|---|---|
| **Student Name** | [Your Name] |
| **Roll Number** | [Your Roll Number] |
| **Course** | MCA — CAP455: Object-Oriented Programming using C++ |
| **Session** | 2026–27 |
| **Phase** | CA1 — Design & Create |
| **Due Date** | 7 days from allocation |
| **Faculty** | [Faculty Name] |
| **Institution** | [Institution Name] |
| **Submission Date** | November 2026 |
| **File Name** | `[RollNo]_CAP455_CA1_CycloneChecklistManager` |

---

## AI Assistance Disclosure

> *[Complete this section accurately before submission. Describe which parts of the code, documentation, and design were assisted by AI tools, and confirm that you can explain and modify every line independently.]*

---

## Table of Contents

1. [Abstract](#1-abstract)
2. [Introduction and Problem Statement](#2-introduction-and-problem-statement)
3. [Project Objectives](#3-project-objectives)
4. [Proposed Solution and Scope](#4-proposed-solution-and-scope)
5. [Entity Description and Data Dictionary](#5-entity-description-and-data-dictionary)
6. [Class Design and OOP Concepts](#6-class-design-and-oop-concepts)
7. [UML Class Diagram](#7-uml-class-diagram)
8. [Application Flowchart and Validation Strategy](#8-application-flowchart-and-validation-strategy)
9. [Test Cases and Results](#9-test-cases-and-results)
10. [Phase Roadmap and References](#10-phase-roadmap-and-references)

---

## 1. Abstract

Cyclones are destructive natural events that unfold across three distinct phases: the period before the storm arrives, the period during which it is active, and the recovery period afterward. Each phase demands specific household actions — stockpiling supplies, following safety protocols, and assessing damage. Without a structured tool, preparation efforts are informal, incomplete, and difficult to track.

This report describes the **Cyclone Preparedness Checklist Manager**, a console-based C++ application developed for a single household. The application allows users to add preparedness tasks to a checklist, categorize them by cyclone stage (Before, During, or After), record whether each task is completed, and view the full checklist organized by stage.

The project is an individual assignment for CAP455 — Object-Oriented Programming using C++ (Session 2026–27), divided into four assessed phases. This document covers Phase CA1 (Design & Create), which focuses on in-memory Create and Read operations using fundamental C++ object-oriented programming concepts from Units I–II of the syllabus: classes, objects, static members, inline functions, friend functions, arrays of objects, pointers, the `this` pointer, and `std::string`.

The application is implemented in a single C++ source file (`src/main.cpp`), compiles cleanly under C++11 with zero warnings, and passes all ten defined test cases covering normal, invalid, and boundary inputs.

---

## 2. Introduction and Problem Statement

### 2.1 Background

A cyclone passes through three distinct phases that each require different household actions:

| Phase | Key Household Actions |
|---|---|
| **Before** | Store water and food, prepare emergency kit, secure loose objects, monitor official alerts |
| **During** | Stay indoors, avoid windows, follow official instructions, do not use electrical equipment |
| **After** | Check surroundings for hazards, inspect the house for damage, report fallen power lines, assist neighbours |

Households in cyclone-prone regions — particularly coastal and low-lying areas — are routinely advised by civil authorities to maintain preparedness checklists. However, most households rely on informal methods: paper notes, memory, or verbal reminders. These methods are error-prone. Tasks may be forgotten, completed tasks may not be recorded, and there is often no clear separation between actions required at each stage.

### 2.2 Problem Statement

There is no simple, structured digital tool for a household to maintain a cyclone preparedness checklist organized by cyclone stage. A lightweight application that allows a household to record, categorize, and review preparedness tasks would meaningfully reduce the risk of oversight.

### 2.3 Why This Project

This project was selected for the following reasons:

1. The domain is practically meaningful and directly applicable to real-world safety.
2. The core data model — a list of categorized records — maps naturally onto the C++ concepts required by CA1 (classes, arrays of objects, attributes, member functions).
3. The application is small enough to implement completely within CA1 while demonstrating every required concept cleanly and without overengineering.
4. The domain is easy to understand and explain during a viva, since the problem being solved is familiar and intuitive.

### 2.4 Intended User

A single household managing one family's cyclone preparedness checklist. There is one checklist per application run. No multi-user, authentication, or household-switching functionality is included.

---

## 3. Project Objectives

### 3.1 Primary Objective

Build a working C++ console application that allows a household to:
- Add preparedness tasks (checklist items) with an ID, name, cyclone stage, and preparation status.
- View all tasks organized by cyclone stage (Before, During, After).
- Prevent duplicate entries by enforcing a unique item ID.

### 3.2 Academic Objectives

Demonstrate all C++ concepts required by CA1 (Units I–II of CAP455):

| Required Concept | Objective |
|---|---|
| Class and objects | Define `ChecklistItem` and `ChecklistManager` classes |
| Static member | Track the total number of items added |
| Inline function | Provide a simple, efficient getter for `itemId` |
| Friend function | Allow a display function to access private attributes directly |
| Array of objects | Maintain checklist entries in a fixed-size array |
| Pointer | Use a pointer to access the next free array slot |
| `this` pointer | Explicitly assign attributes in `setData()` |
| `std::string` | Store item names and cyclone stage values |

### 3.3 Quality Objectives

- Input validation must prevent invalid IDs, empty names, out-of-range menu selections, and non-integer input from crashing or corrupting the application.
- The source code must be readable and explainable by the student at the viva.
- All documentation must accurately describe the implemented application.

### 3.4 Scope Objective

Implement only CA1 functionality. No searching, updating, deleting, file saving, or other future-phase features are to be included.

---

## 4. Proposed Solution and Scope

### 4.1 Solution Overview

The application is a C++ console program with a three-option main menu:

```
MAIN MENU
1. Add Checklist Item
2. View All Checklist Items
3. Exit
```

**Add Checklist Item** prompts the user for four inputs — item ID, item name, cyclone stage (selected from a numbered menu), and preparation status (selected from a numbered menu). It validates each input before accepting it, checks for duplicate IDs, and stores the item in a fixed-size array.

**View All Checklist Items** displays all stored items grouped under three stage headers: BEFORE THE CYCLONE, DURING THE CYCLONE, and AFTER THE CYCLONE. If a stage has no entries, a suitable message is shown. If the checklist is empty, an empty-state message is shown.

**Exit** terminates the program cleanly.

### 4.2 Storage

All data is held in memory during the program's execution. A fixed-size array of 50 `ChecklistItem` objects is used. No file handling, database, or external storage is used in CA1.

```
ChecklistItem items[50];   // array of objects — capacity 50
int count = 0;             // tracks how many entries are occupied
```

### 4.3 In Scope (CA1)

| Feature | Included |
|---|---|
| Add a checklist item | ✅ Yes |
| View all items by stage | ✅ Yes |
| Input validation (all fields) | ✅ Yes |
| Duplicate ID prevention | ✅ Yes |
| In-memory array storage (50 items) | ✅ Yes |
| All required CA1 OOP concepts | ✅ Yes |

### 4.4 Explicitly Out of Scope (CA1)

The following features are **not** implemented in CA1 and are reserved for later phases:

| Feature | Reason Excluded |
|---|---|
| Search by ID, name, or stage | CA2 requirement |
| Save/load checklist to/from file | CA2 requirement |
| Update an existing item | CA3 requirement |
| Delete a checklist item | CA3 requirement |
| Inheritance and class reuse | CA3 requirement |
| Sorting of items | Not required in any phase |
| Multi-household or authentication | Outside project scope |
| Priority levels, categories, timestamps | Not part of agreed data model |

### 4.5 Known Limitations

- Data is not saved between runs. All entries are lost when the program exits.
- Maximum 50 items per session (array capacity).
- No search, update, or delete functionality in this phase.

---

## 5. Entity Description and Data Dictionary

### 5.1 Primary Entity: `ChecklistItem`

Represents one preparedness task in the household checklist. Implemented as a C++ class in `src/main.cpp`.

### 5.2 Attribute Table

| # | Attribute | C++ Type | Access | Description | Validation Rules | Unique Key |
|---|---|---|---|---|---|---|
| 1 | `itemId` | `int` | `private` | Integer identifier for the checklist entry, assigned by the user | Must be a positive integer (> 0). Must not duplicate any existing `itemId`. | **Yes — Primary Key** |
| 2 | `itemName` | `std::string` | `private` | The name or short description of the preparedness task (e.g., "Store drinking water") | Must not be empty. Must not consist entirely of whitespace characters. | No |
| 3 | `stage` | `std::string` | `private` | Cyclone stage: one of `"Before"`, `"During"`, or `"After"` | Input via numbered menu only (1 = Before, 2 = During, 3 = After). Any other value is rejected. | No |
| 4 | `isPrepared` | `bool` | `private` | Whether the task has been completed: `true` = Prepared, `false` = Not Prepared | Input via numbered menu only (1 = Prepared, 2 = Not Prepared). Any other value is rejected. | No |

### 5.3 Static Member

| Member | C++ Type | Access | Description |
|---|---|---|---|
| `totalItems` | `int` (static) | `private` | Class-level counter. Incremented every time `setData()` is called. Shared by all `ChecklistItem` objects. |

### 5.4 Sample Records

| ID | Item Name | Stage | Prepared |
|---|---|---|---|
| 101 | Store drinking water | Before | Yes |
| 102 | Prepare emergency kit | Before | No |
| 103 | Monitor official alerts | During | Yes |
| 104 | Stay away from windows | During | No |
| 105 | Check surroundings for hazards | After | No |
| 106 | Inspect the house for damage when safe | After | Yes |

These are illustrative examples. Users supply their own entries at runtime.

### 5.5 Design Notes

- All four instance attributes are `private`. They are accessed externally through member functions or the `displayItem` friend function.
- `stage` is stored as a `std::string` (`"Before"`, `"During"`, `"After"`) rather than an integer code or enumeration, to keep the implementation simple and readable.
- `isPrepared` is a `bool` as specified in the project requirements.
- All fields are mandatory when adding an item.

---

## 6. Class Design and OOP Concepts

### 6.1 Class: `ChecklistItem`

**Responsibility:** Encapsulates the data and simple operations for a single checklist entry.

**Private attributes:**

```cpp
int    itemId;
string itemName;
string stage;
bool   isPrepared;
static int totalItems;   // shared across all ChecklistItem objects
```

**Public member functions:**

| Function | Signature | Description |
|---|---|---|
| `getItemId` | `inline int getItemId() const` | Returns `itemId`. Inline because it is defined inside the class body. |
| `setData` | `void setData(int, const string&, const string&, bool)` | Assigns all four attributes using `this` pointer. Increments `totalItems`. |
| `getStage` | `string getStage() const` | Returns `stage`. Used by `ChecklistManager` to filter by stage. |
| `getIsPrepared` | `bool getIsPrepared() const` | Returns `isPrepared`. Available for future use. |
| `getTotalItems` | `static int getTotalItems()` | Returns the shared item counter. |

**Friend function:**

```cpp
friend void displayItem(const ChecklistItem& item);
```

`displayItem` is declared a friend so it can read all four private attributes directly when printing a checklist entry. This avoids adding extra getters for `itemName` and `isPrepared` solely for display purposes.

### 6.2 Class: `ChecklistManager`

**Responsibility:** Maintains the array of checklist items and provides the user-facing Add and View operations.

**Private attributes:**

```cpp
static const int CAPACITY = 50;
ChecklistItem    items[CAPACITY];   // array of objects
int              count;             // number of occupied entries
```

**Public member functions:**

| Function | Description |
|---|---|
| `ChecklistManager()` | Constructor — initialises `count` to 0. |
| `addItem()` | Collects and validates all user input, checks for duplicates and capacity, then stores the item using a pointer. |
| `viewAll()` | Displays the entire checklist organized into three stage sections. |

**Private helper functions:**

| Function | Description |
|---|---|
| `isDuplicate(int id)` | Scans `items[0..count-1]` and returns `true` if any entry has the given `id`. |
| `displayByStage(string label)` | Iterates the array and calls `displayItem()` for each entry matching `label`. |

### 6.3 Free Functions

| Function | Purpose |
|---|---|
| `readInt(string reprompt)` | Reads one integer from stdin. Clears error flags and re-prompts on bad input. Used for all numeric input. |
| `displayItem(const ChecklistItem& item)` | Friend of `ChecklistItem`. Prints one entry using private members directly. |
| `printSeparator()` | Prints a visual divider line between menu sections. |

### 6.4 Required CA1 Concepts — Where Each Is Implemented

| Concept | Exact Location in `src/main.cpp` |
|---|---|
| **Class and objects** | `ChecklistItem` class (entity); `ChecklistManager` class (manager); `ChecklistManager manager;` in `main()` |
| **Static member** | `static int totalItems` declared in `ChecklistItem`; defined as `int ChecklistItem::totalItems = 0;` outside the class |
| **Inline function** | `inline int getItemId() const { return itemId; }` — defined inside the class body |
| **Friend function** | `friend void displayItem(const ChecklistItem& item);` declared in `ChecklistItem`; reads `item.itemId`, `item.itemName`, `item.stage`, `item.isPrepared` directly |
| **Array of objects** | `ChecklistItem items[CAPACITY];` inside `ChecklistManager` |
| **Pointer** | `ChecklistItem* ptr = &items[count];` in `addItem()`; followed by `ptr->setData(...)` |
| **`this` pointer** | `this->itemId = id; this->itemName = name; this->stage = stg; this->isPrepared = prepared;` inside `setData()` |
| **`std::string`** | `itemName` and `stage` attributes; function parameters; reprompt messages |

### 6.5 Why Two Classes?

Separating `ChecklistItem` from `ChecklistManager` follows the **Single Responsibility Principle** at a basic level:

- `ChecklistItem` knows about one task's data.
- `ChecklistManager` knows about the collection and the operations on it.

This separation makes the code easier to read, explain, and extend in future phases (e.g., CA2 could add a `save()` method to `ChecklistManager` without touching `ChecklistItem`).

---

## 7. UML Class Diagram

### 7.1 Class Diagram

```
┌──────────────────────────────────────────────────┐
│                  <<class>>                       │
│               ChecklistItem                      │
├──────────────────────────────────────────────────┤
│ - itemId       : int                             │
│ - itemName     : std::string                     │
│ - stage        : std::string                     │
│ - isPrepared   : bool                            │
│ - totalItems   : int   {static}                  │
├──────────────────────────────────────────────────┤
│ + getItemId()      : int   {inline}              │
│ + setData(id, name, stg, prepared) : void        │
│ + getStage()       : std::string                 │
│ + getIsPrepared()  : bool                        │
│ + getTotalItems()  : int   {static}              │
├──────────────────────────────────────────────────┤
│ <<friend>> displayItem(item : ChecklistItem)     │
└──────────────────────────────────────────────────┘
                        ▲
              composition (owns 50)
                        │
┌──────────────────────────────────────────────────┐
│                  <<class>>                       │
│              ChecklistManager                    │
├──────────────────────────────────────────────────┤
│ - CAPACITY     : int = 50   {static const}       │
│ - items        : ChecklistItem[50]               │
│ - count        : int                             │
├──────────────────────────────────────────────────┤
│ + ChecklistManager()                             │
│ + addItem()    : void                            │
│ + viewAll()    : void   {const}                  │
│ - isDuplicate(id : int) : bool   {const}         │
│ - displayByStage(label : string) : void  {const} │
└──────────────────────────────────────────────────┘

Free functions (outside any class):
┌──────────────────────────────────────────────────┐
│ displayItem(item : const ChecklistItem&) : void  │
│   <<friend of ChecklistItem>>                    │
│ readInt(reprompt : const string&) : int          │
│ printSeparator() : void                          │
└──────────────────────────────────────────────────┘
```

### 7.2 Relationship Description

| Relationship | Type | Description |
|---|---|---|
| `ChecklistManager` → `ChecklistItem[50]` | Composition | The array is a direct member of `ChecklistManager`. When `ChecklistManager` is destroyed, so are all items. |
| `displayItem` ↔ `ChecklistItem` | Friend | The free function has direct access to all private members of `ChecklistItem`. |
| `ChecklistManager` → `displayItem` | Uses | `displayByStage()` calls `displayItem()` for each matching entry. |

---

## 8. Application Flowchart and Validation Strategy

### 8.1 Top-Level Application Flow

```
START
  │
  ▼
Display banner ("CYCLONE PREPAREDNESS CHECKLIST MANAGER")
  │
  ▼
┌─────────────────────────────┐
│          MAIN MENU          │◄──────────────────────────────┐
│  1. Add Checklist Item      │                               │
│  2. View All Checklist Items│                               │
│  3. Exit                    │                               │
└─────────────────────────────┘                               │
  │                                                           │
  ▼ Read menu choice                                          │
  ├─ non-integer ──► "[!] Invalid input" ─────────────────────┤
  ├─ 1 ──────────► [ADD ITEM FLOW] ──────────── returns ──────┤
  ├─ 2 ──────────► [VIEW FLOW] ───────────────── returns ──────┤
  ├─ 3 ──────────► "Exiting..." ──► END
  └─ other int ──► "[!] Invalid choice" ──────────────────────┘
```

### 8.2 Add Item Flow

```
[ADD ITEM]
  │
  ├─ count >= 50? ──YES──► "[!] Checklist is full" ──► return to menu
  │
  NO ▼
  Prompt: "Enter Item ID (positive integer)"
  │
  ├─ not an integer? ──► "[!] Invalid input, try again" ──► re-prompt ID
  ├─ id <= 0?        ──► "[!] ID must be > zero"         ──► re-prompt ID
  ├─ isDuplicate(id)? ──► "[!] Duplicate rejected"        ──► return to menu
  │
  ▼
  Prompt: "Enter Item Name"  [getline]
  │
  ├─ empty or whitespace-only? ──► "[!] Name cannot be empty" ──► return to menu
  │
  ▼
  Prompt: "Select Stage (1/2/3)"
  │
  ├─ not 1/2/3? ──► "[!] Please enter 1, 2, or 3" ──► re-prompt stage
  │
  ▼  Map: 1→"Before", 2→"During", 3→"After"
  │
  Prompt: "Select Status (1/2)"
  │
  ├─ not 1/2? ──► "[!] Please enter 1 or 2" ──► re-prompt status
  │
  ▼  Map: 1→true (Prepared), 2→false (Not Prepared)
  │
  ChecklistItem* ptr = &items[count]
  ptr->setData(id, name, stage, isPrepared)
  count++
  │
  ▼
  "[OK] Item added successfully. Total items: N"
  │
  ▼
  return to main menu
```

### 8.3 View Checklist Flow

```
[VIEW CHECKLIST]
  │
  ├─ count == 0? ──► "Checklist is currently empty." ──► return to menu
  │
  ▼
  Display banner header
  │
  ▼
  "--- BEFORE THE CYCLONE ---"
  for each item in items[0..count-1]:
    if stage == "Before" → displayItem(item)   [friend function]
  if no match → "No items added for this stage."
  │
  ▼
  "--- DURING THE CYCLONE ---"
  for each item in items[0..count-1]:
    if stage == "During" → displayItem(item)
  if no match → "No items added for this stage."
  │
  ▼
  "--- AFTER THE CYCLONE ---"
  for each item in items[0..count-1]:
    if stage == "After" → displayItem(item)
  if no match → "No items added for this stage."
  │
  ▼
  Display total item count
  return to main menu
```

### 8.4 Validation Strategy Summary

| Input | Validation Approach |
|---|---|
| **Menu choice** | `cin >> int`. On failure: `cin.clear()` + `cin.ignore()`. Re-display menu. |
| **Item ID** | `readInt()` helper loops until a valid integer is entered. Separate `while (id <= 0)` loop rejects non-positive values. Duplicate check via `isDuplicate()`. |
| **Item name** | `getline(cin, name)` after `cin.ignore()`. Scan character-by-character to detect whitespace-only strings. |
| **Cyclone stage** | `readInt()` loop. `while (choice < 1 || choice > 3)` rejects out-of-range values. |
| **Preparation status** | `readInt()` loop. `while (choice < 1 || choice > 2)` rejects out-of-range values. |

The `readInt()` helper function centralizes the `cin.clear()` / `cin.ignore()` pattern to avoid code duplication and correctly handles the transition from `cin >>` to `getline`.

---

## 9. Test Cases and Results

### 9.1 Compilation

```bash
g++ -std=c++11 -Wall -Wextra -o cyclone_manager.exe src/main.cpp
# Output: (no warnings or errors)
# Exit code: 0
```

### 9.2 Test Results

| ID | Test Description | Input Summary | Expected Output | Result |
|---|---|---|---|---|
| TC-01 | View empty checklist | Menu: 2, 3 | "The checklist is currently empty." | ✅ PASS |
| TC-02 | Add valid item (Before, Prepared) and view | ID:101, name:"Store drinking water", stage:1, status:1 | Item appears under "BEFORE THE CYCLONE" as Prepared | ✅ PASS |
| TC-03 | Duplicate ID rejected | Add ID 101 twice | Second attempt: "[!] Duplicate rejected." | ✅ PASS |
| TC-04 | Negative and zero ID rejected | ID: -5 then 0 then 101 | "[!] ID must be greater than zero." twice, then proceeds | ✅ PASS |
| TC-05 | Empty name rejected | Name: (Enter — empty line) | "[!] Item name cannot be empty." | ✅ PASS |
| TC-06 | Invalid stage selection | Stage: 5, 0, then 1 | "[!] Please enter 1, 2, or 3:" twice, then accepts | ✅ PASS |
| TC-07 | Invalid status selection | Status: 5, 0, then 1 | "[!] Please enter 1 or 2:" twice, then accepts | ✅ PASS |
| TC-08 | Items display under correct stages | Add Before/During/After items | Each item under correct stage header; none misplaced | ✅ PASS |
| TC-09 | Non-integer at main menu | Menu: "abc" | "[!] Invalid input. Please enter 1, 2, or 3." No crash. | ✅ PASS |
| TC-10 | Out-of-range integers at main menu | Menu: 9, -1, 0, 3 | "[!] Invalid choice." three times, then exits | ✅ PASS |

### 9.3 TC-08 Detailed Output

```
  --- BEFORE THE CYCLONE ---
  [ID: 101]  Store drinking water  |  Prepared

  --- DURING THE CYCLONE ---
  [ID: 102]  Monitor alerts  |  Prepared

  --- AFTER THE CYCLONE ---
  [ID: 103]  Check hazards  |  Not Prepared

  Total items: 3
```

### 9.4 Array Capacity Boundary (TC-11)

The boundary check `if (count >= CAPACITY)` is present in `addItem()`. When `count` reaches 50, the application prints `"[!] Checklist is full. Cannot add more items."` and returns to the menu. This was verified by code review. The check is a single conditional — straightforward and correct.

### 9.5 Summary

**10 of 10 test cases executed and passed.** The application handles all tested normal, invalid, and boundary inputs correctly and does not crash on any tested input.

---

## 10. Phase Roadmap and References

### 10.1 Four-Phase Project Roadmap

| Phase | Deadline | Units | Focus | Key Features |
|---|---|---|---|---|
| **CA1** (current) | Day 7 | I–II | Design & Create | Entity class with static/inline/friend; array of objects; pointers; `this`; in-memory Create & Read; input validation; no duplicates |
| **CA2** | Day 14 | III | Persist & Search | Default, parameterised, and copy constructors; destructor; `fstream` file I/O; binary fixed-size records; search with `seekg`; basic update; data must survive a restart |
| **CA3** | Day 28 | IV–V | Reuse & Delete | ≥2 derived classes; operator overloading (`<<`, `>>`, `==`, `<`, +); `new`/`delete`; virtual functions; virtual destructor; safe Update & Delete with confirmation |
| **CA4** | Day 42 | VI | Integrate & Defend | Full CRUD menu; `try`/`catch`/`throw`; custom exception class; function and class templates; STL (`vector`/`list` with `sort`, `find_if`, `count_if`); ≥1 advanced feature (backup, CSV export, or undo stack); live demo and viva |

### 10.2 CA1 Marking Criteria Checklist

Each CA has 5 criteria worth 5 marks each (25 marks per phase).

| # | CA1 Criterion | Status |
|---|---|---|
| 1 | Design & UML — class diagram, relationships, attribute types | ✅ Complete |
| 2 | Class features — static member, inline function, friend function | ✅ Complete |
| 3 | Pointers, arrays, strings — `this`, pointer to object, array of objects, `std::string` | ✅ Complete |
| 4 | Create & Read — Add Item with validation; View by stage; duplicate prevention | ✅ Complete |
| 5 | Docs & Code quality — proposal, data dictionary, flowchart, test cases, clean code, viva readiness | ✅ Complete |

### 10.3 Submission Checklist (CA1)

- [x] Ten-page project proposal (`docs/proposal.md`)
- [x] Data dictionary (`docs/data_dictionary.md`)
- [x] UML class diagram (`docs/uml.md`)
- [x] Application flowchart (`docs/flowchart.md`)
- [x] C++ source code (`src/main.cpp`)
- [x] Test cases with results (`docs/test_cases.md`)
- [x] README with compilation and execution instructions
- [x] Version-controlled repository with meaningful commits
- [ ] AI assistance disclosure — **complete before submission**
- [ ] Personal details filled in (name, roll number, institution, faculty)

### 10.4 Repository Structure

```
[RollNo]_CAP455_CA1_CycloneChecklistManager/
│
├── docs/
│   ├── proposal.md           ← This document (10-page report)
│   ├── data_dictionary.md    ← ChecklistItem attribute table
│   ├── uml.md                ← UML class diagram
│   ├── flowchart.md          ← Add item and view flowcharts
│   ├── test_cases.md         ← 10 test cases with results
│   └── CA1_summary.md        ← Implementation summary and viva Q&A
│
├── src/
│   └── main.cpp              ← Complete C++ source (single file)
│
├── data/
│   └── README.md             ← Placeholder (no file I/O in CA1)
│
├── tests/
│   └── CA1_test_cases.md     ← Test log
│
├── cyclone_manager.exe       ← Compiled Windows executable
└── README.md                 ← Project overview and build instructions
```

### 10.5 Compilation and Execution

```bash
# Compile (from project root)
g++ -std=c++11 -Wall -Wextra -o cyclone_manager.exe src/main.cpp

# Run
cyclone_manager.exe
```

No external libraries or dependencies are required. The application uses only `<iostream>`, `<string>`, and `<limits>` from the C++ standard library.

### 10.6 References

1. Stroustrup, B. (2013). *The C++ Programming Language* (4th ed.). Addison-Wesley Professional.
2. Lippman, S. B., Lajoie, J., & Moo, B. E. (2012). *C++ Primer* (5th ed.). Addison-Wesley Professional.
3. Schildt, H. (2003). *C++: The Complete Reference* (4th ed.). McGraw-Hill/Osborne.
4. cppreference.com. *C++ Reference*. Retrieved November 2026 from https://en.cppreference.com
5. National Disaster Management Authority (NDMA), India. *Cyclone Preparedness Guidelines*. Retrieved from https://ndma.gov.in
6. India Meteorological Department. *Cyclone Awareness and Preparedness*. Retrieved from https://mausam.imd.gov.in

---

*End of Report — CA1: Design & Create*  
*CAP455 — Object-Oriented Programming using C++ | Session 2026–27*
