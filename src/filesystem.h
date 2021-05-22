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
#define CMD_HELP "help"
#define CMD_SET "set"
#define CMD_PRINT "print"
#define CMD_FIND "find"
#define CMD_LIST "list"
#define CMD_SEARCH "search"
#define CMD_DELETE "delete"

/*** Command Errors ***/

#define ERR_NOT_FOUND "not found"
#define ERR_NO_DATA "no data"

/*** Help Text ***/

#define MAX_HELP_LINE_LEN 62
#define HELP_LINES                                                     \
	{ "help: Imprime os comandos disponíveis",                         \
	  "quit: Termina o programa.",                                     \
	  "set: Adiciona ou modifica o valor a armazenar.",                \
	  "print: Imprime todos os caminhos e valores.",                   \
	  "find: Imprime o valor armazenado.",                             \
	  "list: Lista todos os componentes imediatos de um sub-caminho.", \
	  "search: Procura o caminho dado um valor.",                      \
	  "delete: Apaga um caminho e todos os subcaminhos." }

/*** Paths ***/

#define ROOT_NAME ""
#define MAX_PATH_LEN 10
#define PATH_SEPARATOR "/"

/*** Return Codes ***/

#define RETCODE_OK 0
#define RETCODE_UNKNOWN_CMD 1

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

typedef struct DN {
	char *name;
	char *value;
	DLL *children;
	AVLNode *abcChildren; /* children, but ordered alphabetically */
	struct DN *parent;
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
void *shiftDLL(DLL *lst);
void *firstMatchingDLL(DLL *lst, void *value, int (*matches)(void *, void *));
void traverseDLL(DLL *lst, void (*f)(void *), int freeNodes);
int emptyDLL(DLL *lst);
DLL *strToDLL(char str[], char delim[]);

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
Dir *newChildDir(Dir *parent, char name[]);
void setValueDir(Dir *dir, char value[]);
void deleteDir(Dir *dir);
void deleteDirWrapper(void *value);
void printDir(Dir *dir);
void printDirWrapper(void *value);
Dir *findDir(Dir *root, DLL *path, int createIfMissing);
void listAbcChildrenDir(Dir *dir);
void printChildDir(void *c);
char *searchDir(Dir *root, char value[]);
int cmpValuesDir(void *a, void *b);
int matchesNameDir(void *a, void *b);

/* Auxiliary */

int isValidChar(char c);
char *strdup(char *s1);

/* Commands */

void cmdHelp();
void cmdSet(Dir *root, char args[]);
void cmdPrint(Dir *root);
void cmdFind(Dir *root, char path[]);
void cmdList(Dir *root, char path[]);
void cmdSearch(Dir *root, char value[]);
void cmdDelete(Dir *root, char path[]);

/* General */

int triage(Dir *root, char cmd[], char args[]);
