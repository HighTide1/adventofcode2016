#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/md5.h>

#define BUFFER_SIZE 128

// Function Prototypes
void print_nums(char* s);

int main(int argc, char** argv){
    char*           door_id;
    unsigned char   hash[MD5_DIGEST_LENGTH];
    char            buffer[BUFFER_SIZE], door_pass[] = "--------";
    int             num_found=0, i=0, j=0, l5=0;
    MD5_CTX         context;

    // Seed random number generator
    srand(time(NULL));

    // Read in the door_id specified by argv
    if(argc==1){
        fprintf(stderr, "No specified door id.\n");
        return -1;
    }else{
        door_id = argv[1]; // 1st Index is 1st arg
    }

    // Begin processing direction instructions until EOF
    while(num_found<8){
        
        // Create a new door_id and index string to hash
        snprintf(buffer, BUFFER_SIZE, "%s%d", door_id, i++);

        // Generate an MD5 hash of the new string combo.
        MD5_Init(&context);  
        MD5_Update(&context, buffer, strlen(buffer));
        MD5_Final(hash, &context);

        // We have a match if the leading 5 numbers in the hash
        // are zero, corresponding to hash[0], hash[1],  and
        // part of hash[2]
        l5 = (hash[0]<<12) | (hash[1]<<4) | (hash[2]&0xF0);
        if(l5==0){
            door_pass[num_found++] = (hash[2] & 0x0F);
        }

        // Little Hacking Animation to keep update on progress
        for(j=num_found;j<8;j++){
            door_pass[j] = (0 + (rand() % 16));
        }
        if(((i-1)%1000)==0){
            printf("\r");
            print_nums(door_pass);
        }
    }
    
    printf("\n");
    printf("The password for the door is ");
    print_nums(door_pass);
    printf(".\n");
    return 0;
}

void print_nums(char* s){
    int j;

    for(j=0;j<strlen(s);j++){
        printf("%x", s[j]);
    }
}
