#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define NUM_REGS 4

// Parameter Type. Can either be a REGISTER or IMMEDIATE.
enum PARAM_TYPE{
    REGISTER,
    IMMEDIATE
};

// Parameter Structure. Acts as an intermmediate type for
// the parameters of an instruction, being either a register
// or an immediate value.
typedef struct param_t{
    enum PARAM_TYPE type;
    union {
        char reg;
        int  val;
    };
} PARAM;

// Instruction ID. Instructions can only be a set number of values.
enum INSTR_ID{
    INC,
    DEC,
    CPY,
    JNZ,
    TGL,
    OUT,
    INV
};

// Instruction Structure. Basically acts as a holder for previous
// instructions, such that the jump instruction will work.
typedef struct instr_t{
    enum INSTR_ID   id;
    PARAM           param1;
    PARAM           param2;
    struct instr_t* prev;
    struct instr_t* next;
} INSTR;
INSTR* head = NULL;
INSTR* tail = NULL;

// Instruction Format Strings.
const char* inc_f   = "inc %c";
const char* dec_f   = "dec %c";
const char* tgl_rf  = "tgl %c";
const char* tgl_if  = "tgl %d";
const char* out_rf  = "out %c";
const char* out_if  = "out %d";
const char* cpy_rrf = "cpy %c %c";
const char* cpy_irf = "cpy %d %c";
const char* jnz_rrf = "jnz %c %c";
const char* jnz_rif = "jnz %c %d";
const char* jnz_irf = "jnz %d %c";
const char* jnz_iif = "jnz %d %d";

// Function Prototypes
void add_instr(char* buffer);
void change_instr(INSTR* curr_instr, int num_away);

int main(int argc, char** argv){
    FILE*  input_txt;
    char   buffer[BUFFER_SIZE];
    int    regs[NUM_REGS] = {0, 0, 0, 0};
    INSTR* curr_instr;
    int    val, jmp_val;
    int    last_out=-1, num_alternating=0, a_start;

    // Read in the input file specified by argv
    if(argc!=2){
        fprintf(stderr, "No specified input file or starting reg A value.\n");
        return -1;
    }else{
        input_txt = fopen(argv[1], "r"); // 1st Index is 1st arg
        if(!input_txt){
            fprintf(stderr, "Unable to open %s.", argv[1]);
            return -1;
        }
        //regs[0] = atoi(argv[2]);
    }

    // Build a linked list of instructions from the file
    fgets(buffer, BUFFER_SIZE, input_txt);
    while(feof(input_txt)==0){
        //printf("%s", buffer);
        add_instr(buffer);
        fgets(buffer, BUFFER_SIZE, input_txt);
    }

    for(a_start=0;num_alternating<50;a_start++){
        printf("%d\n", a_start);
        last_out = -1;
        num_alternating = 0;
        regs[0] = a_start;
        regs[1] = 0;
        regs[2] = 0;
        regs[3] = 0;

        curr_instr = head;
        while(curr_instr!=NULL){
            switch(curr_instr->id){
                case INC:
                    regs[(curr_instr->param1).reg-'a']++;
                    curr_instr = curr_instr->next;
                    break;
                case DEC:
                    regs[(curr_instr->param1).reg-'a']--;
                    curr_instr = curr_instr->next;
                    break;
                case TGL:
                    if((curr_instr->param1).type==REGISTER){
                        change_instr(curr_instr, regs[(curr_instr->param1).reg-'a']);
                    }else{
                        change_instr(curr_instr, (curr_instr->param1).val);
                    }
                    curr_instr = curr_instr->next;
                    break;
                case OUT:
                    if((curr_instr->param1).type==REGISTER){
                        val = regs[(curr_instr->param1).reg-'a'];
                    }else{
                        val = (curr_instr->param1).val;
                    }
                    if(last_out==-1){
                        last_out = val;
                        curr_instr = curr_instr->next;
                    }else{
                        if(val!=last_out){
                            num_alternating++;
                            last_out = val;
                            curr_instr = curr_instr->next;
                        }else{
                            curr_instr = NULL;
                        }
                    }
                    //printf("%d ", val);
                    //curr_instr = curr_instr->next;
                    break;
                case CPY:
                    if((curr_instr->param2).type!=IMMEDIATE){
                    if((curr_instr->param1).type==REGISTER){
                        val = regs[(curr_instr->param1).reg-'a'];
                    }else{
                        val = (curr_instr->param1).val;
                    }
                    regs[(curr_instr->param2).reg-'a'] = val;
                }
                curr_instr = curr_instr->next;
                break;
                case JNZ:
                    // For JNZ Instructions, we need first to get out comparison
                    // value, and our jump amount.
                    if((curr_instr->param1).type==REGISTER){
                        val = regs[(curr_instr->param1).reg-'a'];
                    }else{
                        val = (curr_instr->param1).val;
                    }
                    if((curr_instr->param2).type==REGISTER){
                        jmp_val = regs[(curr_instr->param2).reg-'a'];
                    }else{
                        jmp_val = (curr_instr->param2).val;
                    }

                    if(val!=0){
                        while(jmp_val!=0 && curr_instr!=NULL){
                            if(jmp_val<0){
                                curr_instr = curr_instr->prev;
                                jmp_val++;
                            }else{
                            curr_instr = curr_instr->next;
                            jmp_val--;
                        }
                    }
                    }else{
                        curr_instr = curr_instr->next;
                    }
                    break;
                default:
                    curr_instr = curr_instr->next;
                    break;
            }
            //printf("Regs: %d, %d, %d, %d\n", regs[0], regs[1], regs[2], regs[3]);
        }
    }   
 
    fclose(input_txt);  // Close the file for cleanup;
    //printf("Regs: %d, %d, %d, %d\n", regs[0], regs[1], regs[2], regs[3]);
    //printf("The value of Reg A is %d.\n", regs[0]);
    printf("The starting state for Reg A is %d.\n", a_start);
    return 0;
}

// Creates a new instruction structure and adds it to the end of the list.
void add_instr(char* buffer){
    INSTR* new_instr = (INSTR*)malloc(sizeof(INSTR));
    PARAM  p1, p2;

    // For adding instructions, we have two primary types to consider:
    // 1 Parameter and 2 Parameter Instructions.
    // There are 4 1-Param Type Instructions to consider (INC, DEC, and TGL)
    // There are 6 2-Param Type Instructions to consider (CPY and JNZ) 
    if(sscanf(buffer, inc_f, &p1.reg)==1){
        new_instr->id = INC;
        p1.type = REGISTER;
    }else if(sscanf(buffer, dec_f, &p1.reg)==1){
        new_instr->id = DEC;
        p1.type = REGISTER;
    }else if(sscanf(buffer, tgl_if, &p1.val)==1){
        new_instr->id = TGL;
        p1.type = IMMEDIATE;
    }else if(sscanf(buffer, tgl_rf, &p1.reg)==1){
        new_instr->id = TGL;
        p1.type = REGISTER;
    }else if(sscanf(buffer, out_if, &p1.val)==1){
        new_instr->id = OUT;
        p1.type = IMMEDIATE;
    }else if(sscanf(buffer, out_rf, &p1.reg)==1){
        new_instr->id = OUT;
        p1.type = REGISTER;
    }else if(sscanf(buffer, cpy_irf, &p1.val, &p2.reg)==2){
        new_instr->id = CPY;
        p1.type = IMMEDIATE;
        p2.type = REGISTER;
    }else if(sscanf(buffer, cpy_rrf, &p1.val, &p2.reg)==2){
        new_instr->id = CPY;
        p1.type = REGISTER;
        p2.type = REGISTER;
    }else if(sscanf(buffer, jnz_iif, &p1.val, &p2.val)==2){
        new_instr->id = JNZ;
        p1.type = IMMEDIATE;
        p2.type = IMMEDIATE;
    }else if(sscanf(buffer, jnz_irf, &p1.val, &p2.reg)==2){
        new_instr->id = JNZ;
        p1.type = IMMEDIATE;
        p2.type = REGISTER;
    }else if(sscanf(buffer, jnz_rif, &p1.reg, &p2.val)==2){
        new_instr->id = JNZ;
        p1.type = REGISTER;
        p2.type = IMMEDIATE;
    }else if(sscanf(buffer, jnz_rrf, &p1.reg, &p2.reg)==2){
        new_instr->id = JNZ;
        p1.type = REGISTER;
        p2.type = REGISTER;
    }else{
        new_instr->id = INV;
    }

    new_instr->param1 = p1;
    new_instr->param2 = p2;

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
        // For 1-Param Instructions, there are 2 cases. Either an INC changes to a DEC, or anything
        // changes to INC.
        switch(temp->id){
            case INC:
                temp->id = DEC;
                break;
            case DEC:
            case OUT:
            case TGL:
                temp->id = INC;
                break;
            case CPY:
                temp->id = JNZ;
                break;
            case JNZ:
                temp->id = CPY;
                break;
            default:
                break;
        }
    }
}

