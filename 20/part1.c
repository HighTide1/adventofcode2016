#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 16384
#define NUM_IPS 4294967295

// Function Prototypes

int main(int argc, char** argv){
    FILE*        input_txt;
    char         buffer[BUFFER_SIZE];
    char*        valid_ips;
    unsigned int i, start, end, min_ip;

    // Read in the input file specified by argv
    if(argc==1){
        fprintf(stderr, "No specified input file.\n");
        return -1;
    }else{
        input_txt = fopen(argv[1], "r"); // 1st Index is 1st arg
        if(!input_txt){
            fprintf(stderr, "Unable to open %s.\n", argv[1]);
            return -1;
        }
    }

    // Allocate space for our array of ips, and initialize to 1.
    valid_ips = (char*)malloc(NUM_IPS*sizeof(char));
    //printf("Initializing IPs.\n");
    for(i=0;i<NUM_IPS;i++){
        valid_ips[i] = 1;
    }

    // Begin processing lines until EOF
    fgets(buffer, BUFFER_SIZE, input_txt);
    while(feof(input_txt)==0){
        printf("%s\n", buffer);
        sscanf(buffer, "%u-%u", &start, &end);
        printf("%u, %u\n", start, end);
        for(i=start;i<=end;i++){
            valid_ips[i] = 0;
        }
        fgets(buffer, BUFFER_SIZE, input_txt);
    }

    // Find the first non-zero value in the valid_ips.
    for(min_ip=0;valid_ips[min_ip]==0;min_ip++){
    }
    
    fclose(input_txt);  // Close the file for cleanup;
    free(valid_ips);
    printf("The lowest-valued IP that is not blocked is %d.\n", min_ip);
    return 0;
}
