/*
 *	File: directory.c
 *	Author: Rafael Serra e Oliveira (ist199311)
 *	Description: Handler functions for the Directory structure
*/

/*** Include project header file ***/
#include "filesystem.h"

Dir *newDir(char name[]) {
	Dir *new = (Dir *)malloc(sizeof(Dir));
	new->name = strdup(name);
	new->value = NULL;
	new->children = newDLL();
	new->abcChildren = NULL;
	new->parent = NULL;
	return new;
}

void setValueDir(Dir *dir, char value[]) {
	if (dir->value) {
		free(dir->value);
	}
	dir->value = strdup(value);
}

Dir *newChildDir(Dir *parent, char name[]) {
	Dir *new = newDir(name);
	new->parent = parent;
	pushDLL(parent->children, new);
	parent->abcChildren = insertAVLNode(parent->abcChildren, new, cmpNamesDir);
	return new;
}

void deleteDir(Dir *dir, int topToDelete) {
	traverseDLL(dir->children, deleteDirWrapper, 1, NULL);
	traverseAVL(dir->abcChildren, FREE, NULL);
	if (dir->parent && topToDelete) {
		removeDLL(dir->parent->children, dir, 1, NULL);
		dir->parent->abcChildren = removeAVLNode(dir->parent->abcChildren, dir,
												 cmpNamesDir, NULL);
	}
	free(dir->children);
	free(dir->value);
	free(dir->name);
	free(dir);
}

void deleteDirWrapper(void *value, void *arg) {
	if (arg == NULL)
		deleteDir((Dir *)value, 0);
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

char *searchDir(Dir *root, char value[]) {
	/* TODO: hashtable */
	if (root)
		return strdup(value);
	return NULL;
}

int cmpNamesDir(void *a, void *b) {
	return strcmp(((Dir *)a)->name, ((Dir *)b)->name);
}

int matchesNameDir(void *a, void *b) {
	return strcmp((char *)a, ((Dir *)b)->name) == 0;
}