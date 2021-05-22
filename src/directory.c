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
	parent->abcChildren = insertAVLNode(parent->abcChildren, new, cmpValuesDir);
	return new;
}

void deleteDir(Dir *dir) {
	traverseDLL(dir->children, deleteDirWrapper, 1);
	traverseAVL(dir->abcChildren, FREE, NULL);
	if (dir->parent) {
		removeDLL(dir->parent->children, dir, 1, NULL);
		dir->parent->abcChildren = removeAVLNode(dir->parent->abcChildren, dir,
												 cmpValuesDir, 0);
	}
	free(dir->children);
	free(dir->value);
	free(dir->name);
	free(dir);
}

void deleteDirWrapper(void *value) {
	deleteDir((Dir *)value);
}

void printDir(Dir *dir) { /* FIXME: */
	static char path[MAX_PATH_LEN];
	if (dir == NULL) {
		path[0] = '\0';
		return;
	}
	strcat(path, PATH_SEPARATOR);
	strcat(path, dir->name);
	if (dir->value != NULL)
		printf("%s %s\n", path, dir->value);
	traverseDLL(dir->children, printDirWrapper, 0);
}

void printDirWrapper(void *value) {
	printDir((Dir *)value);
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

int cmpValuesDir(void *a, void *b) {
	char *v1 = ((Dir *)a)->value, *v2 = ((Dir *)b)->value;
	if (v1 == NULL)
		return -1;
	else if (v2 == NULL)
		return 1;
	else
		return strcmp(v1, v2);
}

int matchesNameDir(void *a, void *b) {
	return strcmp((char *)a, ((Dir *)b)->name) == 0;
}