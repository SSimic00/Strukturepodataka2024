#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BODOVI 100
#define MAX_NAME 50
#define MAX_SURNAME 50

typedef struct {
    char ime[MAX_NAME];
    char prezime[MAX_SURNAME];
    double bodovi;
} student;

int citacredaka(char* datoteka);
student* alokacijamemorije(int brojstudenata, char* datoteka);
double maxbrojbodova(student* studenti, int brojstudenata);

int main() {
    int brojstudenata = 0;
    student* studenti = NULL;
    char* datoteka = "tekst.txt";
    double maxbodovi = 0;

    brojstudenata = citacredaka(datoteka);
    studenti = alokacijamemorije(brojstudenata, datoteka);
    maxbodovi = maxbrojbodova(studenti, brojstudenata);

    if (studenti == NULL) {
        printf("Alokacija memorije nije uspjela!\n");
        return 1;
    }

    if (maxbodovi == 0) {
        printf("Greška: Maksimalan broj bodova je 0, ne mogu se izračunati relativni bodovi.\n");
        free(studenti);
        return 1;
    }

    printf("Broj studenata je %d\n", brojstudenata);
    printf("Maksimalan broj bodova: %.2lf\n", maxbodovi);

    for (int i = 0; i < brojstudenata; i++) {
        printf("Student %d: %s %s, Bodovi: %.2lf, Relativni bodovi: %.2lf%%\n", i + 1, studenti[i].ime, studenti[i].prezime, studenti[i].bodovi, ((studenti[i].bodovi / maxbodovi) * 100));
    }

    free(studenti);
    return 0;
}

int citacredaka(char* datoteka) {
    int brojredova = 0;
    char charbuffer[1024];
    FILE* filepointer = fopen(datoteka, "r");

    if (filepointer == NULL) {
        printf("Datoteka nije pronađena!\n");
        return -1;
    }

    while (fgets(charbuffer, sizeof(charbuffer), filepointer) != NULL) {
        if (charbuffer[0] == '\n' || charbuffer[0] == '\0') {
            continue;
        }
        brojredova++;
    }

    fclose(filepointer);
    return brojredova;
}

student* alokacijamemorije(int brojstudenata, char* datoteka) {
    int brojac = 0;
    FILE* filepointer = fopen(datoteka, "r");

    if (filepointer == NULL) {
        printf("Neuspješno otvaranje datoteke.\n");
        return NULL;
    }

    student* studenti = (student*)malloc(brojstudenata * sizeof(student));
    if (studenti == NULL) {
        printf("Neuspješna alokacija memorije.\n");
        fclose(filepointer);
        return NULL;
    }

    while (brojac < brojstudenata) {
        fscanf(filepointer, "%s %s %lf", studenti[brojac].ime, studenti[brojac].prezime, &studenti[brojac].bodovi);
        brojac++;
    }

    fclose(filepointer);
    return studenti;
}

double maxbrojbodova(student* studenti, int brojstudenata) {
    double maxi = studenti[0].bodovi;

    for (int i = 1; i < brojstudenata; i++) {
        if (studenti[i].bodovi > maxi) {
            maxi = studenti[i].bodovi;
        }
    }

    return maxi;
}
