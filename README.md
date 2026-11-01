# Smart Expense Tracker (C Programming)

A simple, menu-driven console application written in C that helps a user record daily expenses, view them, calculate the total, search by category and delete records. All data is saved in a text file (`expenses.txt`), so it is still there when the program is restarted.

## Objectives
- Apply C basics (structures, arrays, functions, pointers, strings, loops) in one practical project.
- Learn file handling by saving and loading records permanently.
- Practise input validation so the program does not crash on wrong input.

## Features
| # | Feature | What it does |
|---|---------|--------------|
| 1 | Add New Expense | Takes amount, category, date (dd-mm-yyyy) and description; gives an automatic ID |
| 2 | View All Expenses | Shows all records in a neat table |
| 3 | Calculate Total | Shows count, total amount and average |
| 4 | Search by Category | Case-insensitive, partial match (`foo` finds `Food`), with subtotal |
| 5 | Delete by ID | Shows the record, asks for confirmation, then deletes |
| 6 | Exit | Data is already saved; program closes safely |

Input validation: amount must be a positive number, the date must be a real date (leap years handled), menu choice and ID must be numbers, empty text is rejected.

## Technologies
- Language: C (standard C99, only `stdio.h`, `stdlib.h`, `string.h`, `ctype.h`)
- Compiler: GCC (or any C compiler)
- Storage: plain text file, one record per line: `id|amount|category|date|description`

## Project Folder Structure
```
SmartExpenseTracker/
├── README.md
├── src/
│   └── expense_tracker.c     <- complete source code
├── docs/
│   ├── REPORT.md             <- project report
│   ├── SPEECH.md             <- short presentation speech
│   └── VIVA_QUESTIONS.md     <- 25 viva questions with answers
└── expenses.txt              <- created automatically on first run
```

## How to Run
**Linux / macOS / Windows (MinGW or Git Bash):**
```bash
cd SmartExpenseTracker
gcc src/expense_tracker.c -o expense_tracker
./expense_tracker          # on Windows: expense_tracker.exe
```
Run it from the project folder so `expenses.txt` is created there.
You can also use Code::Blocks, Dev-C++ or VS Code: open `expense_tracker.c`, build and run.

## Demonstration Steps (for your viva / lab)
1. Start the program. It says `0 saved expense(s) loaded`.
2. Choose **1**, enter `250.50`, `Food`, `15-08-2025`, `Lunch at canteen`. Add two more (e.g. `1200`, `Transport`, `01-09-2025`, `Monthly bus pass`).
3. Choose **2** to show the table. Choose **3** to show the total (1450.50 for the first two).
4. Choose **4**, type `food` -> only Food records appear (shows case-insensitive search).
5. Show validation: type `abc` as the amount, `31-02-2025` as the date, `9` in the menu.
6. Choose **5**, enter ID `1`, answer `y` -> record deleted. Choose **2** to confirm.
7. Choose **6** to exit, run the program again and choose **2** -> the records are still there (file handling works). Open `expenses.txt` to show the stored data.

## Sample Output
```
  ==============================================
  |          SMART EXPENSE TRACKER             |
  |     Track your spending the easy way       |
  ==============================================

  0 saved expense(s) loaded from expenses.txt

  Enter your choice (1-6): 2

  --- All Expenses ---

  ID   Date        Amount       Category       Description
  ---- ----------- ------------ -------------- --------------------
  1    15-08-2025  250.50       Food           Lunch at canteen
  2    01-09-2025  1200.00      Transport      Monthly bus pass
  3    03-09-2025  499.00       Books          C programming textbook

  Total records: 3

  Enter your choice (1-6): 3

  --- Total Expenses ---
  Number of expenses : 3
  Total amount spent : 1949.50
  Average per expense: 649.83
```
Contents of `expenses.txt` after this session:
```
1|250.50|Food|15-08-2025|Lunch at canteen
2|1200.00|Transport|01-09-2025|Monthly bus pass
3|499.00|Books|03-09-2025|C programming textbook
```

## Limitations / Future Scope
Holds up to 500 records; no edit option; no monthly report. These can be added later (edit expense, monthly totals, sorting, a graphical front end).
