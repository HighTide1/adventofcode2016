#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 16384
#define NUM_IPS 4294967296U

// Function Prototypes

int main(int argc, char** argv){
    FILE*             input_txt;
    char              buffer[BUFFER_SIZE];
    char*             valid_ips;
    unsigned long int i, start, end, num_ips=0;

    //printf("%d\n", sizeof(int));

    // Read in the input file specified by argv
    if(argc!=2){
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
        if(i%100000000==0){
            printf("%lu\n", i);
        }
    }

    // Begin processing lines until EOF
    fgets(buffer, BUFFER_SIZE, input_txt);
    while(feof(input_txt)==0){
        //printf("%s", buffer);
        sscanf(buffer, "%lu-%lu", &start, &end);
        //printf("%u, %u\n", start, end);
        for(i=start;i<=end;i++){
            valid_ips[i] = 0;
        }
        fgets(buffer, BUFFER_SIZE, input_txt);
    }

    // Find the first non-zero value in the valid_ips.
    for(i=0;i<NUM_IPS;i++){
        if(valid_ips[i]==1){
            num_ips++;
        }
    }
    
    fclose(input_txt);  // Close the file for cleanup;
    free(valid_ips);
    printf("The number of valid IPs is %lu.\n", num_ips);
    return 0;
}
