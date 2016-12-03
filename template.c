#include <stdio.h>

int main(int argc, char** argv){
    FILE* input_txt;
    char  instr;
    
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

    // Begin processing direction instructions until EOF
    while((instr=fgetc(input_txt))!=EOF){
        
    }
    
    fclose(input_txt);  // Close the file for cleanup;
    //printf();
    return 0;
}
