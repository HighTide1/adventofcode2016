#include <stdio.h>

int main(int argc, char** argv){
    FILE* input_txt;
    int s1, s2, s3;
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
    while(fscanf(input_txt, "%d %d %d ", &s1, &s2, &s3)!=EOF){
        if(s1+s2>s3 && s1+s3>s2 && s2+s3>s1){
            num_tri++;
        }
    }
    
    fclose(input_txt);  // Close the file for cleanup;
    printf("The number of valid triangles is %d.\n", num_tri);
    return 0;
}
