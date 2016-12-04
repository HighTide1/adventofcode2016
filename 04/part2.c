#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv){
    FILE* input_txt;
    char  *token;
    char  room_data[256], room_name[256];
    int   i=0, sec_id=0, length=0, tmp=0;
    
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
        // Divide the string into dash seperated tokens,
        // and copy each part into the room name
        token = strtok(room_data, "-[");
        room_name[0] = '\0';
        while(token!=NULL){
            if(isalpha(token[0])){
                // Loop through the tokenized string,
                // appending it to the room name
                length += (strlen(token) + 1);
                strcat(room_name, token);
                strcat(room_name, "-");
                token = strtok(NULL, "-[");
            }else{
                room_name[length] = '\0';
                // Once we've hit the section id, we can 
                // shift our letters and get a room name
                sec_id = atoi(token);
                for(i=0;i<length;i++){
                    if(room_name[i]!='-' && room_name[i]!='\0'){
                        tmp = room_name[i] - 'a';
                        room_name[i]='a' + ((tmp + sec_id) %26);
                    }else{
                        room_name[i] = ' ';
                    }
                }
                if(room_name[0]=='n'){
                    printf("%s%d\n", room_name, sec_id);
                }
                token = NULL;
                length = 0;
            }
        }
    }
    
    fclose(input_txt);  // Close the file for cleanup;
    return 0;
}
