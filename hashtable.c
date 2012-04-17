#include "hashtable.h"

#include <stdlib.h>

unsigned int getHash(hash_table *hashTable, char *string) {
	unsigned int hashValue = 0;

	while (*string != '\0') {
		hashValue = *string + (hashValue << 5) - hashValue;
		string++;
	}

	return hashValue;
}

unsigned int getPosition(hash_table *hashTable, char *string) {
    int hashValue = getHash(hashTable, string);

    int position = hashValue % (1 << hashTable->exponent);
    if (position < hashTable->nextSplit) {
        position = hashValue % (1 << (hashTable->exponent + 1));
    }
    
    return position;
}

hash_table *getNewHashTable(int size) {
    int i;
    hash_table *hashTable = (hash_table *) malloc(sizeof(hash_table));
    hashTable->elements = (ht_list *) malloc(sizeof(ht_list) * size);
    hashTable->allocatedBlocks = size;

    for (i = 0; i < size; i++) {
        hashTable->elements[i]->string = NULL;
        hashTable->elements[i]->next = NULL;
    }

    hashTable->size = size;

    for (i = size; i > 1; i >> 1) {
        hashTable->exponent++;
    }

    hashTable->nextSplit = 0;

    return hashTable;
}
