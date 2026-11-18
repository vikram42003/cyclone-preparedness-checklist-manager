# README — Cyclone Preparedness Checklist Manager

**Course:** CAP455 — Object-Oriented Programming using C++  
**Session:** 2026–27  
**Current Phase:** CA2 — Persist & Search

---

## Project Description

The **Cyclone Preparedness Checklist Manager** is a C++ console application for a single household to maintain a checklist of preparedness tasks organized by the three stages of a cyclone:

- Before the Cyclone
- During the Cyclone
- After the Cyclone

Each entry records a task ID, name, stage, and whether it is Prepared or Not Prepared.

---

## Features by Phase

### CA1 — Design & Create ✅
- Add a checklist item (ID, name, stage, preparation status)
- View all items grouped by cyclone stage
- Input validation; duplicate ID prevention
- In-memory fixed-size array (50 items)

### CA2 — Persist & Search ✅
- **Binary file persistence** — data survives application restarts (`data/checklist.dat`)
- **Load on startup** — records read using `fstream` and `seekg`
- **Search by ID** — reads directly from binary file using `seekg`
- **Search by Stage** — filters the in-memory array
- **Update Preparation Status** — modifies and immediately saves to file
- Default, parameterised, and copy constructors + explicit destructor

---

## Menu (CA2)

```
1. Add Checklist Item
2. View All Checklist Items
3. Search by ID (file seekg)
4. Search by Stage
5. Update Preparation Status
6. Exit
```

---

## C++ Requirements

- **Standard:** C++11 or later
- **Compiler:** Any standard C++ compiler (tested with GCC 16.1.0 / MSYS2 on Windows)
- **Headers used:** `<iostream>`, `<string>`, `<limits>`, `<fstream>`, `<cstring>`

---

## Compilation

```bash
g++ -std=c++11 -Wall -Wextra -o cyclone_manager.exe src/main.cpp
```

---

## Execution

```bash
cyclone_manager.exe        # Windows
./cyclone_manager          # Linux / macOS
```

The file `data/checklist.dat` is created automatically when the first item is added.  
Ensure the `data/` directory exists (it is committed to the repository).

---

## Repository Structure

```
cyclone-preparedness-checklist-manager/
├── docs/
│   ├── proposal.md           ← CA1 10-page report
│   ├── CA2_proposal.md       ← CA2 10-page report
│   ├── CA1_summary.md        ← CA1 implementation summary + viva Q&A
│   ├── CA2_summary.md        ← CA2 implementation summary + viva Q&A
│   ├── data_dictionary.md    ← Attribute and file format reference
│   ├── flowchart.md          ← CA1 flowchart (CA2 flowcharts in CA2_proposal.md)
│   ├── test_cases.md         ← CA1 test cases
│   └── uml.md                ← CA1 UML
├── src/
│   └── main.cpp              ← All source code (CA1 + CA2)
├── data/
│   ├── checklist.dat         ← Binary data file (auto-created; gitignored)
│   └── README.md
├── tests/
│   ├── CA1_test_cases.md
│   └── CA2_test_cases.md
├── cyclone_manager.exe       ← Compiled executable (gitignored)
└── README.md
```

---

## Known Limitations

- Data file is binary and not human-readable.
- No delete functionality (planned for CA3).
- Only `isPrepared` can be updated; name and stage are immutable in CA2.
- Maximum 50 items per session / file.

---

## Future Phase Overview

| Phase | Focus | Planned |
|---|---|---|
| CA3 | Reuse & Delete | Inheritance; operator overloading; dynamic memory; delete |
| CA4 | Integrate & Defend | Templates; STL; exceptions; full CRUD; live demo |

---

## AI Assistance Disclosure

> *[Complete accurately before submission.]*

---

## Author

**Name:** [Your Name] | **Roll:** [Roll Number] | **Institution:** [Institution]
