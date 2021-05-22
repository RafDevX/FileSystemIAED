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

#define MAX_INSTR_LENGTH 65535
#define MAX_CMD_LENGTH 6
#define CMD_QUIT "quit"

/*** Return Codes ***/

#define RETCODE_OK 0
#define RETCODE_UNKNOWN_CMD 1

/*** General ***/

#define ROOT_NAME ""

/************************
 ***** Custom Types *****
 ************************/

/* Doubly-Linked List */

typedef struct DLLN {
	void *value;
	struct DLLN *prev;
	struct DLLN *next;
} DLLNode;

typedef struct {
	DLLNode *head;
	DLLNode *tail;
} DLL;

/* AVL Tree */

typedef struct AVLN {
	void *value;
	int height;
	struct AVLN *right;
	struct AVLN *left;
} AVLNode;

/* Directory */

typedef struct {
	char *name;
	char *value;
	DLL *children;
	AVLNode *abcChildren; /* children, but ordered alphabetically */
} Dir;

/*****************
 ***** Enums *****
 *****************/

enum AVLTraversalType {
	PRE_ORDER,
	IN_ORDER,
	POST_ORDER,
	FREE
};

/*********************
 ***** Functions *****
 *********************/

/* Interfaces */

DLLNode *newDLLNode();
void setValueDLLNode(DLLNode *node, void *val);
DLL *newDLL();
DLLNode *pushDLL(DLL *lst, void *value);
void removeDLL(DLL *lst, void *val, int onlyFirst, void (*freeValue)(void *));
void traverseDLL(DLL *lst, void (*f)(void *), int freeNodes);

AVLNode *newAVLNode(void *value);
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
					   void (*freeValue)(void *));
void traverseAVL(AVLNode *root, enum AVLTraversalType type, void (*f)(void *));
void *searchAVL(AVLNode *root, void *key, int (*cmp)(void *, void *));

Dir *newDir(char name[]);
int cmpValuesDir(void *a, void *b);
Dir *newChildDir(Dir *parent, char name[]);
void setValueDir(Dir *dir, char value[]);
void deleteDir(Dir *dir, Dir *parent);
void deleteDirWrapper(void *value);

/* Auxiliary */

int isValidChar(char c);
char *strdup(char *s1);

/* Commands */

/* General */

int triage(Dir *root, char cmd[], char args[]);
