#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 16384

// Input Format Strins
char* swp = "swap position %d with position %d";
char* swl = "swap letter %c with letter %c";
char* rol = "rotate left %d steps";
char* ror = "rotate right %d steps";
char* rop = "rotate based on position of letter %c";
char* rp  = "reverse positions %d through %d";
char* mp  = "move position %d to position %d";

// Function Prototypes
void generate_permutations(char* input, char* goal, char* s, int l, int r);
void process_operation(char* s, char* inst);
void run_file(char* input, char* s);
void swap(char* s, int i, int j);

int main(int argc, char** argv){
    char* input_txt;
    char* goal_str;
    char* in_str = "abcdefgh";

    // Read in the input file specified by argv
    if(argc!=3){
        fprintf(stderr, "No specified scrambling directions or input.\n");
        return -1;
    }else{
        input_txt = argv[1];
        goal_str = argv[2];
    }

    // Since we only have 8! different string combinations, we
    // can just try all different permutations to get our goal
    generate_permutations(input_txt, goal_str, in_str, 0, 7);
    return 0;
}

// Generate and test all permutations of the string to test them.
void generate_permutations(char* input, char* goal, char* s, int l, int r){
    int  i;

    if(l==r){
        run_file(input, s);
        if(strcmp(s, goal)==0){
            printf("The unscrambled string is %s.\n", s);
        }
    }else{
        //printf("%s\n", s);
        for(i=l;i<=r;i++){
            //printf("Attempting Swap.\n");
            swap(s, l, i);
            //printf("Generating Permutation.\n");
            generate_permutations(input, goal, s, l+1, r);
            //printf("Attempting Swap.\n");
            swap(s, l, i);
        }
    }
}

// Process the current scramble operation, and edit the given string to match.
void process_operation(char* s, char* inst){
    char* tmp;
    char  l1, l2;
    int   i, p1, p2, num_steps, sl = strlen(s);

    // Allocate space for the temporary string buffer. (For rotate operations).
    //printf("String Length: %d.\n", sl);
    tmp = (char*)malloc((sl+1)*sizeof(char));
    for(i=0;i<sl+1;i++){
        tmp[i] = s[i];
    }

    // In regards to scrambling operations, we have 6 operations to consider:
    // Swapping based on position/lettersm rotating based of steps and positions,
    // reversing positions, and moving positions.
    if(sscanf(inst, swp, &p1, &p2)==2){        // Swap Position Instruction
        //printf("Swapping Positions %d and %d.\n", p1, p2);
        s[p1] = tmp[p2];
        s[p2] = tmp[p1];
    }else if(sscanf(inst, swl, &l1, &l2)==2){  // Swap Letters Instruction
        //printf("Swapping Letters %c and %c.\n", l1, l2);
        for(i=0;i<sl;i++){
            if(s[i]==l1){
                s[i] = l2;
            }else{
                if(s[i]==l2){
                    s[i] = l1;
                }
            }
        }
    }else if(sscanf(inst, rol, &num_steps)==1){ // Rotate Left Instruction
        //printf("Rotating Left %d.\n", num_steps);
        num_steps = (sl - (num_steps % sl) % sl);
        snprintf(inst, BUFFER_SIZE, ror, num_steps);
        //printf("New Instr: %s\n", inst);
        process_operation(s, inst);
    }else if(sscanf(inst, ror, &num_steps)==1){ // Rotate Right Instruction
        //printf("Rotating Right %d.\n", num_steps);
        num_steps = num_steps % sl;
        for(i=0;i<sl;i++){
            s[(i+num_steps)%sl] = tmp[i];
        }
    }else if(sscanf(inst, rop, &l1)==1){        // Rotate Position Instruction
        //printf("Rotating Position %c.\n", l1);
        for(i=0;s[i]!=l1;i++){}
        num_steps = i + 1;
        if(i>=4){
            num_steps++;
        }
        snprintf(inst, BUFFER_SIZE, ror, num_steps);
        //printf("New Instr: %s\n", inst);
        process_operation(s, inst);
    }else if(sscanf(inst, rp, &p1, &p2)==2){
        //printf("Reversing Positions %d and %d.\n", p1, p2);
        for(i=p1;i<=(p1+((p2-p1)/2));i++){
            s[i] = tmp[p2-(i-p1)];
            s[p2-(i-p1)] = tmp[i];
            //printf("Temp String: %s\n", s);
        }
    }else if(sscanf(inst, mp, &p1, &p2)==2){    //ERROE
        //printf("Moving Position %d and %d.\n", p1, p2);
        if(p1<p2){
            for(i=p1;i<p2;i++){
                s[i] = tmp[i+1];
            }
            s[p2] = tmp[p1];
        }else{
            s[p2] = tmp[p1];            
            for(i=p2+1;i<=p1;i++){
                s[i] = tmp[i-1];
            }
        }
    }

    free(tmp);
}

void run_file(char* input, char* s){
    FILE* input_txt;
    char buffer[BUFFER_SIZE];
    
    input_txt = fopen(input, "r"); // 1st Index is 1st arg
    if(!input_txt){
        fprintf(stderr, "Unable to open %s.\n", input);
    }

    // Begin processing lines until EOF
    fgets(buffer, BUFFER_SIZE, input_txt);
    while(feof(input_txt)==0){
        process_operation(s, buffer);
        fgets(buffer, BUFFER_SIZE, input_txt);
    }
    fclose(input_txt);
}

void swap(char* s, int i, int j){
    char tmp;

    tmp = s[i];
    s[i] = s[j];
    s[j] = tmp;
}
