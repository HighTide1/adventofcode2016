#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 67108864

// Function Prototypes
void generate_checksum(char* s, int l);

int main(int argc, char** argv){
    char* init_data;
    int   data_size;    
    
    // Read in the input file specified by argv
    if(argc!=3){
        fprintf(stderr, "No specified input data or data length.\n");
        return -1;
    }else{
        init_data = argv[1];
        data_size = atoi(argv[2]);
    }

    generate_checksum(init_data, data_size);
    return 0;
}

// Extends the input string to be a minimum of l characters long, and
// subsequently generates the checksum for the new data.
void generate_checksum(char* s, int l){
    char buffer[BUFFER_SIZE];
    char check[BUFFER_SIZE];
    char data[BUFFER_SIZE];
    int  i, dl;

    strcpy(data, s);
     
    // While our 'random' data sequence is less than the given size,
    // we need to continuously reverse the current data sequence,
    // replace 0 and 1 with their opposites, and append it to the current data.
    while(strlen(data)<l){
        dl = strlen(data);
        for(i=0;i<dl;i++){
            if(data[dl-i-1]=='1'){
                buffer[i] = '0';
            }else{
                buffer[i] = '1';
            }
        }
        buffer[dl] = '\0';
        strcat(data, "0");
        strcat(data, buffer);
        //printf("Curr Data: %s\n", data);
    }

    // For generating the checksum, we need to check the look for certain char
    // groupings, and continue regenerating until the checksum is an odd length.
    dl = l;
    strcpy(check, data);
    while((dl%2)==0){
        for(i=0;i<dl;i+=2){
            if(check[i]==check[i+1]){
                check[i/2] = '1';
            }else{
                check[i/2] = '0';
            }
        }
        check[dl/2] = '\0';
        dl = strlen(check);
        //printf("Curr Check: %s\n", check);
    }

    printf("The generated checksum for the data is %s.\n", check);
}
