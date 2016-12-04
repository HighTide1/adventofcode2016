#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function Prototypes
void make_chk(char* l, char* c);

int main(int argc, char** argv){
    FILE* input_txt;
    char  room_data[256], *token;
    char  letters[26] = {0}, room_chk[6] = "     ";
    int   i=0, sec_id=0, sum_sec=0;
    
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

    // Begin reading in room data until EOF
    while(fscanf(input_txt, "%s ", room_data)!=EOF){
        //printf("RD: %s\n", room_data);
        // Divide the string into dash seperated tokens,
        // and count the number of letters in each section.
        token = strtok(room_data, "-[");
        while(token!=NULL){
            if(isalpha(token[0])){
                //printf("Token: %s\n", token);
                // Loop through the tokenized string,
                // reading each character and incrementing
                // the current count.
                for(i=0;i<strlen(token);i++){
                    letters[token[i]-'a']++;
                }
            }else{
                // Once we've hit the section id, we can 
                // construct a possible checksum and compare
                // against the proposed match.
                make_chk(letters, room_chk);
                sec_id = atoi(token);
                token = strtok(NULL, "[]");
                //printf("%s, %s\n", room_chk, token);
                if(strcmp(room_chk, token)==0){
                    //printf("Sec ID: %d\n", sec_id);
                    sum_sec += sec_id;
                }
                for(i=0;i<26;i++){
                    letters[i]=0;
                }
            }
            token = strtok(NULL, "-[");
        }
    }
    
    fclose(input_txt);  // Close the file for cleanup;
    printf("The sum of sector IDs is %d.\n", sum_sec);
    return 0;
}

// Constructs a 5 letter checksum
void make_chk(char* l, char* c){
    int i, j, max_ind=0;
    
    for(i=0;i<5;i++){
        for(j=0;j<26;j++){
            if(l[j]>l[max_ind]){
                max_ind=j;
            }
        }
        l[max_ind]=0;
        c[i] = 'a'+max_ind;
    }
    c[5] = '\0';    // Null terminate the string
}
