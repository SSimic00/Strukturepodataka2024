#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//item Structure 
typedef struct Item {
    char name[50];
    int quantity;
    float price;
    struct Item* next;
} Item;

// Structure for an invoice
typedef struct Invoice {
    char date[11]; // YYYY-MM-DD
    Item* items; // Head list
    struct Invoice* next;
} Invoice;

// Function to create item
Item* createItem(char* name, int quantity, float price) {
    Item* item = (Item*)malloc(sizeof(Item));
    strcpy(item->name, name);
    item->quantity = quantity;
    item->price = price;
    item->next = NULL;
    return item;
}

// Function to add an item to a sorted list
Item* addItem(Item* head, Item* newItem) {
    if (!head || strcmp(newItem->name, head->name) < 0) {
        newItem->next = head;
        return newItem;
    }
    Item* current = head;
    while (current->next && strcmp(current->next->name, newItem->name) < 0) {
        current = current->next;
    }
    newItem->next = current->next;
    current->next = newItem;
    return head;
}

// Function to create a new invoice
Invoice* createInvoice(char* date) {
    Invoice* invoice = (Invoice*)malloc(sizeof(Invoice));
    strcpy(invoice->date, date);
    invoice->items = NULL;
    invoice->next = NULL;
    return invoice;
}

// Function to add an invoice to a sorted list
Invoice* addInvoice(Invoice* head, Invoice* newInvoice) {
    if (!head || strcmp(newInvoice->date, head->date) < 0) {
        newInvoice->next = head;
        return newInvoice;
    }
    Invoice* current = head;
    while (current->next && strcmp(current->next->date, newInvoice->date) < 0) {
        current = current->next;
    }
    newInvoice->next = current->next;
    current->next = newInvoice;
    return head;
}

// Function to calculate total spending on a specific item within a date range
void queryTotal(Invoice* head, char* itemName, char* startDate, char* endDate) {
    float totalCost = 0;
    int totalQuantity = 0;

    while (head) {
        if (strcmp(head->date, startDate) >= 0 && strcmp(head->date, endDate) <= 0) {
            Item* currentItem = head->items;
            while (currentItem) {
                if (strcmp(currentItem->name, itemName) == 0) {
                    totalCost += currentItem->price * currentItem->quantity;
                    totalQuantity += currentItem->quantity;
                }
                currentItem = currentItem->next;
            }
        }
        head = head->next;
    }

    printf("Total spent on '%s': %.2f\n", itemName, totalCost);
    printf("Total quantity purchased: %d\n", totalQuantity);
}

// Function to read invoices from file
Invoice* readInvoices(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return NULL;
    }

    Invoice* invoices = NULL;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char date[11];
        sscanf(line, "%s", date);
        Invoice* invoice = createInvoice(date);

        while (fgets(line, sizeof(line), file) && strchr(line, ',')) {
            char name[50];
            int quantity;
            float price;
            sscanf(line, "%[^,], %d, %f", name, &quantity, &price);
            Item* item = createItem(name, quantity, price);
            invoice->items = addItem(invoice->items, item);
        }

        invoices = addInvoice(invoices, invoice);
    }

    fclose(file);
    return invoices;
}

// Main function
int main() {
    Invoice* invoices = readInvoices("racuni.txt");
    if (!invoices) {
        return 1;
    }

    char itemName[50], startDate[11], endDate[11];
    printf("Enter item name: ");
    scanf("%s", itemName);
    printf("Enter start date (YYYY-MM-DD): ");
    scanf("%s", startDate);
    printf("Enter end date (YYYY-MM-DD): ");
    scanf("%s", endDate);

    queryTotal(invoices, itemName, startDate, endDate);

    return 0;
}
