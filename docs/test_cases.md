# Test Cases — CA1

**Project:** Cyclone Preparedness Checklist Manager  
**Phase:** CA1 — Design & Create  
**Course:** CAP455 — Object-Oriented Programming using C++

---

## Test Environment

- **Compiler:** GCC 16.1.0 (MSYS2/MinGW, Windows)
- **Standard:** C++11
- **Source:** `src/main.cpp`
- **Executable:** `cyclone_manager.exe`
- **Compilation command:** `g++ -std=c++11 -Wall -Wextra -o cyclone_manager.exe src/main.cpp`

---

## Test Cases

### TC-01: View Empty Checklist

| Field | Detail |
|---|---|
| **Description** | View the checklist when no items have been added |
| **Input** | Menu: 2 (View), then 3 (Exit) |
| **Expected Output** | "The checklist is currently empty. Add items using option 1." |
| **Result** | ✅ PASS |

---

### TC-02: Add a Valid Checklist Item

| Field | Detail |
|---|---|
| **Description** | Add a single valid item (Before, Prepared) and verify it appears in the view |
| **Input** | Menu: 1 → ID: 101 → Name: "Store drinking water" → Stage: 1 (Before) → Status: 1 (Prepared) → Menu: 2 (View) → 3 (Exit) |
| **Expected Output** | "[OK] Item added successfully. Total items in checklist: 1" and display shows `[ID: 101]  Store drinking water  \|  Prepared` under "BEFORE THE CYCLONE" |
| **Result** | ✅ PASS |

---

### TC-03: Duplicate ID Prevention

| Field | Detail |
|---|---|
| **Description** | Attempt to add a second item with an ID that already exists |
| **Input** | Add ID 101 successfully, then attempt to add ID 101 again |
| **Expected Output** | "[!] An item with ID 101 already exists. Duplicate rejected." Second item is not stored. |
| **Result** | ✅ PASS |

---

### TC-04: Negative and Zero ID Rejection

| Field | Detail |
|---|---|
| **Description** | Enter -5 and then 0 as item IDs; both should be rejected with a re-prompt |
| **Input** | Menu: 1 → ID: -5 → ID: 0 → ID: 101 → valid name/stage/status |
| **Expected Output** | "[!] ID must be greater than zero. Try again:" (twice), then proceeds normally |
| **Result** | ✅ PASS |

---

### TC-05: Empty Name Rejection

| Field | Detail |
|---|---|
| **Description** | Enter an empty item name (press Enter without typing) |
| **Input** | Menu: 1 → ID: 101 → Name: (empty / press Enter) |
| **Expected Output** | "[!] Item name cannot be empty." Returns to main menu without storing the item. |
| **Result** | ✅ PASS |

---

### TC-06: Invalid Stage Selection

| Field | Detail |
|---|---|
| **Description** | Enter invalid stage values (5, 0) before entering a valid choice |
| **Input** | Valid ID and name, then stage: 5 → 0 → 1 (valid) |
| **Expected Output** | "[!] Please enter 1, 2, or 3:" (twice), then accepts stage 1 (Before) |
| **Result** | ✅ PASS |

---

### TC-07: Invalid Status Selection

| Field | Detail |
|---|---|
| **Description** | Enter invalid status values (5, 0) before entering a valid choice |
| **Input** | Valid ID, name, stage, then status: 5 → 0 → 1 (valid) |
| **Expected Output** | "[!] Please enter 1 or 2:" (twice), then accepts status 1 (Prepared) |
| **Result** | ✅ PASS |

---

### TC-08: Items in All Three Stages

| Field | Detail |
|---|---|
| **Description** | Add one item to each stage and verify all three sections appear correctly in the view |
| **Input** | Add ID 101 (Before, Prepared), ID 102 (During, Prepared), ID 103 (After, Not Prepared) → View |
| **Expected Output** | Each item appears under the correct stage header. No "No items" message shown for any stage. |
| **Result** | ✅ PASS |

---

### TC-09: Invalid Menu Input (Non-Integer)

| Field | Detail |
|---|---|
| **Description** | Enter a non-integer string at the main menu (e.g., "abc") |
| **Input** | Menu: "abc" |
| **Expected Output** | "[!] Invalid input. Please enter 1, 2, or 3." Loop continues and re-displays the menu. Application does not crash. |
| **Result** | ✅ PASS |

---

### TC-10: Invalid Menu Input (Out-of-Range Integer)

| Field | Detail |
|---|---|
| **Description** | Enter integers 9, -1, 0 at the main menu — none of which map to a valid option |
| **Input** | Menu: 9 → -1 → 0 → 3 (Exit) |
| **Expected Output** | "[!] Invalid choice. Please enter 1, 2, or 3." (three times), then exits cleanly. |
| **Result** | ✅ PASS |

---

## Summary

| Test | Description | Result |
|---|---|---|
| TC-01 | View empty checklist | ✅ PASS |
| TC-02 | Add valid item and view | ✅ PASS |
| TC-03 | Duplicate ID rejected | ✅ PASS |
| TC-04 | Negative and zero ID rejected | ✅ PASS |
| TC-05 | Empty name rejected | ✅ PASS |
| TC-06 | Invalid stage selection handled | ✅ PASS |
| TC-07 | Invalid status selection handled | ✅ PASS |
| TC-08 | Items display under correct stage | ✅ PASS |
| TC-09 | Non-integer menu input handled | ✅ PASS |
| TC-10 | Out-of-range menu input handled | ✅ PASS |

**Array capacity full (TC-11):** The 50-item limit is enforced by the `count >= CAPACITY` check inside `addItem()`. The message "[!] Checklist is full. Cannot add more items." is displayed. This was verified by code review (filling 50 items via script was not run due to time, but the boundary check is straightforward).

All 10 actively executed test cases passed.
