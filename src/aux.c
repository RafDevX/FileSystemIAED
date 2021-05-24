/*
 *	File: aux.c
 *	Author: Rafael Serra e Oliveira (ist199311)
 *	Description: Auxiliary functions for the whole project.
*/

/*** Include project header file ***/
#include "filesystem.h"

/* Dupicate a string into a new(ly allocated) place in memory */
char *strdup(char s1[]) {
	char *s2 = (char *)malloc(sizeof(char) * (strlen(s1) + 1));
	if (s2 == NULL)
		return s2;
	strcpy(s2, s1);
	return s2;
}

/* Calculate a hash between 0 and M-1 (inclusive) for key */
long int hashS(void *key, long int M) {
	char *v = (char *)key;
	long int h, a = 31415, b = 27183;

	for (h = 0; *v != '\0'; v++, a = a * b % (M - 1))
		h = (a * h + *v) % M;
	return h;
}

/* Auxiliary function for the search command to aid with traversal; finds first
 * finds first with Dir specified value */
void searchAux(void *val, void *args) {
	Dir *dir = (Dir *)val;
	Dir **result = ((void **)args)[0];
	char *value = ((void **)args)[1];
	if (*result == NULL && strcmp(dir->value, value) == 0)
		*result = dir;
}

/* Auxiliary function for the delete command to aid with traversal; for each Dir
 * to be removed, if it has a value, remove it from the values hash table */
void deleteAux(Dir *dir, void *valuesTable) {
	if (dir->value != NULL)
		removeHashT((HashT *)valuesTable, dir);
}