#include <stdio.h>
#include <string.h>
 
/* Function to swap values at two pointers */
void swap(char* s, int i, int j)
{
    char temp = s[i];
    s[i] = s[j];
    s[j] = temp;
}
 
/* Function to print permutations of string
   This function takes three parameters:
   1. String
   2. Starting index of the string
   3. Ending index of the string. */
void permute(char *a, int l, int r)
{
   int i;
   if (l == r)
     printf("%s\n", a);
   else
   {
       for (i = l; i <= r; i++)
       {
          swap(a, l, i);
          permute(a, l+1, r);
          swap(a, l, i);//backtrack
       }
   }
}
 
/* Driver program to test above functions */
int main()
{
    char str[] = "abcdefgh";
    int n = strlen(str);
    permute(str, 0, 7);
    return 0;
}
