#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define NUM_REGS 4

// Instruction Structure. Basically acts as a holder for previous
// instructions, such that the jump instruction will work.
typedef struct instr_t{
    char   buffer[BUFFER_SIZE];
    struct instr_t* prev;
    struct instr_t* next;
} INSTR;
INSTR* head = NULL;
INSTR* tail = NULL;

// Function Prototypes
void add_instr(char* buffer);

int main(int argc, char** argv){
    FILE*  input_txt;
    char   buffer[BUFFER_SIZE];
    int    regs[NUM_REGS] = {0, 0, 1, 0};
    char   reg, tmp_reg;
    int    val, tmp_val;
    INSTR* curr_instr;

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

    // Build a linked list of instructions from the file
    fgets(buffer, BUFFER_SIZE, input_txt);
    while(feof(input_txt)==0){
        add_instr(buffer);
        fgets(buffer, BUFFER_SIZE, input_txt);
    }

    curr_instr = head;
    while(curr_instr!=NULL){
        memcpy(buffer, curr_instr->buffer, BUFFER_SIZE);
        //printf("%s", buffer);
        if(sscanf(buffer, "jnz %d %d", &tmp_val, &val)==2){
            if(tmp_val!=0){
                while(val!=0 && curr_instr!=NULL && curr_instr!=head){
                    if(val<0){
                        curr_instr = curr_instr->prev;
                        val++;
                    }else{
                        curr_instr = curr_instr->next;
                        val--;
                    }
                }
            }else{
                curr_instr = curr_instr->next;
            }
        }else if(sscanf(buffer, "jnz %c %d", &reg, &val)==2){
            if(regs[reg-'a']!=0){
                while(val!=0 && curr_instr!=NULL && curr_instr!=head){
                    if(val<0){
                        curr_instr = curr_instr->prev;
                        val++;
                    }else{
                        curr_instr = curr_instr->next;
                        val--;
                    }
                }
            }else{
                curr_instr = curr_instr->next;
            }
        }else{
            if(sscanf(buffer, "cpy %d %c", &val, &reg)==2){
                regs[reg-'a'] = val;
            }else if(sscanf(buffer, "cpy %c %c", &reg, &tmp_reg)==2){
                regs[tmp_reg-'a'] = regs[reg-'a'];
            }else if(sscanf(buffer, "inc %c", &reg)==1){
                regs[reg-'a'] += 1;
            }else if(sscanf(buffer, "dec %c", &reg)==1){
                regs[reg-'a'] -= 1;
            }
            curr_instr = curr_instr->next;
        }
        //printf("(%d, %d, %d, %d)\n", regs[0], regs[1], regs[2], regs[3]);
    } 
    
    fclose(input_txt);  // Close the file for cleanup;
    //printf("Regs: %d, %d, %d, %d\n", regs[0], regs[1], regs[2], regs[3]);
    printf("The value of Reg A is %d.\n", regs[0]);
    return 0;
}

// Creates a new instruction structure and adds it to the end of the list.
void add_instr(char* buffer){
    INSTR* new_instr = (INSTR*)malloc(sizeof(INSTR));
    memcpy(new_instr->buffer, buffer, BUFFER_SIZE);
    if(head==NULL){
        new_instr->prev = NULL;
        new_instr->next = NULL;
        head = new_instr;
        tail = new_instr;
    }else{
        new_instr->prev = tail;
        new_instr->next = NULL;
        tail->next = new_instr;
        tail = new_instr;
    }
}
