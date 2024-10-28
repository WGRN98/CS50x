#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

int count = 0; // Number of nodes

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Hashing word in table
    int index = hash(word);

    // Go through list until NULL is found in next, with pointer at head of the table
    for (node *tmp = table[index]; tmp != NULL; tmp = tmp->next)
    {
        if (strcasecmp(tmp->word, word) == 0) // strcmp case insensitive comparison in strings.h
            return true;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Changed tolower from toupper, more lowercase words than uppercase
    // How can I optimize this further?
    return tolower(word[0]) - 'a';
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open the dictionary file
    FILE *source = fopen(dictionary, "r");
    if (source == NULL)
    {
        printf("Error reading dictionary file!\n");
        return false;
    }

    // Character array for the words
    char word[LENGTH + 1];

    // While file isn't on EOF print the words into the character array word
    while (fscanf(source, "%s", word) != EOF)
    {
        // Allocate memory for each new node
        node *new_node = malloc(sizeof(node));

        // Check for NULL node
        if (new_node == NULL)
            return 1;

        // Initialize the new node by copying the word to the next node.
        strcpy(new_node->word, word);
        new_node->next = NULL;

        // Add to hash table each element of the hashtable is a linked list
        int index = hash(word);

        // If the index is not yet assigned, put the newest node at the head of this index.
        if (table[index] == NULL)
            table[index] = new_node;

        // If the index is assigned point the existing head node at the table[index] and then make
        // the new node the head.
        else
        {
            // Makes the next the new head
            new_node->next = table[index];
            // Head points to the new node.
            table[index] = new_node;
        }

        // Increase counter of nodes being made
        count++;
    }

    // Close file and return true when successful
    fclose(source);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *ptr = table[i];

        while (ptr)
        {
            node *tmp = ptr;
            ptr = ptr->next;
            free(tmp);
        }
    }
    return true;
}
