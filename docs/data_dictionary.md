# Data Dictionary — ChecklistItem

**Project:** Cyclone Preparedness Checklist Manager  
**Phase:** CA1 — Design & Create  
**Course:** CAP455 — Object-Oriented Programming using C++

---

## Entity: `ChecklistItem`

Represents a single preparedness task stored in the household checklist.  
Implemented as a C++ class in `src/main.cpp`.

---

## Attributes

| # | Attribute Name | C++ Type | Access | Description | Validation Rules | Unique Key |
|---|---|---|---|---|---|---|
| 1 | `itemId` | `int` | `private` | Unique integer identifier for the checklist entry. Assigned by the user at the time of adding a new item. | Must be a positive integer (> 0). Must not duplicate any existing `itemId` in the checklist. | **Yes** (Primary Key) |
| 2 | `itemName` | `std::string` | `private` | The name or short description of the preparedness task (e.g., "Store drinking water"). | Must not be empty. Must not consist entirely of whitespace characters (spaces, tabs). | No |
| 3 | `stage` | `std::string` | `private` | The cyclone stage to which this task belongs. Accepts only three values: `"Before"`, `"During"`, or `"After"`. | Input accepted via a numbered menu (1 = Before, 2 = During, 3 = After). Any other selection is rejected. | No |
| 4 | `isPrepared` | `bool` | `private` | Indicates whether the task has been prepared. `true` = Prepared; `false` = Not Prepared. | Input accepted via a numbered menu (1 = Prepared, 2 = Not Prepared). Any other selection is rejected. | No |

---

## Static Member

| Name | C++ Type | Access | Description |
|---|---|---|---|
| `totalItems` | `int` (static) | `private` | Class-level counter. Incremented each time `setData()` is called. Shared across all `ChecklistItem` objects. |

---

## Notes

- All four instance attributes are `private`. They are accessed externally through member functions or the `displayItem` friend function.
- `itemId` is the only unique key. No two items may share the same `itemId`.
- `stage` is stored as a `std::string` (`"Before"`, `"During"`, `"After"`) rather than an enumeration to keep the implementation simple.
- `isPrepared` is stored as a `bool` as specified in the project requirements.
- No optional or nullable attributes. All four fields must be supplied when adding an item.
- CA1 does not persist data. All attribute values exist in memory only during program execution.
