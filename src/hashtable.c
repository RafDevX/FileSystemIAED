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
	HashT *table = (HashT *)malloc(sizeof(HashT));
	DLL **entries = (DLL **)malloc(sizeof(DLL *) * dim);
	for (i = 0; i < dim; i++)
		entries[i] = newDLL();
	table->dim = dim;
	table->getKey = getKey;
	table->hash = hash;
	table->entries = entries;
	return table;
}

DLL *searchHashT(HashT *table, void *key) {
	long int hash = table->hash(key, table->dim);
	DLL *lst = table->entries[hash];
	return lst;
}

void insertHashT(HashT *table, void *value) {
	void *key = table->getKey(value);
	pushDLL(searchHashT(table, key), value);
}

void removeHashT(HashT *table, void *value) {
	void *key = table->getKey(value);
	DLL *lst = searchHashT(table, key);
	removeDLL(lst, value, 1, NULL);
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