#include "hashtable.h"

#include <stdlib.h>
#include <string.h>

unsigned long get_hash(char *string) {
	unsigned long hash_value = 5381;

	while (*string != '\0') {
		hash_value = *string + ((hash_value << 5) + hash_value);
		string++;
	}

	return hash_value;
}

unsigned int get_hash_table_position(hash_table_t *hash_table, char *string) {
    unsigned long hash_value = get_hash(string);

    unsigned int position = hash_value % (1 << hash_table->exponent);
    if (position < hash_table->next_split) {
        position = hash_value % (1 << (hash_table->exponent + 1));
    }
    
    return position;
}

hash_table_t *get_hash_table(int size) {
    if (size < 1) {
        return NULL; //We don't want to have to deal with the special case of empty tables
    }
    int i;

    hash_table_t *hash_table = (hash_table_t *) malloc(sizeof(hash_table_t));
    if (hash_table == NULL) {
        return NULL;
    }
    
    hash_table->elements = (hash_table_list_t **) malloc(sizeof(hash_table_list_t) * size);
    if (hash_table->elements == NULL) {
        free(hash_table);
        return NULL;
    }

    for (i = 0; i < size; i++) {
        hash_table->elements[i] = NULL;
    }

    hash_table->size = size;
    hash_table->exponent = 0;

    for (i = size; i > 1; i >> 1) {
        hash_table->exponent++;
    }

    hash_table->next_split = 0;
    hash_table->n_elements = 0;

    return hash_table;
}

int has_hash_table_element(hash_table_t *hash_table, char *string) {
    unsigned int position = get_hash_table_position(hash_table, string);
    hash_table_list_t *list = hash_table->elements[position];

    while (list != NULL) {
        if (strcmp(string, list->string) == 0) {
            return 1;
        }
        list = list->next;
    }

    return 0; 
}

hash_table_list_t *get_hash_table_element(hash_table_t *hash_table, char *string) {
    unsigned int position = get_hash_table_position(hash_table, string);
    hash_table_list_t *list = hash_table->elements[position];

    while (list != NULL) {
        if (strcmp(string, list->string) == 0) {
            return list;
        }
        list = list->next;
    }

    return NULL;
}

hash_table_list_t *add_hash_table_element(hash_table_t *hash_table, char *string) {
    if (has_hash_table_element(hash_table, string)) {
        return get_hash_table_element(hash_table, string);
    }

    unsigned int position = get_hash_table_position(hash_table, string);
	hash_table_list_t *new_element = (hash_table_list_t *) malloc(sizeof(hash_table_list_t));
    if (new_element == NULL) {
        return NULL;
    }

    int length = strlen(string) + 1;
    new_element->string = (char *) malloc(sizeof(char) * length);
    if (new_element->string == NULL) {
        free(new_element);
        return NULL;
    }
    memcpy(new_element->string, string, length);

    new_element->next = NULL;

    hash_table_list_t **list_element = &(hash_table->elements[position]);
    while (*list_element != NULL) {
        list_element = &((*list_element)->next);
    }

    *list_element = new_element;
    hash_table->n_elements++;

    if (((float) hash_table->n_elements/hash_table->size) > HASH_TABLE_GROW_SIZE) {
        //There are so many entries that we should grow the table
        hash_table_list_t *elements = (hash_table_list_t *) realloc(hash_table->elements, hash_table->size + sizeof(hash_table_list_t));
        if (elements == NULL) {
            //We couldn't increase the size of the list, maybe we should have
            //a way of handling errors?
            return new_element;
        }
        hash_table->elements = elements;
        hash_table->size++;

        hash_table_list_t **old_element = &(hash_table->elements[hash_table->next_split]);
        hash_table_list_t **new_element = &(hash_table->elements[hash_table->size-1]);

        while (*old_element != NULL) {
            if (get_hash_table_position(hash_table, *old_element->string) != hash_table->next_split) {
                *new_element = *old_element;
                old_element = &((*old_element)->next);
                *new_element->next = null;
                new_element = &((*new_element)->next);
            } else {
                old_element = &((*old_element)->next);
            }
        }
        hash_table->next_split++;
        
        if ((1 << hash_table->exponent) >= hash_table->size) {
            hash_table->exponent++;
            hash_table->next_split = 0;
        }
    }

    return new_element;
}

void remove_hash_table_element(hash_table_t *hash_table, char *string) {
    //TODO: Possibly shrink the table when it is filled less than x%?
    unsigned int position = get_hash_table_position(hash_table, string);
    
    hash_table_list_t *prev = NULL;
    hash_table_list_t *list = hash_table->elements[position];
    while (list != NULL) {
        if (strcmp(string, list->string) == 0) {
            if (prev != NULL) {
                prev->next = list->next;
            } else {
                hash_table->elements[position] = list->next;
            }

            free(list);
            hash_table->n_elements--;
            break;
        }

        prev = list;
        list = list->next;
    }
}

void free_hash_table(hash_table_t *hash_table) {
    int i;
    hash_table_list_t *list_item, next;
    for (int i = 0; i < size; i++) {
        list_item = hash_table->elements[i];
        while (list_item != NULL) {
            next = list_item->next;
            free(list_item->string);
            free(list_item);
            list_item = next;
        }
    }
    free(hash_table->elements);
    free(hash_table);
}
