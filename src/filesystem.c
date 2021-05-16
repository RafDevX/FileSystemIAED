/*
 *	File: filesystem.c
 *	Author: Rafael Serra e Oliveira (ist199311)
 *	Description: Main project file.
*/

/*** Include project header file ***/
#include "filesystem.h"

/* Master logic: start up, read a command key and send it to triage */
int main() {
	FSNode *root;
	char cmd[MAX_CMD_LENGTH];
	/* TODO: startup */

	while (scanf("%s", cmd) && isValidChar(cmd[0]) && !strcmp(cmd, CMD_QUIT))
		if (!triage(root, cmd))
			return RETCODE_UNKNOWN_CMD;

	/* call cleanup function */

	return RETCODE_OK;
}

/* Call the appropriate function for a command. Return whether it succeeded. */
int triage(FSNode *root, char cmd[]) {
	/* */
	return 1;
}