/*
 *	File: doublylinkedlist.c
 *	Author: Rafael Serra e Oliveira (ist199311)
 *	Description: Handler functions for the Doubly-Linked List structure
*/

/*** Include project header file ***/
#include "filesystem.h"

DLLNode *newDLLNode() {
	DLLNode *new = (DLLNode *)malloc(sizeof(DLLNode));
	new->prev = NULL;	
	new->next = NULL;
	new->value = NULL;
	return new;
}

void freeValueDLLNode(DLLNode *node) {
	if (node->value != NULL) {
		free(node->value);
		node->value = NULL;
	}
}

void setValueDLLNode(DLLNode *node, void *val) {
	freeValueDLL(node);
	node->value = val;
}

DLL *newDLL() {
	DLL *new = (DLL *)malloc(sizeof(DLL));
	DLL->head = NULL;
	DLL->tail = NULL;
	return new;
}

DLLNode *pushDLL(DLL *lst, void *value) {
	DLLNode *node = newDLLNode();
	node->value = value;
	if (lst->head) { 
		(lst->tail)->next = node;
		node->prev = lst->tail;
	} else {
		lst->head = node;
	}
	lst->tail = node;
	return node;
}

void removeDLL(DLL *lst, int (*shouldRemoveNode)(void*), int onlyFirst) {
	DLLNode *aux, *node = lst->head;
	while (node) {
		if (shouldRemoveNode(node->value)) {
			if (node->prev)
				(node->prev)->next = node->next;
			if (node->next)
				(node->next)->prev = node->prev;
			freeValueDLLNode(node);
			aux = node->next;
			free(node);
			if (onlyFirst)
				break;
			node = aux;
		} else {
			node = node->next;
		}
	}
}

void freeDLL(DLL *lst) {
	removeDLL(lst, &isTruthy, 0);
	free(DLL);
}
