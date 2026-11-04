# Application Flowchart — Cyclone Preparedness Checklist Manager

**Project:** Cyclone Preparedness Checklist Manager  
**Phase:** CA1 — Design & Create  
**Course:** CAP455 — Object-Oriented Programming using C++

---

## Overall Application Flow

```
[START]
   |
   v
Display banner / title
   |
   v
+---------------------+
|     MAIN MENU       |
| 1. Add Item         |
| 2. View Checklist   |
| 3. Exit             |
+---------------------+
   |
   |-- Read menu choice --|
   |                      |
   | (non-integer input)  v
   |               [!] Invalid input → loop back to menu
   |
   |---- choice == 1 ----> [ADD ITEM FLOW]
   |
   |---- choice == 2 ----> [VIEW FLOW]
   |
   |---- choice == 3 ----> Display "Exiting..." → [END]
   |
   +---- any other int --> [!] Invalid choice → loop back to menu
```

---

## Add Item Flow

```
[ADD ITEM]
   |
   v
Is checklist full? (count >= 50)
   |-- YES --> [!] "Checklist is full" → return to menu
   |
   NO
   |
   v
Prompt: "Enter Item ID"
   |
   v
Is input a valid integer?
   |-- NO  --> [!] "Invalid input" → re-prompt ID
   |
   YES
   |
   v
Is ID > 0?
   |-- NO  --> [!] "ID must be > zero" → re-prompt ID
   |
   YES
   |
   v
Is ID already in checklist? (isDuplicate)
   |-- YES --> [!] "Duplicate rejected" → return to menu
   |
   NO
   |
   v
Prompt: "Enter Item Name"
   |
   v
Is name empty or only whitespace?
   |-- YES --> [!] "Name cannot be empty" → return to menu
   |
   NO
   |
   v
Prompt: "Select Stage (1/2/3)"
   |
   v
Is input a valid integer 1–3?
   |-- NO  --> [!] "Please enter 1, 2, or 3" → re-prompt stage
   |
   YES
   |
   v
Map choice → stageStr ("Before" / "During" / "After")
   |
   v
Prompt: "Select Status (1/2)"
   |
   v
Is input a valid integer 1–2?
   |-- NO  --> [!] "Please enter 1 or 2" → re-prompt status
   |
   YES
   |
   v
Map choice → isPrepared (true / false)
   |
   v
Obtain pointer to items[count]
Call ptr->setData(id, name, stageStr, isPrepared)
   |
   v
count++
Increment totalItems (inside setData)
   |
   v
Display "[OK] Item added successfully. Total items: N"
   |
   v
Return to main menu
```

---

## View Checklist Flow

```
[VIEW CHECKLIST]
   |
   v
Is count == 0?
   |-- YES --> Display "Checklist is currently empty." → return to menu
   |
   NO
   |
   v
Display banner header
   |
   v
Display "--- BEFORE THE CYCLONE ---"
   |
   v
Iterate items[0..count-1]:
  Any item with stage == "Before"?
   |-- YES --> displayItem(item) [friend function prints ID, name, status]
   |-- NO  --> Display "No items added for this stage."
   |
   v
Display "--- DURING THE CYCLONE ---"
   |
   v
Iterate items[0..count-1]:
  Any item with stage == "During"?
   |-- YES --> displayItem(item)
   |-- NO  --> Display "No items added for this stage."
   |
   v
Display "--- AFTER THE CYCLONE ---"
   |
   v
Iterate items[0..count-1]:
  Any item with stage == "After"?
   |-- YES --> displayItem(item)
   |-- NO  --> Display "No items added for this stage."
   |
   v
Display total item count
   |
   v
Return to main menu
```

---

## Exit Flow

```
[EXIT — choice == 3]
   |
   v
Display "Exiting. Stay prepared and stay safe!"
   |
   v
while loop terminates (choice == 3)
   |
   v
main() returns 0
   |
   v
[END]
```

---

## Notes

- The application never exits unexpectedly on bad input. All loops re-prompt until valid input is supplied or EOF is reached.
- After a failed `addItem()` (duplicate, full array, empty name), control returns cleanly to the main menu. The user does not need to restart.
- The `displayItem` friend function accesses private members of `ChecklistItem` directly — no intermediate getters are needed for display.
