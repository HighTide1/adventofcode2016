#include <stdio.h>

int main(int argc, char** argv){
    FILE* input_txt;
    int s[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0}, i=0, j=0;
    int num_tri=0;
    
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

    // Begin processing direction instructions until EOF
    while(fscanf(input_txt,"%d %d %d ",&s[i],&s[i+1],&s[i+2])!=EOF){
        i+=3;
        if(i==9){
            for(j=0;j<3;j++){
                if(s[j]+s[j+3]>s[j+6] && 
                   s[j]+s[j+6]>s[j+3] && 
                   s[j+3]+s[j+6]>s[j]){
                    num_tri++;
                }
            }
            i = 0;
        }
    }
    
    fclose(input_txt);  // Close the file for cleanup;
    printf("The number of valid triangles is %d.\n", num_tri);
    return 0;
}
