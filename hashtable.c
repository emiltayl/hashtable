#include "hashtable.h"

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
