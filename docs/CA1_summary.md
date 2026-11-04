# CA1 — Cyclone Preparedness Checklist Manager: Implementation Summary

> [!IMPORTANT]
> Phase CA1 is **complete**. The application compiles with zero warnings, all 10 test cases pass, and all required documentation is in place.

---

## Repository Structure

```
cyclone-preparedness-checklist-manager/
├── src/
│   └── main.cpp              ← All C++ source code (single file)
├── docs/
│   ├── proposal.md           ← 10-section project proposal
│   ├── data_dictionary.md    ← ChecklistItem attribute table
│   ├── uml.md                ← UML class diagram (text-based)
│   ├── flowchart.md          ← Application + validation flowcharts
│   └── test_cases.md         ← All 10 test cases with results
├── data/
│   └── README.md             ← Placeholder (no file I/O in CA1)
├── tests/
│   └── CA1_test_cases.md     ← Test log (references docs/test_cases.md)
├── cyclone_manager.exe       ← Compiled Windows executable
└── README.md                 ← Project readme
```

---

## Compilation & Execution

```bash
# Compile
g++ -std=c++11 -Wall -Wextra -o cyclone_manager.exe src/main.cpp

# Run
cyclone_manager.exe
```

---

## Class Summary

### `ChecklistItem` — the entity class

| Member | Kind | Purpose |
|---|---|---|
| `itemId` | `private int` | Unique ID |
| `itemName` | `private std::string` | Task name |
| `stage` | `private std::string` | "Before" / "During" / "After" |
| `isPrepared` | `private bool` | Prepared status |
| `totalItems` | `private static int` | Shared item counter |
| `getItemId()` | `public inline` | Returns `itemId` |
| `setData()` | `public` | Assigns all 4 attrs using `this` |
| `getStage()` | `public` | Returns `stage` |
| `getIsPrepared()` | `public` | Returns `isPrepared` |
| `getTotalItems()` | `public static` | Returns `totalItems` |
| `displayItem()` | `friend function` | Prints item — reads private members directly |

### `ChecklistManager` — the manager class

| Member | Kind | Purpose |
|---|---|---|
| `CAPACITY` | `private static const int = 50` | Array limit |
| `items[50]` | `private ChecklistItem[]` | Array of objects |
| `count` | `private int` | Current number of stored items |
| `addItem()` | `public` | Full validated add flow |
| `viewAll()` | `public const` | Display by stage |
| `isDuplicate()` | `private const` | Checks for existing ID |
| `displayByStage()` | `private const` | Filters items by stage |

---

## Where Each CA1 Concept Is Used

| Concept | Exact location |
|---|---|
| **Class and objects** | `ChecklistItem` and `ChecklistManager` class definitions; `ChecklistManager manager` in `main()` |
| **Static member** | `static int totalItems` in `ChecklistItem`; defined as `int ChecklistItem::totalItems = 0;` outside the class |
| **Inline function** | `inline int getItemId() const` defined inside `ChecklistItem` class body |
| **Friend function** | `friend void displayItem(const ChecklistItem& item)` declared in `ChecklistItem`; defined as a free function |
| **Array of objects** | `ChecklistItem items[CAPACITY]` inside `ChecklistManager` |
| **Pointer** | `ChecklistItem* ptr = &items[count];` in `addItem()` |
| **`this` pointer** | `this->itemId = id; this->itemName = name; ...` inside `setData()` |
| **`std::string`** | `itemName` and `stage` attributes; function parameters; reprompt strings |

---

## Test Results

| # | Test | Result |
|---|---|---|
| TC-01 | View empty checklist | ✅ PASS |
| TC-02 | Add valid item and view | ✅ PASS |
| TC-03 | Duplicate ID rejected | ✅ PASS |
| TC-04 | Negative/zero ID rejected | ✅ PASS |
| TC-05 | Empty name rejected | ✅ PASS |
| TC-06 | Invalid stage handled | ✅ PASS |
| TC-07 | Invalid status handled | ✅ PASS |
| TC-08 | Items under correct stages | ✅ PASS |
| TC-09 | Non-integer menu input | ✅ PASS |
| TC-10 | Out-of-range menu input | ✅ PASS |

---

## Viva Preparation — Key Questions & Answers

### Q1: What is a static member? Why is `totalItems` static?
**A:** A static member belongs to the class, not to any individual object. All objects of the class share one copy of it. `totalItems` counts the total number of items added to the checklist across the entire program, so it makes sense for all `ChecklistItem` objects to share it. It is defined outside the class as `int ChecklistItem::totalItems = 0;` to actually allocate memory for it.

### Q2: What is an inline function? Where did you use it?
**A:** An inline function is one whose body is substituted at the call site by the compiler (instead of a regular function call). Defining a function inside the class body makes it implicitly inline. I used it for `getItemId()` — it is a simple getter that just returns one value, so inlining it is appropriate.

### Q3: What is a friend function? Why did you use it?
**A:** A friend function is a non-member function that is granted access to private (and protected) members of a class. I used `displayItem()` as a friend of `ChecklistItem` so it can directly read `itemId`, `itemName`, `stage`, and `isPrepared` to print them — without needing separate getters for each.

### Q4: Where did you use the `this` pointer?
**A:** In `ChecklistItem::setData()`. I wrote `this->itemId = id;` to explicitly show that `itemId` refers to the calling object's attribute, distinguishing it from the parameter named `id`. It is not strictly necessary here, but it demonstrates the concept clearly.

### Q5: Where did you use a pointer?
**A:** In `addItem()`: `ChecklistItem* ptr = &items[count];`. This creates a pointer to the next free slot in the array. I then call `ptr->setData(...)` to assign values to that slot. This demonstrates pointer usage to access an existing object.

### Q6: Why did you use a fixed-size array instead of a vector?
**A:** CA1 restricts us to basic arrays. `std::vector` and STL containers are not permitted in this phase. The fixed size of 50 is a reasonable upper bound for a household checklist.

### Q7: What happens if someone enters the same ID twice?
**A:** `isDuplicate()` scans the existing `items[0..count-1]` array. If any entry has the same `itemId`, it returns `true` and `addItem()` immediately prints "[!] Duplicate rejected." and returns without storing the new entry.

### Q8: How does the application handle non-integer input at the menu?
**A:** `cin >> choice` fails and sets `cin` into an error state. The code calls `cin.clear()` to clear the error flag and `cin.ignore(...)` to discard the bad input from the buffer. `choice` is set to `0` so the loop continues and re-displays the menu.

### Q9: What is the difference between your two classes?
**A:** `ChecklistItem` is an **entity** — it models a single data record with four attributes and operations on that record. `ChecklistManager` is a **manager** — it maintains the collection (array) and provides the user-facing operations (add and view). Separating these responsibilities is good OOP practice.

### Q10: What would you add in CA2?
**A:** File-based persistence (save and load the checklist using `fstream`) and a search feature (find an item by ID or stage). No CA2 functionality has been implemented in CA1.

---

## Outstanding Items

- [ ] Fill in your Name, Roll Number, Institution, Faculty Name, and Date in `docs/proposal.md` and `README.md`.
- [ ] Complete the AI assistance disclosure section in `README.md` accurately.
- [ ] Make your initial Git commit with a meaningful message, e.g.: `feat: CA1 complete - in-memory CRUD with OOP concepts`
- [ ] Add `.gitignore` to exclude the compiled `.exe` from version control if desired.
