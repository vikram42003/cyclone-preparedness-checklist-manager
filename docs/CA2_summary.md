# CA2 Summary — Persist & Search
## Cyclone Preparedness Checklist Manager | CAP455

---

## What CA2 Adds on Top of CA1

| Feature | Detail |
|---|---|
| **Default constructor** | `ChecklistItem()` — zero-initialises all fields; used for blank array slots |
| **Parameterised constructor** | `ChecklistItem(id, name, stage, prepared)` — creates a fully initialised item in one step |
| **Copy constructor** | `ChecklistItem(const ChecklistItem&)` — deep copies all fields; does not increment `totalItems` |
| **Destructor** | `~ChecklistItem()` — explicitly defined; no dynamic memory, so body is empty |
| **Binary file I/O** | `fstream` with `ios::binary`; `FileRecord` POD struct for fixed-size disk layout |
| **`seekg` for search** | `searchByIdFromFile()` jumps to each record's byte offset: `fin.seekg(i * sizeof(FileRecord), ios::beg)` |
| **Load on startup** | `loadFromFile()` called from constructor; data survives application restart |
| **Save on change** | `saveAllToFile()` called after every Add and Update |
| **Search by ID** | Menu option 3 — reads directly from file using `seekg` |
| **Search by Stage** | Menu option 4 — filters the in-memory array |
| **Update status** | Menu option 5 — changes `isPrepared` for a given ID; persists immediately |

---

## New Menu (CA2)

```
1. Add Checklist Item
2. View All Checklist Items
3. Search by ID (file seekg)
4. Search by Stage
5. Update Preparation Status
6. Exit
```

---

## Key Design Decision: `FileRecord` Struct

`std::string` is variable-length and cannot be written to a binary file as a fixed-size block.  
`FileRecord` uses fixed-width `char` arrays so every record occupies **exactly the same number of bytes**, making `seekg(i * sizeof(FileRecord))` reliable.

```cpp
struct FileRecord {
    int  itemId;        // 4 bytes
    char itemName[60];  // 60 bytes
    char stage[10];     // 10 bytes
    bool isPrepared;    // 1 byte
};
// Total: ~76 bytes per record (compiler may add 1 byte padding)
```

The `ChecklistItem` class converts to/from `FileRecord` using `toFileRecord()` and `fromFileRecord()`.

---

## Where Each CA2 Concept Is Implemented

| Concept | Location |
|---|---|
| **Default constructor** | `ChecklistItem::ChecklistItem()` — initialiser list sets all fields to zero/empty |
| **Parameterised constructor** | `ChecklistItem::ChecklistItem(int, string, string, bool)` |
| **Copy constructor** | `ChecklistItem::ChecklistItem(const ChecklistItem&)` |
| **Destructor** | `ChecklistItem::~ChecklistItem()` |
| **`fstream` write** | `saveAllToFile()` — opens with `ios::binary | ios::trunc`, writes `FileRecord` structs |
| **`fstream` read** | `loadFromFile()` — opens with `ios::binary`, reads `FileRecord` structs |
| **`seekg`** | `searchByIdFromFile()`: `fin.seekg(i * sizeof(FileRecord), ios::beg)` |
| **Search** | `searchByIdFromFile()` (file), `searchByStage()` (in-memory) |
| **Update** | `updateStatus()` — modifies `isPrepared` then calls `saveAllToFile()` |
| **Persistence** | Constructor calls `loadFromFile()`; Add and Update call `saveAllToFile()` |

---

## Test Results (CA2)

| # | Test | Result |
|---|---|---|
| TC-01 | Fresh start (no data file) — view shows empty | ✅ PASS |
| TC-02 | Add 3 items (Before/During/After) — view shows all correctly | ✅ PASS |
| TC-03 | Restart app — all 3 items loaded from file | ✅ PASS |
| TC-04 | Search by ID (seekg) — ID 102 found at record #1 | ✅ PASS |
| TC-05 | Search by ID — ID 999 not found | ✅ PASS |
| TC-06 | Search by Stage — During returns only ID 102 | ✅ PASS |
| TC-07 | Update status of ID 103 (Not Prepared → Prepared) | ✅ PASS |
| TC-08 | Restart after update — ID 103 now shows Prepared | ✅ PASS |
| TC-09 | Duplicate ID rejected; invalid menu inputs handled | ✅ PASS |

---

## Compilation & Execution

```bash
g++ -std=c++11 -Wall -Wextra -o cyclone_manager.exe src/main.cpp
cyclone_manager.exe
```

Data file saved to: `data/checklist.dat` (binary, auto-created on first Add).

---

## Viva Q&A — CA2

**Q: Why can't you write `std::string` directly to a binary file?**  
A: `std::string` stores its characters on the heap — writing the object copies only the internal pointer, not the actual string data. On the next run the pointer is invalid. `char` arrays store data inline, so `sizeof()` gives the true byte count and `fwrite`/`fout.write` copies the actual characters.

**Q: What is `seekg` and why did you use it?**  
A: `seekg` moves the file's read position to a specific byte offset. Because every `FileRecord` is the same size, record `i` always starts at byte `i × sizeof(FileRecord)`. I use `fin.seekg(i * sizeof(FileRecord), ios::beg)` to jump directly to any record without reading those before it.

**Q: What is the difference between the three constructors?**  
A: Default — creates an empty "blank" slot for the array; no counter increment. Parameterised — creates a real item with all four fields in one step; used in future phases. Copy — duplicates an existing item field-by-field; used when restoring from file or passing by value.

**Q: Why does the destructor have an empty body?**  
A: `ChecklistItem` only uses `std::string` (which manages its own memory) and `int`/`bool`. No `new` or `delete` is used, so there is nothing to free manually. The destructor is defined explicitly to satisfy the CA2 requirement.

**Q: How does persistence work across restarts?**  
A: The `ChecklistManager` constructor calls `loadFromFile()`, which opens the binary file, calculates the number of records from the file size, then uses `seekg` to read each `FileRecord`. After every Add or Update, `saveAllToFile()` rewrites the whole file. So on the next run the data is there.

**Q: What is `ios::trunc` and why do you use it in `saveAllToFile`?**  
A: `ios::trunc` truncates (empties) the file before writing. Since we rewrite every record from the in-memory array, we need a clean file — without `trunc`, old records beyond `count` would remain in the file.
