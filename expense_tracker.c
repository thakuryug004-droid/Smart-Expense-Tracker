/*
 * ============================================================
 *  SMART EXPENSE TRACKER  -  A beginner-friendly C project
 * ============================================================
 *  Features : Add, View, Total, Search (by category), Delete
 *  Storage  : expenses.txt  (one expense per line, fields
 *             separated by the '|' character)
 *  Concepts : structures, arrays, functions, pointers,
 *             strings, loops, file handling
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FILE_NAME    "expenses.txt"
#define MAX_EXPENSES 500      /* maximum records we can hold in memory */
#define CAT_LEN      30
#define DATE_LEN     11       /* dd-mm-yyyy + '\0' */
#define DESC_LEN     60

/* One expense record */
typedef struct {
    int   id;
    float amount;
    char  category[CAT_LEN];
    char  date[DATE_LEN];
    char  description[DESC_LEN];
} Expense;

/* ---------- Function prototypes ---------- */
void  readLine(const char *prompt, char *buffer, int size);
int   readInt(const char *prompt);
float readAmount(const char *prompt);
void  readDate(const char *prompt, char *date);
int   isValidDate(const char *date);
void  toLowerStr(const char *src, char *dest);
void  removeBar(char *text);

int   loadExpenses(Expense *list);
void  saveExpenses(const Expense *list, int count);
int   getNextId(const Expense *list, int count);

void  addExpense(Expense *list, int *count);
void  viewExpenses(const Expense *list, int count);
void  calculateTotal(const Expense *list, int count);
void  searchExpenses(const Expense *list, int count);
void  deleteExpense(Expense *list, int *count);

void  printBanner(void);
void  printMenu(void);
void  printTableHeader(void);
void  printExpenseRow(const Expense *e);

/* ============================================================
 *  INPUT HELPERS  (these handle invalid input safely)
 * ============================================================ */

/* Read a full line of text. Re-asks if the user enters nothing. */
void readLine(const char *prompt, char *buffer, int size)
{
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, size, stdin) == NULL) {   /* input ended: quit safely */
            printf("\n  Input ended. Exiting.\n");
            exit(0);
        }

        /* if there is no newline, the line was too long: throw away the extra */
        if (strchr(buffer, '\n') == NULL) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) { }
        }
        buffer[strcspn(buffer, "\n")] = '\0';       /* remove the newline */

        if (strlen(buffer) > 0) {
            removeBar(buffer);
            return;
        }
        printf("  [!] Input cannot be empty. Try again.\n");
    }
}

/* Replace '|' with '-' because '|' is our file separator */
void removeBar(char *text)
{
    char *p;
    for (p = text; *p != '\0'; p++) {
        if (*p == '|') *p = '-';
    }
}

/* Read a whole number. Keeps asking until valid. */
int readInt(const char *prompt)
{
    char line[50];
    char *end;
    long value;

    while (1) {
        printf("%s", prompt);
        if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
        value = strtol(line, &end, 10);
        /* valid only if we read digits and nothing but spaces follow */
        if (end != line && (*end == '\n' || *end == '\0')) {
            return (int)value;
        }
        printf("  [!] Invalid number. Please enter digits only.\n");
    }
}

/* Read a positive money amount. Keeps asking until valid. */
float readAmount(const char *prompt)
{
    char line[50];
    char *end;
    double value;

    while (1) {
        printf("%s", prompt);
        if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
        value = strtod(line, &end);
        if (end != line && (*end == '\n' || *end == '\0') && value > 0 && value < 100000000) {
            return (float)value;
        }
        printf("  [!] Invalid amount. Enter a positive number (e.g. 250.50).\n");
    }
}

/* Check that a date looks like dd-mm-yyyy and is a real date */
int isValidDate(const char *date)
{
    int d, m, y, i;
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (strlen(date) != 10) return 0;
    for (i = 0; i < 10; i++) {
        if (i == 2 || i == 5) {
            if (date[i] != '-') return 0;
        } else if (!isdigit((unsigned char)date[i])) {
            return 0;
        }
    }
    sscanf(date, "%d-%d-%d", &d, &m, &y);
    if (y < 1900 || y > 2100 || m < 1 || m > 12) return 0;

    /* leap year: February gets 29 days */
    if ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0)) daysInMonth[1] = 29;

    return (d >= 1 && d <= daysInMonth[m - 1]);
}

/* Keep asking until the user types a valid date */
void readDate(const char *prompt, char *date)
{
    char line[50];
    while (1) {
        readLine(prompt, line, sizeof(line));
        if (isValidDate(line)) {
            strcpy(date, line);
            return;
        }
        printf("  [!] Invalid date. Use dd-mm-yyyy (e.g. 15-08-2025).\n");
    }
}

/* Copy src to dest in lowercase (used for case-insensitive search) */
void toLowerStr(const char *src, char *dest)
{
    while (*src != '\0') {
        *dest = (char)tolower((unsigned char)*src);
        src++;
        dest++;
    }
    *dest = '\0';
}

/* ============================================================
 *  FILE HANDLING
 * ============================================================ */

/* Read all records from expenses.txt into the array.
 * Returns how many records were loaded. */
int loadExpenses(Expense *list)
{
    FILE *fp = fopen(FILE_NAME, "r");
    char line[200];
    int count = 0;

    if (fp == NULL) return 0;      /* file doesn't exist yet: no records */

    while (count < MAX_EXPENSES && fgets(line, sizeof(line), fp) != NULL) {
        Expense *e = &list[count];     /* pointer to the current slot */
        /* line format:  id|amount|category|date|description */
        if (sscanf(line, "%d|%f|%29[^|]|%10[^|]|%59[^\n]",
                   &e->id, &e->amount, e->category, e->date, e->description) == 5) {
            count++;
        }
    }
    fclose(fp);
    return count;
}

/* Write ALL records back to the file (overwrites the old file) */
void saveExpenses(const Expense *list, int count)
{
    FILE *fp = fopen(FILE_NAME, "w");
    int i;

    if (fp == NULL) {
        printf("  [!] Error: could not save to %s\n", FILE_NAME);
        return;
    }
    for (i = 0; i < count; i++) {
        fprintf(fp, "%d|%.2f|%s|%s|%s\n", list[i].id, list[i].amount,
                list[i].category, list[i].date, list[i].description);
    }
    fclose(fp);
}

/* New ID = (largest existing ID) + 1, so IDs are never reused */
int getNextId(const Expense *list, int count)
{
    int i, max = 0;
    for (i = 0; i < count; i++) {
        if (list[i].id > max) max = list[i].id;
    }
    return max + 1;
}

/* ============================================================
 *  DISPLAY HELPERS
 * ============================================================ */

void printBanner(void)
{
    printf("\n");
    printf("  ==============================================\n");
    printf("  |          SMART EXPENSE TRACKER             |\n");
    printf("  |     Track your spending the easy way       |\n");
    printf("  ==============================================\n");
}

void printMenu(void)
{
    printf("\n  ---------------- MAIN MENU ----------------\n");
    printf("   1. Add New Expense\n");
    printf("   2. View All Expenses\n");
    printf("   3. Calculate Total Expenses\n");
    printf("   4. Search Expenses by Category\n");
    printf("   5. Delete an Expense (by ID)\n");
    printf("   6. Exit\n");
    printf("  --------------------------------------------\n");
}

void printTableHeader(void)
{
    printf("\n  %-4s %-11s %-12s %-14s %s\n", "ID", "Date", "Amount", "Category", "Description");
    printf("  ---- ----------- ------------ -------------- --------------------\n");
}

void printExpenseRow(const Expense *e)
{
    printf("  %-4d %-11s %-12.2f %-14s %s\n",
           e->id, e->date, e->amount, e->category, e->description);
}

/* ============================================================
 *  MAIN FEATURES
 * ============================================================ */

/* Feature 1: add a new expense */
void addExpense(Expense *list, int *count)
{
    Expense e;

    if (*count >= MAX_EXPENSES) {
        printf("  [!] Storage full. Delete some expenses first.\n");
        return;
    }

    printf("\n  --- Add New Expense ---\n");
    e.id     = getNextId(list, *count);
    e.amount = readAmount("  Enter amount        : ");
    readLine("  Enter category      : ", e.category, CAT_LEN);
    readDate("  Enter date (dd-mm-yyyy): ", e.date);
    readLine("  Enter description   : ", e.description, DESC_LEN);

    list[*count] = e;      /* put the new record at the end of the array */
    (*count)++;            /* increase record count through the pointer  */
    saveExpenses(list, *count);

    printf("\n  [OK] Expense added successfully with ID %d.\n", e.id);
}

/* Feature 2: show every expense */
void viewExpenses(const Expense *list, int count)
{
    int i;

    printf("\n  --- All Expenses ---\n");
    if (count == 0) {
        printf("  No expenses recorded yet.\n");
        return;
    }
    printTableHeader();
    for (i = 0; i < count; i++) {
        printExpenseRow(&list[i]);
    }
    printf("\n  Total records: %d\n", count);
}

/* Feature 3: add up all amounts */
void calculateTotal(const Expense *list, int count)
{
    int i;
    float total = 0;

    for (i = 0; i < count; i++) {
        total += list[i].amount;
    }
    printf("\n  --- Total Expenses ---\n");
    printf("  Number of expenses : %d\n", count);
    printf("  Total amount spent : %.2f\n", total);
    if (count > 0) {
        printf("  Average per expense: %.2f\n", total / count);
    }
}

/* Feature 4: search by category (not case sensitive, partial match ok) */
void searchExpenses(const Expense *list, int count)
{
    char key[CAT_LEN], keyLower[CAT_LEN], catLower[CAT_LEN];
    int i, found = 0;
    float subtotal = 0;

    printf("\n  --- Search by Category ---\n");
    readLine("  Enter category to search: ", key, CAT_LEN);
    toLowerStr(key, keyLower);

    for (i = 0; i < count; i++) {
        toLowerStr(list[i].category, catLower);
        if (strstr(catLower, keyLower) != NULL) {   /* is keyLower inside catLower? */
            if (!found) printTableHeader();
            printExpenseRow(&list[i]);
            subtotal += list[i].amount;
            found++;
        }
    }

    if (found == 0) {
        printf("  No expenses found for category \"%s\".\n", key);
    } else {
        printf("\n  Found %d record(s). Subtotal: %.2f\n", found, subtotal);
    }
}

/* Feature 5: delete one expense by its ID */
void deleteExpense(Expense *list, int *count)
{
    int id, i, pos = -1;
    char answer[10];

    printf("\n  --- Delete Expense ---\n");
    if (*count == 0) {
        printf("  There are no expenses to delete.\n");
        return;
    }

    id = readInt("  Enter the ID to delete: ");

    for (i = 0; i < *count; i++) {       /* find the position of this ID */
        if (list[i].id == id) { pos = i; break; }
    }
    if (pos == -1) {
        printf("  [!] No expense found with ID %d.\n", id);
        return;
    }

    printTableHeader();
    printExpenseRow(&list[pos]);
    readLine("\n  Are you sure? (y/n): ", answer, sizeof(answer));
    if (tolower((unsigned char)answer[0]) != 'y') {
        printf("  Delete cancelled.\n");
        return;
    }

    /* shift all later records one place to the left */
    for (i = pos; i < *count - 1; i++) {
        list[i] = list[i + 1];
    }
    (*count)--;
    saveExpenses(list, *count);
    printf("  [OK] Expense with ID %d deleted.\n", id);
}

/* ============================================================
 *  MAIN
 * ============================================================ */
int main(void)
{
    Expense expenses[MAX_EXPENSES];            /* array of structures */
    int count = loadExpenses(expenses);        /* load saved data at start-up */
    int choice;

    printBanner();
    printf("\n  %d saved expense(s) loaded from %s\n", count, FILE_NAME);

    do {
        printMenu();
        choice = readInt("  Enter your choice (1-6): ");

        switch (choice) {
            case 1: addExpense(expenses, &count);     break;
            case 2: viewExpenses(expenses, count);    break;
            case 3: calculateTotal(expenses, count);  break;
            case 4: searchExpenses(expenses, count);  break;
            case 5: deleteExpense(expenses, &count);  break;
            case 6: printf("\n  Data saved. Thank you for using Smart Expense Tracker. Goodbye!\n\n"); break;
            default: printf("  [!] Invalid choice. Please enter a number from 1 to 6.\n");
        }
    } while (choice != 6);

    return 0;
}
