/*
 *	File: filesystem.c
 *	Author: Rafael Serra e Oliveira (ist199311)
 *	Description: Main project file.
*/

/*** Include project header file ***/
#include "filesystem.h"

/* Master logic: start up, read a command key and send it to triage */
int main() {
	Dir *root = newDir(ROOT_NAME);
	char instr[MAX_INSTR_LENGTH], cmd[MAX_CMD_LENGTH];

	while (fgets(instr, MAX_INSTR_LENGTH, stdin)) {
		instr[strlen(instr) - 1] = '\0'; /* remove newline at the end */
		sscanf(instr, "%s", cmd);
		if (strcmp(cmd, CMD_QUIT) == 0)
			break;

		if (!triage(root, cmd, instr + strlen(cmd) + 1))
			return RETCODE_UNKNOWN_CMD;
	}

	deleteDir(root, NULL);

	return RETCODE_OK;
}

/* Call the appropriate function for a command. Return whether it succeeded. */
int triage(Dir *root, char cmd[], char args[]) {
	if (strcmp(cmd, CMD_HELP) == 0)
		cmdHelp();
	else
		return 0;
	return 1;
}