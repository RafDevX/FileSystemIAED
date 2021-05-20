/*
 *	File: avltree.c
 *	Author: Rafael Serra e Oliveira (ist199311)
 *	Description: Handler functions for the AVL Tree  structure
*/

/*** Include project header file ***/
#include "filesystem.h"

AVLNode *newAVLNode() {
	AVLNode *new = (AVLNode *)malloc(sizeof(AVLNode));
	new->right = NULL;
	new->left = NULL;
	new->value = NULL;
	new->height = 1;
	return new;
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
