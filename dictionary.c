// Implements a dictionary's functionality

#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 5381;
unsigned int word_count = 0; // Number of words in dict

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{

    int index = hash(word); // Get hash index for word to check
    node *head = table[index]; // Get head node of linked list at that index
    while (head != NULL) // Traverse through linked list
    {
        if ((strcasecmp(head->word, word) == 0)) // Compare each input word to dict words
        {
            return true;
        }
        head = head->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{

    /* By researching through web */
    unsigned int hash = 5381;
    int c;

    while ((c = *word++))        // *word++ is going to the next address in memory, where the next char in the string is stored
    {
        if (isupper(c))
        {
            c = c + 32;
        }
        hash = ((hash << 5) + hash) + c; // hash * 33 + c   // hash << 5 = hash * 2^5
    }

    return hash % N;

}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{

    FILE *file = fopen(dictionary, "r"); // Opening dictionary
    if (file == NULL)
    {
        return false;
    }
    char buffer[LENGTH + 1]; // Defining buffer to store each string
    while ((fscanf(file, "%s", buffer)) != EOF)
    {
        node *n = malloc(sizeof(node)); // Creating node to store each word of dict
        if (n == NULL)
        {
            return false;
        }
        strcpy(n->word, buffer); // Copying word from buffer to node
        n->next = NULL;
        int index = hash(buffer); // Getting hash index for that word
        if (table[index] == NULL) // i.e If hash table is of size 0 or precisely first word at that index
        {
            table[index] = n;
        }
        else
        {
            // Inserting at beginnig of linked list
            n->next = table[index]; // Updating next first so we don't lose the head of linked list
            table[index] = n; // Updating head to n
        }
        word_count++;
    }
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{

    return word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{

    for (int i = 0; i < N; i++) // For all linked list from 0 to N ie 5891 linked lists
    {
        node *curr = table[i]; // Getting head node for each linked list every iteration
        while (curr != NULL) // Traversing through end of linked list
        {
            node *tmp = curr; // Storing curr node
            curr = curr->next; // Updating curr to curr->next so we can free curr
            free(tmp);
        }

        if (curr == NULL && i == N-1) // Checking if we reached at end of linked list and end of array
        {
            return true;
        }
    }

    return false;
}
