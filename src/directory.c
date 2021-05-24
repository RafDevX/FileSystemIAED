/*
 *	File: directory.c
 *	Author: Rafael Serra e Oliveira (ist199311)
 *	Description: Handler functions for the Directory structure
*/

/*** Include project header file ***/
#include "filesystem.h"

/* Allocate memory for and instantiate a new directory */
Dir *newDir(char name[]) {
	Dir *new = (Dir *)malloc(sizeof(Dir));
	if (new == NULL)
		return new;
	new->name = strdup(name);
	if (new->name == NULL)
		return NULL;
	new->value = NULL;
	new->children = newDLL();
	if (new->children == NULL)
		return NULL;
	new->abcChildren = NULL;
	new->parent = NULL;
	return new;
}

/* Set the value for a directory, freeing the previous one if it exists */
int setValueDir(Dir *dir, char value[]) {
	if (dir == NULL)
		return 0;
	if (dir->value)
		free(dir->value);
	dir->value = strdup(value);
	return dir->value != NULL;
}

/* Create a new directory as a child of a Dir */
Dir *newChildDir(Dir *parent, char name[]) {
	Dir *new = newDir(name);
	if (new == NULL)
		return NULL;
	new->parent = parent;
	if (pushDLL(parent->children, new) == NULL)
		return NULL;
	parent->abcChildren = insertAVLNode(parent->abcChildren, new, cmpNamesDir);
	if (parent->abcChildren == NULL)
		return NULL;
	return new;
}

/* Delete (and free) a directory and its children; top should be 1 if the Dir is
 * the top one to delete, skipTop should be 1 if the top directory should not be
 * deleted (but its children should), newCback should be used to set a new
 * callback function that will be invoked for each Dir before it is deleted and
 * arg will be passed as an argument to the set callback function */
void deleteDir(Dir *dir, int top, int skipTop,
			   void (*newCback)(Dir *, void *), void *arg) {
	static void (*callback)(Dir *, void *) = NULL;
	if (newCback != NULL)
		callback = newCback;
	traverseDLL(dir->children, deleteDirWrapper, 1, arg);
	traverseAVL(dir->abcChildren, FREE, NULL);
	if (!(top && skipTop)) {
		if (dir->parent && top) {
			removeDLL(dir->parent->children, dir, 1, NULL);
			dir->parent->abcChildren = removeAVLNode(dir->parent->abcChildren,
													 dir, cmpNamesDir, NULL);
		}
		if (callback)
			callback(dir, arg);
		free(dir->children);
		free(dir->value);
		free(dir->name);
		free(dir);
	} else {
		free(dir->children);
		dir->children = newDLL();
		dir->abcChildren = NULL;
	}
}

/* Wrapper for deleteDir to facilitate using both traversal and recursion */
void deleteDirWrapper(void *value, void *arg) {
	deleteDir((Dir *)value, 0, 0, NULL, arg);
}

/* Stub function to be used as "no operation", to be passed as the newCback
 * argument of the deleteDir function in order to clear the previously set
 * callback, since NULL would just maintain the current value. */
void deleteDirNOP(Dir *a, void *b) {
	if (a && b) { /* suppress unused variables warning */

		/* No Operation - do nothing */
	};
}

/* Recursively output a Dir (only if it has a value) and its children */
void printDir(Dir *dir, char buffer[]) {
	int origSz = strlen(buffer);
	if (strlen(dir->name) > 0)
		strcat(buffer, PATH_SEPARATOR);
	strcat(buffer, dir->name);
	if (dir->value)
		printf(OUT_FORMAT_PRINT_CMD, buffer, dir->value);
	traverseDLL(dir->children, printDirWrapper, 0, buffer);
	buffer[origSz] = '\0';
}

/* Wrapper to facilitate both traversal and recursion with printDir */
void printDirWrapper(void *value, void *arg) {
	printDir((Dir *)value, (char *)arg);
}

/* Find a Dir by following a path from a root Dir, optionally creating it if it
 * is missing. NOTE: The DLL passed as *path will be modified! */
Dir *findDir(Dir *root, DLL *path, int createIfMissing) {
	char *cur;
	Dir *dir;
	if (root == NULL || emptyDLL(path))
		return root;
	cur = (char *)shiftDLL(path);
	dir = (Dir *)firstMatchingDLL(root->children, cur, matchesNameDir);
	if (dir == NULL && createIfMissing) {
		dir = newChildDir(root, cur);
	}
	free(cur);
	return findDir(dir, path, createIfMissing);
}

/* Output a Dir's children in ASCII order */
void listAbcChildrenDir(Dir *dir) {
	traverseAVL(dir->abcChildren, IN_ORDER, printChildDir);
}

/* Helper function for traversal; actually output a child */
void printChildDir(void *c) {
	printf(OUT_FORMAT_LIST_CMD, ((Dir *)c)->name);
}

/* Calculate a Dir's path from root */
char *calcPathDir(Dir *dir) {
	int first = 1;
	char buffer[MAX_PATH_LEN] = "";
	char *path = malloc(sizeof(char) * MAX_PATH_LEN);
	if (path == NULL)
		return path;
	path[0] = '\0';
	while (dir != NULL) {
		strcpy(buffer, path);
		strcpy(path, dir->name);
		if (first)
			first = 0;
		else
			strcat(path, PATH_SEPARATOR);
		strcat(path, buffer);
		dir = dir->parent;
	}
	return path;
}

/* Wrapper for strcmp for two Dirs' "name" field */
int cmpNamesDir(void *a, void *b) {
	return strcmp(((Dir *)a)->name, ((Dir *)b)->name);
}

/* Return whether the first argument is an identical string to the second
 * argument (a Dir)'s "name" field". */
int matchesNameDir(void *a, void *b) {
	return strcmp((char *)a, ((Dir *)b)->name) == 0;
}

/* Cast an argument to Dir and return its value. */
void *getValueDir(void *dir) {
	return ((Dir *)dir)->value;
}