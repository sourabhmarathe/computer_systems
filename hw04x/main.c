//
// Created by Sourabh Marathe on 2/2/17.
//
#include "ivec.h"
#include <stdio.h>
#include <stdlib.h>

int main(int _ac, char* _av[]) {
  Ivec* ivec = make_ivec();
  int max_length = 80;
  // Read in strings from the user until CTRL + D is hit
  char* line = malloc(max_length * sizeof(char));
  while(!feof(stdin)) {
    fgets(line, max_length, stdin);
    if(feof(stdin)) {
      break;
    }
    pushBack(ivec, line);
  }
  free(line);

  //Sort the ivec of strings
  sort_ivec(ivec);
  for(int ii = 0; ii < ivec->size; ii++) {
    printf("%s", get_ivec(ivec, ii));
  }

  // Free the ivec to prevent memory leak, return 0 for success.
  free_ivec(ivec);
  return 0;
}
