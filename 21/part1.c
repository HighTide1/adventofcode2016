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
void process_operation(char* s, char* inst);

int main(int argc, char** argv){
    FILE* input_txt;
    char  buffer[BUFFER_SIZE];
    char* input_str;

    // Read in the input file specified by argv
    if(argc!=3){
        fprintf(stderr, "No specified scrambling directions or input.\n");
        return -1;
    }else{
        input_txt = fopen(argv[1], "r"); // 1st Index is 1st arg
        if(!input_txt){
            fprintf(stderr, "Unable to open %s.\n", argv[1]);
            return -1;
        }
        input_str = argv[2];
    }

    // Begin processing lines until EOF
    //printf("Starting Input: %s\n", input_str);
    fgets(buffer, BUFFER_SIZE, input_txt);
    while(feof(input_txt)==0){
        //printf("Current Instruction: %s", buffer);
        process_operation(input_str, buffer);
        //printf("Current Input: %s\n", input_str);
        fgets(buffer, BUFFER_SIZE, input_txt);
    }
    
    fclose(input_txt);  // Close the file for cleanup;
    printf("The resulting scrambled text is %s.\n", input_str);
    return 0;
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
