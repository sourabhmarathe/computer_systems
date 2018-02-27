#ifndef HW_05_UTIL_H
#define HW_05_UTIL_H

#include <stdbool.h>
#include "ivec.h"

void parse_arguments(char* cmd, Ivec* args); 
bool user_exit(char* exit_request);
bool handle_cd(char* cmd);

// Operator handlers
void input_redirect(Ivec* cmd, int index);
void output_redirect(Ivec* cmd, int index);

bool check_for_and(Ivec* args);
bool check_for_or(Ivec* args);
bool check_for_semi_colon(Ivec* args);
void parse_first_cmd(char* cmd, char first_cmd[256]);
void parse_rest_of_cmd(char* cmd, char rest_of_cmd[256]);

bool check_for_background(Ivec* args);
bool check_for_pipe(Ivec* args);
#endif //HW_05_UTIL_H
