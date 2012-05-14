#include "hashtable.h"

#include <stdlib.h>
#include <string.h>

unsigned long hash_get_value(char *string) {
	//Magic initial number, found in article about hash algorithm.
	unsigned long hash_value = 5381;

	while (*string != '\0') {
		//Multiply hash_value by 33 (hash_value << 5 + hash_value) and add the
		//value of the current character.
		hash_value = *string + ((hash_value << 5) + hash_value);
		string++;
	}

	return hash_value;
}

size_t hash_table_get_position(hash_table_t *hash_table, char *string) {
    unsigned long hash_value = hash_get_value(string);

    size_t position = hash_value % (1 << hash_table->exponent);
	//If we're about to put the string in a cell that has been split since the
	//last time exponent was increased, we have to calculate the position with
	//hash_value modulo (exponent + 1).
    if (position < hash_table->next_split) {
        position = hash_value % (1 << (hash_table->exponent + 1));
    }
    
    return position;
}

hash_table_t *hash_table_create(int size) {
	//The first thing we check is whether we're trying to create a table with a
	//valid size (greater than 0), if not we simply return NULL, as we could
	//not create a table with the requested size.
    if (size < 1) {
        return NULL;
    }

	//Allocate memory for the hash_table_t struct. If we can't allocate the
	//memory we return NULL.
    hash_table_t *hash_table = (hash_table_t *) malloc(sizeof(hash_table_t));
    if (hash_table == NULL) {
        return NULL;
    }
    
	//Next we need to allocate enough room for the pointers to all the elements
	//in the hash table. If this fails we also return NULL.
    hash_table->elements = (hash_table_list_t **) malloc(sizeof(hash_table_list_t *) * size);
    if (hash_table->elements == NULL) {
        free(hash_table);
        return NULL;
    }

	//Do initialising, set default and initial values. Empty cells are simply
	//NULL pointers.
    int i;
    for (i = 0; i < size; i++) {
        hash_table->elements[i] = NULL;
    }

    hash_table->size = size;
    hash_table->exponent = 0;

    for (i = size; i > 1; i = i >> 1) {
        hash_table->exponent++;
    }

    hash_table->next_split = hash_table->size - (1 << hash_table->exponent);
    hash_table->n_elements = 0;

    return hash_table;
}

int hash_table_has_element(hash_table_t *hash_table, char *string) {
    size_t position = hash_table_get_position(hash_table, string);
    hash_table_list_t *list = hash_table->elements[position];

	//Loop through all the entries in the cell string should be in, return 1 if
	//we find what we're looking for, 0 otherwise.
    while (list != NULL) {
        if (strcmp(string, list->string) == 0) {
            return 1;
        }
        list = list->next;
    }

    return 0; 
}

hash_table_list_t *hash_table_get_element(hash_table_t *hash_table, char *string) {
    unsigned int position = hash_table_get_position(hash_table, string);
    hash_table_list_t *list = hash_table->elements[position];

	//Same as hash_table_has_element, loop through all the entries in the cell
	//the string should be in. If we find the entry we're looking for return a
	//pointer to it, otherwise we return a NULL-pointer.
    while (list != NULL) {
        if (strcmp(string, list->string) == 0) {
            return list;
        }
        list = list->next;
    }

    return NULL;
}

hash_table_list_t *hash_table_add_element(hash_table_t *hash_table, char *string) {
	//If the hash table already has an element with the same key we're trying
	//to insert, simply return the entry already in the hash table.
    if (hash_table_has_element(hash_table, string)) {
        return hash_table_get_element(hash_table, string);
    }

    unsigned int position = hash_table_get_position(hash_table, string);
	//Allocate memory for the new entry, return NULL if we didn't manage to
	//allocate the memory.
	hash_table_list_t *new_element = (hash_table_list_t *) malloc(sizeof(hash_table_list_t));
    if (new_element == NULL) {
        return NULL;
    }

	//Try to allocate space for the key, return NULL on failure. On success, we
	//copy the string to the new position.
    int length = strlen(string) + 1;
    new_element->string = (char *) malloc(sizeof(char) * length);
    if (new_element->string == NULL) {
        free(new_element);
        return NULL;
    }
    memcpy(new_element->string, string, length);

    new_element->next = NULL;

	//Now we have to find out where to insert the new entry, in case something
	//else already is on that specific position in the hash table.
	//It might be confusing with all the pointers, list_element is the position
	//we want to try to insert the element at. If the pointer pointed to by
	//list_element is not NULL, we have to check the next element in the list
	//until we find a free position to insert the new entry.
    hash_table_list_t **list_element = &(hash_table->elements[position]);
    while (*list_element != NULL) {
        list_element = &((*list_element)->next);
    }

    *list_element = new_element;
    hash_table->n_elements++;

	//Next we check if the hash table has so many entries that we should
	//increase its size.
    if (((float) hash_table->n_elements/hash_table->size) > HASH_TABLE_GROW_SIZE) {
		//The first thing we do is try to allocate enough space for
		//another pointer in the elements-array. If we can't, we just give up
		//trying to increase the size.
        hash_table_list_t **elements = (hash_table_list_t **) realloc(hash_table->elements, (hash_table->size + 1) * sizeof(hash_table_list_t *));
        if (elements == NULL) {
            return new_element;
        }
		//The pointer might have been changed to a new location, so we just
		//overwrite it.
        hash_table->elements = elements;
		hash_table->elements[hash_table->size] = NULL;
        hash_table->size++;

		//Now it's time to split the cell. Here old_element is a pointer to
		//the elements we have to check whether we should move or not, and
		//new_element is a pointer to the position we eventually should insert
		//old_element at.
        hash_table_list_t **old_element = &(hash_table->elements[hash_table->next_split]);
        hash_table_list_t **new_element = &(hash_table->elements[hash_table->size-1]);

        hash_table->next_split++;
        while (*old_element != NULL) {
			//Now that size is updated, hash_table_get_position will either
			//return the old location (we should not move, or the new location.
            if (hash_table_get_position(hash_table, (*old_element)->string) != hash_table->next_split - 1) {
                *new_element = *old_element;
				*old_element = (*old_element)->next;
                new_element = &((*new_element)->next);
                *new_element = NULL;
            } else {
                old_element = &((*old_element)->next);
            }
        }
        
		//If we just split the element created last time we increased the
		//exponent, we increase the exponent and set the next cell to be split
		//to be 0.
        if ((1 << (hash_table->exponent + 1)) <= hash_table->size) {
            hash_table->exponent++;
            hash_table->next_split = 0;
        }
    }

    return new_element;
}

void hash_table_remove_element(hash_table_t *hash_table, char *string) {
    unsigned int position = hash_table_get_position(hash_table, string);
    
    hash_table_list_t *prev = NULL;
    hash_table_list_t *list = hash_table->elements[position];
    while (list != NULL) {
        if (strcmp(string, list->string) == 0) {
            if (prev != NULL) {
                prev->next = list->next;
            } else {
                hash_table->elements[position] = list->next;
            }

			free(list->string);
            free(list);
            hash_table->n_elements--;
            break;
        }

        prev = list;
        list = list->next;
    }

	if (((float) hash_table->n_elements/hash_table->size) < HASH_TABLE_SHRINK_SIZE) {
		//The hash table is so small that we can shrink it
		hash_table->size--;
		hash_table_list_t *hash_table_element = hash_table->elements[hash_table->size];
		hash_table_list_t *hash_table_previous_element = hash_table_element;
		hash_table_list_t **insertion_point;

		if ((1 << hash_table->exponent) > hash_table->size) {
			hash_table->exponent--;
			hash_table->next_split = (1 << hash_table->exponent) - 1;
		} else {
			hash_table->next_split--;
		}

		while (hash_table_element != NULL) {
			insertion_point = &(hash_table->elements[hash_table_get_position(hash_table, hash_table_element->string)]);
			while (*insertion_point != NULL) {
				insertion_point = &((*insertion_point)->next);
			}
			*insertion_point = hash_table_element;
			hash_table_element = hash_table_element->next;
			hash_table_previous_element->next = NULL;
			hash_table_previous_element = hash_table_element;
		}

		hash_table_list_t **elements = (hash_table_list_t **) realloc(hash_table->elements, (hash_table->size) * sizeof(hash_table_list_t *));
        if (elements != NULL) {
			hash_table->elements = elements;
        }
	}
}

void hash_table_free(hash_table_t *hash_table) {
    int i;
    hash_table_list_t *list_item, *next;
    for (i = 0; i < hash_table->size; i++) {
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
