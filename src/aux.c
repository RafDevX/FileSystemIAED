/*
 *	File: aux.c
 *	Author: Rafael Serra e Oliveira (ist199311)
 *	Description: Auxiliary functions for the whole project.
*/

/*** Include project header file ***/
#include "filesystem.h"

char *strdup(char s1[]) {
	char *s2 = (char *)malloc(sizeof(char) * (strlen(s1) + 1));
	strcpy(s2, s1);
	return s2;
}

long int hashS(void *key, long int M) {
	char *v = (char *)key;
	long int h, a = 31415, b = 27183;

	for (h = 0; *v != '\0'; v++, a = a * b % (M - 1))
		h = (a * h + *v) % M;
	return h;
}