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
	enum TriageStatus status = SUCCESS;

	while (status == SUCCESS && fgets(instr, MAX_INSTR_LEN, stdin)) {
		int lastPos = strlen(instr) - 1;
		if (instr[lastPos] == '\n')
			instr[lastPos] = '\0';
		sscanf(instr, "%s", cmd);
		if (strcmp(cmd, CMD_QUIT) == 0)
			break;

		status = triage(root, valuesTable, cmd, instr + strlen(cmd) + 1);
	}

	if (status == UNKNOWN_CMD)
		return RETCODE_UNKNOWN_CMD;
	else if (status == NO_MEMORY)
		printf("%s\n", ERR_NO_MEMORY);

	deleteDir(root, 1, 0, deleteDirNOP, NULL);
	freeHashT(valuesTable);

	return RETCODE_OK;
}

/* Call the appropriate function for a command. Return whether it succeeded. */
enum TriageStatus triage(Dir *root, HashT *vTable, char cmd[], char args[]) {
	int ok;
	if (strcmp(cmd, CMD_HELP) == 0)
		ok = cmdHelp();
	else if (strcmp(cmd, CMD_SET) == 0)
		ok = cmdSet(root, vTable, args);
	else if (strcmp(cmd, CMD_PRINT) == 0)
		ok = cmdPrint(root);
	else if (strcmp(cmd, CMD_FIND) == 0)
		ok = cmdFind(root, args);
	else if (strcmp(cmd, CMD_LIST) == 0)
		ok = cmdList(root, args);
	else if (strcmp(cmd, CMD_SEARCH) == 0)
		ok = cmdSearch(vTable, args);
	else if (strcmp(cmd, CMD_DELETE) == 0)
		ok = cmdDelete(root, vTable, args);
	else
		return UNKNOWN_CMD;

	if (ok)
		return SUCCESS;
	else
		return NO_MEMORY;
}