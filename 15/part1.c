#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 16384

// Disc Structure. A Disc is characterized by its period (num positions) and 
// initial starting position. Also holds a pointer to the next disc.
typedef struct disc_t{
    int period;
    int init_pos;
    struct disc_t* next;
} DISC;
DISC* head = NULL;

// Function Prototypes
DISC* create_disc(int p, int i_p);
void  create_sculpture(FILE* input);
void  free_list(void);
int   valid_time(int t);

int main(int argc, char** argv){
    FILE* input_txt;
    int   time=0, retrieved=0;

    // Read in the input file specified by argv
    if(argc==1){
        fprintf(stderr, "No specified input file.\n");
        return -1;
    }else{
        input_txt = fopen(argv[1], "r"); // 1st Index is 1st arg
        if(!input_txt){
            fprintf(stderr, "Unable to open %s.\n", argv[1]);
            return -1;
        }
    }

    create_sculpture(input_txt);
    while(retrieved==0){
        if(valid_time(time)==1){
            retrieved = 1;
        }else{
            time += 1;
        }
    }

    fclose(input_txt);  // Close the file for cleanup;
    free_list();
    printf("At time=%d, the capsule can drop through the disks.\n", time);
    return 0;
}

// Creates and allocates a disk from the given parameters.
DISC* create_disc(int p, int i_p){
    DISC* new_disc = (DISC*)malloc(sizeof(DISC));
    
    new_disc->period = p;
    new_disc->init_pos = i_p;
    new_disc->next = NULL;
    return new_disc;
}

// Create a linked-list based off the input text file. Requires
// that the input file lists the disks in-order.
void create_sculpture(FILE* input){
    char  buffer[BUFFER_SIZE];
    DISC* end;
    DISC* temp;
    int   d, t, p;

    // Process the input file until EOF
    fgets(buffer, BUFFER_SIZE, input);
    while(feof(input)==0){
        sscanf(buffer, "Disc #%d has %d positions; at time=0, it is at position %d.", &d, &t, &p);
        temp = create_disc(t, p);
        if(head==NULL){
            head = temp;
            end = temp;
        }else{
            end->next = temp;
            end = temp;
        }
        fgets(buffer, BUFFER_SIZE, input);
    }
}

// Frees the Linked-List back to memory.
void free_list(void){
    DISC* temp = head;
    
    while(temp!=NULL){
        head = head->next;
        free(temp);
        temp = head;
    }
}

// Determines if the current time interval is valid (i.e. can drop the capsule through)
int valid_time(int t){
    DISC* curr_disc = head;
    while(curr_disc!=NULL){
        t += 1;
        if(((curr_disc->init_pos+t)%curr_disc->period)==0){
            //printf("Passed through at t=%d\n", t);
            curr_disc = curr_disc->next;
        }else{
            return 0;
        }
    }
    return 1;
}
