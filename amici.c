#include "HashADT.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *strdup(const char *str);


// Define a structure to represent a person
typedef struct person_s {
    char *name;                 // Name of the person
    char *handle;               // Handle of the person
    struct person_s **friends;  // Dynamic collection of friends
    size_t friend_count;        // Current number of friends
    size_t max_friends;         // Current limit on friends
} person_t;

// Function to initialize a person structure
person_t *initializePerson(const char *name, const char *handle) {
    person_t *newPerson = (person_t *)malloc(sizeof(person_t));
    if (newPerson == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for name and handle
    newPerson->name = strdup(name);
    newPerson->handle = strdup(handle);

    if (newPerson->name == NULL || newPerson->handle == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    // Initialize friends array
    newPerson->friends = NULL;
    newPerson->friend_count = 0;
    newPerson->max_friends = 0;

    return newPerson;
}


// Function to add a friend to a person's list
void addFriend(person_t *person, person_t *friend) {
    // Check if the friend array needs resizing
    if (person->friend_count == person->max_friends) {
        size_t new_size = person->max_friends == 0 ? 1 : 2 * person->max_friends;
        person->friends = (person_t **)realloc(person->friends, new_size * sizeof(person_t *));
        if (person->friends == NULL) {
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }
        person->max_friends = new_size;
    }

    // Add the friend to the array
    person->friends[person->friend_count++] = friend;
}



/*
void removeFriend(const char *handle1, const char *handle2) {
    person_t *person1 = getPerson(handle1);
    person_t *person2 = getPerson(handle2);

    if (person1 == NULL || person2 == NULL) {
        fprintf(stderr, "Error: One or more handles are unknown\n");
        return;
    }

    // Check if the friendship exists
    int found1 = 0, found2 = 0;
    for (size_t i = 0; i < person1->friend_count; ++i) {
        if (person1->friends[i] == person2) {
            found1 = 1;
            break;
        }
    }

    for (size_t i = 0; i < person2->friend_count; ++i) {
        if (person2->friends[i] == person1) {
            found2 = 1;
            break;
        }
    }

    if (!found1 || !found2) {
        fprintf(stderr, "Error: Friendship does not exist\n");
        return;
    }

    // Remove the friendship
    person_t **newFriends1 = (person_t **)malloc(person1->friend_count * sizeof(person_t *));
    person_t **newFriends2 = (person_t **)malloc(person2->friend_count * sizeof(person_t *));

    if (newFriends1 == NULL || newFriends2 == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    size_t newCount1 = 0, newCount2 = 0;

    for (size_t i = 0; i < person1->friend_count; ++i) {
        if (person1->friends[i] != person2) {
            newFriends1[newCount1++] = person1->friends[i];
        }
    }

    for (size_t i = 0; i < person2->friend_count; ++i) {
        if (person2->friends[i] != person1) {
            newFriends2[newCount2++] = person2->friends[i];
        }
    }

    // Update the friendship lists
    free(person1->friends);
    free(person2->friends);

    person1->friends = newFriends1;
    person1->friend_count = newCount1;

    person2->friends = newFriends2;
    person2->friend_count = newCount2;

    printf("%s and %s are no longer friends\n", handle1, handle2);
}

// Function to print information about a person
void printPerson(person_t *person) {
    printf("%s (%s) has %zu friends\n", person->handle, person->name, person->friend_count);

    for (size_t i = 0; i < person->friend_count; ++i) {
        printf("  →  %s (%s)\n", person->friends[i]->handle, person->friends[i]->name);
    }
}
*/



// Function to free memory allocated for a person and their friends
void freePerson(person_t *person) {
    free(person->name);
    free(person->handle);
    free(person->friends);
    free(person);
}

size_t hash_person(const void *key) {
    // Implement your hash function for person handles
    // Example: using a simple hash function for strings
    const char *handle = (const char *)key;
    size_t hash = 0;
    while (*handle) {
        hash = (hash * 31) + (*handle++);
    }
    return hash;
}

bool equals_person(const void *key1, const void *key2) {
    // Implement your equality comparison for person handles
    return strcmp((const char *)key1, (const char *)key2) == 0;
}

void delete_person(void *key, void *value) {
    // Implement your deletion function for person handles
    // Note: Depending on your memory management, you may need to free the 'value' as well.
    person_t *person = (person_t *)value;
    free(person->name);
    free(person->handle);
    free(person->friends);
    free(person);
}

void print_person(const void *key, const void *value) {
    
    // Implement your print function for person handles
    printf("Person: %s\n", (const char *)key);
}

void printAmici(person_t *person) {
    printf("%s (%s) has %zu friends\n", person->handle, person->name, person->friend_count);

    for (size_t i = 0; i < person->friend_count; ++i) {
        printf("  →  %s (%s)\n", person->friends[i]->handle, person->friends[i]->name);
    }
}


int main() {

    HashADT amici_table = ht_create(hash_person, equals_person, print_person, delete_person);
    person_t *new_person1 = initializePerson("John Doe", "john123");
    ht_put(amici_table, new_person1->handle, new_person1);

    person_t *new_person2 = initializePerson("DICKHAVER", "test1234");
    ht_put(amici_table, new_person2->handle, new_person2);



    ht_dump(amici_table, true);

    person_t *person1 = ht_get(amici_table, new_person2->handle);

    printAmici(person1);

    

    


    //new_person->name = "John Doe";  // Replace with actual names
    //new_person->handle = "johndoe"; // Replace with actual handles

    //addFriend(john, alice);

    //printPerson(john);


    //removeFriend(john, alice);

    //printPerson(john);

    // Free allocated memory
    //freePerson(john);
    //freePerson(alice);

    return 0;
}