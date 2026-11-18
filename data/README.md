# Data Directory

This directory contains the binary data file used by the application from CA2 onwards.

## File: `checklist.dat`

- **Format:** Binary (raw `FileRecord` structs, fixed-size)
- **Record size:** `sizeof(FileRecord)` bytes (~76 bytes each)
- **Created:** Automatically when the first checklist item is added
- **Updated:** After every Add and Update Status operation
- **Read:** On application startup using `fstream` and `seekg`

The file is **not** human-readable. Use the application's View or Search options to inspect data.

> `checklist.dat` is listed in `.gitignore` and is **not** committed to version control.  
> This directory itself is committed so the path `data/` exists when the application runs.
