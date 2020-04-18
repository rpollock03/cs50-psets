// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>

#include "dictionary.h"

int words = 0; //variable to keep track of number of words loaded

//Represents number of linked lists or 'buckets' in a hash table
//REMEMBER, hashtable is simply an array of linked lists. Each element in array contains POINTER to the first node of the linked list.
#define N 26 // for each letter in alphabet

// HASHTAG NODE DEFINITION
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node; //variable of type node contains a word and a pointer

// Hashtable definition declaration. pointer to array of 26 buckets ie linked lists
node *hashtable[N]; // N is defined as 26 ie for each letter in alphabet.

// Hashes word to a number between 0 and 25, inclusive, based on its first letter
unsigned int hash(const char *word)
{
    return tolower(word[0]) - 'a'; // a ascii code is 97 basically converting letters to numbers 1-26
}

// LOAD DICTIONARY INTO HASHTABLE
bool load(const char *dictionary)
{
    // Initialize hash table - all buckets/pointers to NULL
    for (int i = 0; i < N; i++) //for each bucket in the list
    {
        hashtable[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Create buffer for a word
    char word[LENGTH + 1];

    // Insert words into hash table
    while (fscanf(file, "%s", word) != EOF)//read the dictionary, store each new word into buffer called word
    {
        //First step, create space for each new node/word with malloc.
        node *new_node = malloc(sizeof(node)); //Malloc returns POINTER to newly created spot of memory for the new node
        //we must always check there's enough memory for the new node.
        if (new_node == NULL)
        {
            unload();
            return false;
        }
        //Now we assign the values to the node. As defined by struct above, node contains value (word from dictionary) and 'next' which is pointer to next node in list
        //Use arrow notation to change the value within the node to the word stored in buffer called word
        strcpy(new_node->word, word); //copy word in buffer (2nd) into the word value contained within newly created node(1st)

        //We can't assign 'next' node without knowing which list to insert new node into. For that we use hash function
        int hashTableIndex = hash(word); //returns index of linked list ie 0-25 (for each letter of alphabet)
        //IMPORTANT - we can't simply change pointer at hashTableIndex to the newNode otherwise we would lose rest of the list
        //so we must first set the newly created node to point at the first item in the linked list.
        new_node->next = hashtable[hashTableIndex]; //pointer to first node in list assigned to 'next' node value of new node
        hashtable[hashTableIndex] = new_node; //now we can make hashtable 'head' element point at the new_node
        //new_node is now the first element/node in the linked list at this index
        words++; //we increment the words variable to keep track of number of words loaded
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return words; //we incremented the words variable as dictionary loaded
}

// Passing in word and returning true if word is in dictionary
bool check(const char *word)
{
//we create a new cursor pointer to search through the hashtable for word.
//we do not need to malloc because this is ONLY a pointer, not creating new node
    node *cursor = hashtable[hash(word)]; //returns pointer of first element of appropriate linked list

    while (cursor != NULL)//search through linked list until end
    {
        if (strcasecmp(word, cursor->word) == 0) //compare word passed into function with word in node. strcasecmp is case insensitive.
        {
            return true; //strcasecmp returns 0 if the same, so return true
        }
        else
        {
            cursor = cursor->next; //if not, move to 'next' element
        }
    }
    return false; //if gets this far, it is because cursor became null ie reached end of LL without finding a strcmp true
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
//If just called free on each list, it would free the head/first elements and the others would be lost
//so we need to maintain the links and free nodes individually
//We COULD remove nodes one at a time from the end of each list BUT that would be slower as would require
//traversing through list each time. Better to remove from the start using temporary pointer.

    for (int i = 0; i < N; i++) //as with intialise, we go through each letter/bucket/linked-list in turn
    {
        node *cursor = hashtable[i]; //point to first element of the linked list
        while (cursor != NULL) //do the following until end of list
        {
            node *temp = cursor; // create temporary pointer to hold onto element while cursor moves to next
            cursor = cursor->next; // move cursor to next element
            free(temp);//free the element pointed to by temp pointer
        }
    }
    return true;
}
