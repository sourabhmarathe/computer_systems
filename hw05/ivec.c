//
// Created by Sourabh Marathe on 2/2/17.
//
#include "ivec.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

Ivec* make_ivec() {
  Ivec* ivec = malloc(sizeof(Ivec));
  ivec->size = 0;
  ivec->capacity = 1;
  ivec->max_length = 80;
  ivec->data = malloc(ivec->capacity * sizeof(char*));
  for(int ii = 0; ii < ivec->capacity; ii++) {
    ivec->data[ii] = malloc(ivec->max_length * sizeof(char));
  }
  return ivec;
}

void free_ivec(Ivec* ivec) {
  for(int ii = 0; ii < ivec->capacity; ii++) {
    free(ivec->data[ii]);
  }
  free(ivec->data);
  free(ivec);
}

void pushBack(Ivec* ivec, char* item) {
  // If within the capacity of the ivec, just assign it
  if(ivec->size >= ivec->capacity) {
    int init_capacity = ivec->capacity;
    ivec->capacity = ivec->capacity + 1;
    ivec->data = realloc(ivec->data, ivec->capacity * sizeof(char *));
    for (int ii = 0; ii < init_capacity; ii++) {
      ivec->data[ii] = realloc(ivec->data[ii], ivec->max_length * sizeof(char));
    }
    for(int ii = init_capacity; ii < ivec->capacity; ii++) {
      ivec->data[ii] = malloc(ivec->max_length * sizeof(char));
    }
  }
  put_ivec(ivec, ivec->size, item);
  ivec->size++;
}

char* get_ivec(Ivec* ivec, int index) {
  assert(index >= 0);
  assert(index <= ivec->size);
  return ivec->data[index];
}

void put_ivec(Ivec* ivec, int index, char* item) {
  assert(index >= 0);
  strcpy(ivec->data[index], item);
}

void sort_ivec(Ivec* ivec) {
  int nn = ivec->size;
  int max_length = 80;
  for(int ii = 0; ii < nn; ii++) {
    for(int jj = 0; jj < nn - ii - 1; jj++) {
      if(strcmp(ivec->data[jj], ivec->data[jj+1]) > 0) {
        char* swap = malloc(max_length*sizeof(char));
        strcpy(swap, ivec->data[jj]);
        strcpy(ivec->data[jj], ivec->data[jj+1]);
        strcpy(ivec->data[jj+1], swap);
        free(swap);
      }
    }
  }
}
