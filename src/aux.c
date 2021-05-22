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

int isTruthy(void *v) {
	return !!v;
}
