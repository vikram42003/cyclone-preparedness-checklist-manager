# Data Dictionary

**Project:** Cyclone Preparedness Checklist Manager  
**Course:** CAP455 — Object-Oriented Programming using C++  
**Updated:** CA2 — Persist & Search

---

## Entity: `ChecklistItem`

Represents one preparedness task. All four instance attributes are `private`.

| # | Attribute | C++ Type | Access | Description | Validation | Key |
|---|---|---|---|---|---|---|
| 1 | `itemId` | `int` | `private` | Unique integer identifier for the entry | Positive integer (> 0); no duplicates allowed | **Yes — Primary Key** |
| 2 | `itemName` | `std::string` | `private` | Name of the preparedness task (e.g., "Store drinking water") | Not empty or whitespace-only; max 59 chars (binary file limit) | No |
| 3 | `stage` | `std::string` | `private` | Cyclone stage: `"Before"`, `"During"`, or `"After"` | Input via numbered menu (1/2/3) only | No |
| 4 | `isPrepared` | `bool` | `private` | `true` = Prepared, `false` = Not Prepared | Input via numbered menu (1/2); can be updated (CA2+) | No |
| 5 | `totalItems` | `int` (static) | `private` | Shared counter across all `ChecklistItem` objects | Auto-managed by constructors and `incrementTotal()` | No |

---

## Struct: `FileRecord` *(CA2 — binary file I/O only)*

A Plain Old Data (POD) struct used exclusively for binary file I/O. `std::string` has variable size; `FileRecord` uses fixed-width `char` arrays so every record is the same number of bytes, enabling `seekg(i * sizeof(FileRecord))`.

| # | Field | C++ Type | Size | Description |
|---|---|---|---|---|
| 1 | `itemId` | `int` | 4 bytes | Copy of `ChecklistItem::itemId` |
| 2 | `itemName` | `char[60]` | 60 bytes | Fixed-width; null-terminated copy of `itemName` |
| 3 | `stage` | `char[10]` | 10 bytes | Fixed-width; null-terminated (`"Before"`, `"During"`, `"After"`) |
| 4 | `isPrepared` | `bool` | 1 byte | Copy of `isPrepared` |

> **Total:** ~76 bytes per record (compiler may add 1 byte padding).

---

## Data File: `data/checklist.dat`

| Property | Detail |
|---|---|
| Format | Binary (raw `FileRecord` structs) |
| Record size | `sizeof(FileRecord)` bytes — fixed |
| Location | `data/checklist.dat` (relative to project root) |
| Created | Automatically on the first successful Add |
| Updated | After every Add and every Update Status operation |
| Read | On application startup via `loadFromFile()` using `seekg` |

---

## Notes

- `FileRecord` is not an OOP entity. It is a helper struct for binary I/O only.
- In CA2, only `isPrepared` is updatable. `itemId`, `itemName`, and `stage` are identity fields.
- `itemName` is validated to ≤ 59 characters before storage to fit within `char itemName[60]`.
