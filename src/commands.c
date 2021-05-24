/*
 *	File: doublylinkedlist.c
 *	Author: Rafael Serra e Oliveira (ist199311)
 *	Description: Handler functions for the Doubly-Linked List structure
*/

/*** Include project header file ***/
#include "filesystem.h"

/* Output each help line */
int cmdHelp() {
	char lines[][MAX_HELP_LINE_LEN] = HELP_LINES;
	int i;
	for (i = 0; i < (int)(sizeof(lines) / sizeof(lines[0])); i++)
		printf(OUT_FORMAT_HELP_CMD, lines[i]);
	return 1;
}

/* Add or modify a Directory from a path and value, adding to the values hash
 * table. */
int cmdSet(Dir *root, HashT *valuesTable, char args[]) {
	char path[MAX_PATH_LEN], *value;
	Dir *dir;
	DLL *pathList;
	sscanf(args, "%s", path);
	value = args + strlen(path) + 1;
	pathList = strToDLL(path, PATH_SEPARATOR);
	if (pathList == NULL)
		return 0;
	dir = findDir(root, pathList, 1);
	if (dir == NULL)
		return 0;
	traverseDLL(pathList, freeWrapper, 1, NULL);
	free(pathList);
	if (getValueDir(dir) != NULL)
		removeHashT(valuesTable, dir);
	return setValueDir(dir, value) && insertHashT(valuesTable, dir);
}

/* Print all saved directories with assigned values */
int cmdPrint(Dir *root) {
	char buffer[MAX_PATH_LEN];
	buffer[0] = '\0';
	printDir(root, buffer);
	return 1;
}

/* Get value for a directory with a specified path */
int cmdFind(Dir *root, char path[]) {
	DLL *pathList = strToDLL(path, PATH_SEPARATOR);
	Dir *dir;
	if (pathList == NULL)
		return 0;
	dir = findDir(root, pathList, 0);
	traverseDLL(pathList, freeWrapper, 1, NULL);
	free(pathList);
	if (dir == NULL)
		printf(OUT_FORMAT_ERR, ERR_NOT_FOUND);
	else if (dir->value == NULL)
		printf(OUT_FORMAT_ERR, ERR_NO_DATA);
	else
		printf(OUT_FORMAT_FIND_CMD, dir->value);
	return 1;
}

/* List all children of a directory with a specified path */
int cmdList(Dir *root, char path[]) {
	DLL *pathList = strToDLL(path, PATH_SEPARATOR);
	Dir *dir;
	if (pathList == NULL)
		return 0;
	dir = findDir(root, pathList, 0);
	traverseDLL(pathList, freeWrapper, 1, NULL);
	free(pathList);
	if (dir == NULL)
		printf(OUT_FORMAT_ERR, ERR_NOT_FOUND);
	else
		listAbcChildrenDir(dir);
	return 1;
}

/* Search for the first directory with a specified value */
int cmdSearch(HashT *valuesTable, char value[]) {
	DLL *possible = searchHashT(valuesTable, value);
	Dir *result = NULL;
	void *auxArgs[2];
	auxArgs[0] = &result;
	auxArgs[1] = value;
	traverseDLL(possible, searchAux, 0, auxArgs);
	if (result != NULL) {
		char *path = calcPathDir(result);
		if (path == NULL)
			return 0;
		printf(OUT_FORMAT_SEARCH_CMD, path);
		free(path);
	} else {
		printf(OUT_FORMAT_ERR, ERR_NOT_FOUND);
	}
	return 1;
}

/* Delete a directory with a specified path */
int cmdDelete(Dir *root, HashT *valuesTable, char path[]) {
	DLL *pathList = strToDLL(path, PATH_SEPARATOR);
	Dir *dir;
	if (pathList == NULL)
		return 0;
	dir = findDir(root, pathList, 0);
	traverseDLL(pathList, freeWrapper, 1, NULL);
	free(pathList);
	if (dir == NULL)
		printf(OUT_FORMAT_ERR, ERR_NOT_FOUND);
	else
		deleteDir(dir, 1, dir == root, deleteAux,
				  valuesTable);
	return 1;
}