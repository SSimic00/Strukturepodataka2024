#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 50

typedef struct dir* dPosition;
typedef struct stack* sPosition;

typedef struct dir {
    char name[MAX];
    dPosition sibling;
    dPosition child;
} Dir;

typedef struct stack {
    dPosition directory;
    sPosition next;
} Stack;

int MakeDir(dPosition, const char*);
int Directory(dPosition);
dPosition ChangeDir(dPosition, sPosition, const char*);
dPosition Back(dPosition, sPosition);
dPosition FindByName(char*, dPosition);
int PushStack(sPosition, dPosition);
dPosition PopStack(sPosition);
void FreeMemory(dPosition);
void Menu(dPosition, sPosition);

int main() {
    dPosition root = (dPosition)malloc(sizeof(Dir));
    strcpy(root->name, "C:");
    root->child = NULL;
    root->sibling = NULL;

    Stack head = { .directory = NULL, .next = NULL };

    printf("Available commands:\n");
    printf("\tmd <name>   - Create a new directory\n");
    printf("\tcd <name>   - Enter a directory\n");
    printf("\tcd..        - Return to the previous directory\n");
    printf("\tdir         - List directories in the current directory\n");
    printf("\texit        - Exit the program\n\n");

    Menu(root, &head);
    FreeMemory(root);
    return 0;
}


int MakeDir(dPosition curr, const char* name) {
    dPosition newDir = (dPosition)malloc(sizeof(Dir));
    if (!newDir) {
        printf("Cannot allocate memory!\n");
        return EXIT_FAILURE;
    }

    strcpy(newDir->name, name);
    newDir->child = NULL;
    newDir->sibling = NULL;

    if (curr->child == NULL) {
        curr->child = newDir;
    }
    else {
        dPosition temp = curr->child;
        while (temp->sibling) {
            temp = temp->sibling;
        }
        temp->sibling = newDir;
    }

    printf("Directory '%s' created successfully!\n", name);
    return EXIT_SUCCESS;
}

int Directory(dPosition curr) {
    if (curr->child == NULL) {
        printf("This directory is empty!\n");
        return EXIT_SUCCESS;
    }

    printf("Directories in '%s':\n", curr->name);
    dPosition temp = curr->child;
    while (temp) {
        printf("\t%s\n", temp->name);
        temp = temp->sibling;
    }
    return EXIT_SUCCESS;
}

dPosition FindByName(char* name, dPosition curr) {
    dPosition temp = curr->child;
    while (temp) {
        if (strcmp(temp->name, name) == 0)
            return temp;
        temp = temp->sibling;
    }
    return NULL;
}

int PushStack(sPosition head, dPosition directory) {
    sPosition newNode = (sPosition)malloc(sizeof(Stack));
    if (!newNode) {
        printf("Cannot allocate memory!\n");
        return EXIT_FAILURE;
    }
    newNode->directory = directory;
    newNode->next = head->next;
    head->next = newNode;
    return EXIT_SUCCESS;
}

dPosition PopStack(sPosition head) {
    if (head->next == NULL)
        return NULL;

    sPosition temp = head->next;
    dPosition dir = temp->directory;
    head->next = temp->next;
    free(temp);
    return dir;
}

dPosition Back(dPosition curr, sPosition head) {
    dPosition parent = PopStack(head);
    if (!parent) {
        printf("Cannot exit the root directory!\n");
        return curr;
    }
    return parent;
}

dPosition ChangeDir(dPosition curr, sPosition head, const char* name) {
    dPosition target = FindByName((char*)name, curr);
    if (!target) {
        printf("Directory '%s' not found!\n", name);
        return curr;
    }
    PushStack(head, curr);
    return target;
}

void FreeMemory(dPosition root) {
    if (root) {
        FreeMemory(root->child);
        FreeMemory(root->sibling);
        free(root);
    }
}

void Menu(dPosition root, sPosition head) {
    dPosition curr = root;
    char command[MAX] = { 0 };
    char name[MAX] = { 0 };

    while (1) {
        printf("%s>", curr->name);
        fgets(command, MAX, stdin);

        if (strncmp(command, "md ", 3) == 0) {
            sscanf(command + 3, "%s", name);
            MakeDir(curr, name);
        }
        else if (strncmp(command, "cd ", 3) == 0) {
            sscanf(command + 3, "%s", name);
            curr = ChangeDir(curr, head, name);
        }
        else if (strcmp(command, "cd..\n") == 0) {
            curr = Back(curr, head);
        }
        else if (strcmp(command, "dir\n") == 0) {
            Directory(curr);
        }
        else if (strcmp(command, "exit\n") == 0) {
            break;
        }
        else {
            printf("Invalid command!\n");
        }
    }
}
