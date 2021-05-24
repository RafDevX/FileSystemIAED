/*
 *	File: hashtable.c
 *	Author: Rafael Serra e Oliveira (ist199311)
 *	Description: Handler functions for the HashTable structure
*/

/*** Include project header file ***/
#include "filesystem.h"

HashT *newHashT(long int dim, void *getKey(void *),
				long int (*hash)(void *, long int)) {
	long int i;
	HashT *table = (HashT *)smalloc(sizeof(HashT));
	DLL **entries = (DLL **)smalloc(sizeof(DLL *) * dim);
	if (table == NULL || entries == NULL)
		return NULL;
	for (i = 0; i < dim; i++)
		entries[i] = NULL;
	table->dim = dim;
	table->getKey = getKey;
	table->hash = hash;
	table->entries = entries;
	return table;
}

DLL *searchHashT(HashT *table, void *key) {
	long int hash = table->hash(key, table->dim);
	DLL *lst = table->entries[hash];
	if (lst == NULL) {
		lst = newDLL();
		table->entries[hash] = lst;
	}
	return lst;
}

int insertHashT(HashT *table, void *value) {
	void *key = table->getKey(value);
	return !!pushDLL(searchHashT(table, key), value);
}

void removeHashT(HashT *table, void *value) {
	void *key = table->getKey(value);
	DLL *lst = searchHashT(table, key);
	removeDLL(lst, value, 1, NULL);
	if (emptyDLL(lst)) {
		traverseDLL(lst, NULL, 1, NULL);
		free(lst);
		table->entries[table->hash(key, table->dim)] = NULL;
	}
}

void freeHashT(HashT *table) {
	long int i;
	for (i = 0; i < table->dim; i++) {
		if (table->entries[i] != NULL) {
			traverseDLL(table->entries[i], NULL, 1, NULL);
			free(table->entries[i]);
		}
	}
	free(table->entries);
	free(table);
}