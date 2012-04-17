#include "hashtable.h"

unsigned int getHash(hash_table *hashTable, char *string) {
	unsigned int hashValue = 0;

	while (*string != '\0') {
		hashValue = *string + (hashValue << 5) - hashValue;
		string++;
	}

	return hashValue % hashTable->size; //TODO: We're not actually supposed to use size
}
