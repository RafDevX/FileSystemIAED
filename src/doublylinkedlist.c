/*
 *	File: doublylinkedlist.c
 *	Author: Rafael Serra e Oliveira (ist199311)
 *	Description: Handler functions for the Doubly-Linked List structure
*/

/*** Include project header file ***/
#include "filesystem.h"

DLLNode *newDLLNode() {
	DLLNode *new = (DLLNode *)malloc(sizeof(DLLNode));
	if (new == NULL)
		return new;
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
	if (new == NULL)
		return new;
	new->head = NULL;
	new->tail = NULL;
	return new;
}

DLLNode *pushDLL(DLL *lst, void *value) {
	DLLNode *node = newDLLNode();
	if (node == NULL)
		return node;
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

void *firstMatchingDLL(DLL *lst, void *value, int (*matches)(void *, void *)) {
	DLLNode *head = lst->head;
	while (head) {
		if (matches(value, head->value))
			return head->value;
		head = head->next;
	}
	return NULL;
}

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

int emptyDLL(DLL *lst) {
	return lst->head == NULL;
}

DLL *strToDLL(char str[], char delim[]) {
	char *token, *dup;
	DLL *lst = newDLL();

	if (lst == NULL)
		return NULL;

	token = strtok(str, delim);
	while (token != NULL) {
		dup = strdup(token);
		if (dup == NULL || pushDLL(lst, dup) == NULL)
			return NULL;
		token = strtok(NULL, delim);
	}

	return lst;
}