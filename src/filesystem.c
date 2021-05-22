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
	Dir *root = newDir(ROOT_NAME);
	char instr[MAX_INSTR_LENGTH], cmd[MAX_CMD_LENGTH];

	while (fgets(instr, MAX_INSTR_LENGTH, stdin)) {
		instr[strlen(instr) - 1] = '\0'; /* remove newline at the end */
		sscanf(instr, "%s", cmd);

		if (!triage(root, cmd, instr + strlen(cmd) + 1))
			return RETCODE_UNKNOWN_CMD;
	}

	deleteDir(root, NULL);

	return RETCODE_OK;
}

/* Call the appropriate function for a command. Return whether it succeeded. */
int triage(Dir *root, char cmd[], char args[]) {
	/* */
	printf("Received cmd = |%s|, args = |%s|\n", cmd, args);
	return !!root;
}