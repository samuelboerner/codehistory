#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <cs50.h>

int main(int argc, string argv[]) {
    if (argc != 2) {
        printf("Must have 1 user argument, try again\n");
        return 1; 
    }
    for (int i = 0; i < strlen(argv[1]); i++)
        if (!isalpha(argv[1][i])) {
            printf("Keyword must only contain characters, try again\n");
            return 1;
     }
    string userinput;
    do
        userinput = GetString();
    while (strlen(userinput) <= 0);
    int i = 0;
    int keyword_num_index = 0;
    while (userinput[i] != 0) {
        if (isalpha(userinput[i])) {
            userinput[i] = ((((userinput[i] - (islower(userinput[i]) ? 97: 65)) + argv[1][keyword_num_index] - (islower(argv[1][keyword_num_index]) ? 97: 65)) % 26) + (islower(userinput[i]) ? 97: 65));
            keyword_num_index = (keyword_num_index + 1) % strlen(argv[1]);
        }
        i++;
    }
    printf("%s\n", userinput);
    return 0;
}
/* 
int main(int argc, string argv[]) {
    if (argc != 2) {
        printf("Must have 1 word of input, try again\n");
        return 1; }
    for (int i = 0; i < strlen(argv[1]); i++)
        if (!isalpha(argv[1][i])) {
            printf("Keyword must only contain characters, try again\n");
            return 1; }
    string userinput;
    do
        userinput = GetString();
    while (strlen(userinput) <= 0);
    int i = 0;
    int keyword_num_index = 0;
    while (userinput[i] != 0) {
        if ((islower(userinput[i]) && islower(argv[1][keyword_num_index]))) {
            userinput[i] = ((((userinput[i] - 97) + argv[1][keyword_num_index] - 97) % 26) + 97);
        }
        else if ((islower(userinput[i]) && isupper(argv[1][keyword_num_index]))) {
            userinput[i] = ((((userinput[i] - 97) + argv[1][keyword_num_index] - 65) % 26) + 97);
        }
        else if ((isupper(userinput[i]) && isupper(argv[1][keyword_num_index]))) {
            userinput[i] = ((((userinput[i] - 65) + argv[1][keyword_num_index] - 65) % 26) + 65);
        }
        else if ((isupper(userinput[i]) && islower(argv[1][keyword_num_index]))) {
            userinput[i] = ((((userinput[i] - 65) + argv[1][keyword_num_index] - 97) % 26) + 65);
        }
        if (isalpha(userinput[i])) {
            keyword_num_index = (keyword_num_index + 1) % strlen(argv[1]);
        }
        i++;
    }
    printf("%s\n", userinput);
    return 0;
}
*/
