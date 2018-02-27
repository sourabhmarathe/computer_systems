#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int _ac, char* _ag[]) {
    // Get number of words to enter
    int nn = atoi(_ag[1]); 
    
    // Read in words from user and store them in array
    int max_length = 80; 
    char str_array[nn][max_length];
    for(int ii = 0; ii < nn; ii++) {
        fgets(str_array[ii], max_length*sizeof(char*), stdin); 
    }

    // Sort str_array using strcmp
    for(int ii = 0; ii < nn; ii++) {
        for(int jj = 0; jj < nn - ii - 1; jj++) {
            if(strcmp(str_array[jj], str_array[jj+1]) > 0) {
                char* swap = malloc(max_length*sizeof(char));
                strcpy(swap, str_array[jj]);
                strcpy(str_array[jj], str_array[jj+1]);
                strcpy(str_array[jj+1], swap);
				free(swap);
            }
        }
    }

    // Print out the sorted array
    for(int ii = 0; ii < nn; ii++) {
        printf("%s", str_array[ii]);
    }

    return 0; 
}
