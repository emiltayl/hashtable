#ifndef HASTABLE_H_
#define HASHTABLE_H_

#include <stddef.h>

//Increase the size of the table when the ratio between the number of stored
//elements and the number of slots is greater than this.
#define HASH_TABLE_GROW_SIZE 0.75
//Decrease the size of the table when the ratio between the number of stored
//elements and the number of slots is less than this.
#define HASH_TABLE_SHRINK_SIZE 0.4

typedef struct _hash_table_list {
	char *string;
	struct _hash_table_list *next;
} hash_table_list_t;

typedef struct _hash_table {
	size_t size;
	//While n_elements potentially could be greater than size_t, this code will
	//not work properly if you try to grow the table further, so having a
	//larger n_elements makes no sense.
	size_t n_elements;
	int exponent;
	size_t next_split;
	hash_table_list_t **elements;
} hash_table_t;

unsigned long hash_get_value(char *);
size_t hash_table_get_position(hash_table_t *, char *);
hash_table_t *hash_table_create(int);
int hash_table_has_element(hash_table_t *, char *);
hash_table_list_t *hash_table_get_element(hash_table_t *, char *);
hash_table_list_t *hash_table_add_element(hash_table_t *, char *);
void hash_table_remove_element(hash_table_t *, char *);
void hash_table_free(hash_table_t *);

#endif /*HASHTABLE_H_*/
