/*
 *	File: doublylinkedlist.c
 *	Author: Rafael Serra e Oliveira (ist199311)
 *	Description: Handler functions for the Doubly-Linked List structure
*/

/*** Include project header file ***/
#include "filesystem.h"

int cmdHelp() {
	char lines[][MAX_HELP_LINE_LEN] = HELP_LINES;
	int i;
	for (i = 0; i < (int)(sizeof(lines) / sizeof(lines[0])); i++)
		printf("%s\n", lines[i]);
	return 1;
}

int cmdSet(Dir *root, HashT *valuesTable, char args[]) {
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
	return setValueDir(dir, value) && insertHashT(valuesTable, dir);
}

int cmdPrint(Dir *root) {
	char buffer[MAX_PATH_LEN];
	buffer[0] = '\0';
	printDir(root, buffer);
	return 1;
}

int cmdFind(Dir *root, char path[]) {
	DLL *pathList = strToDLL(path, PATH_SEPARATOR);
	Dir *dir;
	if (pathList == NULL)
		return 0;
	dir = findDir(root, pathList, 0);
	free(pathList);
	if (dir == NULL)
		printf("%s\n", ERR_NOT_FOUND);
	else if (dir->value == NULL)
		printf("%s\n", ERR_NO_DATA);
	else
		printf("%s\n", dir->value);
	return 1;
}

int cmdList(Dir *root, char path[]) {
	DLL *pathList = strToDLL(path, PATH_SEPARATOR);
	Dir *dir;
	if (pathList == NULL)
		return 0;
	dir = findDir(root, pathList, 0);
	free(pathList);
	if (dir == NULL)
		printf("%s\n", ERR_NOT_FOUND);
	else
		listAbcChildrenDir(dir);
	return 1;
}

int cmdSearch(HashT *valuesTable, char value[]) {
	DLL *possible = searchHashT(valuesTable, value);
	Dir *result = NULL;
	void *auxArgs[2];
	auxArgs[0] = &result;
	auxArgs[1] = value;
	traverseDLL(possible, searchAux, 0, auxArgs);
	if (result != NULL) {
		char *path = calcPathDir(result);
		printf("%s\n", path);
		free(path);
	} else {
		printf("%s\n", ERR_NOT_FOUND);
	}
	return 1;
}

int cmdDelete(Dir *root, HashT *valuesTable, char path[]) {
	DLL *pathList = strToDLL(path, PATH_SEPARATOR);
	Dir *dir;
	if (pathList == NULL)
		return 0;
	dir = findDir(root, pathList, 0);
	free(pathList);
	if (dir == NULL)
		printf("%s\n", ERR_NOT_FOUND);
	else
		deleteDir(dir, 1, dir == root, deleteAux,
				  valuesTable);
	return 1;
}