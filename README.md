# README — Cyclone Preparedness Checklist Manager

**Course:** CAP455 — Object-Oriented Programming using C++  
**Session:** 2026–27  
**Phase:** CA1 — Design & Create

---

## Project Description

The **Cyclone Preparedness Checklist Manager** is a simple console-based C++ application that helps a single household maintain a checklist of preparedness tasks organized by the three stages of a cyclone:

- Before the Cyclone  
- During the Cyclone  
- After the Cyclone

Each checklist entry records a task ID, task name, its cyclone stage, and whether it is Prepared or Not Prepared.

---

## Features Implemented in CA1

| Feature | Status |
|---|---|
| Add a checklist item (with ID, name, stage, and preparation status) | ✅ Implemented |
| View all checklist items grouped by cyclone stage | ✅ Implemented |
| Input validation (ID, name, stage, status, menu) | ✅ Implemented |
| Duplicate ID prevention | ✅ Implemented |
| Fixed-size in-memory array of objects (capacity: 50) | ✅ Implemented |
| Static member, inline function, friend function, pointer, `this` pointer, `std::string` | ✅ Implemented |

---

## C++ Requirements

- **Standard:** C++11 or later
- **Compiler:** Any standard C++ compiler (tested with GCC 16.1.0 via MSYS2 on Windows)
- **Dependencies:** None (uses only `<iostream>`, `<string>`, `<limits>`)

---

## Compilation Instructions

```bash
g++ -std=c++11 -Wall -Wextra -o cyclone_manager src/main.cpp
```

On Windows using MSYS2/MinGW:

```bash
g++ -std=c++11 -Wall -Wextra -o cyclone_manager.exe src/main.cpp
```

---

## Execution Instructions

```bash
./cyclone_manager        # Linux / macOS
cyclone_manager.exe      # Windows
```

Use the numbered menu to add items and view the checklist. Enter `3` to exit.

---

## Repository Structure

```
cyclone-preparedness-checklist-manager/
│
├── docs/
│   ├── proposal.md          # Project proposal (CA1 submission)
│   ├── data_dictionary.md   # Data dictionary for ChecklistItem
│   ├── uml.md               # UML class diagram
│   ├── flowchart.md         # Application flowchart
│   └── test_cases.md        # Test cases and expected results
│
├── src/
│   └── main.cpp             # Complete C++ source code
│
├── data/
│   └── README.md            # Note: CA1 uses no persistent storage
│
├── tests/
│   └── CA1_test_cases.md    # Detailed test log
│
├── cyclone_manager.exe      # Compiled executable (Windows)
└── README.md                # This file
```

---

## Known Limitations (CA1)

- Data is **not saved** between runs — all records are in-memory only.
- No search, update, or delete functionality (planned for CA2 and CA3).
- Maximum of 50 checklist entries per session.
- Single household — no multi-user or authentication support.

---

## Future Phase Overview

| Phase | Focus | Planned Features |
|---|---|---|
| CA2 | Persist & Search | Save/load checklist to/from file; search by ID or stage |
| CA3 | Reuse & Delete | Inheritance/reuse; delete and update entries |
| CA4 | Integrate & Defend | Integration, final testing, viva |

---

## AI Assistance Disclosure

<!-- 
  PLACEHOLDER: Please complete this section accurately to reflect 
  the actual extent of AI assistance used in this project.
-->

*[To be completed by the student before submission.]*

---

## Author

**Name:** [Your Name]  
**Roll Number:** [Your Roll Number]  
**Course:** MCA — CAP455 OOP using C++  
**Institution:** [Your Institution Name]
