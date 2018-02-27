#include "util.h"
#include "ivec.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>

void parse_arguments(char* cmd, Ivec* args) {
	char arg[256]; 
	int sub_ii = 0;
	int arg_ii = 0; 
	for (int ii = 0; ii < strlen(cmd); ++ii) {
		arg[sub_ii] = cmd[ii]; 
		sub_ii++;
		if (cmd[ii] == ' ' || ii == strlen(cmd) - 1) {
			sub_ii = sub_ii - 1; 
			arg[sub_ii] = 0; 
			pushBack(args, arg); 
			sub_ii = 0; 
			arg_ii++; 
		}
	}
}

bool user_exit(char* exit_request) {
	char* exit = "exit"; 
	if (strlen(exit_request) == 1) {
	    return false; 
	}
	for (int ii = 0; ii < strlen(exit_request) - 1; ii++) {
		if (exit_request[ii] != exit[ii]) {
			return false; 
		}
	}
	return true;
}

bool handle_cd(char* cmd) {
	// check to see if cmd starts with 'cd '
	bool cd_flag = (cmd[0] == 'c' && cmd[1] == 'd' && cmd[3] != ' '); 
	if (!cd_flag) {
		return false;
	}

	// parse commands and call execvp
	Ivec* args = make_ivec();
	parse_arguments(cmd, args);

	char* inputs[args->size + 1]; 
	inputs[args->size] = 0; 
	for (int ii = 0; ii < args->size; ii++) {
		inputs[ii] = get_ivec(args, ii); 
	}
	chdir(inputs[1]); 
	free_ivec(args);
	return true;
}

// Redirection handlers --------------------------------------------------
void input_redirect(Ivec* args, int index) {
	char* filename = get_ivec(args, index); 
	char* mode = "r";
	FILE* stream = stdin; 
	
	// Redirect the file to stdin
	freopen(filename, mode, stream);	
	return;
}

void output_redirect(Ivec* args, int index) {
	char* filename = get_ivec(args, index);
	char* mode = "w";
	FILE* stream = stdout;

	// Redirect the stdin to the file
	freopen(filename, mode, stream);	
	return;
}

// && and || operators -------------------------------------------------
bool check_for_and(Ivec* args) {
	for (int ii = 0; ii < args->size; ii++) {
		if (strcmp(args->data[ii], "&&") == 0) {
			return true;
		}
	}
	return false;
}

bool check_for_or(Ivec* args) {
	for (int ii = 0; ii < args->size; ii++) {
		if (strcmp(args->data[ii], "||") == 0) {
			return true;
		}
	}
	return false;
}

bool check_for_semi_colon(Ivec* args) {
	for (int ii = 0; ii < args->size; ii++) {
		if (strcmp(args->data[ii], ";") == 0) {
			return true;
		}
	}
	return false;
}

void parse_first_cmd(char* cmd, char* first_cmd) {
	for (int ii = 0; ii < strlen(cmd); ii++) {
		if (cmd[ii] == '&' && cmd[ii + 1] == '&') {
			first_cmd[ii] = 0;
			break;
		}
		else if (cmd[ii] == '|' && cmd[ii + 1] == '|') {
			first_cmd[ii] = 0;
			break;
		}
		else if (cmd[ii] == ';') {
			break;
		}
		else if (cmd[ii] == '|') {
			first_cmd[ii] = 0;
			break;
		}
		else {
			first_cmd[ii] = cmd[ii];
		}
	}
	return;
}

void parse_rest_of_cmd(char* cmd, char* rest_of_cmd) {
	bool operator_found = false;
	bool and_found, or_found, semi_colon_found, pipe_found;
	int offset = 0; 
	for (int ii = 0; ii < strlen(cmd); ii++) {
		and_found = (cmd[ii] == '&') && (cmd[ii + 1] == '&');
		or_found = (cmd[ii] == '|') && (cmd[ii + 1] == '|');
		semi_colon_found = (cmd[ii] == ';');
		pipe_found = (cmd[ii] == '|');
		if (and_found || or_found) {
			offset  = ii + 3; 
			break;
		}
		if (semi_colon_found || pipe_found) {
			offset = ii + 2; 
			break;
		}
	}
	for (int jj = 0; jj < strlen(cmd); jj++) {
		rest_of_cmd[jj] = cmd[jj + offset];
	}
	return;
}

bool check_for_background(Ivec* args) {
	if (strcmp(args->data[args->size - 1], "&") == 0) {
		return true;
	}
return false;
}

bool check_for_pipe(Ivec* args) {
	for (int ii = 0; ii < args->size; ii++) {
		if (strcmp(args->data[ii], "|") == 0) {
			return true;
		}
	}
	return false;
}
