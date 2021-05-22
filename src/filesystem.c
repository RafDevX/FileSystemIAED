/*
 *	File: filesystem.c
 *	Author: Rafael Serra e Oliveira (ist199311)
 *	Description: Main project file.
*/

/*** Include project header file ***/
#include "filesystem.h"

/* FIXME: TESTING - DELETE THIS */
struct number {
	int num;
};

struct number *ns(int a) {
	struct number *n = (struct number *)malloc(sizeof(struct number *));
	n->num = a;
	return n;
}

int num_cmp(void *a, void *b) {
	int n1 = ((struct number *)a)->num;
	int n2 = ((struct number *)b)->num;
	if (n1 < n2)
		return -1;
	else if (n1 > n2)
		return 1;
	else
		return 0;
}

int cmp2(void *a, void *b) {
	int n1 = (int)a;
	int n2 = ((struct number *)b)->num;
	if (n1 < n2)
		return -1;
	else if (n1 > n2)
		return 1;
	else
		return 0;
}

void print_n(void *a) {
	printf("%d\n", ((struct number *)a)->num);
}
/* END TESTING */

/* Master logic: start up, read a command key and send it to triage */
int main() {
	Dir *root;
	char cmd[MAX_CMD_LENGTH];
	/* TODO: startup */

	AVLNode *h = newAVLNode(ns(20));
	h = insertAVLNode(h, ns(30), num_cmp);
	h = insertAVLNode(h, ns(37), num_cmp);
	h = insertAVLNode(h, ns(32), num_cmp);
	h = insertAVLNode(h, ns(27), num_cmp);
	h = insertAVLNode(h, ns(24), num_cmp);
	h = insertAVLNode(h, ns(35), num_cmp);
	h = insertAVLNode(h, ns(29), num_cmp);
	h = removeAVLNode(h, ns(30), num_cmp, 0);
	traverseAVL(h, POST_ORDER, print_n);
	print_n(searchAVL(h, 32, cmp2));

	while (scanf("%s", cmd) && isValidChar(cmd[0]) && !strcmp(cmd, CMD_QUIT))
		if (!triage(root, cmd))
			return RETCODE_UNKNOWN_CMD;

	/* call cleanup function */

	return RETCODE_OK;
}

/* Call the appropriate function for a command. Return whether it succeeded. */
int triage(Dir *root, char cmd[]) {
	/* */
	return !!root && !!cmd;
}