/*
 *	File: linkedlist.c
 *	Author: Rafael Serra e Oliveira (ist199311)
 *	Description: Handler functions for the LinkedList structure
*/

/*** Include project header file ***/
#include "filesystem.h"

LLNode *newNodeLL() {
	LLNode *new = (LLNode *)malloc(sizeof(LLNode));
	new->next = NULL;
	new->value = NULL;
	return new;
}

void freeValueLL(LLNode *node) {
	if (node->value != NULL) {
		free(node->value);
		node->value = NULL;
	}
}

void setValueLL(LLNode *node, void *val) {
	freeValueLL(node);
	node->value = val;
}

LLNode *getTailLL(LLNode *head) {
	for (; head->next; head = head->next)
		;
	return head;
}

void pushLL(LLNode *head, void *newVal) {
	LLNode *tail = getTailLL(head), *new = newNodeLL();
	setValueLL(new, newVal);
	tail->next = new;
}