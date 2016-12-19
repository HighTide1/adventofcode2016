#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

// Function Prototypes
int  generate_map(char* start_row, int num_rows);
void print_map(char** map, int n_r, int n_c);

int main(int argc, char** argv){
    FILE* input_txt;
    char  start_row[BUFFER_SIZE];
    int   num_rows, num_safe;

    // Read in the input file specified by argv
    if(argc!=3){
        fprintf(stderr, "No specified input file or map length.\n");
        return -1;
    }else{
        input_txt = fopen(argv[1], "r"); // 1st Index is 1st arg
        if(!input_txt){
            fprintf(stderr, "Unable to open %s.\n", argv[1]);
            return -1;
        }
        num_rows = atoi(argv[2]);
    }

    // Get the input row from the file, and generate a map based off the info.
    fgets(start_row, BUFFER_SIZE, input_txt);
    num_safe = generate_map(start_row, num_rows);
    
    fclose(input_txt);  // Close the file for cleanup;
    printf("There are %d safe tiles on the map.\n", num_safe);
    return 0;
}

// Generate a tile map based off the trap/safe conditions and the input
// values
int generate_map(char* start_row, int num_rows){
    char** map = (char**)malloc(num_rows*sizeof(char*));
    char   l, c, r;
    int    i, j, num_cols=strlen(start_row)-1, num_safe=0;
    
    // Allocate space for the variable-sized map.
    for(i=0;i<num_rows;i++){
        map[i] = (char*)malloc(num_cols*sizeof(char));
    }
    memcpy(map[0], start_row, num_cols);
    
    // Once we've done the initial setup for the map, we loop through each row
    // and column, and assign traps and safe depending on the rules.
    for(i=1;i<num_rows;i++){
        for(j=0;j<num_cols;j++){
            // Get the Left, Center, and Right map tiles
            if(j>0){
                l = map[i-1][j-1];
            }else{
                l = '.';
            }
            c = map[i-1][j];
            if(j<num_cols-1){
                r = map[i-1][j+1];
            }else{
                r = '.';
            }

            // For determining whether the new tile is a trap, there are 4 cases.
            // 1) Left and Center are traps, but Right is not.
            // 2) Center and Right are traps, but Left is not.
            // 3) Only Left is Trap.
            // 4) Only Right is Trap.
            map[i][j] = '.';
            if(l=='^' && c=='^' && r=='.'){
                map[i][j] = '^';
            }
            if(l=='.' && c=='^' && r=='^'){
                map[i][j] = '^';
            }
            if(l=='^' && c=='.' && r=='.'){
                map[i][j] = '^';
            }
            if(l=='.' && c=='.' && r=='^'){
                map[i][j] = '^';
            }
        }
    }
    //print_map(map, num_rows, num_cols);

    // After the map is generated, count the number of safe tiles per row,
    // and free the row afterwards.
    for(i=0;i<num_rows;i++){
        for(j=0;j<num_cols;j++){
            if(map[i][j]=='.'){
                num_safe++;
            }
        }
        free(map[i]);
    }
    free(map);
    return num_safe;
}

// Prints the 2D map.
void print_map(char** map, int n_r, int n_c){
    int i, j;

    for(i=0;i<n_r;i++){
        for(j=0;j<n_c;j++){
            printf("%c", map[i][j]);
        }
        printf("\n");
    } 
}
