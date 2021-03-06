#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FLOORS 4
#define OBJECTS 10
#define ELEMENTS 5

// State Structure. Represents a node our Breadth-First Tree,
// composed of an item-floor mapping, sum distance from goal,
// and a pointer to the next in the list.
typedef struct state_t{
    char            map[FLOORS][OBJECTS][3];
    int             distance;
    int             floor;
    int             num_steps;
    struct state_t* next;
} STATE;
STATE* head = NULL;
int size=0;

// Function Prototypes
int                 calc_distance(char map[FLOORS][OBJECTS][3]);
STATE*              create_state(char map[FLOORS][OBJECTS][3], int f, int s);
void                free_list(void);
int                 has_equivalent(STATE* s);
int                 is_valid(STATE* s);
char[ELEMENTS][2]   make_pair_mapping(char map[FLOORS][OBJECTS][3]);
int                 move_level(int s);
void                print_map(char map[FLOORS][OBJECTS][3]);

int main(int argc, char** argv){
    char map[FLOORS][OBJECTS][3] = {
        {"LG", "  ", "TG", "TM", "PG", "  ", "RG", "RM", "CG", "CM"}, 
        {"  ", "LM", "  ", "  ", "  ", "PM", "  ", "  ", "  ", "  "}, 
        {"  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  "}, 
        {"  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  "}};

    // Initialize the root of the tree to be our starting state,
    // and begin looping until a minimum step count is found.
    head = create_state(map, 0, 0);
    //printf("Starting Distance: %d\n", head->distance);
    while(move_level(head->num_steps)==0){}

    printf("The minimum number of steps is %d\n.", (head->num_steps+1));
    free_list();
    return 0;
}

// Calculate the current total distance of the items from the 4th floor.
int calc_distance(char map[FLOORS][OBJECTS][3]){
    int i, j, dist = 0;

    for(i=0;i<FLOORS;i++){
        for(j=0;j<OBJECTS;j++){
            // For non-empty indexes, just add the current floor distance.
            if(strcmp(map[i][j], "  ")!=0){
                dist += (3 - i);
            }
        }
    }
    return dist;
}

// Create and allocate a new STATE on the heap.
STATE* create_state(char map[FLOORS][OBJECTS][3], int f, int s){
    STATE* new_state = (STATE*)malloc(sizeof(STATE));

    memcpy(new_state->map, map, FLOORS*OBJECTS*3);
    new_state->distance = calc_distance(new_state->map);
    new_state->floor = f;
    new_state->num_steps = s;
    new_state->next = NULL;
    return new_state;
}

// Frees the list back into memory.
void free_list(void){
    STATE* temp = head;

    while(temp!=NULL){
        head = head->next;
        free(temp);
        temp = head;
    }
}

// Determines if the current state has any equivalent
int has_equivalent(STATE* s){
    char p1[ELEMENTS][2] = make_pair_mapping(s->map);
    char p2[ELEMENTS][2];
    int  i, equivalent;
    STATE* temp = head;

    while(temp!=NULL){
        p2 = make_pair_mapping(temp->map);
        equivalent = 1;
        for(i=0;i<ELEMENTS;i++){
            if(p1[i][0]!=p2[i][0] || p1[i][1]!=p2[i][1]){
                equivalent = 0;
                break;
            }
        }
        if(equivalent==1){
            return 1;
        }
        temp = temp->next;
    }
    return 0;
    /*char m1[FLOORS][OBJECTS][3];
    char m2[FLOORS][OBJECTS][3];
    char p1[ELEMENTS][2];
    char p2[ELEMENTS][2];
    int  i, j, equivalent=1;
    STATE* temp = head;

    // Perform initial item to item check. If a mismatch is found,
    // we may need to look and see if they are pair-equivalent.
    memcpy(m1, s->map, FLOORS*OBJECTS*3);
    while(temp!=NULL){
        memcpy(m2, temp->map, FLOORS*OBJECTS*3);
        for(i=0;i<FLOORS;i++){
            for(j=0;j<OBJECTS;j++){
                if(strcmp(m1[i][j], m2[i][j])!=0){
                    equivalent = 0;
                }
                if(strcmp(m1[i][j], "  ")!=0){   
                    p1[j/2][m1[i][j][1]!='G'] = i;
                }
                if(strcmp(m2[i][j], "  ")!=0){
                    p2[j/2][m2[i][j][1]!='G'] = i;
                }
            }
        }

        /*printf("[");
        for(i=0;i<ELEMENTS;i++){
            printf("[");
            for(j=0;j<2;j++){
                printf("%d, ", p1[i][j]);
                //printf("%d, ", p2[i][j]);
            }
            printf("], ");
        }
        printf("\n");*/

        // The two states may also be pair equivalent, meaning that
        // they only differ by a pair moved. These can be ignored.
        /*if(equivalent==0){
            equivalent = 1;
            for(i=0;i<ELEMENTS;i++){
                if(p1[i][0]!=p2[i][0] || p1[i][1]!=p2[i][1]){
                    equivalent = 0;
                    break;
                }
            }
        }
        
        if(equivalent==1){
            //printf("--------------------\n");
            //print_map(m1);
            //print_map(m2);
            return 1;
        }
        temp = temp->next;
    }
    return 0;*/
}

// Determines if a given state is "valid", meaning that it would not
// lead to the undo destruction of a microchip.
int is_valid(STATE* s){
    char pairs[ELEMENTS][2] = make_pair_mapping(s->map);
    int i, j, cf;

    for(i=0;i<ELEMENTS;i++){
        cf = pairs[i][0];
        for(j=0;j<ELEMENTS;j++){
            if(j!=i && pairs[j][1]==cf && pairs[j][0]!=cf){
                return 0;
            }
        }
    }
    return 1;
    /*char map[FLOORS][OBJECTS][3];
    int i, j, gc, mc, p;

    memcpy(map, s->map, FLOORS*OBJECTS*3);
    for(i=0;i<FLOORS;i++){
        gc = 0;
        mc = 0;
        p = 0;
        for(j=1;j<OBJECTS;j+=2){
            if(map[i][j-1][1]=='G'){
                gc++;
            }
            if(map[i][j][1]=='M'){
                mc++;
                if(map[i][j-1][1]=='G'){
                    p++;
                }
            }
        }
        if(gc>0 && p!=mc){
            return 0;
        }
    }
    //print_map(map);
    return 1;*/
}

// Returns a 2D Array consisting of an element pair mapping
char[ELEMENTS][2] make_pair_mapping(char map[FLOORS][OBJECTS][3]){
    char pairs[ELEMENTS][2];
    int  i, j;
    
    for(i=0;i<FLOORS;i++){
        for(j=0;j<OBJECTS;j++){
            if(strcmp(map[i][j], "  ")!=0){
                pairs[j/2][map[i][j][1]!='G'] = i;
            }
        }
    }
    return pairs;
}

// Performs all valid and non-equivalent permutations of a given step
// count. Determines if the goal state is among these.
int move_level(int s){
    char m[FLOORS][OBJECTS][3];
    char nm1[FLOORS][OBJECTS][3];
    char nm2[FLOORS][OBJECTS][3];
    int  i, j, cf;
    STATE* temp = head;
    STATE* perm;
   
    printf("Curr Step: %d\n", s); 
    while(temp!=NULL && temp->num_steps==s){
        memcpy(m, temp->map, FLOORS*OBJECTS*3);
        cf = temp->floor;

        for(i=0;i<OBJECTS;i++){
            for(j=i;j<OBJECTS;j++){
                memcpy(nm1, temp->map, FLOORS*OBJECTS*3);
                memcpy(nm2, temp->map, FLOORS*OBJECTS*3);
                if(cf>0 && strcmp(m[cf][i], "  ")!=0 && strcmp(m[cf][j], "  ")!=0){
                    memcpy(nm1[cf-1][i], m[cf][i], 3);
                    memcpy(nm1[cf][i], "  ", 3);
                    memcpy(nm1[cf-1][j], m[cf][j], 3);
                    memcpy(nm1[cf][j], "  ", 3);
                    perm = create_state(nm1, cf-1, s+1);
                    if(is_valid(perm)==1 && has_equivalent(perm)==0){
                        perm->next = head;
                        head = perm;
                        size++;
                        if(perm->distance==0){
                            return 1;
                        }
                    }else{
                        free(perm);
                    }
                }
                if(cf<3 && strcmp(m[cf][i], "  ")!=0 && strcmp(m[cf][j], "  ")!=0){
                    memcpy(nm2[cf+1][i], m[cf][i], 3);
                    memcpy(nm2[cf][i], "  ", 3);
                    memcpy(nm2[cf+1][j], m[cf][j], 3);
                    memcpy(nm2[cf][j], "  ", 3);
                    
                    perm = create_state(nm2, cf+1, s+1);
                    if(is_valid(perm)==1 && has_equivalent(perm)==0){
                        perm->next = head;
                        head = perm;
                        size++;
                        if(perm->distance==0){
                            return 1;
                        }
                    }else{
                        free(perm);
                    }
                }
                //printf("Size: %d\n", size);
            }
        }
        temp = temp->next;
    }
    return 0;
}

void print_map(char map[FLOORS][OBJECTS][3]){
    int i, j;
    
    printf("------------\n");
    for(i=0;i<FLOORS;i++){
        for(j=0;j<OBJECTS;j++){
            printf("%s ", map[i][j]);
        }
        printf("\n");
    }
}
