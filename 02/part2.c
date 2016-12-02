#include <stdio.h>

int main(int argc, char** argv){
    FILE* input_txt;
    char  instr;
    int   keys[25] = {0, 0, 1, 0, 0,
                      0, 2, 3, 4, 0,
                      5, 6, 7, 8, 9,
                      0, 0xA, 0xB, 0xC, 0,
                      0, 0, 0xD, 0, 0};
    int   code=0, k=10;

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
            printf("Index: %d\n", k);
            code = (code<<4)|keys[k];
        }else{
            switch(instr){
                case 'U':
                    k -= 5;
                    if(k<0 || keys[k]==0){k+=5;}
                    break;
                case 'D':
                    k+=5;
                    if(k>25 || keys[k]==0){k-=5;}
                    break;
                case 'L':
                    k--;
                    if(keys[k]==0){k++;}
                    break;
                case 'R':
                    k++;
                    if(keys[k]==0){k--;}
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
    printf("The Bathroom code is %x. Enjoy\n", code);
    return 0;
}

int positive_modulo(int i, int n){
    return ((i%n)+n)%n;
}
