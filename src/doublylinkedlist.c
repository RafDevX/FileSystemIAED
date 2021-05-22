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

void setValueDLLNode(DLLNode *node, void *val) {
	node->value = val;
}

DLL *newDLL() {
	DLL *new = (DLL *)malloc(sizeof(DLL));
	new->head = NULL;
	new->tail = NULL;
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

void removeDLL(DLL *lst, void *val, int onlyFirst, void (*freeValue)(void *)) {
	DLLNode *aux, *node = lst->head;
	while (node) {
		if (node->value == val) {
			if (node->prev)
				(node->prev)->next = node->next;
			if (node->next)
				(node->next)->prev = node->prev;
			if (freeValue)
				freeValue(node->value);
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

void traverseDLL(DLL *lst, void (*f)(void *), int freeNodes) {
	if (lst != NULL) {
		DLLNode *next, *head = lst->head;
		while (head) {
			if (f)
				f(head->value);
			next = head->next;
			if (freeNodes)
				free(head);
			head = next;
		}
	}
}

void freeDLL(DLL *lst) {
	if (lst != NULL) {
	}
}