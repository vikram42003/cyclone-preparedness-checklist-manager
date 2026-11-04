# UML Class Diagram — Cyclone Preparedness Checklist Manager

**Project:** Cyclone Preparedness Checklist Manager  
**Phase:** CA1 — Design & Create  
**Course:** CAP455 — Object-Oriented Programming using C++

---

## Class Diagram

```
+---------------------------------------------+
|              <<class>>                      |
|            ChecklistItem                    |
+---------------------------------------------+
| - itemId     : int                          |
| - itemName   : std::string                  |
| - stage      : std::string                  |
| - isPrepared : bool                         |
| - totalItems : int  {static}                |
+---------------------------------------------+
| + getItemId()     : int   {inline}          |
| + setData(id: int, name: string,            |
|           stg: string,                      |
|           prepared: bool) : void            |
| + getStage()      : std::string             |
| + getIsPrepared() : bool                    |
| + getTotalItems() : int   {static}          |
+---------------------------------------------+
| <<friend>> displayItem(item: ChecklistItem) |
+---------------------------------------------+

                    uses ▼  (composition / "owns")

+---------------------------------------------+
|              <<class>>                      |
|           ChecklistManager                  |
+---------------------------------------------+
| - CAPACITY  : int = 50  {static const}      |
| - items     : ChecklistItem[50]             |
| - count     : int                           |
+---------------------------------------------+
| + ChecklistManager()   {constructor}        |
| + addItem()   : void                        |
| + viewAll()   : void  {const}               |
| - isDuplicate(id: int) : bool  {const}      |
| - displayByStage(stageLabel: string) : void |
|                                   {const}   |
+---------------------------------------------+

Free functions (not part of any class):
+---------------------------------------------+
| displayItem(item: const ChecklistItem&)     |
|   : void                                    |
|   <<friend of ChecklistItem>>               |
+---------------------------------------------+
| readInt(reprompt: const string&) : int      |
+---------------------------------------------+
| printSeparator() : void                     |
+---------------------------------------------+
```

---

## Relationships

| Relationship | Description |
|---|---|
| `ChecklistManager` **owns** `ChecklistItem[50]` | Composition — the array is a fixed member of `ChecklistManager`. Its lifetime is tied to `ChecklistManager`. |
| `displayItem` is **friend** of `ChecklistItem` | The free function `displayItem` has direct read access to all private members of `ChecklistItem`. |
| `ChecklistManager` **calls** `displayItem` | `displayByStage()` calls the friend function to print each matching item. |

---

## Notes on Required CA1 Concepts

| Concept | Location |
|---|---|
| Class and objects | `ChecklistItem` and `ChecklistManager` |
| Static member | `ChecklistItem::totalItems` (instance counter) |
| Inline function | `ChecklistItem::getItemId()` (defined inside class body) |
| Friend function | `displayItem()` (accesses private members of `ChecklistItem`) |
| Array of objects | `ChecklistItem items[50]` inside `ChecklistManager` |
| Pointer | `ChecklistItem* ptr = &items[count]` inside `addItem()` |
| `this` pointer | Used in `ChecklistItem::setData()` to assign attributes |
| `std::string` | `itemName` and `stage` attributes |
