#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "ivec.h"
#include "util.h"

void
execute(char* cmd, int pipe_case, int pfd[])
{
    int cpid;
	if (user_exit(cmd)) {
		strcpy(cmd, "exit"); 
		return;
	}
	
	bool cd_flag;
	cd_flag = handle_cd(cmd);
	if (cd_flag) {
		return;
	}

	Ivec* args = make_ivec();
	parse_arguments(cmd, args);
	char first_cmd[256];
	char rest_of_cmd[256]; 
	bool and_flag = check_for_and(args);
	bool or_flag = check_for_or(args);
	bool semi_colon_flag = check_for_semi_colon(args);
	bool pipe_flag = check_for_pipe(args);
	if (and_flag || or_flag || semi_colon_flag || pipe_flag) {
		parse_first_cmd(cmd, first_cmd);
		parse_rest_of_cmd(cmd, rest_of_cmd);
		strcpy(cmd, first_cmd);
	}

	if (pipe_flag) {
		pipe(pfd);
		execute(cmd, 1, pfd);
		execute(rest_of_cmd, 2, pfd);
		close(pfd[0]);
		close(pfd[1]);
		return;
	}

	bool background_flag = check_for_background(args);
	int status;
    if ((cpid = fork())) {
		// parent process
		if (pipe_case == 1) {
			while ((cpid = wait(&status)) != -1);
			return;
		}
		else if (pipe_case == 2) {
			return;
		}
        // Child may still be running until we wait.
		if (background_flag) {
			waitpid(cpid, &status, WNOHANG);
			return;
		}
		else {
			waitpid(cpid, &status, 0);

			if (WIFEXITED(status)) {
				if (and_flag && WEXITSTATUS(status) == 0) {
					execute(rest_of_cmd, pipe_case, pfd);
				}
				else if (or_flag && WEXITSTATUS(status) != 0) {
					execute(rest_of_cmd, pipe_case, pfd);
				}
				else if (semi_colon_flag) {
					execute(rest_of_cmd, pipe_case, pfd);
				}
			}
		}
    }
    else {
        // child process
		// Redirect file stream based on pipe
		if (pipe_case == 1) {
			dup2(pfd[1], 1);
			close(pfd[0]);
		}
		else if (pipe_case == 2) {
			dup2(pfd[0], 0);
			close(pfd[1]);
		}

		// Parsing of the command
		Ivec* args = make_ivec();
		parse_arguments(cmd, args); 
		
		char* inputs[args->size + 1];
		char* input; 
		inputs[args->size] = 0; 
		int inputs_index = 0; 
		bool operator_present_flag = false;
		for (int ii = 0; ii < args->size; ii++) {
			if (operator_present_flag) {
				ii = ii - 1; 
				operator_present_flag = false; 
			}
			input = get_ivec(args, ii);
			// Check for operators
			if (strcmp(input, "<") == 0) {
				ii = ii + 2;
				input_redirect(args, ii - 1); 
				operator_present_flag = true;	 
			}
			if (strcmp(input, ">") == 0) {
				ii = ii + 2; 
				output_redirect(args, ii - 1);
				operator_present_flag = true;
			}
			if (strcmp(input, "&") == 0) {
				inputs[inputs_index] = 0;
				break;
			}
			if (ii >= args->size) {
				input = 0;
				inputs[inputs_index] = input;
				inputs_index++;
				break;
			}
			else if (!operator_present_flag) {
				inputs[inputs_index] = input;
				inputs_index++;
			}
		}
        execvp(args->data[0], inputs);
		free_ivec(args);  		
	}
}


int
main(int argc, char* argv[])
{
	char* cmd;
	Ivec* args; 
	int pfd[2] = {-1, -1};
	bool cd_flag, pipe_flag, background_flag; 
	bool exit_flag = false;
	if (argc == 1) {
		while(!feof(stdin)) {
			if (exit_flag) {
				return 0;
			}
			if (argc == 1) {
				if (!pipe_flag && !background_flag) {
					printf("nush$ ");
				}
				fflush(stdout);
			}
			cmd = malloc(sizeof(char) * 256);
			fgets(cmd, 256, stdin);
			exit_flag = user_exit(cmd);
			if (exit_flag || feof(stdin)) {
				return 0;
			}
			args = make_ivec();
			parse_arguments(cmd, args);
			pipe_flag = check_for_pipe(args);
			background_flag = check_for_background(args);
			free(args);
			if (strlen(cmd) > 1) {
				execute(cmd, -1, pfd);
			}
			free(cmd); 	
			int status;
			if (pipe_flag) {
				waitpid(-1, &status, 0);
				printf("nush$ ");
			}
			else if (background_flag) {
				waitpid(-1, &status, WNOHANG);
				printf("nush$ ");
			}
		}
	}
	else if (argc == 2) {
		
		char* filename;
		FILE* fp;
		if (argc == 2) {
			filename = argv[1];
			fp = fopen(filename, "r");
		}
		while(!feof(fp)) {
			if (exit_flag) {
				return 0;
			}
			if (argc == 1) {
				fflush(stdout);
			}
			cmd = malloc(sizeof(char) * 256);
			fgets(cmd, 256, fp);
			exit_flag = user_exit(cmd);
			if (feof(fp)) {
				return 0;
			}
			args = make_ivec();
			parse_arguments(cmd, args);
			pipe_flag = check_for_pipe(args);
			background_flag = check_for_background(args);
			free(args);
			if (strlen(cmd) > 1) {
				execute(cmd, -1, pfd);
			}
			free(cmd); 	
			int status;
			if (pipe_flag) {
				waitpid(-1, &status, 0);
			}
			else if (background_flag) {
				waitpid(-1, &status, WNOHANG);
			}
		}
	}

	return 0; 
}
