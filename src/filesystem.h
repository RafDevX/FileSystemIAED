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

/* Doubly-Linked Lists */

typedef struct DLLN {
	void *value;
	struct DLLN *prev;
	struct DLLN *next;
} DLLNode;

typedef struct {
	DLLNode *head;
	DLLNode *tail;
} DLL;

/* AVL Trees */

typedef struct AVLN {
	void *value;
	int height;
	struct AVLN *right;
	struct AVLN *left;
} AVLNode;

/* FileSystem Node */

typedef struct {
	char *name;
	char *value;
	DLL *children;
	AVLNode *abcChildren; /* children, but ordered alphabetically */
} FSNode;

/*****************
 ***** Enums *****
 *****************/

enum AVLTraversalType { PRE_ORDER,
						IN_ORDER,
						POST_ORDER };

/*********************
 ***** Functions *****
 *********************/

/* Interfaces */

AVLNode *newAVLNode(void *value);
void freeValueAVLNode(AVLNode *node);
void setValueAVLNode(AVLNode *node, void *value);
AVLNode *maxAVLNode(AVLNode *root);
int getHeightAVLNode(AVLNode *node);
int calcHeightAVLNode(AVLNode *node);
AVLNode *rotLeftAVLNode(AVLNode *h);
AVLNode *rotRightAVLNode(AVLNode *h);
AVLNode *rotLeftRightAVLNode(AVLNode *node);
AVLNode *rotRightLeftAVLNode(AVLNode *node);
int calcBalanceAVLNode(AVLNode *node);
AVLNode *balanceAVL(AVLNode *root);
AVLNode *insertAVLNode(AVLNode *root, void *value, int (*cmp)(void *, void *));
AVLNode *removeAVLNode(AVLNode *root, void *rem, int (*cmp)(void *, void *),
					   int freeValue);
void traverseAVL(AVLNode *root, enum AVLTraversalType type, void (*f)(void *));

/* Auxiliary */

int isValidChar(char c);
int isTruthy(void *v);

/* Commands */

/* General */

int triage(FSNode *root, char cmd[]);
