/*
* File: HashADT.c
* Decription: 
* implements a hash table as an abstract
* data type
* 
* Author: Connor Patterson
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "HashADT.h" 

/*
*  struct hashtab_s: 
*  A structure representing a hash table
*
*  This structure manages key-value pairs within a hash table. 
*  It includes members for tracking table metrics,
*  such as capacity, size, load threshold, resize factor, 
*  collision count, and rehash count.
*/
typedef struct hashtab_s {
    
    size_t capacity; 
    size_t size; 
    float load_threshold; 
    size_t resize_factor; 
    size_t collisions; 
    size_t rehashes; 

    size_t (*hash)(const void *key); 
    bool (*equals)(const void *key1, const void *key2); 
    void (*print)(const void *key, const void *value); 
    void (*delete)(void *key, void *value); 

    void **keys; 
    void **values; 

} hashtab;


/*
*  HashADT ht_create:
*  creates a new table
*
*  @param hash: A function pointer to the hash function that hashes keys.
*  @param equals: A function pointer to the comparison function that checks equality between keys.
*  @param print: A function pointer to the function that prints key-value pairs.
*  @param delete: A function pointer to the function that deletes key-value pairs.
*
*  Returns a HashADT.
*/
HashADT ht_create(
    size_t (*hash)(const void *key),
    bool (*equals)(const void *key1, const void *key2),
    void (*print)(const void *key, const void *value),
    void (*delete)(void *key, void *value)
){
    assert(hash != NULL && equals != NULL && print != NULL);

    // Allocate memory for the HashADT structure
    HashADT new_table = (HashADT)malloc(sizeof(struct hashtab_s));

    // Exception handling
    assert(new_table != NULL); // Check if memory allocation failed

    new_table->hash = hash;
    new_table->equals = equals;
    new_table->print = print;
    new_table->delete = delete;
    new_table->capacity = INITIAL_CAPACITY;
    new_table->size = 0;
    new_table->load_threshold = LOAD_THRESHOLD;
    new_table->resize_factor = RESIZE_FACTOR;
    new_table->collisions = 0;
    new_table->rehashes = 0;

    // Allocate memory for keys and values arrays
    new_table->keys = (void **)calloc(new_table->capacity, sizeof(void *));
    new_table->values = (void **)calloc(new_table->capacity, sizeof(void *));

    // Exception handling for memory allocation
    assert(new_table->keys != NULL && new_table->values != NULL);

    return new_table;
}


/*
*  struct ht_destroy: 
*
*  Destroys the HashADT structure and frees the allocated memory.
*  Checks if the provided HashADT 't' is NULL. If true, returns without performing any action.
*  If 'delete' function pointer within 't' is not NULL, iterates through 
*  the capacity of the HashADT and deletes each key-value pair.
*  Frees the memory allocated for the keys and values arrays within 't'.
*  Frees the memory allocated for the HashADT structure 't'.
*
*  @param t: HashADT structure to be destroyed.
*/
void ht_destroy(HashADT t){
    
    if (t == NULL) {
        return;
    }

   if (t->delete != NULL) {
        for (size_t i = 0; i < t->capacity; ++i) {
            if (t->keys[i] != NULL && t->values[i] != NULL) {
                t->delete(t->keys[i], t->values[i]);
            }
        }
    }
    free(t->keys);
    free(t->values);

    free(t);

}

/*
*  struct void ht_dump(const HashADT t, bool contents): 
*
*  This function, ht_dump, prints information about the hash table 't'. 
*  It displays the size, capacity, collisions, 
*  and rehashes. If 'contents' is true, it also prints the contents of 
*  the hash table, including non-empty buckets 
*  along with their respective keys and values.
*  
*  @t: A pointer to the hash table to be dumped.
*  @contents: A boolean flag indicating whether to display the contents of the hash table.
*/
void ht_dump(const HashADT t, bool contents){
    if (t == NULL) {
        printf("The hash table is NULL.\n");
        return;
    }

    printf("Hash Table Information:\n");
    printf("Size: %zu, Capacity: %zu, Collisions: %zu, Rehashes: %zu\n", t->size, t->capacity, t->collisions, t->rehashes);

    if (contents) {
        printf("Hash Table Contents:\n");
        for (size_t i = 0; i < t->capacity; ++i) {
            if (t->keys[i] != NULL) {
                printf("Bucket %zu: ", i);
                t->print(t->keys[i], t->values[i]);
                printf("\n");
            }
        }
    }
}

/*
*  struct ht_get: 
*
*  Retrieves the value associated with the given key in the Hash Table.
*  If the table or key is NULL, returns NULL. If the key is present, 
*  retrieves and returns its associated value.
*  
*  @param t: The Hash Table instance.
*  @param key: The key whose associated value needs to be retrieved.
*  @return: Returns the associated value for the given key or NULL 
*  if the key is not found or the table is NULL.
*/
const void *ht_get(const HashADT t, const void *key){

    if (t == NULL || key == NULL) {
        return NULL;
    }

    if (ht_has(t, key)) {
        size_t index = t->hash(key) % t->capacity; 
        while (t->keys[index] != NULL) {
            if (t->equals(t->keys[index], key)) {
                return t->values[index]; 
            }
            index = (index + 1) % t->capacity; // Handle collisions 
        }
    }

    return NULL; 
}

/*
*  struct ht_has: 
*
*  Checks if the given key exists in the Hash Table.
*  If the table or key is NULL, returns false. Searches for the 
*  key in the table and returns true if found, false otherwise.
*  
*  @param t: The Hash Table instance.
*  @param key: The key to be checked for existence in the table.
*  @return: Returns true if the key exists in the table, otherwise false.
*/
bool ht_has(const HashADT t, const void *key){

    if (t == NULL || key == NULL) {
        return false;
    }

    size_t index = t->hash(key) % t->capacity; 

    while (t->keys[index] != NULL) {
        if (t->equals(t->keys[index], key)) {
            return true; 
        }
        index = (index + 1) % t->capacity; // Handle collisions 
    }

    return false; 
}

/*
*  struct ht_put: 
*
*  Inserts or updates a key-value pair in the Hash Table.
*  If the table or key is NULL, returns NULL. If the key already exists, 
*  updates the value and returns the previous value; otherwise, inserts the new key-value pair.
*  
*  @param t: The Hash Table instance where the key-value pair will be inserted/updated.
*  @param key: The key to be inserted/updated in the table.
*  @param value: The value associated with the key.
*  @return: Returns the previous value associated with the key if it existed, NULL otherwise.
*/
void *ht_put(HashADT t, const void *key, const void *value){

    if (t == NULL || key == NULL) {
        return NULL;
    }

    size_t index = t->hash(key) % t->capacity; 
    void *old_value = NULL;

    // Find the index for the key
    while (t->keys[index] != NULL) {
        if (t->equals(t->keys[index], key)) {
            old_value = t->values[index];
            t->values[index] = (void *)value;
            return old_value; 
        }
        index = (index + 1) % t->capacity; // Handle collisions 
    }

    t->keys[index] = (void *)key;
    t->values[index] = (void *)value;
    t->size++; 

    if ((float)t->size / t->capacity > LOAD_THRESHOLD) {
        size_t new_capacity = t->capacity * RESIZE_FACTOR;
        void **new_keys = calloc(new_capacity, sizeof(void *));
        void **new_values = calloc(new_capacity, sizeof(void *));

        // Rehashing: iterate through existing elements, recalculate hashes, and insert into the new table
        for (size_t i = 0; i < t->capacity; i++) {
            
            if (t->keys[i] != NULL) {
                size_t new_index = t->hash(t->keys[i]) % new_capacity;
                t->rehashes ++;

                while (new_keys[new_index] != NULL) {
                    new_index = (new_index + 1) % new_capacity; // Handle collisions in the new table
                    t->collisions ++;
                }
                new_keys[new_index] = t->keys[i];
                new_values[new_index] = t->values[i];
            }
        }

        // Free the old table's keys and values arrays
        free(t->keys);
        free(t->values);

        // Update the hash table with the new values and parameters
        t->keys = new_keys;
        t->values = new_values;
        t->capacity = new_capacity;

        // Update other parameters such as load_threshold or resize_factor as necessary
    }

    return old_value; 
}

void **ht_keys(const HashADT t){
    
    if (t == NULL) {
        return NULL;
    }

    void **keys = (void **)malloc(sizeof(void *) * t->size);

    if (keys == NULL) {
        return NULL; 
    }

    size_t index = 0;

    for (size_t i = 0; i < t->capacity; ++i) {
        if (t->keys[i] != NULL) {
            keys[index] = t->keys[i];
            index++;
        }
    }

    return keys; 
}

/*
*  struct ht_keys: 
*
*  Retrieves an array of keys from the Hash Table.
*  If the table is NULL, returns NULL. Allocates memory for an array 
*  of keys and populates it with the existing keys from the table.
*  
*  @param t: The Hash Table instance from which keys will be retrieved.
*  @return: Returns an array containing the keys from the table or NULL 
*  if the table is empty or NULL.
*/
void **ht_values(const HashADT t){
    if (t == NULL) {
        return NULL;
    }

    void **values = (void **)malloc(sizeof(void *) * t->size);

    if (values == NULL) {
        return NULL; 
    }

    size_t index = 0;

    for (size_t i = 0; i < t->capacity; ++i) {
        if (t->keys[i] != NULL) {
            values[index] = t->values[i];
            index++;
        }
    }

    return values; 
}

