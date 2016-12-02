#include <stdio.h>

int main(int argc, char** argv){
    FILE* input_txt;
    char  instr;
    int   keys[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int   code=0, k=4;

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
        // Once we've hit a newline, we can add to our key code
        if(instr=='\n'){
            code = (code*10)+keys[k];
        }else{
            switch(instr){
                case 'U':
                    if(k>2){k-=3;}
                    break;
                case 'D':
                    if(k<6){k+=3;}
                    break;
                case 'L':
                    if((k%3)>0){k--;}
                    break;
                case 'R':
                    if((k%3)<2){k++;}
                    break;
                default:
                    fprintf(stderr, "Unknown Instruction.\n");
                    return -1;
            }
        }
    }
    // Get that last code from the EOF
    //code = (code*10)+keys[k];

    fclose(input_txt);  // Close the file for cleanup;
    printf("The Bathroom code is %d. Enjoy\n", code);
    return 0;
}

int positive_modulo(int i, int n){
    return ((i%n)+n)%n;
}
