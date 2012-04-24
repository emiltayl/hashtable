#ifndef HASTABLE_H_
#define HASHTABLE_H_

#define HASHTABLE_DEFAULT_SIZE 128
#define HASHTABLE_GROW_SIZE 0.75

typedef struct _ht_list {
	char *string;
	struct _ht_list *next;
} ht_list;

typedef struct _hash_table {
	int size;
	int n_elements;
	int exponent;
	int nextSplit;
	ht_list *elements;
} hash_table;

unsigned int get_hash(hash_table *, char *);
unsigned int get_position(hash_table *, char *);
hash_table *get_hash_table(int);
#define get_hash_table() get_hash_table(HASHTABLE_DEFAULT_SIZE)
int has_element(hash_table *, char *);
ht_list *get_element(hash_table *, char *);
ht_list *add_element(hash_table *, char *);
void remove_element(hash_table *, char *);
void free_table(hash_table *);

#endif /*HASHTABLE_H_*/
