/*
dictionary.c

Computer Science 50
Problem Set 5

Implements a dictionary's functionality.
*/

// while (char nextchar = fgetc(dictionary) != NULL)

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>

#include "dictionary.h"

typedef struct node {
    bool is_word;
    struct node* children[27];
}
node;

node* root;
node* curnode;
int dictwords = 0;
uint8_t loc;

/*
Returns true if word is in dictionary else false.
*/
bool check(const char* word) {
    curnode = root;
    for (int i = 0; i < strlen(word); i++) {
        loc = isalpha(word[i])?toupper(word[i]) - 65:26;
        //printf("Current char: %c\n", word[i]);
        if (curnode->children[loc] == NULL) 
            //printf("Loc %i doesn't exist. Terminating...\n\n", loc);
            return false;
        //printf("Loc %i exists! Going there...\n\n", loc);
        curnode = curnode->children[loc];
    }
    //if (curnode->is_word == true) printf("\"%s\" is a word!\n\n\n", word);
    return curnode->is_word;
}

/*
Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary) {
    FILE* dictionaryp = fopen(dictionary, "r");
    if (!isalpha(fgetc(dictionaryp))) return false;
    fseek(dictionaryp, -1, SEEK_CUR);
    root = calloc(1, sizeof(node));
    curnode = root;
    char curchar;
    for (curchar = fgetc(dictionaryp); curchar != EOF; curchar = fgetc(dictionaryp)) {
        if (curchar == LINEFEED) {
            dictwords++;
            curnode->is_word = true;
            curnode = root;
        }
        else if (isalpha(curchar) || curchar == '\'') {
            loc = isalpha(curchar)?toupper(curchar) - 65:26;
            if (curnode->children[loc] == NULL)
                curnode->children[loc] = calloc(1, sizeof(node));
            curnode = curnode->children[loc];
        }
        else return false;
    }
    // Checks for last word
    fseek(dictionaryp, -1, SEEK_CUR);
    if (isalpha(fgetc(dictionaryp))) {
        dictwords++;
        curnode->is_word = true;
    }
    fclose(dictionaryp);
    return true;
}

/*
Recursive unload
*/
bool recur_unload(node* node) {
    for (int i = 0; i < 27; i++)
        if (node->children[i] != NULL)
            recur_unload(node->children[i]);
    free(node);
    return true;
}