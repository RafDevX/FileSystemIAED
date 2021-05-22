/*
 *	File: aux.c
 *	Author: Rafael Serra e Oliveira (ist199311)
 *	Description: Auxiliary functions for the whole project.
*/

/*** Include project header file ***/
#include "filesystem.h"

int isValidChar(char c) {
	return c != '\0' && c != EOF;
}

char *strdup(char s1[]) {
	char *s2 = (char *)malloc(sizeof(char) * (strlen(s1) + 1));
	strcpy(s2, s1);
	return s2;
}