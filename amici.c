#include "HashADT.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


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

size_t findFriendIndex(person_t *person, person_t *friend) {
    for (size_t i = 0; i < person->friend_count; ++i) {
        if (person->friends[i] == friend) {
            return i;
        }
    }
    return SIZE_MAX; // Indicates friend not found
}

void unfriend(person_t *person, person_t *enemy){

    size_t enemy_index = findFriendIndex(person, enemy);;



    
    if (enemy_index != SIZE_MAX) {
        // Move the last friend to the removed friend's position
        person->friends[enemy_index] = person->friends[person->friend_count - 1];

        // Decrement the friend count
        --person->friend_count;
    } else {
        // Friend not found
    }
}



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

void processCommand(HashADT amici_table, char *command, char *arg1, char *arg2, char *arg3) {
    printf("Debug: Processing command - Command: %s", command);

    if (arg1 != NULL) {
        printf(", Arg1: %s", arg1);
    }

    if (arg2 != NULL) {
        printf(", Arg2: %s", arg2);
    }

    if (arg3 != NULL) {
        printf(", Arg3: %s", arg3);
    }

    printf("\n");

    if (strcmp(command, "add")==0){
        char *full_name = malloc(strlen(arg1) + strlen(arg2) + 2);
        strcpy(full_name, arg1);
        strcat(full_name, " ");
        strcat(full_name, arg2);


        person_t *new_person = initializePerson(full_name, arg3);
        ht_put(amici_table, new_person->handle, new_person);

        printf("yuh\n");
    }
    if (strcmp(command, "print")==0){
        const person_t *const_person = ht_get(amici_table, arg1);
        person_t *person = (person_t *)const_person;

        ht_dump(amici_table, true);
        printf("\n");
        printf("\n");
        printAmici(person);
    }
    if(strcmp(command, "friend")==0){
        const person_t *const_requester = ht_get(amici_table, arg1);
        const person_t *const_receiver = ht_get(amici_table, arg2);

        person_t *requester = (person_t *)const_requester;
        person_t *receiver = (person_t *)const_receiver;

        addFriend(requester, receiver);
        addFriend(receiver, requester);

        printf("%s and %s are now friends\n", requester->handle, receiver->handle);
    }
    if(strcmp(command, "unfriend")==0){
        const person_t *const_requester = ht_get(amici_table, arg1);
        const person_t *const_receiver = ht_get(amici_table, arg2);

        person_t *requester = (person_t *)const_requester;
        person_t *receiver = (person_t *)const_receiver;

        unfriend(requester, receiver);
        unfriend(receiver, requester);

        
    }

    // Add your logic to handle different commands
    // For now, we are just printing the processed command
}




int main(int argc, char *argv[]) {
    HashADT amici_table = ht_create(hash_person, equals_person, print_person, delete_person);


    // Check the number of command-line arguments
    if (argc < 1 || argc > 2) {
        fprintf(stderr, "error: usage: %s [datafile]\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Check if a datafile is provided
    if (argc == 2) {
        // Process commands from the datafile
        FILE *file = fopen(argv[1], "r");
        if (file == NULL) {
            perror("error");
            return EXIT_FAILURE;
        }

        char input[1024];

        while (fgets(input, sizeof(input), file) != NULL) {
            char command[256], arg1[256], arg2[256], arg3[256];


            printf("\n");

            // Parse the input into command and arguments
            if (sscanf(input, "%255s %255s %255s %255s", command, arg1, arg2, arg3) >= 1) { // buffer overflow
                processCommand(amici_table, command, arg1, arg2, arg3);

                // Free allocated memory
                
       
                
            } else {
                fprintf(stderr, "error: Unable to parse input\n");
            }
        }

        fclose(file);
    } else {
        // Process commands from the user input
        char input[1024];

        printf("Amici> ");

        while (fgets(input, sizeof(input), stdin) != NULL) {
            char command[256], arg1[256], arg2[256], arg3[256];

            // Parse the input into command and arguments
            if (sscanf(input, "%255s %255s %255s %255s", command, arg1, arg2, arg3) >= 1) {
                // Call the processCommand function with the parsed values
                processCommand(amici_table, command, arg1, arg2, arg3);

                // Free allocated memory
                /*
                free(command);
                free(arg1);
                free(arg2);
                free(arg3);
                */
            } else {
                fprintf(stderr, "error: Unable to parse input\n");
            }

            printf("Amici> ");
        }
    }

    return 0;
}