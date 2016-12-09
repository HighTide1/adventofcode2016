#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 256

// Function Prototypes
int fgetl(char* str, int n, FILE* stream);

int main(int argc, char** argv){
    FILE* input_txt;
    char  buffer[BUFFER_SIZE];
    
    // Read in the input file specified by argv
    if(argc==1){
        fprintf(stderr, "No specified input file.\n");
        return -1;
    }else{
        input_txt = fopen(argv[1], "r"); // 1st Index is 1st arg
        if(!input_txt){
            fprintf(stderr, "Unable to open %s.", argv[1]);
            return -1;
        }
    }

    // Begin processing lines until EOF
    while(fgetl(buffer, BUFFER_SIZE, input_txt))!=EOF){
        
    }
    
    fclose(input_txt);  // Close the file for cleanup;
    //printf();
    return 0;
}

// Wrapper function for fgets to return EOF when encountered.
int fgetl(char* str, int n, FILE* stream){
    fgets(str, n, stream);
    if(feof(stream)){
        return EOF;
    }
    return 0;
}
