/*
 *	File: filesystem.h
 *	Author: Rafael Serra e Oliveira (ist199311)
 *	Description: Header file for the whole project, in the same place
 *		for easy configuration. Includes constants and prototypes.
*/

/*** Include standard libraries ***/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**********************************
 ***** Project-Wide Constants *****
 **********************************/

/*** Commands ***/

#define MAX_CMD_LENGTH 6
#define CMD_QUIT "quit"

/*** Return Codes ***/

#define RETCODE_OK 0
#define RETCODE_UNKNOWN_CMD 1

/************************
 ***** Custom Types *****
 ************************/

typedef struct LLN {
	void *value;
	struct LLN *next;
} LLNode;

typedef struct TN {
	void *value;
	struct TN *childRight;
	struct TN *childLeft;
} TreeNode;

typedef struct {
	char *name;
	char *value;
	LLNode *children;
	TreeNode *abcChildren; /* children, but ordered alphabetically */
} FSNode;

/*********************
 ***** Functions *****
 *********************/

/* Auxiliary */

int isValidChar(char c);

/* Commands */

/* General */

int triage(FSNode *root, char cmd[]);