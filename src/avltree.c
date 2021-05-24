/*
 *	File: avltree.c
 *	Author: Rafael Serra e Oliveira (ist199311)
 *	Description: Handler functions for the AVL Tree structure
*/

/*** Include project header file ***/
#include "filesystem.h"

AVLNode *newAVLNode(void *value) {
	AVLNode *new = (AVLNode *)malloc(sizeof(AVLNode));
	new->right = NULL;
	new->left = NULL;
	new->value = value;
	new->height = 1;
	return new;
}

AVLNode *maxAVLNode(AVLNode *root) {
	if (root == NULL || root->right == NULL)
		return root;
	else
		return maxAVLNode(root->right);
}

int getHeightAVLNode(AVLNode *node) {
	if (node == NULL)
		return 0;
	return node->height;
}

int calcHeightAVLNode(AVLNode *node) {
	int left, right;
	left = getHeightAVLNode(node->left);
	right = getHeightAVLNode(node->right);
	return left > right ? left + 1 : right + 1;
}

AVLNode *rotLeftAVLNode(AVLNode *h) {
	AVLNode *x = h->right;

	h->right = x->left;
	x->left = h;
	h->height = calcHeightAVLNode(h);
	x->height = calcHeightAVLNode(x);

	return x;
}

AVLNode *rotRightAVLNode(AVLNode *h) {
	AVLNode *x = h->left;

	h->left = x->right;
	x->right = h;
	h->height = calcHeightAVLNode(h);
	x->height = calcHeightAVLNode(x);

	return x;
}

AVLNode *rotLeftRightAVLNode(AVLNode *node) {
	if (node == NULL)
		return node;
	node->left = rotLeftAVLNode(node->left);
	return rotRightAVLNode(node);
}

AVLNode *rotRightLeftAVLNode(AVLNode *node) {
	if (node == NULL)
		return node;
	node->right = rotRightAVLNode(node->right);
	return rotLeftAVLNode(node);
}

int calcBalanceAVLNode(AVLNode *node) {
	if (node == NULL)
		return 0;
	return getHeightAVLNode(node->left) - getHeightAVLNode(node->right);
}

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

AVLNode *insertAVLNode(AVLNode *root, void *value, int (*cmp)(void *, void *)) {
	if (root == NULL)
		return newAVLNode(value);
	else if (cmp(value, root->value) <= 0)
		root->left = insertAVLNode(root->left, value, cmp);
	else
		root->right = insertAVLNode(root->right, value, cmp);
	return balanceAVL(root);
}

AVLNode *removeAVLNode(AVLNode *root, void *rem, int (*cmp)(void *, void *),
					   void (*freeValue)(void *)) {
	/*
		cmp(rem, A) == 0  <=>  ToRemove == A
		cmp(rem, A) == 1  <=>  ToRemove > A
		cmp(rem, A) == -1 <=>  ToRemove < A
	*/
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