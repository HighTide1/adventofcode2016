#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>

#define BUFFER_SIZE 16384

// Path Structure. Holds the traversed path, current location, and a pointer
// to the next path in a list.
typedef struct path_t{
    char           path[BUFFER_SIZE];
    int            length;
    int            r;
    int            c;
    struct path_t* next;
} PATH;
PATH* queue = NULL;
PATH* shortest = NULL;
PATH* longest = NULL;

// Function Prototypes
void  add_queue(PATH* q, PATH* p);
PATH* create_path(char* p, int r, int c);
PATH* generate_paths(char* pass);
void  print_hash(unsigned char* h);

int main(int argc, char** argv){
    char* passcode;
    
    // Read in the input file specified by argv
    if(argc!=2){
        fprintf(stderr, "No specified passcode.\n");
        return -1;
    }else{
        passcode = argv[1];
    }

    // Add our starting location to the queue, and keep going until the 
    // all paths possible have been traversed.
    queue = create_path("", 0, 0);
    while(queue!=NULL){
        queue = generate_paths(passcode);
    }

    printf("The shortest path to the vault is %s.\n", shortest->path);
    printf("It is %d steps long.\n", shortest->length);
    printf("The longest path to the vault is %s.\n", longest->path);
    printf("It is %d steps long.\n", longest->length);
    free(shortest);
    free(longest);
    return 0;
}

// Creates and allocates a new path from the given path and location.
PATH* create_path(char* p, int r, int c){
    PATH* new_path = (PATH*)malloc(sizeof(PATH));
    
    strcpy(new_path->path, p);
    new_path->length = strlen(p);
    new_path->r = r;
    new_path->c = c;
    new_path->next = NULL;
    return new_path;
}

// Generate a queue of new paths to take given the current queue.
PATH* generate_paths(char* pass){
    MD5_CTX       context;
    PATH*         new_queue = NULL;
    PATH*         curr_path = queue;
    PATH*         temp;
    char          buffer[BUFFER_SIZE];
    unsigned char hash[MD5_DIGEST_LENGTH];
    int           u, d, l, r;

    while(curr_path!=NULL){
        // If our current path is at the destination location, we don't need
        // to consider any additional paths, and instead just look for shortest
        // and longest and subsequently free them.
        if(curr_path->r==3 && curr_path->c==3){
            if(shortest==NULL){
                shortest = curr_path;
            }
            if(longest!=shortest && longest!=NULL){
                free(longest);
            }
            longest = curr_path;
        }else{
            // For determining which doors are open and closed,
            // we need to take the MD5 hash of the passcode and path taken.
            strcpy(buffer, pass);
            strcat(buffer, curr_path->path);
            //printf("%s\n", buffer);
            MD5_Init(&context);
            MD5_Update(&context, buffer, strlen(buffer));
            MD5_Final(hash, &context);
            //print_hash(hash);

            u = (hash[0] & 0xF0) >> 4;
            d = hash[0] & 0x0F;
            l = (hash[1] & 0xF0) >> 4;
            r = hash[1] & 0x0F;
            if(u>=0xb && curr_path->r>0){
                strcpy(buffer, curr_path->path);
                strcat(buffer, "U");
                temp = create_path(buffer, curr_path->r-1, curr_path->c);
                if(new_queue==NULL){
                    new_queue = temp;
                }else{
                    temp->next = new_queue;
                    new_queue = temp;
                }
            }
            if(d>=0xb && curr_path->r<3){
                strcpy(buffer, curr_path->path);
                strcat(buffer, "D");
                temp = create_path(buffer, curr_path->r+1, curr_path->c);
                if(new_queue==NULL){
                    new_queue = temp;
                }else{
                    temp->next = new_queue;
                    new_queue = temp;
                }
            }
            if(l>=0xb && curr_path->c>0){
                strcpy(buffer, curr_path->path);
                strcat(buffer, "L");
                temp = create_path(buffer, curr_path->r, curr_path->c-1);
                if(new_queue==NULL){
                    new_queue = temp;
                }else{
                    temp->next = new_queue;
                    new_queue = temp;
                }
            }
            if(r>=0xb && curr_path->c<3){
                strcpy(buffer, curr_path->path);
                strcat(buffer, "R");
                temp = create_path(buffer, curr_path->r, curr_path->c+1);
                if(new_queue==NULL){
                    new_queue = temp;
                }else{
                    temp->next = new_queue;
                    new_queue = temp;
                }
            }
        }
        temp = curr_path;
        curr_path = curr_path->next;
        if(temp!=longest && temp!=shortest){
            free(temp);
        }
    }
    return new_queue;
}

// Prints the given hash in hexadecimal format.
void print_hash(unsigned char* h){
    int i;

    for(i=0;i<MD5_DIGEST_LENGTH;i++){
        printf("%.2x", h[i]);
    }
    printf("\n");
}
