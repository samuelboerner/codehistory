#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <cs50.h>

int main(int argc, string argv[]) {
    if (argc != 2 || atoi(argv[1]) < 0) {
        printf("Hey dingus I only accept 1 positve number as input dont waste my time\n");
        return 1;
    }
    for (int i = 0; i < strlen(argv[1]); i++)
        if (isalpha(argv[1][i])) {
            printf("Hey dingus I only accept 1 positve number as input dont waste my time\n");
            return 1;
        }
    int shift = atoi(argv[1]);
    string userinput;
    do
        userinput = GetString();
    while (strlen(userinput) <= 0);
    int i = 0;
    while (userinput[i] != 0) {
        if ((userinput[i] >= 97) && (userinput[i] <= 122)) 
            userinput[i] = ((((userinput[i] - 97) + shift) % 26) + 97);
        else if ((userinput[i] >= 65 && (userinput[i] <= 90))) 
            userinput[i] = ((((userinput[i] - 65) + shift) % 26) + 65);
        printf("%c", userinput[i]);
        i++;
    }
    printf("\n");
    return 0;
}