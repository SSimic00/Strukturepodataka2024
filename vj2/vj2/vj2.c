#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE (256)
#define EXIT_SUCCESS 0

struct _person;
typedef struct _person* Position;

typedef struct _person {
    char name[MAX_LINE];
    char surname[MAX_LINE];
    int year;
    Position next;
} person;

Position Input(char*, char*, int);
Position Findbysurname(Position, char*);
Position findprevious(char*, Position);
int unosP(Position, Position);
int unosK(Position, Position);
int ispis(Position);
int delete(char*, Position);
int unosIza(char*, Position, Position);
int unosIspred(char*, Position, Position);
int unosDatoteka(char*, Position);
int ispisDatoteka(char*);

int main() {
    person head = { .name = {0}, .surname = {0}, .year = 0, .next = NULL };
    Position P = NULL;
    Position Q = NULL;
    Position tempp = NULL;
    char name[MAX_LINE] = { 0 };
    char surname[MAX_LINE] = { 0 };
    char temp[MAX_LINE] = { 0 };
    char filename[MAX_LINE] = { 0 };
    int year = 0, br = 0, izbor = 0, status = 0;

    printf("How many people do you want to enter? ");
    scanf("%d", &br);
    printf("\n");

    for (int i = 0; i < br; i++) {
        printf("Where do you want to place the person?\n");
        printf("\t1 - top\n\t2 - bottom\n");
        scanf("%d", &izbor);

        printf("\nName: ");
        scanf(" %s", name);
        printf("Surname: ");
        scanf(" %s", surname);
        printf("Year: ");
        scanf("%d", &year);
        printf("\n");

        P = Input(name, surname, year);
        if (P == NULL) {
            return EXIT_FAILURE;
        }

        switch (izbor) {
        case 1:
            unosP(&head, P);
            break;
        case 2:
            unosK(&head, P);
            break;
        default:
            printf("Invalid choice!\n");
            break;
        }
    }
    ispis(&head);

    printf("\nEnter the surname before which you want to enter the element: ");
    scanf(" %s", temp);

    printf("\nName: ");
    scanf(" %s", name);
    printf("Surname: ");
    scanf(" %s", surname);
    printf("Year: ");
    scanf("%d", &year);
    printf("\n");

    P = Input(name, surname, year);
    if (P == NULL) {
        return EXIT_FAILURE;
    }
    status = unosIspred(temp, &head, P);
    if (status != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }
    ispis(&head);

    printf("\nEnter the surname after which you want to enter the element: ");
    scanf(" %s", temp);

    printf("\nName: ");
    scanf(" %s", name);
    printf("Surname: ");
    scanf(" %s", surname);
    printf("Year: ");
    scanf("%d", &year);
    printf("\n");

    P = Input(name, surname, year);
    if (P == NULL) {
        return EXIT_FAILURE;
    }
    unosIza(temp, &head, P);
    ispis(&head);

    printf("\nWrite the surname you want to find: ");
    scanf(" %s", temp);

    Q = Findbysurname(&head, temp);
    if (Q != NULL) {
        printf("%s %s %d\n", Q->name, Q->surname, Q->year);
    }
    else {
        printf("Person with surname %s not found.\n", temp);
    }

    printf("\nWrite the surname you want to delete: ");
    scanf(" %s", temp);
    delete(temp, &head);
    ispis(&head);
    printf("\n");

    printf("Name of the file: ");
    scanf(" %s", filename);
    printf("\n");

    unosDatoteka(filename, &head);
    ispisDatoteka(filename);

    ispis(&head);

    while (head.next != NULL) {
        tempp = head.next;
        head.next = head.next->next;
        free(tempp);
    }

    return EXIT_SUCCESS;
}

Position Input(char* name, char* surname, int year) {
    Position p = (Position)malloc(sizeof(person));
    if (!p) {
        printf("Memory allocation failed.\n");
        return NULL;
    }
    strcpy(p->name, name);
    strcpy(p->surname, surname);
    p->year = year;
    p->next = NULL;
    return p;
}

int unosP(Position head, Position p) {
    p->next = head->next;
    head->next = p;
    return EXIT_SUCCESS;
}

int ispis(Position head) {
    Position p = head->next;
    int i = 1;
    while (p != NULL) {
        printf("%d. %s %s %d\n", i++, p->name, p->surname, p->year);
        p = p->next;
    }
    return EXIT_SUCCESS;
}

int unosK(Position head, Position p) {
    while (head->next != NULL) {
        head = head->next;
    }
    head->next = p;
    return EXIT_SUCCESS;
}

Position Findbysurname(Position head, char* prez) {
    Position q = head->next;
    while (q != NULL) {
        if (strcmp(q->surname, prez) == 0) {
            return q;
        }
        q = q->next;
    }
    return NULL;
}

Position findprevious(char* surname, Position head) {
    Position p = head->next;
    Position prev = head;
    while (p != NULL && strcmp(p->surname, surname) != 0) {
        prev = p;
        p = p->next;
    }
    return p ? prev : NULL;
}

int delete(char* prez, Position head) {
    Position prev = findprevious(prez, head);
    if (prev == NULL || prev->next == NULL) {
        printf("Person with surname %s not found.\n", prez);
        return EXIT_FAILURE;
    }
    Position temp = prev->next;
    prev->next = temp->next;
    free(temp);
    return EXIT_SUCCESS;
}

int unosIza(char* prez, Position head, Position p) {
    Position q = Findbysurname(head, prez);
    if (!q) {
        printf("Person with surname %s not found.\n", prez);
        return EXIT_FAILURE;
    }
    p->next = q->next;
    q->next = p;
    return EXIT_SUCCESS;
}

int unosIspred(char* surname, Position head, Position p) {
    Position prev = findprevious(surname, head);
    if (!prev) {
        printf("Person with surname %s not found.\n", surname);
        return EXIT_FAILURE;
    }
    p->next = prev->next;
    prev->next = p;
    return EXIT_SUCCESS;
}

int unosDatoteka(char* filename, Position head) {
    FILE* fp = fopen(filename, "w");
    if (!fp) {
        printf("Error opening the file!\n");
        return EXIT_FAILURE;
    }
    Position current = head->next;
    while (current != NULL) {
        fprintf(fp, "%s %s %d\n", current->name, current->surname, current->year);
        current = current->next;
    }
    fclose(fp);
    return EXIT_SUCCESS;
}

int ispisDatoteka(char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("Error opening the file!\n");
        return EXIT_FAILURE;
    }
    char name[MAX_LINE] = { 0 }, surname[MAX_LINE] = { 0 };
    int year = 0, i = 1;
    while (fscanf(fp, "%s %s %d", name, surname, &year) == 3) {
        printf("%d. %s %s %d\n", i++, name, surname, year);
    }
    fclose(fp);
    return EXIT_SUCCESS;
}
