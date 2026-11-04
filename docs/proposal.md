# Project Proposal
## Cyclone Preparedness Checklist Manager

---

**Course:** CAP455 — Object-Oriented Programming using C++  
**Session:** 2026–27  
**Phase:** CA1 — Design & Create  
**Student Name:** [Your Name]  
**Roll Number:** [Your Roll Number]  
**Institution:** [Your Institution Name]  
**Submission Date:** [Date]  
**Supervisor / Faculty:** [Faculty Name]

---

## Abstract

Cyclones are powerful natural disasters that cause significant damage to life and property. Adequate preparation before, during, and after a cyclone can greatly reduce harm and aid recovery. However, households often struggle to keep track of the many preparation tasks required at each stage.

This project proposes the **Cyclone Preparedness Checklist Manager**, a simple console-based application developed in C++ that allows a single household to maintain a structured checklist of preparedness tasks. The application organizes tasks by the three stages of a cyclone — Before, During, and After — and records whether each task is Prepared or Not Prepared.

The project is developed as a phased individual assignment for CAP455 — Object-Oriented Programming using C++. Phase 1 (CA1) focuses on designing and implementing the core in-memory Create and Read operations using fundamental C++ object-oriented concepts including classes, objects, static members, inline functions, friend functions, arrays of objects, pointers, the `this` pointer, and `std::string`.

---

## 1. Introduction and Problem Statement

### 1.1 Background

A cyclone passes through three distinct phases: the period before it arrives, the period during which it is active, and the recovery period after it has passed. Each phase requires specific preparations. Before the cyclone, households must stockpile essential supplies, secure their homes, and monitor official advisories. During the cyclone, they must follow safety protocols indoors. After the cyclone, they must assess damage and report hazards.

Managing these tasks mentally or on paper is error-prone. Tasks may be forgotten, completed tasks may not be recorded, and the organization of tasks by cyclone stage may be unclear.

### 1.2 Problem Statement

A simple, structured checklist application can help households maintain clear visibility of their preparedness tasks. Without such a tool, preparation efforts are ad hoc and potentially incomplete.

### 1.3 Motivation

This project was selected because:
- Disaster preparedness is a practically relevant and important domain.
- The domain naturally models itself as a list of categorized records — a good fit for demonstrating OOP concepts.
- The application is small enough to implement completely in CA1 while demonstrating the required C++ features.

---

## 2. Project Objectives

The objectives of this project are:

1. **Primary:** Build a working console application that allows a household to add and view preparedness checklist items organized by cyclone stage.
2. **Academic:** Demonstrate the required C++ concepts from Units I–II of CAP455, including classes, objects, static members, inline functions, friend functions, arrays of objects, pointers, `this` pointer, and `std::string`.
3. **Quality:** Implement reliable input validation that prevents duplicate IDs, empty names, and invalid menu selections.
4. **Clarity:** Produce readable, well-organized source code that can be explained and defended in a viva.
5. **Documentation:** Prepare a complete set of submission documents (proposal, data dictionary, UML, flowchart, test cases) that accurately reflect the implemented application.

---

## 3. Proposed Solution

The proposed solution is a C++ console application that:

- Maintains a fixed-size in-memory array of checklist items (capacity: 50).
- Provides a simple three-option main menu: Add Item, View All Items, Exit.
- Allows users to add checklist items with four attributes: an integer ID, a string task name, a cyclone stage (Before / During / After), and a preparation status (Prepared / Not Prepared).
- Prevents duplicate item IDs.
- Displays all items organized into three clearly separated stage sections.
- Handles invalid input gracefully without crashing.

The solution is intentionally minimal. No file handling, searching, deletion, or update functionality is included in Phase 1.

---

## 4. Scope and Limitations

### 4.1 In Scope (CA1)

| Feature | Included |
|---|---|
| Add a checklist item | Yes |
| View all items by stage | Yes |
| Input validation | Yes |
| Duplicate ID prevention | Yes |
| In-memory storage (50 items max) | Yes |

### 4.2 Out of Scope (CA1)

The following features are explicitly outside the scope of CA1 and are reserved for future phases:

| Feature | Phase |
|---|---|
| Search for items by ID or name | CA2 |
| Save/load data to/from files | CA2 |
| Update an existing item | CA3 |
| Delete a checklist item | CA3 |
| Inheritance and class reuse | CA3 |
| Advanced integration and testing | CA4 |

### 4.3 Known Limitations

- Data is not saved between application runs. All entries are lost when the program exits.
- The application supports only one household. There is no multi-user, authentication, or household-switching functionality.
- The array capacity is fixed at 50 entries.
- No sorting of checklist items is performed.

---

## 5. Entity Description and Data Dictionary

The application uses one primary entity: `ChecklistItem`.

### 5.1 Entity: ChecklistItem

Represents a single preparedness task.

| Attribute | C++ Type | Description | Validation | Key |
|---|---|---|---|---|
| `itemId` | `int` | Unique integer identifier | Must be > 0; must not be a duplicate | **Primary Key** |
| `itemName` | `std::string` | Name of the task | Must not be empty or whitespace-only | No |
| `stage` | `std::string` | Cyclone stage: "Before", "During", or "After" | Accepted via numbered menu (1/2/3) only | No |
| `isPrepared` | `bool` | Preparation status: true = Prepared, false = Not Prepared | Accepted via numbered menu (1/2) only | No |

### 5.2 Supporting Static Member

| Member | Type | Description |
|---|---|---|
| `ChecklistItem::totalItems` | `int` (static) | Shared counter of items added across all objects |

---

## 6. Class Design and OOP Concepts

The application uses two classes.

### 6.1 Class: ChecklistItem

- **Purpose:** Encapsulates the data for one checklist entry.
- **Attributes:** Four private attributes as described in Section 5.
- **Methods:**
  - `setData()` — assigns all four attributes using the `this` pointer.
  - `getItemId()` — inline getter returning the item's ID.
  - `getStage()`, `getIsPrepared()` — simple accessors.
  - `getTotalItems()` — static function returning the shared item count.
- **Friend:** `displayItem()` — a free function that accesses private members directly for output.

### 6.2 Class: ChecklistManager

- **Purpose:** Manages the array of checklist items and provides menu operations.
- **Attributes:** Fixed array `ChecklistItem items[50]`, integer `count`, and static constant `CAPACITY = 50`.
- **Methods:**
  - `addItem()` — validates all input, checks for duplicates and capacity, then stores the new item using a pointer.
  - `viewAll()` — displays all items grouped by stage.
  - `isDuplicate()` — private helper to scan for an existing ID.
  - `displayByStage()` — private helper to display items matching a given stage.

### 6.3 Required OOP Concepts — Implementation Summary

| Concept | Location in Code |
|---|---|
| Class and objects | `ChecklistItem` and `ChecklistManager` classes |
| Static member | `ChecklistItem::totalItems` (counts all added items) |
| Inline function | `ChecklistItem::getItemId()` (defined inside class body) |
| Friend function | `displayItem()` reads private members of `ChecklistItem` directly |
| Array of objects | `ChecklistItem items[50]` in `ChecklistManager` |
| Pointer | `ChecklistItem* ptr = &items[count]` in `addItem()` |
| `this` pointer | `this->itemId = id; this->itemName = name;` in `setData()` |
| `std::string` | `itemName` and `stage` attributes |

---

## 7. UML Class Diagram

See [`docs/uml.md`](uml.md) for the full UML class diagram.

**Summary:**

```
ChecklistItem                        ChecklistManager
--------------                       ----------------
- itemId : int                       - CAPACITY : const int = 50
- itemName : std::string             - items : ChecklistItem[50]
- stage : std::string                - count : int
- isPrepared : bool                  ----------------------------
- totalItems : int {static}          + addItem() : void
--------------                       + viewAll() : void
+ getItemId() : int {inline}         - isDuplicate(id) : bool
+ setData(...) : void                - displayByStage(label) : void
+ getStage() : string
+ getIsPrepared() : bool
+ getTotalItems() : int {static}
<<friend>> displayItem()
```

`ChecklistManager` **owns** `ChecklistItem[50]` (composition).  
`displayItem` is a **friend** of `ChecklistItem`.

---

## 8. Application Flowchart and Validation Strategy

See [`docs/flowchart.md`](flowchart.md) for the full flowchart.

### 8.1 Summary

1. The application starts and displays a main menu.
2. The user selects Add Item, View All, or Exit.
3. For Add Item:
   - The user is prompted for ID, name, stage (via menu), and status (via menu).
   - Each input is validated before the next is requested.
   - If any validation fails, the operation is cancelled and the user returns to the main menu.
   - On success, the item is stored using a pointer and the total count is incremented.
4. For View All:
   - The array is scanned three times — once per stage.
   - Items matching each stage are displayed under their respective header.
   - If no items match a stage, "No items added for this stage." is shown.
5. Exit terminates the loop and the program ends cleanly.

### 8.2 Validation Strategy

| Input | Validation Applied |
|---|---|
| Menu choice | Must be a valid integer 1–3; non-integers handled with `cin.clear()` and `cin.ignore()` |
| Item ID | Must be a valid integer > 0; must not exist in the array |
| Item name | Must not be empty or only whitespace |
| Stage | Numeric menu 1–3; loop re-prompts until valid |
| Status | Numeric menu 1–2; loop re-prompts until valid |

A helper function `readInt()` encapsulates the integer-reading loop to avoid code repetition.

---

## 9. Testing Approach

The application is tested against the following categories:

| Category | Tests |
|---|---|
| Normal cases | Add valid items, view by stage |
| Duplicate key | Add same ID twice → rejected |
| Boundary — ID | Negative and zero IDs rejected |
| Invalid input — name | Empty name rejected |
| Invalid input — stage/status | Out-of-range menu values re-prompted |
| Invalid input — menu | Non-integers and out-of-range integers handled |
| Empty state | View with no items shows appropriate message |

See [`docs/test_cases.md`](test_cases.md) for detailed test cases and actual results.

**All 10 executed test cases passed.**

---

## 10. Phase Roadmap

| Phase | Timeline | Focus | Planned Features |
|---|---|---|---|
| **CA1** | Day 7 | Design & Create | In-memory Create and Read; core OOP concepts |
| **CA2** | Day 14 | Persist & Search | File-based save/load; search by ID, name, or stage |
| **CA3** | Day 28 | Reuse & Delete | Update status; delete items; possible class reuse/inheritance |
| **CA4** | Day 42 | Integrate & Defend | Integration testing; final polish; viva demonstration |

---

## References

1. Stroustrup, B. (2013). *The C++ Programming Language* (4th ed.). Addison-Wesley.
2. Lippman, S. B., Lajoie, J., & Moo, B. E. (2012). *C++ Primer* (5th ed.). Addison-Wesley.
3. Schildt, H. (2003). *C++: The Complete Reference* (4th ed.). McGraw-Hill.
4. National Disaster Management Authority (NDMA), India. Cyclone Preparedness Guidelines. Retrieved from https://ndma.gov.in
5. India Meteorological Department. Cyclone Awareness. Retrieved from https://mausam.imd.gov.in
6. cppreference.com — C++ Reference Documentation. Retrieved from https://en.cppreference.com

---

*End of Project Proposal — CA1*
