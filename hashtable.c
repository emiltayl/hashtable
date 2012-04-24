#include "hashtable.h"

#include <stdlib.h>

unsigned int get_hash(hash_table *hashTable, char *string) {
	unsigned int hashValue = 0;

	while (*string != '\0') {
		hashValue = *string + (hashValue << 5) - hashValue;
		string++;
	}

	return hashValue;
}

unsigned int get_position(hash_table *hashTable, char *string) {
    int hashValue = get_hash(hashTable, string);

    int position = hashValue % (1 << hashTable->exponent);
    if (position < hashTable->nextSplit) {
        position = hashValue % (1 << (hashTable->exponent + 1));
    }
    
    return position;
}

hash_table *get_hash_table(int size) {
    int i;
    hash_table *hashTable = (hash_table *) malloc(sizeof(hash_table));
    hashTable->elements = (ht_list *) malloc(sizeof(ht_list) * size);

    for (i = 0; i < size; i++) {
        hashTable->elements[i] = NULL;
    }

    hashTable->size = size;

    for (i = size; i > 1; i >> 1) {
        hashTable->exponent++;
    }

    hashTable->nextSplit = 0;
    hashTable->n_elements = 0;

    return hashTable;
}

int has_element(hash_table *hashTable, char *string) {
    int position = get_position(hashTable, string);
    ht_list *list = hashTable->elements[position];

    while (list != NULL) {
        if (strcmp(string, list->string) == 0) {
            return 1;
        }
        list = list->next;
    }

    return 0; 
}

ht_list *get_element(hash_table *hashTable, char *string) {
    int position = get_position(hashTable, string);
    ht_list *list = hashTable->elements[position];

    while (list != NULL) {
        if (strcmp(string, list->string) == 0) {
            return list;
        }
        list = list->next;
    }

    return NULL;
}
