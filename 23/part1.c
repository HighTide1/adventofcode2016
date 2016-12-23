#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define NUM_REGS 4

// Instruction Structure. Basically acts as a holder for previous
// instructions, such that the jump instruction will work.
typedef struct instr_t{
    char   buffer[BUFFER_SIZE];
    int    num_args;
    struct instr_t* prev;
    struct instr_t* next;
} INSTR;
INSTR* head = NULL;
INSTR* tail = NULL;

// Function Prototypes
void add_instr(char* buffer);
void change_instr(INSTR* curr_instr, int num_away);
int  is_valid(INSTR* curr_instr);

int main(int argc, char** argv){
    FILE*  input_txt;
    char   buffer[BUFFER_SIZE];
    int    regs[NUM_REGS] = {0, 0, 0, 0};
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
        regs[0] = atoi(argv[2]);
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
        printf("%s", buffer);
        if(is_valid(curr_instr)==1){
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
            }else if(sscanf(buffer, "jnz %d %c", &tmp_val, &reg)==2){
                if(tmp_val!=0){
                    val = regs[reg-'a'];
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
            }else if(sscanf(buffer, "tgl %c", &reg)==1){
                change_instr(curr_instr, regs[reg-'a']);
                curr_instr = curr_instr->next;
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
            printf("(%d, %d, %d, %d)\n", regs[0], regs[1], regs[2], regs[3]);
        } 
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
    
    if(buffer[0]=='i' || buffer[0]=='d' || buffer[0]=='t'){
        new_instr->num_args = 1;
    }else{
        new_instr->num_args = 2;
    }

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

// Changes an instruction based off the tgl instruction.
void change_instr(INSTR* curr_instr, int num_away){
    INSTR* temp = curr_instr;

    // Firstly, we need to determine which instruction we will potentially be modifying.
    // This behaves like a normal jump, moving either forwards or backwards.
    while(num_away!=0 && temp!=NULL){
        if(num_away<0){
            temp = temp->prev;
            num_away++;
        }else{
            temp = temp->next;
            num_away--;
        }
    }

    // For toggle instructions, we only consider those that would modify existing
    // instructions, and would not modify themselves.
    if(temp!=NULL && temp!=curr_instr){
        if(temp->num_args==1){
            if(temp->buffer[0]=='i'){
                temp->buffer[0] = 'd';
                temp->buffer[1] = 'e';
                temp->buffer[2] = 'c';
            }else{
                temp->buffer[0] = 'i';
                temp->buffer[1] = 'n';
                temp->buffer[2] = 'c';
            }
        }else{
            if(temp->buffer[0]=='j'){
                temp->buffer[0] = 'c';
                temp->buffer[1] = 'p';
                temp->buffer[2] = 'y';
            }else{
                temp->buffer[0] = 'j';
                temp->buffer[1] = 'n';
                temp->buffer[2] = 'z';
            }
        }
    }
}

// Determines if the given buffer contains a valid instruction (i.e. one not made invalid
// by the tgl instruction.).
int is_valid(INSTR* curr_instr){
    int v1, v2;
    
    if(curr_instr->num_args==1){
        return 1;
    }else{
        if(sscanf(curr_instr->buffer, "cpy %d %d", &v1, &v2)==2){
            printf("Invalid Instr.\n");
            return 0;
        }else{
            return 1;
        }
    }    
}
