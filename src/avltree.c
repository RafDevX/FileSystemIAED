/*
 *	File: avltree.c
 *	Author: Rafael Serra e Oliveira (ist199311)
 *	Description: Handler functions for the AVL Tree structure
*/

/*** Include project header file ***/
#include "filesystem.h"

/* Allocate and return a new AVL Node */
AVLNode *newAVLNode(void *value) {
	AVLNode *new = (AVLNode *)malloc(sizeof(AVLNode));
	if (new == NULL)
		return new;
	new->right = NULL;
	new->left = NULL;
	new->value = value;
	new->height = 1;
	return new;
}

/* Return the node with the max value within a subtree */
AVLNode *maxAVLNode(AVLNode *root) {
	if (root == NULL || root->right == NULL)
		return root;
	else
		return maxAVLNode(root->right);
}

/* Read height for an AVL node or 0 if no node is passed */
int getHeightAVLNode(AVLNode *node) {
	if (node == NULL)
		return 0;
	return node->height;
}

/* Calculate the height for an AVL node based on its children's height */
int calcHeightAVLNode(AVLNode *node) {
	int left, right;
	left = getHeightAVLNode(node->left);
	right = getHeightAVLNode(node->right);
	return left > right ? left + 1 : right + 1;
}

/* Execute a left rotation on an AVL node */
AVLNode *rotLeftAVLNode(AVLNode *h) {
	AVLNode *x = h->right;

	h->right = x->left;
	x->left = h;
	h->height = calcHeightAVLNode(h);
	x->height = calcHeightAVLNode(x);

	return x;
}

/* Execute a right rotation on an AVL node */
AVLNode *rotRightAVLNode(AVLNode *h) {
	AVLNode *x = h->left;

	h->left = x->right;
	x->right = h;
	h->height = calcHeightAVLNode(h);
	x->height = calcHeightAVLNode(x);

	return x;
}

/* Execute a left-right rotation on an AVL node */
AVLNode *rotLeftRightAVLNode(AVLNode *node) {
	if (node == NULL)
		return node;
	node->left = rotLeftAVLNode(node->left);
	return rotRightAVLNode(node);
}

/* Execute a right-left rotation on an AVL node */
AVLNode *rotRightLeftAVLNode(AVLNode *node) {
	if (node == NULL)
		return node;
	node->right = rotRightAVLNode(node->right);
	return rotLeftAVLNode(node);
}

/* Calculate the balance factor for an AVL node */
int calcBalanceAVLNode(AVLNode *node) {
	if (node == NULL)
		return 0;
	return getHeightAVLNode(node->left) - getHeightAVLNode(node->right);
}

/* Balance an AVL tree by performing rotation operations */
AVLNode *balanceAVL(AVLNode *root) {
	int balance;
	if (root == NULL)
		return root;
	balance = calcBalanceAVLNode(root);
	if (balance > 1)
		if (calcBalanceAVLNode(root->left) >= 0)
			root = rotRightAVLNode(root);
		else
			root = rotLeftRightAVLNode(root);
	else if (balance < -1)
		if (calcBalanceAVLNode(root->right) <= 0)
			root = rotLeftAVLNode(root);
		else
			root = rotRightLeftAVLNode(root);
	else
		root->height = calcHeightAVLNode(root);
	return root;
}

/* Inserts a new value into an AVL tree */
AVLNode *insertAVLNode(AVLNode *root, void *value, int (*cmp)(void *, void *)) {
	if (root == NULL)
		return newAVLNode(value);
	else if (cmp(value, root->value) <= 0)
		root->left = insertAVLNode(root->left, value, cmp);
	else
		root->right = insertAVLNode(root->right, value, cmp);
	return balanceAVL(root);
}

/* Removes the AVL node with a certain value from an AVL tree; compare function
 * should be such that (with rem being the value to remove):
 * 		cmp(rem, A) ==  0 <=> ToRemove == A
 * 		cmp(rem, A) ==  1 <=> ToRemove > A
 * 		cmp(rem, A) == -1 <=> ToRemove < A */
AVLNode *removeAVLNode(AVLNode *root, void *rem, int (*cmp)(void *, void *),
					   void (*freeValue)(void *)) {
	int c;
	if (root == NULL)
		return root;
	c = cmp(rem, root->value);
	if (c < 0) {
		root->left = removeAVLNode(root->left, rem, cmp, freeValue);
	} else if (c > 0) {
		root->right = removeAVLNode(root->right, rem, cmp, freeValue);
	} else {
		root = auxRemoveAVLNode(root, cmp, freeValue);
	}
	return balanceAVL(root);
}

/* Helper function to actually remove a correct node after it is found */
AVLNode *auxRemoveAVLNode(AVLNode *root, int (*cmp)(void *, void *),
						  void (*freeValue)(void *)) {
	if (root->right != NULL && root->left != NULL) {
		AVLNode *max = maxAVLNode(root->left);
		void *aux = max->value;
		max->value = root->value;
		root->value = aux;
		root->left = removeAVLNode(root->left, max->value, cmp, freeValue);
	} else {
		AVLNode *aux = root;
		if (root->right == NULL && root->left == NULL)
			root = NULL;
		else if (root->right == NULL)
			root = root->left;
		else
			root = root->right;
		if (freeValue != NULL)
			freeValue(aux->value);
		free(aux);
	}
	return root;
}

/* Traverse an AVL Tree in a specified order, invoking a callback for each */
void traverseAVL(AVLNode *root, enum AVLTraversalType type, void (*f)(void *)) {
	if (root == NULL) {
		return;
	} else if (type == PRE_ORDER) {
		f(root->value);
		traverseAVL(root->left, type, f);
		traverseAVL(root->right, type, f);
	} else if (type == IN_ORDER) {
		traverseAVL(root->left, type, f);
		f(root->value);
		traverseAVL(root->right, type, f);
	} else if (type == POST_ORDER || type == FREE) {
		traverseAVL(root->left, type, f);
		traverseAVL(root->right, type, f);
		if (f != NULL)
			f(root->value);
		if (type == FREE)
			free(root);
	}
}

/* Find an element in an AVL Tree based on its value */
void *searchAVL(AVLNode *root, void *key, int (*cmp)(void *, void *)) {
	int c;
	if (root == NULL)
		return root;
	c = cmp(key, root->value);
	if (c < 0)
		return searchAVL(root->left, key, cmp);
	else if (c > 0)
		return searchAVL(root->right, key, cmp);
	else
		return root->value;
}