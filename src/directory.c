/*
 *	File: directory.c
 *	Author: Rafael Serra e Oliveira (ist199311)
 *	Description: Handler functions for the Directory structure
*/

/*** Include project header file ***/
#include "filesystem.h"

Dir *newDir(char name[]) {
	Dir *new = (Dir *)smalloc(sizeof(Dir));
	if (new == NULL)
		return new;
	new->name = strdup(name);
	if (new->name == NULL)
		return NULL;
	new->value = NULL;
	new->children = newDLL();
	new->abcChildren = NULL;
	new->parent = NULL;
	return new;
}

void *getValueDir(void *dir) {
	return ((Dir *)dir)->value;
}

int setValueDir(Dir *dir, char value[]) {
	if (dir->value) {
		free(dir->value);
	}
	dir->value = strdup(value);
	return dir->value != NULL;
}

Dir *newChildDir(Dir *parent, char name[]) {
	Dir *new = newDir(name);
	new->parent = parent;
	pushDLL(parent->children, new);
	parent->abcChildren = insertAVLNode(parent->abcChildren, new, cmpNamesDir);
	return new;
}

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

void deleteDirWrapper(void *value, void *arg) {
	deleteDir((Dir *)value, 0, 0, NULL, arg);
}

void deleteDirNOP(Dir *a, void *b) {
	if (a && b) { /* suppress unused variables warning */

		/* No Operation - do nothing */
	};
}

void printDir(Dir *dir, char buffer[]) {
	int origSz = strlen(buffer);
	if (strlen(dir->name) > 0)
		strcat(buffer, PATH_SEPARATOR);
	strcat(buffer, dir->name);
	if (dir->value)
		printf("%s %s\n", buffer, dir->value);
	traverseDLL(dir->children, printDirWrapper, 0, buffer);
	buffer[origSz] = '\0';
}

void printDirWrapper(void *value, void *arg) {
	printDir((Dir *)value, (char *)arg);
}

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

void listAbcChildrenDir(Dir *dir) {
	traverseAVL(dir->abcChildren, IN_ORDER, printChildDir);
}

void printChildDir(void *c) {
	printf("%s\n", ((Dir *)c)->name);
}

char *calcPathDir(Dir *dir) {
	int first = 1;
	char buffer[MAX_PATH_LEN] = "";
	char *path = smalloc(sizeof(char) * MAX_PATH_LEN);
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

int cmpNamesDir(void *a, void *b) {
	return strcmp(((Dir *)a)->name, ((Dir *)b)->name);
}

int matchesNameDir(void *a, void *b) {
	return strcmp((char *)a, ((Dir *)b)->name) == 0;
}