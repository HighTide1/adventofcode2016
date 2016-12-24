#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 16384
#define ROWS 43 
#define COLS 179
#define MASKS 128

// State Structure. Each State is represented by its location on a 2D grid,
// the 'important' locations encountered thusfar, and the number of steps taken.
typedef struct state_t{
    int    r;
    int    c;
    int    loc_mask;
    int    num_steps;
    struct state_t* next;
} state;

// Queue Structure.
typedef struct queue_t{
    state* head;
    state* tail;
    int    size;
} queue;

// Function Prototypes
int    build_map(char* input_file, char map[ROWS][COLS]);
state* create_state(int r, int c, int l_m, int n_s);
queue* create_queue(void);
int    find_shortest(char* input_file);
void   free_queue(queue* q);
int    in_list(state* l, state* s);
state* pop(queue* q);
void   push(queue* q, state* s);

int main(int argc, char** argv){
    char*  input_file;
    
    // Read in the input file specified by argv
    if(argc!=2){
        fprintf(stderr, "No specified input file.\n");
        return -1;
    }else{
        input_file = argv[1];
    }
    
    printf("The shortest path is %d steps.\n", find_shortest(input_file));
    return 0;
}

// Build a 2D map off the specified input file, and return the number of 'important'
// points found in the map.
int build_map(char* input_file, char map[ROWS][COLS]){
    FILE* input_txt;
    char  buffer[BUFFER_SIZE];
    int   r=0, c, num_points=0;

    // Open the given file for reading
    input_txt = fopen(input_file, "r");
    if(!input_txt){
        fprintf(stderr, "Cannot open %s for reading.\n", input_file);
        return -1;
    }

    fgets(buffer, BUFFER_SIZE, input_txt);
    while(feof(input_txt)==0){
        memcpy(map[r++], buffer, COLS);
        for(c=0;c<COLS;c++){
            if(buffer[c]>='1' && buffer[c]<='9'){
                num_points++;
            }
        }
        fgets(buffer, BUFFER_SIZE, input_txt);
    }
    
    fclose(input_txt);
    return num_points;
}

// Allocate/create a new state.
state* create_state(int r, int c, int l_m, int n_s){
    state* new_state = (state*)malloc(sizeof(state));

    new_state->r = r;
    new_state->c = c;
    new_state->loc_mask = l_m;
    new_state->num_steps = n_s;
    new_state->next = NULL;
    return new_state;
}

// Allocate/create a new queue
queue* create_queue(void){
    queue* new_queue = (queue*)malloc(sizeof(queue));
    
    new_queue->head = NULL;
    new_queue->tail = NULL;
    new_queue->size = 0;
    return new_queue;
}

// Find the shortest path that goes over every point using a BFS.
int find_shortest(char* input_file){
    queue* state_queue;
    state* curr_state;
    state* temp;
    char hvac_map[ROWS][COLS];
    int  visited[ROWS][COLS][MASKS];
    int  dr[] = {-1, 1, 0, 0};
    int  dc[] = {0, 0, -1, 1};
    int  i, j, k, r, c, s_r=0, s_c=0;
    int  curr_mask, goal_mask, num_points, num_steps=0;

    // Initialize our visited map to be -1 for all locations.
    for(i=0;i<ROWS;i++){
        for(j=0;j<COLS;j++){
            for(k=0;k<MASKS;k++){
                visited[i][j][k] = -1;
            }
        }
    }

    // Allocate our location queue, and build up our HVAC map to parse.
    // Then find our starting location on the HVAC map.
    state_queue = create_queue();
    num_points = build_map(input_file, hvac_map);
    goal_mask = (1 << num_points) - 1;
    for(r=0;r<ROWS;r++){
        for(c=0;c<COLS;c++){
            if(hvac_map[r][c]=='0'){
                break;
            }
        }
        if(hvac_map[r][c]=='0'){
            s_r = r;
            s_c = c;
            break;
        }
    }
    temp = create_state(s_r, s_c, 0, 0);
    visited[s_r][s_c][temp->loc_mask] = temp->num_steps;
    push(state_queue, temp);
  
    //printf("Num Points: %d\n", num_points);
    //printf("Goal Mask: %d\n", goal_mask);
    //printf("Starting Loc: %d, %d\n", s_r, s_c);
     
    while(state_queue->size!=0){
        curr_state = pop(state_queue);
        r = curr_state->r;
        c = curr_state->c;
        curr_mask = curr_state->loc_mask;
        num_steps = curr_state->num_steps;

        //printf("%d, %d, %d, %d\n", r, c, curr_mask, num_steps);
        // If our state gets us on an 'important' location, update the mask.
        if(hvac_map[r][c]>='1' && hvac_map[r][c]<='9'){
            //printf("Old Mask: %d\n", curr_mask);
            //printf("Important Loc: %c\n", hvac_map[r][c]);
            curr_mask |= (1 << ((hvac_map[r][c] - '0') - 1));
            //printf("New Mask: %d %d\n", curr_mask, num_steps);
        }
    
        // If we've reached our goal state, we can just free everything else off
        // the queue and return our current step count.
        if(curr_mask==goal_mask && r==s_r && c==s_c){
            free(curr_state);
            free_queue(state_queue);
            return num_steps;
        }else{
            // Otherwise, we need to check for all possible moves and go from there.
            num_steps += 1;
            for(i=0;i<4;i++){
                if((r+dr[i])>=0 && (c+dc[i])>=0 && (r+dr[i])<ROWS && (c+dc[i])<COLS && hvac_map[r+dr[i]][c+dc[i]]!='#'){
                    temp = create_state(r+dr[i], c+dc[i], curr_mask, num_steps);
                    if(visited[temp->r][temp->c][temp->loc_mask]==-1){
                        //printf("Not in List. Adding ");
                        //printf("%d, %d, %d, %d\n", r+dr[i], c+dc[i], curr_mask, num_steps);
                        visited[temp->r][temp->c][temp->loc_mask] = num_steps;
                        push(state_queue, temp);
                    }else{
                        //printf("Already in List.\n");
                        free(temp);
                    }
                }
            }
            free(curr_state);
        }
        //printf("%d, %d\n", num_steps, state_queue->size);
    }
    return -1;
}

// Free the specified queue from memory.
void free_queue(queue* q){
    state* temp;

    temp = pop(q);
    while(temp!=NULL){
        free(temp);
        temp = pop(q);
    }
    free(q);
}

// Determines if the given state is already in the list.
// Compares row, column, and current bitmask.
/*int in_list(state* l, state* s){
    state* temp = l;

    while(temp!=NULL){
        if(temp->r==s->r && temp->c==s->c && temp->loc_mask==s->loc_mask){
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}*/

// Pop the oldest state from the queue, if it exists.
state* pop(queue* q){
    state* temp;

    if(q->head==NULL){
        return NULL;
    }else{
        temp = q->head;
        if(q->head==q->tail){
            q->head = NULL;
            q->tail = NULL;
        }else{
            q->head = q->head->next;
        }
        q->size--;
        return temp;
    }
}

// Push the given state to the queue.
void push(queue* q, state* s){
    
    if(q->head==NULL){
        q->head = s;
        q->tail = s;
    }else{
        q->tail->next = s;
        q->tail = s;
    }
    q->size++;
}
