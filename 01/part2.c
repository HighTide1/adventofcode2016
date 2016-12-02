#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

// Point Structure. Represents a node in a linked list containing
// a x and y coordinate.
typedef struct point_t{
    int    x;
    int    y;
    struct point_t* next;
} Point;

// Function Prototypes
Point* create_point(int x, int y);
void   free_list(Point* h);
int    has_visited(Point* h, Point* p);
int    positive_modulo(int i, int n);

int main(int argc, char** argv){
    FILE*          input_txt;
    Point          *head, *tmp;
    char           instr;
    int            visit2=0, dir=0, x=0, y=0, dist=0;

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

    // Add in the origin as the first point.
    head = create_point(x, y);

    // Begin processing direction instructions until EOF
    while((instr=fgetc(input_txt))!=EOF){
        // Skip past commas and spaces
        if(instr==',' || instr==' '){continue;}
        
        // For directional instructions, we have 4 cardinal dirs.
        // to consider.
        if(instr=='L' || instr=='R'){
            if(instr=='L'){
                dir = positive_modulo(dir-1, 4);
            }else{
                dir = positive_modulo(dir+1, 4);
            }
        }else{
            // Otherwise, it's a movement instruction. So we need
            // to read in the entire movement bearing, and then
            // determine how we move.
            while(isdigit(instr)){
                dist = (dist*10)+(instr-'0'); // Compute bearing
                instr = fgetc(input_txt);
            }
            while(visit2==0 && dist>0){
                switch(dir){
                    case 0: // North
                        tmp = create_point(x, ++y);
                        break;
                    case 1: // East
                        tmp = create_point(++x, y);
                        break;
                    case 2: // South
                        tmp = create_point(x, --y);
                        break;
                    case 3: // West
                        tmp = create_point(--x, y);
                        break;
                    default:
                        fprintf(stderr, "Unknown Direction.\n");
                        return -1;   
                }
                dist--;
                visit2 = has_visited(head, tmp);
                //printf("Curr Location: %d, %d\n", x, y);
                // Append point to list
                tmp->next = head;
                head = tmp;
            }
            if(visit2==1){
                dist = abs(tmp->x) + abs(tmp->y);
                break;
            }
        }
    }
    
    // Free all the allocated points in memory
    while(head!=NULL){
        tmp = head;
        head = head->next;
        free(tmp);
    }

    fclose(input_txt);  // Close the file for cleanup;
    printf("The EBH is %d blocks away.\n", dist);
    return 0;
}

Point* create_point(int x, int y){
    Point* new_point = (Point*)malloc(sizeof(Point));
    new_point->x = x;
    new_point->y = y;
    new_point->next = NULL;
    return new_point;
}

void free_list(Point* h){
    Point* tmp;

    while(h!=NULL){
        tmp = h;
        h = h->next;
        free(tmp);
    }
}

int has_visited(Point* h, Point* p){
    Point* tmp;

    tmp = h;
    while(tmp!=NULL){
        if(p->x==tmp->x && p->y==tmp->y){
            return 1;
        }
        tmp = tmp->next;
    }
    return 0;
}

int positive_modulo(int i, int n){
    return ((i%n)+n)%n;
}
