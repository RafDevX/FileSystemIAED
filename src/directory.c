/*
 *	File: directory.c
 *	Author: Rafael Serra e Oliveira (ist199311)
 *	Description: Handler functions for the Directory structure
*/

/*** Include project header file ***/
#include "filesystem.h"

Dir *newDir(char name[]) {
	Dir *new = (Dir *)malloc(sizeof(Dir));
	new->name = name;
	new->value = NULL;
	new->children = newDLL();
	new->abcChildren = NULL;
	return new;
}

int cmpValuesDir(void *a, void *b) {
	char *v1 = ((Dir *)a)->value, *v2 = ((Dir *)b)->value;
	return strcmp(v1, v2);
}

Dir *newChildDir(Dir *parent, char name[]) {
	Dir *new = newDir(name);
	pushDLL(parent->children, new);
	parent->abcChildren = insertAVLNode(parent->abcChildren, new, cmpValuesDir);
	return new;
}

void setValueDir(Dir *dir, char value[]) {
	if (dir->value) {
		free(dir->value);
	}
	dir->value = strdup(value);
}