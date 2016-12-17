#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>

#define BUFFER_SIZE 1024
#define NUM_KEYS 64
#define WINDOW 1000

// Function Prototypes
char has_triple(unsigned char* h);
int  has_quint(unsigned char* h, char m);
void print_hash(unsigned char* h);

int main(int argc, char** argv){
    MD5_CTX       context;
    char*         salt;
    char          buffer[BUFFER_SIZE];
    unsigned char hash[MD5_DIGEST_LENGTH];
    char          m;
    int           i=0, j, keys_found=0;

    // Read in the input salt to generate hashes off of.
    if(argc!=2){
        fprintf(stderr, "No specified hash salt.\n");
        return -1;
    }else{
        salt = argv[1];
    }

    // Generate and check hashes until 64 keys have been found.
    while(keys_found<NUM_KEYS){
        // Create the current salt + index hash. 
        snprintf(buffer, BUFFER_SIZE, "%s%d", salt, i);
        MD5_Init(&context);
        MD5_Update(&context, buffer, strlen(buffer));
        MD5_Final(hash, &context);
        
        // If our current hash has a triple, then we check the next additional
        // 1000 indexes for a quintuple match.
        if((m=has_triple(hash))!=-1){
            for(j=i+1;j<i+1+WINDOW;j++){
                snprintf(buffer, BUFFER_SIZE, "%s%d", salt, j);
                MD5_Init(&context);
                MD5_Update(&context, buffer, strlen(buffer));
                MD5_Final(hash, &context);
                if(has_quint(hash, m)==1){
                    keys_found += 1;
                    break;
                }
            }
        }
        i += 1;
    }
    
    printf("The 64th key integer is %d.\n", i - 1);
    return 0;
}

// Determines if the passed hash has a triple character, and returns the first occuring
// triple if it does
char has_triple(unsigned char* h){
    int  i;
    char m, c1, c2, c3;

    for(i=0;i<MD5_DIGEST_LENGTH-1;i++){
        m = (h[i] & 0xF0) >> 4;
        c1 = h[i] & 0x0F;
        c2 = (h[i+1] & 0xF0) >> 4;
        c3 = h[i+1] & 0x0F;
        //printf("%x %x\n", h[i], h[i+1]);
        //printf("%d %d %d %d\n", m, c1, c2, c3);
        //printf("%x - %x, %x\n", m, c1, c2);
        if(c1==m && c2==m){
            return m;
        }
        m = c1;
        //printf("%x - %x, %x\n", m, c2, c3);
        if(c2==m && c3==m){
            return m;
        }
    }
    return -1;
}

// Determines if the given hash contains a quintuple of the given character
int has_quint(unsigned char* h, char m){
    int  i;
    char c1, c2, c3, c4, c5, c6;

    for(i=0;i<MD5_DIGEST_LENGTH-2;i++){
        c1 = (h[i] & 0xF0) >> 4;
        c2 = h[i] & 0x0F;
        c3 = (h[i+1] & 0xF0) >> 4;
        c4 = h[i+1] & 0x0F;
        c5 = (h[i+2] & 0xF0) >> 4;        
        c6 = h[i+2] & 0x0F;

        if(c2==m && c3==m && c4==m && c5==m && (c1==m || c6==m)){
            return 1;
        }
    }
    return 0;
}

void print_hash(unsigned char* h){
    int i;

    for(i=0;i<MD5_DIGEST_LENGTH;i++){
        printf("%.2x", h[i]);
    }
    printf("\n");
}
