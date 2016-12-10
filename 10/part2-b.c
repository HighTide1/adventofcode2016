#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 16384

// Bot Structure
typedef struct bot_t{
    int bot_num;        // Number of the Bot
    int m1;             // Microchip 1 Value
    int m2;             // Microchip 2 Value
    int type;           // Type of Bot (Bot, Output)
    struct bot_t* l_r;  // Low Num Receiver
    struct bot_t* h_r;  // High Num Receiver
    struct bot_t* next; // Pointer to next
} BOT;
BOT* head=NULL;          // Linked List of Bots

// Function Prototypes
void assign_num(BOT* b, int n);
int  check_primed(BOT* b);
BOT* create_bot(int n, int t, int m1, int m2, BOT* lr, BOT* hr);
BOT* find_bot(int n, int t);
int  mod_bot(char* s);
void free_list(void);

int main(int argc, char** argv){
    FILE* input_txt;
    char  buffer[BUFFER_SIZE];
    int   ans=1;
    BOT*  temp; 
   
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

    // Begin processing lines until EOF
    fgets(buffer, BUFFER_SIZE, input_txt);
    while(feof(input_txt)==0){
        mod_bot(buffer);
        fgets(buffer, BUFFER_SIZE, input_txt);
    }

    temp = head;
    while(temp!=NULL){
        if(temp->type==1){
            if(temp->bot_num<3){
                ans *= abs(temp->m1);
                ans *= abs(temp->m2);
            }
        }
        temp = temp->next;
    }
    
    fclose(input_txt);  // Close the file for cleanup;
    free_list();

    printf("The multiplication of the O1, O2, and O3 is %d.\n", ans);
    return 0;
}

// Assigns a number to the bot listed. Assumes at most 1 valid
// number. Places the lower of the two in m1, and the higher in m2.
void assign_num(BOT* b, int n){
    int m = b->m1;

    if(m==-1){
        b->m1 = n;
    }else{
        if(n>m){
            b->m2 = n;
        }else{
            b->m1 = n;
            b->m2 = m;
        }
    }
}

// Recursivelly called function to check and see if a given
// bot is primed. If it is, it gives out its chips,
// and checks to see if recievers are primed as well.
int check_primed(BOT* b){
    int m1, m2, found_num=-1;

    if(b!=NULL && b->m1!=-1 && b->m2!=-1 && b->l_r!=NULL && b->h_r!=NULL){
        m1 = b->m1;
        m2 = b->m2;        
        b->m1 = -1;
        b->m2 = -1;
        if(m1 > m2){
            assign_num(b->l_r, m2);
            assign_num(b->h_r, m1);
        }else{
            assign_num(b->l_r, m1);
            assign_num(b->h_r, m2);
        }
        
        found_num = check_primed(b->l_r);
        if(found_num!=-1){
            return found_num;
        }else{
            found_num = check_primed(b->h_r);
            return found_num;
        }
    }
    return -1;
}

// Creates a bot of the given parameters, and pushesit.
BOT* create_bot(int n, int t, int m1, int m2, BOT* lr, BOT* hr){
    BOT* temp = (BOT*)malloc(sizeof(BOT));
   
    temp->bot_num = n;
    temp->type = t;
    temp->m1 = m1;
    temp->m2 = m2;
    temp->l_r = lr;
    temp->h_r = hr;

    temp->next = head;
    head = temp;
    return temp;
}

// Determines if the given bot exists.
BOT* find_bot(int n, int t){
    BOT* temp=head;
    
    while(temp!=NULL){
        if(temp->bot_num==n && temp->type==t){
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

// Creates or modifies an existing bot in the linked list.
// If this modification gives the bot 2 numbers, the instructions
// are run through until completion.
int mod_bot(char* s){
    BOT*  temp, *t1=NULL, *t2=NULL;
    int   bn, m, r1, ty1, r2, ty2, cb=-1;
    char* v="value %d goes to bot %d";
    char* g1="bot %d gives low to bot %d and high to bot %d";
    char* g2="bot %d gives low to output %d and high to bot %d";
    char* g3="bot %d gives low to output %d and high to output %d";

    // For instructions, there are 2 primary types: Value Assign
    // and Give Instructions.
    printf("%s\n", s);
    if(sscanf(s, v, &m, &bn)==2){
        temp = find_bot(bn, 0);
        if(temp!=NULL){
            if(temp->m1==-1){
                temp->m1 = m;
            }else{
                assign_num(temp, m);
            }
        }else{
            create_bot(bn, 0, m, -1, NULL, NULL);
        }
    }else{
        // For Give Instructions, there are a couple cases
        // to consider. We either give from 1 bot to 2 bots,
        // or 1 bot to a bot and output, or just 1 bot to 2 outs.
        if(sscanf(s, g1, &bn, &r1, &r2)==3){
            ty1 = 0;
            ty2 = 0;
        }else if(sscanf(s, g2, &bn, &r1, &r2)==3){
            ty1 = 1;
            ty2 = 0;
        }else if(sscanf(s, g3, &bn, &r1, &r2)==3){
            ty1 = 1;
            ty2 = 1;
        }

        t1 = find_bot(r1, ty1);
        if(t1==NULL){
            t1 = create_bot(r1, ty1, -1, -1, NULL, NULL);
        }

        t2 = find_bot(r2, ty2);
        if(t2==NULL){
            t2 = create_bot(r2, ty2, -1, -1, NULL, NULL);
        }
    
        temp = find_bot(bn, 0);
        if(temp!=NULL){
            temp->l_r = t1;
            temp->h_r = t2;
        }else{
            create_bot(bn, 0, -1, -1, t1, t2);
        }
    }

    // Now we need to check and see if a bot is primed.
    temp = head;
    while(temp!=NULL){
        cb = check_primed(temp);
        if(cb!=-1){
            return cb;
        }
        temp = temp->next;
    }
    return -1;
}

// Helper Function to free the linked list.
void free_list(void){
    BOT* temp=head;

    while(temp!=NULL){
        head = head->next;
        free(temp);
        temp = head;
    }
}
