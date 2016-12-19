#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 16384

// Function Prototypes
int steal_presents(int num_elves);

int main(int argc, char** argv){
    int num_elves;
    
    // Read in the input file specified by argv
    if(argc!=2){
        fprintf(stderr, "No specified elf number.\n");
        return -1;
    }else{
        num_elves = atoi(argv[1]);
    }
    
    printf("The Elf that gets all the presents is #%d.\n", steal_presents(num_elves));
    return 0;
}

// Runs an elf 'White Elephant' party, until only a single ELF has the presents left.
int steal_presents(int num_elves){
    int* elves = (int*)malloc(num_elves*sizeof(int));
    int   i, j, l, num_left = num_elves;

    // Initialize all elves to have 1 present.
    for(i=0;i<num_elves;i++){
        elves[i] = 1;
    }

    // Until only a single elf is left, what need to run through an elf circle
    // swapping presents with the leftmost neighbor.
    while(num_left!=1){
        for(i=0;i<num_elves;i++){
            if(elves[i]!=0){
                l = (i + 1) % num_elves;
                while(elves[l]==0){
                    l = (l + 1) % num_elves;
                }
                elves[i] += elves[l];
                elves[l] = 0;
                num_left--;
            }
            if(num_left==1){
                break;
            }
        }
        /*for(j=0;j<num_elves;j++){
            printf("%d ", elves[j]);
        }*/
        //printf("Num Left: %d\n", num_left);
    }
    
    free(elves);
    return i+1;
}
