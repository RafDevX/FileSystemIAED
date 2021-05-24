/*
 *	File: doublylinkedlist.c
 *	Author: Rafael Serra e Oliveira (ist199311)
 *	Description: Handler functions for the Doubly-Linked List structure
*/

/*** Include project header file ***/
#include "filesystem.h"

/* Allocate memory for and instantiate a new DLL Node */
DLLNode *newDLLNode() {
	DLLNode *new = (DLLNode *)malloc(sizeof(DLLNode));
	new->prev = NULL;
	new->next = NULL;
	new->value = NULL;
	return new;
}

/* Allocate memory for and instantiate a new DLL */
DLL *newDLL() {
	DLL *new = (DLL *)malloc(sizeof(DLL));
	new->head = NULL;
	new->tail = NULL;
	return new;
}

/* Insert a new value at the end of a DLL */
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

/* Remove nodes with a matching value from a DLL, optionally only removing the
 * first ocurrence and invoking a callback to free the value before deletion. */
void removeDLL(DLL *lst, void *val, int onlyFirst, void (*freeValue)(void *)) {
	DLLNode *aux, *node = lst->head;
	while (node) {
		if (node->value == val) {
			if (node->prev)
				(node->prev)->next = node->next;
			else
				lst->head = node->next;
			if (node->next)
				(node->next)->prev = node->prev;
			else
				lst->tail = node->prev;
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

/* Remove the first element from a DLL and return it */
void *shiftDLL(DLL *lst) {
	DLLNode *item = lst->head;
	void *val = item->value;
	lst->head = item->next;
	if (item->next)
		(item->next)->prev = item->prev;
	if (item == lst->tail)
		lst->tail = NULL;
	free(item);
	return val;
}

/* Return the first element of a DLL that matches a given value. */
void *firstMatchingDLL(DLL *lst, void *value, int (*matches)(void *, void *)) {
	DLLNode *head = lst->head;
	while (head) {
		if (matches(value, head->value))
			return head->value;
		head = head->next;
	}
	return NULL;
}

/* Traverse a DLL in order, optionally calling a callback for each element with
 * an argument and/or freeing nodes. */
void traverseDLL(DLL *ls, void (*f)(void *, void *), int freeNodes, void *arg) {
	if (ls != NULL) {
		DLLNode *next, *head = ls->head;
		while (head) {
			if (f)
				f(head->value, arg);
			next = head->next;
			if (freeNodes)
				free(head);
			head = next;
		}
	}
}

/* Return whether a DLL has no elements */
int emptyDLL(DLL *lst) {
	return lst->head == NULL;
}

/* Receive a string and a delimiter, breaking it up at each ocurrence of the
 * delimiter and creating a DLL with each chunk as an element */
DLL *strToDLL(char str[], char delim[]) {
	char *token;
	DLL *lst = newDLL();

	token = strtok(str, delim);
	while (token != NULL) {
		pushDLL(lst, strdup(token));
		token = strtok(NULL, delim);
	}

	return lst;
}