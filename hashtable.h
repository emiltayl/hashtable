#ifndef HASTABLE_H_
#define HASHTABLE_H_

#define HASH_TABLE_DEFAULT_SIZE 128
#define HASH_TABLE_GROW_SIZE 0.75

typedef struct _hash_table_list {
	const char *string;
	struct _hash_table_list *next;
} hash_table_list_t;

typedef struct _hash_table {
	int size;
	int n_elements;
	int exponent;
	int next_split;
	hash_table_list_t *elements;
} hash_table_t;

unsigned long get_hash(char *);
unsigned int get_hash_table_position(hash_table_t *, char *);
hash_table_t *get_hash_table(int);
#define get_hash_table() get_hash_table(HASH_TABLE_DEFAULT_SIZE)
int has_hash_table_element(hash_table_t *, char *);
hash_table_list_t *get_hash_table_element(hash_table_t *, char *);
hash_table_list_t *add_hash_table_element(hash_table_t *, char *);
void remove_hash_table_element(hash_table_t *, char *);
void free_hash_table(hash_table_t *);

#endif /*HASHTABLE_H_*/
