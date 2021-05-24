/*
 *	File: doublylinkedlist.c
 *	Author: Rafael Serra e Oliveira (ist199311)
 *	Description: Handler functions for the Doubly-Linked List structure
*/

/*** Include project header file ***/
#include "filesystem.h"

/* Output each help line */
void cmdHelp() {
	char lines[][MAX_HELP_LINE_LEN] = HELP_LINES;
	int i;
	for (i = 0; i < (int)(sizeof(lines) / sizeof(lines[0])); i++)
		printf(OUT_FORMAT_HELP_CMD, lines[i]);
}

/* Add or modify a Directory from a path and value, adding to the values hash
 * table. */
void cmdSet(Dir *root, HashT *valuesTable, char args[]) {
	char path[MAX_PATH_LEN], *value;
	Dir *dir;
	DLL *pathList;
	sscanf(args, "%s", path);
	value = args + strlen(path) + 1;
	pathList = strToDLL(path, PATH_SEPARATOR);
	dir = findDir(root, pathList, 1);
	free(pathList);
	if (getValueDir(dir) != NULL)
		removeHashT(valuesTable, dir);
	setValueDir(dir, value);
	insertHashT(valuesTable, dir);
}

/* Print all saved directories with assigned values */
void cmdPrint(Dir *root) {
	char buffer[MAX_PATH_LEN];
	buffer[0] = '\0';
	printDir(root, buffer);
}

/* Get value for a directory with a specified path */
void cmdFind(Dir *root, char path[]) {
	DLL *pathList = strToDLL(path, PATH_SEPARATOR);
	Dir *dir = findDir(root, pathList, 0);
	free(pathList);
	if (dir == NULL)
		printf(OUT_FORMAT_ERR, ERR_NOT_FOUND);
	else if (dir->value == NULL)
		printf(OUT_FORMAT_ERR, ERR_NO_DATA);
	else
		printf(OUT_FORMAT_FIND_CMD, dir->value);
}

/* List all children of a directory with a specified path */
void cmdList(Dir *root, char path[]) {
	DLL *pathList = strToDLL(path, PATH_SEPARATOR);
	Dir *dir = findDir(root, pathList, 0);
	free(pathList);
	if (dir == NULL)
		printf(OUT_FORMAT_ERR, ERR_NOT_FOUND);
	else
		listAbcChildrenDir(dir);
}

/* Search for the first directory with a specified value */
void cmdSearch(HashT *valuesTable, char value[]) {
	DLL *possible = searchHashT(valuesTable, value);
	Dir *result = NULL;
	void *auxArgs[2];
	auxArgs[0] = &result;
	auxArgs[1] = value;
	traverseDLL(possible, searchAux, 0, auxArgs);
	if (result != NULL) {
		char *path = calcPathDir(result);
		printf(OUT_FORMAT_SEARCH_CMD, path);
		free(path);
	} else {
		printf(OUT_FORMAT_ERR, ERR_NOT_FOUND);
	}
}

/* Delete a directory with a specified path */
void cmdDelete(Dir *root, HashT *valuesTable, char path[]) {
	DLL *pathList = strToDLL(path, PATH_SEPARATOR);
	Dir *dir = findDir(root, pathList, 0);
	free(pathList);
	if (dir == NULL)
		printf(OUT_FORMAT_ERR, ERR_NOT_FOUND);
	else
		deleteDir(dir, 1, dir == root, deleteAux,
				  valuesTable);
}