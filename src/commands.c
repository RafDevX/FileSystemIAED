/*
 *	File: doublylinkedlist.c
 *	Author: Rafael Serra e Oliveira (ist199311)
 *	Description: Handler functions for the Doubly-Linked List structure
*/

/*** Include project header file ***/
#include "filesystem.h"

void cmdHelp() {
	char lines[][MAX_HELP_LINE_LEN] = HELP_LINES;
	int i;
	for (i = 0; i < (int)(sizeof(lines) / sizeof(lines[0])); i++)
		printf("%s\n", lines[i]);
}

void cmdSet(Dir *root, char args[]) {
	char path[MAX_PATH_LEN], *value;
	Dir *dir;
	DLL *pathList;
	sscanf(args, "%s", path);
	value = args + strlen(path) + 1;
	pathList = strToDLL(path, PATH_SEPARATOR);
	dir = findDir(root, pathList, 1);
	setValueDir(dir, value);
	free(pathList);
}

void cmdPrint(Dir *root) {
	char buffer[MAX_PATH_LEN];
	buffer[0] = '\0';
	printDir(root, buffer);
}

void cmdFind(Dir *root, char path[]) {
	DLL *pathList = strToDLL(path, PATH_SEPARATOR);
	Dir *dir = findDir(root, pathList, 0);
	if (dir == NULL)
		printf("%s\n", ERR_NOT_FOUND);
	else if (dir->value == NULL)
		printf("%s\n", ERR_NO_DATA);
	else
		printf("%s\n", dir->value);
	free(pathList);
}

void cmdList(Dir *root, char path[]) {
	DLL *pathList = strToDLL(path, PATH_SEPARATOR);
	Dir *dir = findDir(root, pathList, 0);
	if (dir == NULL)
		printf("%s\n", ERR_NOT_FOUND);
	else
		listAbcChildrenDir(dir);
	free(pathList);
}

void cmdSearch(Dir *root, char value[]) {
	char *path = searchDir(root, value);
	if (path == NULL)
		printf("%s\n", ERR_NOT_FOUND);
	else
		printf("%s\n", path);
	free(path);
}

void cmdDelete(Dir *root, char path[]) {
	DLL *pathList = strToDLL(path, PATH_SEPARATOR);
	Dir *dir = findDir(root, pathList, 0);
	if (dir == NULL)
		printf("%s\n", ERR_NOT_FOUND);
	else
		deleteDir(dir, 1);
	free(pathList);
}