#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

// Function Prototypes
int positive_modulo(int i, int n);

int main(int argc, char** argv){
    FILE* input_txt;
    char  instr;
    int   dir=0, x=0, y=0;
    int   dist=0;

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
        // Skip past commas and spaces
        if(instr==',' || instr==' '){continue;}
        
        // For directional instructions, we have 4 cardinal dirs.
        // to consider.
        if(instr=='L' || instr=='R'){
            if(instr=='L'){
                dir = positive_modulo(dir-1, 4);
            }else{
                dir = positive_modulo(dir+1, 4);
            }
            //printf("New Direction: %d\n", dir);
        }else{
            // Otherwise, it's a movement instruction. So we need
            // to read in the entire movement bearing, and then
            // determine how we move.
            while(isdigit(instr)){
                dist = (dist*10)+(instr-'0'); // Compute bearing
                //printf("New Distance: %d\n", dist);
                instr = fgetc(input_txt);
            }
            switch(dir){
                case 0:
                    y += dist;
                    break;
                case 1:
                    x += dist;
                    break;
                case 2:
                    y -= dist;
                    break;
                case 3:
                    x -= dist;
                    break;
                default:
                    fprintf(stderr, "Unknown Direction.\n");
                    return -1;
            }
            dist = 0;
        }
    }
    dist = abs(x) + abs(y);


    fclose(input_txt);  // Close the file for cleanup;
    printf("The EBH is %d blocks away.\n", dist);
    return 0;
}

int positive_modulo(int i, int n){
    return ((i%n)+n)%n;
}
