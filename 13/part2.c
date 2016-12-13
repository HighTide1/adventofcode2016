#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROWS 500
#define COLS 500
#define INIT_DIST 10000

// Pair Structure. Represents a X and Y coordinate
typedef struct pair_t{
    int r;
    int c;
    int d;
    struct pair_t* next;
} PAIR;
PAIR* visited;
PAIR* unvisited;
int size = 0;

// Function Prototypes
PAIR* create_pair(int r, int c, int d);
int   find_parity(int num);
int   find_reachable(char map[ROWS][COLS], int r_lim);
void  free_list(void);
PAIR* get_min(void);
int   in_list(PAIR* l, int r, int c);
void  print_layout(char map[ROWS][COLS]);

int main(int argc, char** argv){
    char layout[ROWS][COLS];
    int  i, j, form, input_num, r, num_reach;

    // Read in the input file specified by argv
    if(argc!=3){
        fprintf(stderr, "No specified input number or goal.\n");
        return -1;
    }else{
        input_num = atoi(argv[1]); // 1st Index is 1st arg
        r = atoi(argv[2]);
    }

    // Build a layout of the map.
    for(i=0;i<ROWS;i++){
        for(j=0;j<COLS;j++){
            form = (j*j)+(3*j)+(2*j*i)+(i)+(i*i);
            form += input_num;

            // If we have an odd number of 1 bits
            if(find_parity(form)==0){
                layout[i][j]='#';
            }else{
                layout[i][j]='.';
            }
        }
    }
    //print_layout(layout);
    num_reach = find_reachable(layout, r);

    printf("The number of spaces reachable in %d steps is %d.\n", r, num_reach);
    return 0;
}

PAIR* create_pair(int r, int c, int d){
    PAIR* new_pair = (PAIR*)malloc(sizeof(PAIR));

    new_pair->r = r;
    new_pair->c = c;
    new_pair->d = d;
    new_pair->next = NULL;
    return new_pair;
}

int find_reachable(char map[ROWS][COLS], int r_lim){
    int distances[ROWS][COLS];
    int r, c, dist, reach=0;
    PAIR* curr_node, *temp;

    // Initialize Distance Map to high value
    for(r=0;r<ROWS;r++){
        for(c=0;c<COLS;c++){
            distances[r][c] = INIT_DIST;
        }
    }
    distances[1][1] = 0; // Starting Distance
    
    unvisited = create_pair(1, 1, 0);
    size++;
    while(unvisited!=NULL){
        curr_node = get_min();
        if(curr_node!=NULL){
            r = curr_node->r;
            c = curr_node->c;
            dist = curr_node->d + 1;

            if(r-1>=0 && dist<distances[r-1][c] && map[r-1][c]=='.'){
                distances[r-1][c] = dist;
                if(in_list(visited, r-1, c)==0){
                    temp = create_pair(r-1, c, dist);
                    temp->next = unvisited;
                    unvisited = temp;
                    size++;
                }
            }
            if(r+1<ROWS && dist<distances[r+1][c] && map[r+1][c]=='.'){
                distances[r+1][c] = dist;
                if(in_list(visited, r+1, c)==0){
                    temp = create_pair(r+1, c, dist);
                    temp->next = unvisited;
                    unvisited = temp;
                    size++;
                }
            }
            if(c-1>=0 && dist<distances[r][c-1]  && map[r][c-1]=='.'){
                distances[r][c-1] = dist;
                if(in_list(visited, r, c)==0){
                    temp = create_pair(r, c-1, dist);
                    temp->next = unvisited;
                    unvisited = temp;
                    size++;
                }
            }
            if(c+1<COLS && dist<distances[r][c+1] && map[r][c+1]=='.'){
                distances[r][c+1] = dist;
                if(in_list(visited, r, c+1)==0){
                    temp = create_pair(r, c+1, dist);
                    temp->next = unvisited;
                    unvisited = temp;
                    size++;
                }
            }

            // Move to next unvisited node, and add the current
            // to the visited list.
            curr_node->next = visited;
            visited = curr_node;
            size--;
            //printf("Curr Size: %d\n", size);
        }
    }

    for(r=0;r<ROWS;r++){
        for(c=0;c<COLS;c++){
            if(distances[r][c]<=r_lim){
                reach++;
            }
        }
    }
    return reach;
}

int find_parity(int num){
    int parity=1;

    while(num!=0){
        parity ^= (num&0x1);
        num = num >> 1;
    }
    return parity;
}

void free_list(void){
    PAIR* temp = visited;

    while(temp!=NULL){
        visited = visited->next;
        free(temp);
        temp = visited;
    }
}

PAIR* get_min(void){
    PAIR* min = unvisited;
    PAIR* curr = unvisited;
    
    while(curr!=NULL){
        if(curr->d<min->d){
            min = curr;
        }
        curr = curr->next;
    }
    curr = unvisited;
    while(curr!=NULL){
        if(curr->next==min){
            break;
        }
        curr = curr->next;
    }
    if(curr==NULL){
        unvisited = min->next;
    }else{
        curr->next = min->next;
    }
    return min;
}

int in_list(PAIR* l, int r, int c){
    PAIR* temp = l;

    while(temp!=NULL){
        if(r==temp->r && c==temp->c){
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}

void print_layout(char map[ROWS][COLS]){
    int i, j;

    for(i=-1;i<ROWS;i++){
        for(j=0;j<COLS;j++){
            if(i==-1){
                printf("-");
            }else{
                printf("%c", map[i][j]);
            }
        }
        printf("\n");
    }
}
