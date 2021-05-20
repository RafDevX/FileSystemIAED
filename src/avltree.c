/*
 *	File: avltree.c
 *	Author: Rafael Serra e Oliveira (ist199311)
 *	Description: Handler functions for the AVL Tree  structure
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

void freeValueAVLNode(AVLNode *node) {
	if (node->value) {
		free(node->value);
		node->value = NULL;
	}
}

void setValueAVLNode(AVLNode *node, void *value) {
	freeValueAVLNode(node);
	node->value = value;
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
	return calcHeightAVLNode(node->left) - calcHeightAVLNode(node->right);
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
			root = rotLeftAVLNode(root);
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

AVLNode *removeAVLNode(AVLNode *root, int (*cmp)(void *)) {
	/*
		cmp(A) == 0  <=>  ToRemove == A
		cmp(A) == 1  <=>  ToRemove > A
		cmp(A) == -1 <=>  ToRemove < A
	*/
	int c;
	if (root == NULL)
		return root;
	c = cmp(root->value);
	if (c < 0) {
		root->left = removeAVLNode(root->left, cmp);
	} else if (c > 0) {
		root->right = removeAVLNode(root->right, cmp);
	} else {
		/* TODO: finish this */
	}
	return balanceAVL(root);
}