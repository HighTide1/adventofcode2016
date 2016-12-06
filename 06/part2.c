#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv){
    FILE* input_txt;
    char  letter_counts[8][26];
    char  err_msg[256]={0}, msg[9]={0};
    int   i, j, low_ind;
 
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

    // Initialize the letter counts to 0
    for(i=0;i<8;i++){
        for(j=0;j<26;j++){
            letter_counts[i][j] = 0;
        }
    }

    // Begin processing direction instructions until EOF
    while(fscanf(input_txt, "%s ", err_msg)!=EOF){
        // For each line in the input file, we need to
        // increment the letter counts at all 8 positions.
        for(i=0;i<8;i++){
            letter_counts[i][err_msg[i]-'a']++;
        }
    }

    // Once we've found the letter counts, we just need to
    // find the low at each position, and will have the 
    // correct password
    for(i=0;i<8;i++){
        low_ind=0;
        for(j=0;j<26;j++){
            if(letter_counts[i][j]<letter_counts[i][low_ind]){
                low_ind=j;
            }
        }
        msg[i] = 'a' + low_ind;
        //printf("%s\n", msg);
    }
    
    fclose(input_txt);  // Close the file for cleanup;
    printf("The corrected message is %s.\n", msg);
    return 0;
}
