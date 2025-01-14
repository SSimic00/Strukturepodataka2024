#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct city;
typedef struct city* CityPosition;
typedef struct city {
    char name[50];
    int residents;
    CityPosition next;
} City;

struct country;
typedef struct country* CountryPosition;
typedef struct country {
    char name[50];
    CityPosition cities;
    CountryPosition next;
} Country;

// Function declarations
CountryPosition AddCountry(CountryPosition head, char* countryName, char* fileName);
CityPosition AddCity(CityPosition head, char* cityName, int residents);
void PrintCountries(CountryPosition head);
void PrintCities(CityPosition head);
CountryPosition FindCountry(CountryPosition head, char* countryName);
void SearchCities(CountryPosition head, char* countryName, int minResidents);
void FreeCities(CityPosition head);
void FreeCountries(CountryPosition head);

int main() {
    CountryPosition countryList = NULL;
    char countryName[50], fileName[50];
    int population;
    FILE* file = fopen("Countries.txt", "r");

    if (!file) {
        perror("ERROR: Unable to open Countries.txt");
        return -1;
    }

    // Read countries and their respective files
    while (fscanf(file, "%s %s", countryName, fileName) == 2) {
        countryList = AddCountry(countryList, countryName, fileName);
    }

    fclose(file);

    // Print countries and cities
    printf("Countries and their cities:\n");
    PrintCountries(countryList);

    // Search functionality
    printf("\nEnter the name of the country to search: ");
    scanf("%s", countryName);

    printf("Enter the minimum population: ");
    scanf("%d", &population);

    SearchCities(countryList, countryName, population);

    // Free memory
    FreeCountries(countryList);

    return 0;
}

CountryPosition AddCountry(CountryPosition head, char* countryName, char* fileName) {
    CountryPosition newCountry = (CountryPosition)malloc(sizeof(Country));
    if (!newCountry) {
        perror("ERROR: Memory allocation failed for country.");
        exit(EXIT_FAILURE);
    }

    strcpy(newCountry->name, countryName);
    newCountry->cities = NULL;
    newCountry->next = NULL;

    // Read cities from file
    FILE* file = fopen(fileName, "r");
    if (!file) {
        printf("ERROR: Unable to open file %s for cities.\n", fileName);
        free(newCountry);
        return head;
    }

    char cityName[50];
    int residents;
    while (fscanf(file, "%s %d", cityName, &residents) == 2) {
        newCountry->cities = AddCity(newCountry->cities, cityName, residents);
    }

    fclose(file);

    // Insert country into sorted linked list
    if (!head || strcmp(countryName, head->name) < 0) {
        newCountry->next = head;
        return newCountry;
    }

    CountryPosition current = head;
    while (current->next && strcmp(countryName, current->next->name) > 0) {
        current = current->next;
    }

    newCountry->next = current->next;
    current->next = newCountry;

    return head;
}

CityPosition AddCity(CityPosition head, char* cityName, int residents) {
    CityPosition newCity = (CityPosition)malloc(sizeof(City));
    if (!newCity) {
        perror("ERROR: Memory allocation failed for city.");
        exit(EXIT_FAILURE);
    }

    strcpy(newCity->name, cityName);
    newCity->residents = residents;
    newCity->next = NULL;

    // Insert city into sorted linked list
    if (!head || residents > head->residents || (residents == head->residents && strcmp(cityName, head->name) < 0)) {
        newCity->next = head;
        return newCity;
    }

    CityPosition current = head;
    while (current->next && (residents < current->next->residents || (residents == current->next->residents && strcmp(cityName, current->next->name) > 0))) {
        current = current->next;
    }

    newCity->next = current->next;
    current->next = newCity;

    return head;
}

void PrintCountries(CountryPosition head) {
    while (head) {
        printf("%s:\n", head->name);
        PrintCities(head->cities);
        head = head->next;
    }
}

void PrintCities(CityPosition head) {
    while (head) {
        printf("\t%s - %d residents\n", head->name, head->residents);
        head = head->next;
    }
}

CountryPosition FindCountry(CountryPosition head, char* countryName) {
    while (head && strcmp(head->name, countryName) != 0) {
        head = head->next;
    }
    return head;
}

void SearchCities(CountryPosition head, char* countryName, int minResidents) {
    CountryPosition country = FindCountry(head, countryName);

    if (!country) {
        printf("Country '%s' not found.\n", countryName);
        return;
    }

    printf("Cities in %s with at least %d residents:\n", country->name, minResidents);

    CityPosition city = country->cities;
    while (city) {
        if (city->residents >= minResidents) {
            printf("\t%s - %d residents\n", city->name, city->residents);
        }
        city = city->next;
    }
}

void FreeCities(CityPosition head) {
    while (head) {
        CityPosition temp = head;
        head = head->next;
        free(temp);
    }
}

void FreeCountries(CountryPosition head) {
    while (head) {
        CountryPosition temp = head;
        head = head->next;
        FreeCities(temp->cities);
        free(temp);
    }
}
