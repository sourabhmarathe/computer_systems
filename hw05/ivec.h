//
// Created by Sourabh Marathe on 2/2/17.
//

#ifndef HW05_IVEC_H
#define HW05_IVEC_H

typedef struct {
  int size;
  int capacity;
  int max_length;
  char **data;
} Ivec;

Ivec* make_ivec();
void free_ivec(Ivec* ivec);

void pushBack(Ivec* ivec, char* item);
char* get_ivec(Ivec* ivec, int index);
void put_ivec(Ivec* ivec, int index, char* item);
void sort_ivec(Ivec* ivec);


#endif //HW05_IVEC_H
