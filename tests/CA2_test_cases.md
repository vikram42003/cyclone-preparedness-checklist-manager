# Test Cases — CA2: Persist & Search

**Project:** Cyclone Preparedness Checklist Manager  
**Phase:** CA2 — Persist & Search  
**Compiler:** GCC 16.1.0 (MSYS2/MinGW, Windows), C++11  
**Source:** `src/main.cpp`  
**Data file:** `data/checklist.dat` (binary)

---

## Test Cases

### TC-01: Fresh Start — No Data File

| Field | Detail |
|---|---|
| **Description** | Run the application when no `checklist.dat` file exists; view the checklist |
| **Pre-condition** | `data/checklist.dat` deleted |
| **Input** | Menu: 2 (View), 6 (Exit) |
| **Expected** | No "Loaded" message; "checklist is currently empty" shown |
| **Result** | ✅ PASS |

---

### TC-02: Add Three Items and View

| Field | Detail |
|---|---|
| **Description** | Add items in all three stages; verify all appear under the correct section |
| **Input** | Add ID 101 (Before, Prepared), 102 (During, Prepared), 103 (After, Not Prepared) → View |
| **Expected** | Each item under the correct stage header; total = 3 |
| **Result** | ✅ PASS |

---

### TC-03: Persistence — Data Survives Restart

| Field | Detail |
|---|---|
| **Description** | Exit and re-launch the application; previously added data must load automatically |
| **Input** | (fresh launch) Menu: 2 (View), 6 (Exit) |
| **Expected** | "[FILE] Loaded 3 record(s) from data/checklist.dat." All 3 items shown |
| **Result** | ✅ PASS |

---

### TC-04: Search by ID (seekg) — Found

| Field | Detail |
|---|---|
| **Description** | Use menu option 3 (direct file read using `seekg`) to find ID 102 |
| **Input** | Menu: 3 → ID: 102 |
| **Expected** | "Record found in file at record #1: [ID: 102] Monitor official alerts | Prepared" |
| **Result** | ✅ PASS |

---

### TC-05: Search by ID — Not Found

| Field | Detail |
|---|---|
| **Description** | Search for an ID that does not exist |
| **Input** | Menu: 3 → ID: 999 |
| **Expected** | "[!] ID 999 not found in file." |
| **Result** | ✅ PASS |

---

### TC-06: Search by Stage

| Field | Detail |
|---|---|
| **Description** | Filter items by the "During" stage |
| **Input** | Menu: 4 → Stage: 2 (During) |
| **Expected** | Only ID 102 displayed |
| **Result** | ✅ PASS |

---

### TC-07: Update Preparation Status

| Field | Detail |
|---|---|
| **Description** | Change ID 103 from Not Prepared to Prepared |
| **Input** | Menu: 5 → ID: 103 → Status: 1 (Prepared) → Menu: 2 (View) |
| **Expected** | "[OK] Status updated to [Prepared]. File saved." ID 103 shows Prepared in view. |
| **Result** | ✅ PASS |

---

### TC-08: Updated Status Survives Restart

| Field | Detail |
|---|---|
| **Description** | After TC-07, re-launch and verify ID 103 is still Prepared |
| **Input** | (fresh launch) Menu: 2, 6 |
| **Expected** | ID 103 shows "Prepared" |
| **Result** | ✅ PASS |

---

### TC-09: Duplicate ID Rejection + Invalid Menu Inputs

| Field | Detail |
|---|---|
| **Description** | Attempt to add existing ID 101; then enter "abc", 7, -1 at the menu |
| **Input** | Menu: 1 → ID: 101; then menu inputs: "abc", 7, -1, 6 |
| **Expected** | Duplicate rejected; each bad menu input shows appropriate error; app does not crash |
| **Result** | ✅ PASS |

---

## Summary

| Test | Description | Result |
|---|---|---|
| TC-01 | Fresh start, empty checklist | ✅ PASS |
| TC-02 | Add 3 items and view | ✅ PASS |
| TC-03 | Data persists after restart | ✅ PASS |
| TC-04 | Search by ID (seekg) — found | ✅ PASS |
| TC-05 | Search by ID — not found | ✅ PASS |
| TC-06 | Search by Stage | ✅ PASS |
| TC-07 | Update status | ✅ PASS |
| TC-08 | Updated status persists after restart | ✅ PASS |
| TC-09 | Duplicate rejection + invalid menu | ✅ PASS |

**9 of 9 test cases: ✅ PASS**
