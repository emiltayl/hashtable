#ifndef HASTABLE_H_
#define HASHTABLE_H_

#define HASH_TABLE_GROW_SIZE 0.75

typedef struct _hash_table_list {
	char *string;
	struct _hash_table_list *next;
} hash_table_list_t;

typedef struct _hash_table {
	int size;
	int n_elements;
	int exponent;
	int next_split;
	hash_table_list_t **elements;
} hash_table_t;

unsigned long hash_get_value(char *);
unsigned int hash_table_get_position(hash_table_t *, char *);
hash_table_t *hash_table_create(int);
int hash_table_has_element(hash_table_t *, char *);
hash_table_list_t *hash_table_get_element(hash_table_t *, char *);
hash_table_list_t *hash_table_add_element(hash_table_t *, char *);
void hash_table_remove_element(hash_table_t *, char *);
void hash_table_free(hash_table_t *);

#endif /*HASHTABLE_H_*/
