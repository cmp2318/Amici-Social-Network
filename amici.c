/*
* File: amici.c
* Decription: 
* implements a "social media" system using a hash table adt
* and allocated "people" within who can friend, unfriend, and print stats
* 
* Author: Connor Patterson
*/

#include "HashADT.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define UNUSED(x) (void)(x)


char *strdup(const char *str);

int num_accounts = 0;
int num_friendships = 0;

// Struct representation of a person in Amici
typedef struct person_s {
    char *name;                 
    char *handle;              
    struct person_s **friends;  
    size_t friend_count;        
    size_t max_friends;         
} person_t;


/*
*  (person_t initializePerson(const char *name, const char *handle))
*
*  Initializes a new person with the given name and handle, allocating 
*  memory for the person structure and duplicating name and handle strings.
*  
*  @param name: The name of the person.
*  @param handle: The handle or username of the person.
*  @return: A pointer to the newly initialized person.
*/
person_t *initializePerson(const char *name, const char *handle) {
    person_t *newPerson = (person_t *)malloc(sizeof(person_t));
    if (newPerson == NULL) {
        perror("Memory allocation error"); 
        exit(EXIT_FAILURE);
    }

    newPerson->name = strdup(name);
    newPerson->handle = strdup(handle);

    if (newPerson->name == NULL || newPerson->handle == NULL) {
        perror("Memory allocation error"); 
        exit(EXIT_FAILURE);
    }

    newPerson->friends = NULL;
    newPerson->friend_count = 0;
    newPerson->max_friends = 0;

    return newPerson;
}

/*
*  (size_t findFriendIndex(person_t *person, person_t *friend))
*
*  Finds the index of a friend within the person's friends array.
*  
*  @param person: The person whose friends are being searched.
*  @param friend: The friend whose index is to be found.
*  @return: The index of the friend in the person's friends array, 
*           or SIZE_MAX if the friend is not found.
*/
size_t findFriendIndex(person_t *person, person_t *friend) {
    for (size_t i = 0; i < person->friend_count; ++i) {
        if (person->friends[i] == friend) {
            return i;
        }
    }
    return SIZE_MAX; // if this is returned, there was no friend found
}

/*
*  (void addFriend(person_t *person, person_t *friend))
*
*  Adds a friend to the person's friends array. Resizes the array if necessary.
*  
*  @param person: The person to whom the friend is being added.
*  @param friend: The person being added as a friend.
*/
void addFriend(person_t *person, person_t *friend) {

    // check if the friend array needs resizing
    if (person->friend_count == person->max_friends) {
        size_t new_size = person->max_friends == 0 ? 1 : 2 * person->max_friends;
        person->friends = (person_t **)realloc(person->friends, new_size * sizeof(person_t *));
        if (person->friends == NULL) {
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }
        person->max_friends = new_size;
    }

    person->friends[person->friend_count++] = friend;
}

/*
*  (void unfriend(person_t *person, person_t *enemy))
*
*  Removes a friend (enemy) from the person's friends array.
*  
*  @param person: The person from whom the friend is being removed.
*  @param enemy: The person being unfriended.
*/
void unfriend(person_t *person, person_t *enemy){

    size_t enemy_index = findFriendIndex(person, enemy);;

    if (enemy_index != SIZE_MAX) {
        person->friends[enemy_index] = person->friends[person->friend_count - 1];

        --person->friend_count;
    } else {
            return;
    }
}

/*
*  (size_t hash(const void *key))
*
*  Computes and returns a hash value for the provided key (handle).
*  
*  @param key: The key (handle) for which the hash value is computed.
*  @return: The computed hash value.
*/
size_t hash(const void *key) {

    const char *handle = (const char *)key;
    size_t hash = 0;
    while (*handle) {
        hash = (hash * 31) + (*handle++);
    }
    return hash;
}

/*
*  (bool equals(const void *key1, const void *key2))
*
*  Compares two keys (handles) for equality.
*  
*  @param key1: The first key to be compared.
*  @param key2: The second key to be compared.
*  @return: True if the keys are equal, false otherwise.
*/
bool equals(const void *key1, const void *key2) {
    return strcmp((const char *)key1, (const char *)key2) == 0;
}

/*
*  (void delete(void *key, void *value))
*
*  Placeholder function for key-value deletion in the hash table. 
*  Uses UNUSED macro.
*  
*  @param key: The key to be deleted.
*  @param value: The associated value to be deleted.
*/
void delete(void *key, void *value) {
    UNUSED(key);    // use of UNUSED macro because I did not 
    UNUSED(value);  
}

/*
*  (void print(const void *key, const void *value))
*
*  Placeholder function for printing key-value pairs in the hash table. 
*  Uses UNUSED macro.
*  
*  @param key: The key to be printed.
*  @param value: The associated value to be printed.
*/
void print(const void *key, const void *value) {
    UNUSED(key);
    UNUSED(value);
    // I realized near the end that I was a different method to print
    // the people within amici so got rid of this method  
    // instead (printAmici)

    // I have in processCommand what I would have here
}

/*
*  (void printAmici(person_t *person))
*
*  Prints the details of a person, including handle, name, and the list of friends.
*  
*  @param person: The person whose details are to be printed.
*/
void printAmici(person_t *person) {
    printf("%s (%s) has %zu friends\n", person->handle, person->name, person->friend_count);

    for (size_t i = 0; i < person->friend_count; ++i) {
        printf("  →  %s (%s)\n", person->friends[i]->handle, person->friends[i]->name);
    }
}

/*
*  (void printFriendCount(const char *handle, const char *name, size_t friendCount))
*
*  Prints the friend count for a person with the specified handle and name.
*  
*  @param handle: The handle of the person.
*  @param name: The name of the person.
*  @param friendCount: The number of friends the person has.
*/
void printFriendCount(const char *handle, const char *name, size_t friendCount) {
    if (friendCount == 0) {
        printf("%s (%s) has no friends\n", handle, name);
    } else {
        printf("%s (%s) has %zu friend%s\n", handle, name, friendCount, (friendCount == 1) ? "" : "s");
    }
}

/*
*  (void processCommand(HashADT amici_table, char *command, char *arg1, char *arg2, char *arg3))
*
*  Processes the given command along with its arguments and performs the corresponding actions 
*  in the social media system.
*  
*  @param amici_table: The hash table storing the people in the social media system.
*  @param command: The command to be processed (add, print, friend, unfriend, size, 
*                  stats, init, quit).
*  @param arg1: The first argument associated with the command.
*  @param arg2: The second argument associated with the command.
*  @param arg3: The third argument associated with the command.
*/
void processCommand(HashADT amici_table, char *command, char *arg1, char *arg2, char *arg3) {

    /* 
    // Prints out each argument for debug usage

    printf("Debug: Processing command - Command: %s", command);

    if (arg1[0] != '\0') {
        printf(", Arg1: %s", arg1);
    }

    if (arg2[0] != '\0') {
        printf(", Arg2: %s", arg2);
    }

    if (arg3[0] != '\0') {
        printf(", Arg3: %s", arg3);
    }
    */

    printf("\n");
   
    if (strcmp(command, "add") == 0) {
        if (arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0') {
            fprintf(stderr, "error: add command requires three arguments\n");
            return;
        }

        const person_t *existing_person = ht_get(amici_table, arg3);
        if (existing_person != NULL) {
            fprintf(stderr, "error: handle \"%s\" is already in use\n", arg3);
            return;
        }

        num_accounts ++;
        char *full_name = malloc(strlen(arg1) + strlen(arg2) + 2);
        strcpy(full_name, arg1);
        strcat(full_name, " ");
        strcat(full_name, arg2);

        person_t *new_person = initializePerson(full_name, arg3);
        ht_put(amici_table, new_person->handle, new_person);

        free(full_name);

        return;

    } if (strcmp(command, "print")==0){

        if (arg1[0] == '\0') {
            fprintf(stderr, "error: print command requires a handle argument\n");
            return;
        }

        const person_t *const_person = ht_get(amici_table, arg1);
        if (const_person == NULL) {
            fprintf(stderr, "error: handle \"%s\" not found\n", arg1);
            return;
        }

        person_t *person = (person_t *)const_person;

        ht_dump(amici_table, true);
        printf("\n");
        printf("\n");
        printAmici(person);

        return;

    } if(strcmp(command, "friend")==0){


        if (arg1[0] == '\0' || arg2[0] == '\0') {
            fprintf(stderr, "error: friend command requires two arguments\n");
            return;
        }

        const person_t *const_requester = ht_get(amici_table, arg1);
        const person_t *const_receiver = ht_get(amici_table, arg2);

        if (const_requester == NULL || const_receiver == NULL) {
            fprintf(stderr, "error: one or more handles not found\n");
            return;
        }

        person_t *requester = (person_t *)const_requester;
        person_t *receiver = (person_t *)const_receiver;

        if (findFriendIndex(requester, receiver) != SIZE_MAX) {
            printf("%s and %s are already friends\n", requester->handle, receiver->handle);
            return;
        }

        addFriend(requester, receiver);
        addFriend(receiver, requester);

        printf("%s and %s are now friends\n", requester->handle, receiver->handle);
        num_friendships ++;

        return;

    } if(strcmp(command, "unfriend")==0){

        if (arg1[0] == '\0' || arg2[0] == '\0') {
            fprintf(stderr, "error: unfriend command requires two arguments\n");
            return;
        }

        const person_t *const_requester = ht_get(amici_table, arg1);
        const person_t *const_receiver = ht_get(amici_table, arg2);

        if (const_requester == NULL || const_receiver == NULL) {
            fprintf(stderr, "error: one or more handles not found\n");
            return;
        }

        person_t *requester = (person_t *)const_requester;
        person_t *receiver = (person_t *)const_receiver;

        unfriend(requester, receiver);
        unfriend(receiver, requester);

        return;

    } if(strcmp(command, "size")==0){

        if (arg1[0] == '\0') {
            fprintf(stderr, "error: size command requires a handle argument\n");
            return;
        }
 
        const person_t *const_person = ht_get(amici_table, arg1);
        
        if (const_person == NULL) {
            fprintf(stderr, "error: handle \"%s\" not found\n", arg1);
            return;
        }
        
        person_t *person = (person_t *)const_person;

        printFriendCount(person->handle, person->name, person->friend_count);
        num_friendships --;

        return;

    } if (strcmp(command, "stats") == 0) {
        printf("Statistics: ");
        printf("%d %s %d %s\n", num_accounts, num_accounts == 1 ? "person" : "people",num_friendships, 
        num_friendships == 1 ? "friendship" : "friendships");

        return;

    } if (strcmp(command, "init") == 0) {

        // ht_destroy(amici_table) - no need and I'm not completely sure why
        // (Valgrind gives me errors when I try to destroy it)

        num_accounts = 0;        
        num_friendships = 0;

        printf("System re-initialized\n");

        return;

    } if (strcmp(command, "quit") == 0) {
        printf("Exiting...\n");

        //ht_destroy(amici_table);

        exit(EXIT_SUCCESS);
    }
    else {

        fprintf(stderr, "error: command not recognized\n");

        return;
    }

}

/*
*  (int main(int argc, char *argv[]))
*
*  The main function responsible for initializing the social media system, 
*  reading commands from a file or user input, and performing 
*  corresponding actions.
*  
*  @param argc: The number of command-line arguments.
*  @param argv: An array containing the command-line arguments.
*  @return: The exit status of the program.
*/
int main(int argc, char *argv[]) {

    HashADT amici_table = ht_create(hash, equals, print, delete);

    if (argc < 1 || argc > 2) {
        fprintf(stderr, "error: usage: %s [datafile]\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (argc == 2) { // if data file is present in command line
        FILE *file = fopen(argv[1], "r");
        if (file == NULL) {
            perror("error");
            return EXIT_FAILURE;
        }

        char input[1024];

        while (fgets(input, sizeof(input), file) != NULL) {
            char command[256], arg1[256], arg2[256], arg3[256];


            printf("\n");

            memset(command, 0, sizeof(command)); // use of memset so there is no need to free
            memset(arg1, 0, sizeof(arg1));       // these values
            memset(arg2, 0, sizeof(arg2));
            memset(arg3, 0, sizeof(arg3));

            if (sscanf(input, "%255s %255s %255s %255s", command, arg1, arg2, arg3) >= 1) { // buffer overflow
                processCommand(amici_table, command, arg1, arg2, arg3);
       
            } else {
                fprintf(stderr, "error: Unable to parse input\n");
            }
        }

        fclose(file);
    } else {
        // process commands from the user input
        char input[1024];

        printf("Amici> ");

        while (fgets(input, sizeof(input), stdin) != NULL) {
            char command[256], arg1[256], arg2[256], arg3[256];

            memset(command, 0, sizeof(command));
            memset(arg1, 0, sizeof(arg1));
            memset(arg2, 0, sizeof(arg2));
            memset(arg3, 0, sizeof(arg3));

            if (sscanf(input, "%255s %255s %255s %255s", command, arg1, arg2, arg3) >= 1) {

                processCommand(amici_table, command, arg1, arg2, arg3);
              
            } else {
                fprintf(stderr, "error: Unable to parse input\n");
            }

            printf("Amici> ");
        }
    }

    return 0;
}
