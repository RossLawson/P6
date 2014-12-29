/****************************************************************************
 * dictionary.c
 * Computer Science 50
 * Problem Set 6
 * Implements a dictionary's functionality.
 ***************************************************************************/
#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

#define LENGTH 45
#define CAPACITY 26

//instantiate dictionary struct
typedef struct node {
    char word[LENGTH + 1];
    struct node* next;
} node;

//hash function
unsigned int hash_func(char* hash_key){
    int hash_func = tolower(hash_key[0]) - 'a';
    return hash_func % CAPACITY;}

//instantiate hash table
node* hashtable[CAPACITY];

// initialize size of dictionary
int dict_counter = 0;
int* dict_counter_ptr = &dict_counter;

/**Return true if word found in dictionary - else return false**/
bool check(const char* word){
    
    bool found = false;
    
    char* wordchecker = malloc(strlen(word)+1*sizeof(char)); //copy word to lower
    
    if (wordchecker == NULL) //error check
        return false; 
        
    strcpy(wordchecker, word);
        
    
    int bucket = hash_func(wordchecker); //hash word with hash_func
    
    //check list bucket
    if (hashtable[bucket] == NULL)
        found = false; //word not found
    else
    {
        //iterate over linked list to NULL 
        node* cursor = hashtable[bucket];
        while (cursor != NULL)
        {
            //compare the word to each of dictionary words 
            if (strcasecmp(wordchecker, cursor->word) == 0)
                found = true; //word found
            
            cursor = cursor->next; //go to next node
        }
    }
    free(wordchecker);
    return found;
}

/**Loads dictionary into memory.  Returns true if successful else false.**/
bool load(const char* dictionary)
{
    // propogate hashtable with NULL in pointer field
    for (int z = 0; z < CAPACITY; z++)
        hashtable[z] = NULL;
    
    //open dictionary file
    FILE* DFile = fopen(dictionary, "r");
    
    if (DFile == NULL)
    {
        fclose(DFile);
        return false;
    }
    
    //hash bucket_index
    int bucket_index;
    while (true)
    {
        // malloc space for new_node
        node* new_node = malloc(sizeof(node));
        
        //check for NULL
        if (new_node == NULL)
        {
            free(new_node);
            return false;
        }
        
        //read word from dictionary, store in new_node
        if (fscanf(DFile, "%s ", new_node->word) != EOF)
        {
            //hash scanned word
            bucket_index = hash_func(new_node->word);
            
            if (hashtable[bucket_index] == NULL)
            {
                //bucket devoid of word, point to new_node
                hashtable[bucket_index] = new_node;
                new_node->next = NULL;
                
                //increase dictionary size
                *dict_counter_ptr = *dict_counter_ptr + 1;
            }
            else //bucket contains words, insert new_node at front
            {
                new_node->next = hashtable[bucket_index];
                hashtable[bucket_index] = new_node;
                *dict_counter_ptr = *dict_counter_ptr + 1;
            }
        }
        else
        {
            free(new_node);
            break;
        }  
    }

    //close file
    fclose(DFile);
    return true;
}

/**
 * Returns number of words in dictionary if loaded.
 */

unsigned int size(void){
    // return value of dict_counter via ptr
    return *dict_counter_ptr;
}

/**Unloads dictionary from memory**/

bool unload(void){
    // iterate over buckets,free memory of nodes
    for (int x = 0; x < CAPACITY; x++)
    {
        //if bucket linked-list contains words
        if (hashtable[x] != NULL)
        {
            node* cursor = hashtable[x]; //track words
            
            //iterate over bucket's linked-list
            while (cursor != NULL)
            {
                node* temp = cursor;
                cursor = cursor->next;
                free(temp);
            }
        }          
    }
    return true;
}
