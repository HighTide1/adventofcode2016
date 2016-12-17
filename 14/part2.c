#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>

#define BUFFER_SIZE 1024
#define NUM_KEYS 64
#define WINDOW 1000
#define KEY_STRETCH 2016
#define TABLE_SIZE  1000000

// Function Prototypes
unsigned char* generate_hash(char* s, int i);
char           has_triple(unsigned char* h);
int            has_quint(unsigned char* h, char m);
void           print_hash(unsigned char* h);

// Cache of generated Hashes
unsigned char HASH_TABLE[TABLE_SIZE][MD5_DIGEST_LENGTH];
int gen_index = -1;

int main(int argc, char** argv){
    char*         salt;
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
        memcpy(hash, generate_hash(salt, i), MD5_DIGEST_LENGTH);
        
        // If our current hash has a triple, then we check the next additional
        // 1000 indexes for a quintuple match.
        if((m=has_triple(hash))!=-1){
            //printf("Triple Found w/ char %x - %d\n", m, i);
            for(j=i+1;j<i+1+WINDOW;j++){
                memcpy(hash, generate_hash(salt, j), MD5_DIGEST_LENGTH);
                if(has_quint(hash, m)==1){
                    //printf("Quint Found: %d\n", i);
                    //print_hash(hash);
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

// Generates a 'stretched' hash of the input salt and index, and stores the result
// in the hash table if not previously determined.
unsigned char* generate_hash(char* s, int i){
    MD5_CTX       context;
    int           j, k;
    char          buffer[BUFFER_SIZE];
    unsigned char hash[MD5_DIGEST_LENGTH];
    
    // If the hash has already been generated, just return it to the caller.
    if(i>=gen_index){
        // Otherwise, create the hash based off the initial salt and index,
        // and perform stretching on it.
        snprintf(buffer, BUFFER_SIZE, "%s%d", s, i);
        MD5_Init(&context);
        MD5_Update(&context, buffer, strlen(buffer));
        MD5_Final(hash, &context);
        //print_hash(hash);
        for(j=0;j<KEY_STRETCH;j++){
            // Convert the last hash into a string representation, to stretch
            for(k=0;k<MD5_DIGEST_LENGTH;k++){
                snprintf(&buffer[2*k], BUFFER_SIZE, "%.2x", hash[k]);
            }
            MD5_Init(&context);
            MD5_Update(&context, buffer, strlen(buffer));
            MD5_Final(hash, &context);
            //print_hash(hash);
        }
        
        // Copy the 'stretched' hash into the hash table for future reference.
        for(j=0;j<MD5_DIGEST_LENGTH;j++){
            HASH_TABLE[i][j] = hash[j];
        }
        gen_index++;
    }
    return HASH_TABLE[i];
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
        if(c1==m && c2==m){
            return m;
        }
        m = c1;
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
