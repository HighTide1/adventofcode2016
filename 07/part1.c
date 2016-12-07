#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function Prototypes
int check_abba(char* s);

int main(int argc, char** argv){
    FILE* input_txt;
    char  curr_line[256];
    char* token;
    int   hypernet=0, abba_found, has_abba, num_ips=0;
    
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

    // Begin processing until EOF
    while(fscanf(input_txt, "%s ", curr_line)!=EOF){
        //printf("%s\n", curr_line);
        hypernet = 0;
        token = strtok(curr_line, "[]");
        while(token!=NULL){
            //printf("%s, %d\n", token, check_abba(token));
            // We consider each section seperatly. If an ABBA
            // is detected in the [], we immediantly move on.
            abba_found = check_abba(token);
            token = strtok(NULL, "[]");
            if(abba_found==1){
                has_abba = 1;
                if(hypernet==1){
                    token = NULL;
                    has_abba = 0;
                }
            }
            hypernet = !hypernet;
        }

        if(has_abba==1){
            num_ips++;
            has_abba = 0;
        }
    }
    
    fclose(input_txt);  // Close the file for cleanup;
    printf("The number of supported TLS IPs is %d.\n", num_ips);
    return 0;
}

int check_abba(char* s){
    char c1, c2;
    int  i;
    
    for(i=0;i<strlen(s)-3;i++){
        c1 = s[i];
        c2 = s[i+1];
        // If the 1st and 4th char match, we may have an
        // ABBA. We just then need to make sure the 2nd
        // and 3rd match, and are not c.
        if(c1==s[i+3]){
            if(c2==s[i+2] && c1!=c2){
                return 1;
            }
        }
    }
    return 0;
}
