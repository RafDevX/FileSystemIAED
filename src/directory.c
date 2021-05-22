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
	pushDLL(parent->children, new);
	parent->abcChildren = insertAVLNode(parent->abcChildren, new, cmpValuesDir);
	return new;
}

void deleteDir(Dir *dir, Dir *parent) {
	traverseDLL(dir->children, deleteDirWrapper, 1);
	traverseAVL(dir->abcChildren, FREE, NULL);
	if (parent) {
		removeDLL(parent->children, dir, 1, NULL);
	}
	free(dir->children);
	free(dir->value);
	free(dir->name);
	free(dir);
}

void deleteDirWrapper(void *value) {
	deleteDir((Dir *)value, NULL);
}

void printDir(Dir *dir) {
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
	if (emptyDLL(path))
		return root;
	cur = (char *)shiftDLL(path);
	dir = (Dir *)firstMatchingDLL(root->children, cur, matchesNameDir);
	if (dir == NULL && createIfMissing) {
		dir = newChildDir(root, cur);
	}
	return findDir(dir, path, createIfMissing);
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
	return strcmp((char *)a, ((Dir *)b)->name);
}