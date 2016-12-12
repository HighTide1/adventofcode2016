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
    char   map[ELEMENTS][2];
    int    distance;
    int    floor;
    int    num_steps;
    struct state_t* next;
} STATE;

STATE* head = NULL;
int size = 0;

// Workaround for Pair Mapping
typedef struct pairs_t{
    char    pairs[ELEMENTS][2];
} PAIRS;

// Function Prototypes
int    calc_distance(char map[ELEMENTS][2]);
STATE* create_state(char map[ELEMENTS][2], int f, int s);
void   free_list(void);
int    has_equivalent(STATE* s);
int    is_valid(STATE* s);
PAIRS  make_pair_mapping(char map[FLOORS][OBJECTS][3]);
int    move_level(int s);
//void   print_map(char map[FLOORS][OBJECTS][3]);

int main(int argc, char** argv){
    char map[FLOORS][OBJECTS][3] = {
        {"LG", "  ", "TG", "TM", "PG", "  ", "RG", "RM", "CG", "CM"}, 
        {"  ", "LM", "  ", "  ", "  ", "PM", "  ", "  ", "  ", "  "}, 
        {"  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  "}, 
        {"  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  "}};

    // Initialize the root of the tree to be our starting state,
    // and begin looping until a minimum step count is found.
    head = create_state(make_pair_mapping(map).pairs, 0, 0);
    printf("Starting Distance: %d\n", head->distance);
    
    while(move_level(head->num_steps)==0){}

    printf("The minimum number of steps is %d\n.", (head->num_steps+1));
    free_list();
    return 0;
}

// Calculate the current total distance of the items from the 4th floor.
int calc_distance(char map[ELEMENTS][2]){
    int i, j, dist = 0;

    for(i=0;i<ELEMENTS;i++){
        for(j=0;j<2;j++){
            dist += (3 - map[i][j]);
        }
    }
    return dist;
}

// Create and allocate a new STATE on the heap.
STATE* create_state(char map[ELEMENTS][2], int f, int s){
    STATE* new_state = (STATE*)malloc(sizeof(STATE));

    memcpy(new_state->map, map, ELEMENTS*2);
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
        size--;
        free(temp);
        temp = head;
    }
}

// Determines if the current state has any equivalent
int has_equivalent(STATE* s){
    char   p1[ELEMENTS][2];
    char   p2[ELEMENTS][2];
    int    i, j, equivalent;
    STATE* temp = head;

    memcpy(p1, s->map, ELEMENTS*2);
    while(temp!=NULL){
        memcpy(p2, temp->map, ELEMENTS*2);
        /*if(s->num_steps==13){
            printf("------------\n");
            for(i=0;i<ELEMENTS;i++){
                printf("%d, %d | %d, %d\n", p1[i][0], p1[i][1], p2[i][0], p2[i][1]);
            }
        }*/

        for(i=0;i<ELEMENTS;i++){
            equivalent = 0;
            for(j=0;j<ELEMENTS;j++){
                if(p1[i][0]==p2[j][0] && p1[i][1]==p2[j][1]){
                    equivalent = 1;
                    p2[j][0] = -1;
                    p2[j][1] = -1;
                    /*if(s->num_steps==13){
                        printf("-!-\n");
                        for(j=0;j<ELEMENTS;j++){
                            printf("%d, %d | %d, %d\n", p1[j][0], p1[j][1], p2[j][0], p2[j][1]);
                        }
                    }*/
                    break;
                }
            }
        }
        if(equivalent==1){
            /*if(s->num_steps==13){
                printf("Equivalent Found.\n");
            }*/
            return 1;
        }
        temp = temp->next;
    }
    /*if(s->num_steps==13){
        printf("Unique Found.\n");
    }*/
    return 0;
}

// Determines if a given state is "valid", meaning that it would not
// lead to the undo destruction of a microchip.
int is_valid(STATE* s){
    char pairs[ELEMENTS][2];
    int  i, j, cf;

    memcpy(pairs, s->map, ELEMENTS*2);
    for(i=0;i<ELEMENTS;i++){
        cf = pairs[i][0];
        for(j=0;j<ELEMENTS;j++){
            if(j!=i && pairs[j][1]==cf && pairs[j][0]!=cf){
                //printf("Invalid Solution.\n");
                return 0;
            }
        }
    }
    //printf("Valid Solution.\n");
    return 1;
}

// Returns a 2D Array consisting of an element pair mapping
PAIRS make_pair_mapping(char map[FLOORS][OBJECTS][3]){
    PAIRS pairs;
    int   i, j;

    for(i=0;i<FLOORS;i++){
        for(j=0;j<OBJECTS;j++){
            if(strcmp(map[i][j], "  ")!=0){
                pairs.pairs[j/2][map[i][j][1]!='G'] = i;
            }
        }
    }
    return pairs;
}

// Performs all valid and non-equivalent permutations of a given step
// count. Determines if the goal state is among these.
int move_level(int s){
    char m[ELEMENTS][2];
    char nm1[ELEMENTS][2];
    char nm2[ELEMENTS][2];
    int  e1, e2, t1, t2, cf;
    STATE* temp = head;
    STATE* perm;
    STATE* new_list = NULL;
   
    printf("Curr Step: %d\n", s); 
    while(temp!=NULL){
        memcpy(m, temp->map, ELEMENTS*2);
        cf = temp->floor;

        for(e1=0;e1<ELEMENTS;e1++){
            for(t1=0;t1<2;t1++){
                for(e2=e1;e2<ELEMENTS;e2++){
                    for(t2=0;t2<2;t2++){
                        if(m[e1][t1]==cf && m[e2][t2]==cf){
                            memcpy(nm1, temp->map, ELEMENTS*2);
                            memcpy(nm2, temp->map, ELEMENTS*2);
                            if(cf>0){
                                nm1[e1][t1] = m[e1][t1] - 1;
                                nm1[e2][t2] = m[e2][t2] - 1;
                                
                                perm = create_state(nm1, cf-1, s+1);
                                if(is_valid(perm)==1 && has_equivalent(perm)==0){
                                    perm->next = new_list;
                                    new_list = perm;
                                    size++;
                                    if(perm->distance==0){
                                        return 1;
                                    }
                                }else{
                                    free(perm);
                                }
                            }
                            if(cf<3){
                                nm2[e1][t1] = m[e1][t1] + 1;
                                nm2[e2][t2] = m[e2][t2] + 1;

                                perm = create_state(nm2, cf+1, s+1);
                                if(is_valid(perm)==1 && has_equivalent(perm)==0){
                                    perm->next = new_list;
                                    new_list = perm;
                                    size++;
                                    if(perm->distance==0){
                                        return 1;
                                    }
                                }else{
                                    free(perm);
                                }
                            }
                        }
                    }
                }
            }
        }
        temp = temp->next;
    }
    free_list();    // Free the old list, and replace it with new one.
    head = new_list;
    printf("Curr Size: %d\n", size);
    return 0;
}
