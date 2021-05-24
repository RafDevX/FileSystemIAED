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

/*** General Settings ***/

/* Max possible length for instructions, including trailing \0 */
#define MAX_INSTR_LEN 65536

/* Max possible command length, including trailing \0 */
#define MAX_CMD_LEN 7

/* Dimension to use for the values hash table, should be prime */
#define VALUES_TABLE_DIM 49157

/* Name for the root directory */
#define ROOT_NAME ""

/* Max possible path len, probably MAX_INSTR_LEN - 6 */
#define MAX_PATH_LEN 65530

/* String to use to separate path components */
#define PATH_SEPARATOR "/"

/*** Commands ***/

/* Available commands */

#define CMD_QUIT "quit"
#define CMD_HELP "help"
#define CMD_SET "set"
#define CMD_PRINT "print"
#define CMD_FIND "find"
#define CMD_LIST "list"
#define CMD_SEARCH "search"
#define CMD_DELETE "delete"

/*** Errors Messages ***/

/* User-oriented error messages */

#define ERR_NOT_FOUND "not found"
#define ERR_NO_DATA "no data"
#define ERR_NO_MEMORY "No memory."

/*** Command Output Formats ***/

/* Output format strings for each command */

#define OUT_FORMAT_HELP_CMD "%s\n"
#define OUT_FORMAT_PRINT_CMD "%s %s\n"
#define OUT_FORMAT_FIND_CMD "%s\n"
#define OUT_FORMAT_LIST_CMD "%s\n"
#define OUT_FORMAT_SEARCH_CMD "%s\n"

/* Output format for error messages */
#define OUT_FORMAT_ERR "%s\n"

/*** Help Text ***/

/* Max possible length for each help line */
#define MAX_HELP_LINE_LEN 62

/* Array of all help lines to output with the help command */
#define HELP_LINES                                                     \
	{ "help: Imprime os comandos disponíveis.",                        \
	  "quit: Termina o programa.",                                     \
	  "set: Adiciona ou modifica o valor a armazenar.",                \
	  "print: Imprime todos os caminhos e valores.",                   \
	  "find: Imprime o valor armazenado.",                             \
	  "list: Lista todos os componentes imediatos de um sub-caminho.", \
	  "search: Procura o caminho dado um valor.",                      \
	  "delete: Apaga um caminho e todos os subcaminhos." }

/*** Return Codes ***/

/* Possible program exit codes */

#define RETCODE_OK 0
#define RETCODE_UNKNOWN_CMD 1

/************************
 ***** Custom Types *****
 ************************/

/*** Doubly-Linked List ***/

typedef struct DLLN {
	void *value;
	struct DLLN *prev;
	struct DLLN *next;
} DLLNode;

typedef struct {
	DLLNode *head;
	DLLNode *tail;
} DLL;

/*** AVL Tree ***/

typedef struct AVLN {
	void *value;
	int height;
	struct AVLN *right;
	struct AVLN *left;
} AVLNode;

/*** Directory ***/

typedef struct DN {
	char *name;
	char *value;
	DLL *children;
	AVLNode *abcChildren; /* children, but ordered alphabetically */
	struct DN *parent;
} Dir;

/*** Hash Table ***/

typedef struct {
	long int dim;
	void *(*getKey)(void *);
	long int (*hash)(void *, long int);
	DLL **entries;
} HashT;

/*****************
 ***** Enums *****
 *****************/

/* How to traverse an AVL Tree */
enum AVLTraversalType {
	PRE_ORDER,
	IN_ORDER,
	POST_ORDER,
	FREE
};

/*********************
 ***** Functions *****
 *********************/

/*** Interfaces ***/

/* Doubly-Linked List Manipulation */

DLLNode *newDLLNode();
DLL *newDLL();
DLLNode *pushDLL(DLL *lst, void *value);
void removeDLL(DLL *lst, void *val, int onlyFirst, void (*freeValue)(void *));
void *shiftDLL(DLL *lst);
void *firstMatchingDLL(DLL *lst, void *value, int (*matches)(void *, void *));
void traverseDLL(DLL *lst, void (*f)(void *, void *), int freeNodes, void *arg);
int emptyDLL(DLL *lst);
DLL *strToDLL(char str[], char delim[]);

/* AVL Tree Manipulation */

AVLNode *newAVLNode(void *value);
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
AVLNode *auxRemoveAVLNode(AVLNode *root, int (*cmp)(void *, void *),
						  void (*freeValue)(void *));
void traverseAVL(AVLNode *root, enum AVLTraversalType type, void (*f)(void *));
void *searchAVL(AVLNode *root, void *key, int (*cmp)(void *, void *));

/* Directory Manipulation */

Dir *newDir(char name[]);
Dir *newChildDir(Dir *parent, char name[]);
void setValueDir(Dir *dir, char value[]);
void deleteDir(Dir *dir, int top, int skipTop,
			   void (*callback)(Dir *, void *), void *arg);
void deleteDirWrapper(void *value, void *arg);
void printDir(Dir *dir, char buffer[]);
void printDirWrapper(void *value, void *arg);
void deleteDirNOP(Dir *a, void *b);
Dir *findDir(Dir *root, DLL *path, int createIfMissing);
void listAbcChildrenDir(Dir *dir);
void printChildDir(void *c);
char *calcPathDir(Dir *dir);
int cmpNamesDir(void *a, void *b);
int matchesNameDir(void *a, void *b);
void *getValueDir(void *dir);

/* Hash Table Manipulation */

HashT *newHashT(long int dim, void *getKey(void *),
				long int (*hash)(void *, long int));
DLL *searchHashT(HashT *table, void *key);
void insertHashT(HashT *table, void *value);
void removeHashT(HashT *table, void *value);
void freeHashT(HashT *table);

/*** Auxiliary ***/

char *strdup(char *s1);
long int hashS(void *key, long int M);
void searchAux(void *val, void *args);
void deleteAux(Dir *dir, void *valuesTable);

/*** Commands ***/

void cmdHelp();
void cmdSet(Dir *root, HashT *valuesTable, char args[]);
void cmdPrint(Dir *root);
void cmdFind(Dir *root, char path[]);
void cmdList(Dir *root, char path[]);
void cmdSearch(HashT *valuesTable, char value[]);
void cmdDelete(Dir *root, HashT *valuesTable, char path[]);

/*** General ***/

int triage(Dir *root, HashT *valuesTable, char cmd[], char args[]);