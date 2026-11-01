# Project Report: Smart Expense Tracker

## 1. Introduction
Managing money starts with knowing where it goes. Smart Expense Tracker is a console program in C that lets a student record daily spending, see all records, find the total, search by category and delete wrong entries. Data is stored in a text file so it is never lost.

## 2. Objectives
1. Build a useful program using core C concepts.
2. Store data permanently using file handling.
3. Make the program safe against wrong input.
4. Keep the code simple, modular and well commented.

## 3. Problem Statement
Writing expenses on paper is slow, easy to lose and hard to total. A small computer program can store, search and add them instantly.

## 4. Scope
Single user, offline, console-based, up to 500 records.

## 5. Tools and Technologies
C language, GCC compiler, any text editor/IDE, plain text file storage.

## 6. System Design
**Data structure:** one `struct Expense` (id, amount, category, date, description). An array of 500 such structures holds the records in memory.

**Storage format:** each line of `expenses.txt` is `id|amount|category|date|description`. The `|` symbol separates fields; the program replaces any `|` typed by the user with `-`.

**Program flow:**
```
Start -> load expenses.txt into array -> show menu
   -> user choice -> run the matching function -> (save file if data changed)
   -> repeat until choice 6 -> Exit
```

**Functions used**
| Group | Functions |
|-------|-----------|
| Input/validation | readLine, readInt, readAmount, readDate, isValidDate, removeBar, toLowerStr |
| File handling | loadExpenses, saveExpenses, getNextId |
| Features | addExpense, viewExpenses, calculateTotal, searchExpenses, deleteExpense |
| Display | printBanner, printMenu, printTableHeader, printExpenseRow |

## 7. Implementation Details
- **Add:** gets next ID, reads and validates input, appends to the array, saves the file.
- **View:** loops through the array and prints a table.
- **Total:** loops and adds all amounts; also prints the average.
- **Search:** converts both the keyword and each category to lowercase, then uses `strstr()` to check a match.
- **Delete:** finds the position of the ID, asks confirmation, shifts later records left, reduces the count, saves the file.
- **Pointers:** `int *count` lets functions change the record count; `Expense *list` passes the array; `fgets`/`sscanf` use pointers to fill variables.
- **Validation:** `strtol` and `strtod` check that the whole input is a valid number; dates are checked for format, month range, days in month and leap years.

## 8. Testing
| Test | Input | Expected | Result |
|------|-------|----------|--------|
| Add expense | 250.50, Food, 15-08-2025, Lunch | Saved with ID 1 | Pass |
| Invalid amount | abc, -5 | Asks again | Pass |
| Invalid date | 31-02-2025, 32-13-2025 | Asks again | Pass |
| Leap-year date | 29-02-2024 | Accepted | Pass |
| Empty text | (just Enter) | Asks again | Pass |
| Invalid menu choice | 9, x | Error message, menu again | Pass |
| Total | 3 records | Correct sum and average | Pass |
| Search | "FOOD" / "xyz" | Matches regardless of case / "not found" | Pass |
| Delete | ID 1 then y | Record removed, file updated | Pass |
| Delete cancel | n | Nothing deleted | Pass |
| Delete missing ID | 77 | "No expense found" | Pass |
| Persistence | Exit and restart | Records still present | Pass |
| Text containing `|` | "Bus pass | monthly" | Saved as "Bus pass - monthly" | Pass |

## 9. Limitations
Maximum 500 records, no edit option, floating-point `float` amounts (small rounding for very large values), no password or multi-user support.

## 10. Future Scope
Edit expenses, monthly and category-wise reports, sorting by date or amount, budget alerts, graphical interface.

## 11. Conclusion
The project meets all required features and shows how structures, arrays, pointers, functions, strings, loops and file handling work together in a real, useful program.
