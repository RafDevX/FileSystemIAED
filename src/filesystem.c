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
	HashT *valuesTable = newHashT(VALUES_TABLE_DIM, getValueDir, hashS);
	char instr[MAX_INSTR_LEN], cmd[MAX_CMD_LEN];

	while (fgets(instr, MAX_INSTR_LEN, stdin)) {
		instr[strlen(instr) - 1] = '\0'; /* remove newline at the end */
		sscanf(instr, "%s", cmd);
		if (strcmp(cmd, CMD_QUIT) == 0)
			break;

		if (!triage(root, valuesTable, cmd, instr + strlen(cmd) + 1))
			return RETCODE_UNKNOWN_CMD;
	}

	deleteDir(root, 1);
	freeHashT(valuesTable);

	return RETCODE_OK;
}

/* Call the appropriate function for a command. Return whether it succeeded. */
int triage(Dir *root, HashT *valuesTable, char cmd[], char args[]) {
	if (strcmp(cmd, CMD_HELP) == 0)
		cmdHelp();
	else if (strcmp(cmd, CMD_SET) == 0)
		cmdSet(root, args);
	else if (strcmp(cmd, CMD_PRINT) == 0)
		cmdPrint(root);
	else if (strcmp(cmd, CMD_FIND) == 0)
		cmdFind(root, args);
	else if (strcmp(cmd, CMD_LIST) == 0)
		cmdList(root, args);
	else if (strcmp(cmd, CMD_SEARCH) == 0)
		cmdSearch(valuesTable, args);
	else if (strcmp(cmd, CMD_DELETE) == 0)
		cmdDelete(root, args);
	else
		return 0;
	return 1;
}