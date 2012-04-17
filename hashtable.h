#ifndef HASTABLE_H_
#define HASHTABLE_H_

#define HASHTABLE_DEFAULT_SIZE 128

typedef struct _ht_list {
	char *string;
	struct _ht_list *next;
} ht_list;

typedef struct _hash_table {
	int size;
	int elements;
	int exponent;
	int nextSplit;
	ht_list ** elements;
} hash_table;

unsigned int getPosition(hash_table *, char *);
hash_table *getNewHashTable(int);
#define getNewHashTable() getNewHashTable(HASHTABLE_DEFAULT_SIZE)
int hasElement(hash_table *, char *);
ht_list *getElement(hash_table *, char *);
ht_list *addElement(hash_table *, char *);
void removeElement(hash_table *, char *);
void freeTable(hash_table *);

#endif /*HASHTABLE_H_*/
