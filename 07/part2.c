#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Pair Structure to hold all possible ABAs
typedef struct pair_t{
    char a;
    char b;
    int  h;
    struct pair_t* next;
} PAIR;

PAIR* head=NULL;

// Function Prototypes
int check_ssl(char* s, int h);

int main(int argc, char** argv){
    FILE* input_txt;
    char  curr_line[256];
    char* token;
    PAIR* temp;
    int   hypernet=0, pair_found, num_ips=0;
    
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
            // We consider each section seperatly. If a BAB is
            // detected in a hypernet, though, we have a match.
            pair_found = check_ssl(token, hypernet);
            token = strtok(NULL, "[]");
            if(pair_found==1){
                token = NULL;
            }
            hypernet = !hypernet;
        }

        if(pair_found==1){
            num_ips++;
            pair_found = 0;
        }
        
        // Free the list after every line.
        temp = head;
        while(temp!=NULL){
            head = head->next;
            free(temp);
            temp = head;
        }
    }
    
    fclose(input_txt);  // Close the file for cleanup;
    printf("The number of supported SSL IPs is %d.\n", num_ips);
    return 0;
}

int check_ssl(char* s, int h){
    char  c1, c2;
    int   i;
    PAIR* temp;
    
    for(i=0;i<strlen(s)-2;i++){
        c1 = s[i];
        c2 = s[i+1];
        // If the 1st and 3rd char match, we may either have
        // an ABA or BAB depending on the type. We need to check
        // our list for a match, and if not, push it to the list.
        if(c1==s[i+2]){
            temp = head;
            while(temp!=NULL){
                if(temp->a==c2 && temp->b==c1 && temp->h!=h){
                    //printf("%s\n", s);
                    return 1;
                }
                temp = temp->next;
            }
            // If no match, just push the pair to the list
            // provided that the middile character is different.
            if(c1!=c2){
                temp = (PAIR*)malloc(sizeof(PAIR));
                temp->a = c1;
                temp->b = c2; 
                temp->h = h;
                temp->next = head;
                head = temp;
            }
        }
    }
    return 0;
}
