#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 16384

// Function Prototypes
int decompress(char* s);

int main(int argc, char** argv){
    FILE* input_txt;
    char  buffer[BUFFER_SIZE];
    int   length=0;
 
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

    // Retrieve the compressed file line from the input and
    // begin processing it.
    fgets(buffer, BUFFER_SIZE, input_txt);
    buffer[strlen(buffer)-1] = '\0';
    length = decompress(buffer);
    
    fclose(input_txt);  // Close the file for cleanup;
    printf("The decompressed file length is %d.\n", length);
    return 0;
}

// Determines the "decompressed" string length.
int decompress(char* s){
    int i=0, num=0, rep=0; 
    int s_l=0, l=0;

    // Process the string until the end is reached.
    s_l = strlen(s);
    while(i<s_l){
        // While processing the string, there are 2 main cases.
        // For marker strings, we need to get the size and rep
        // to consider, and increment i to match. Otherwise, we
        // just increase i.
        if(s[i]=='('){
            // Retrieve the number and repetition count.
            i++;
            while(s[i]!='x'){
                num = (num * 10) + (s[i++] - '0');
            }
            i++;
            while(s[i]!=')'){
                rep = (rep * 10) + (s[i++] - '0');
            }
            i++;
            i += num;
            l += (num * rep);
            num = 0;
            rep = 0;
        }else{
            i++;
            l++;
        }
    }
    return l;
}
