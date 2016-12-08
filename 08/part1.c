#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 256

// Function Prototypes
void rotate_screen(int type, int c, int a, char s[][50]);

int main(int argc, char** argv){
    FILE* input_txt;
    char  buffer[BUFFER_SIZE];
    char  screen[6][50];   
    char* token;
    int   i, j, x, y, amount, num_lit=0;
 
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

    // Initialize Screen to be blank "."
    for(i=0;i<6;i++){
        for(j=0;j<50;j++){
            screen[i][j]='.';
        }
    }

    // Begin processing lines until EOF
    fgets(buffer, BUFFER_SIZE, input_txt);
    while(!feof(input_txt)){
        // Begin tokenizing the string. We have 3 options,
        // and need to perform specific actions based off each.
        token = strtok(buffer, " ");
        if(strcmp(token, "rect")==0){
            // The "rect" command will turn on all pixels
            // specified in the given rectangle.
            token = strtok(NULL, "x");
            x = atoi(token);
            token = strtok(NULL, "x");
            y = atoi(token);
    
            for(i=0;i<y;i++){
                for(j=0;j<x;j++){
                    if(screen[i][j]!='#'){
                        num_lit++;
                    }
                    screen[i][j] = '#';
                }
            }
        }else{
            // Otherwise, we have the "rotate" command,
            // which falls into row and column rotations.
            token = strtok(NULL, " ");
            if(strcmp(token, "row")==0){
                token = strtok(NULL, " ");  // Get the y= part
                y = atoi(token+2);
                token = strtok(NULL, " ");
                token = strtok(NULL, " ");
                amount = atoi(token);
                rotate_screen(0, y, amount, screen);
            }else{
                token = strtok(NULL, " ");
                x = atoi(token+2);
                token = strtok(NULL, " ");
                token = strtok(NULL, " ");
                amount = atoi(token);
                rotate_screen(1, x, amount, screen);
            }
        }
        fgets(buffer, BUFFER_SIZE, input_txt);
    }
    
    fclose(input_txt);  // Close the file for cleanup;
    for(i=0;i<6;i++){
        for(j=0;j<50;j++){
            printf("%c", screen[i][j]);
        }
        printf("\n");
    }
    printf("The number of pixels that are lit is %d.\n", num_lit);
    return 0;
}

void rotate_screen(int type, int c, int a, char s[][50]){
    int  i;
    char nr[50], nc[6];
    
    // If type == 0, we have a row rotation to handle.
    // Otherwise, its a column rotation.
    if(type==0){
        a = a % 50;
        for(i=0;i<50;i++){
            if(i+a>=50){
                nr[(i+a)%50] = s[c][i];
            }else{
                nr[i+a] = s[c][i];
            }
        }
        for(i=0;i<50;i++){
            s[c][i] = nr[i];
        }
    }else{
        a = a % 6;
        for(i=0;i<6;i++){
            if(i+a>=6){
                nc[(i+a)%6] = s[i][c];
            }else{
                nc[i+a] = s[i][c];
            }
        }
        for(i=0;i<6;i++){
            s[i][c] = nc[i];
        }
    }
}
