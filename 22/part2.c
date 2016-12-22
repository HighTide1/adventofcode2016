#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 16384
#define ROWS 30
#define COLS 34

// Node Structure. A Node is defined by its position in the
// grid-center, and its current use/avail statistics/
typedef struct node_t{
    int r;
    int c;
    int used_space;
    int free_space;
} NODE;

// Function Prototypes
void create_gridcenter(char* input_name);
int  find_pairs(void);
void print_gridcenter(void);

// Usage Format String
char* df = "/dev/grid/node-x%d-y%d %dT %dT %dT %d%%";

// Global Node Array.
NODE gridcenter[ROWS][COLS];

int main(int argc, char** argv){
    char* input_name;

    // Read in the input file specified by argv
    if(argc!=2){
        fprintf(stderr, "No specified input file.\n");
        return -1;
    }else{
        input_name = argv[1];
    }
    create_gridcenter(input_name);
    print_gridcenter();
 
    printf("To solve, count the number of moves required to move _ to the goal, and then to the readable data.\n");
    return 0;
}

// Initializes the gridcenter from the given input file name.
void create_gridcenter(char* input_name){
    FILE* input_txt = fopen(input_name, "r");
    char  buffer[BUFFER_SIZE];    
    int   r, c, size, used, avail, use;

    if(!input_txt){
        fprintf(stderr, "Unable to open %s.\n", input_name);
    }
    
    // Skip the first 2 lines of the file. (Data starts at 3rd)
    fgets(buffer, BUFFER_SIZE, input_txt);
    fgets(buffer, BUFFER_SIZE, input_txt);
    fgets(buffer, BUFFER_SIZE, input_txt);
    while(feof(input_txt)==0){
        if(sscanf(buffer, df, &c, &r, &size, &used, &avail, &use)==6){
            gridcenter[r][c].r = r;
            gridcenter[r][c].c = c;
            gridcenter[r][c].used_space = used;
            gridcenter[r][c].free_space = avail;
        }else{
            printf("Error processing file.\n");
        }
        fgets(buffer, BUFFER_SIZE, input_txt);
    }
    fclose(input_txt);
}

// Finds the number of viable pairs in the gridcenter.
int find_pairs(void){
    NODE n1, n2;
    int  r1, c1, r2, c2, num_pairs=0;

    for(r1=0;r1<ROWS;r1++){
        for(c1=0;c1<COLS;c1++){
            n1 = gridcenter[r1][c1];
            for(r2=0;r2<ROWS;r2++){
                for(c2=0;c2<COLS;c2++){
                    n2 = gridcenter[r2][c2];
                                       
                    //printf("Comparing (%d, %d) and (%d, %d)\n", r1, c1, r2, c2);
                    // A Pair is considered viable such that the 2 nodes are not the same,
                    // its used space is nonzero, and A's data would fit in B.
                    if(n1.r!=n2.r || n1.c!=n2.c){
                        if(n1.used_space!=0 && n1.used_space<=n2.free_space){
                            num_pairs++;
                            //printf("Viable Pair.\n");
                        }
                    }/*else{
                        printf("Same Node Detected.\n");
                    }*/
                }
            }
        }
    }
    return num_pairs;
}

// Prints a summary of the gridcenter to the screen.
void print_gridcenter(void){
    NODE c_node;
    int  r, c;

    for(r=0;r<ROWS;r++){
        for(c=0;c<COLS;c++){
            c_node = gridcenter[r][c];
            if(c_node.used_space==0){
                printf("  _   ");
            }else if(c_node.used_space>100){
                printf("  |   ");
            }else{
                printf("%d/%d ", c_node.used_space, c_node.used_space+c_node.free_space);
            }
        }
        printf("\n");
    }
}
