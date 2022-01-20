// Implementation for Binary Search Tree

// Linked List functions written by UNSW for COMP2521
// Modified by Mehak Arya for COMP2521 Assignment 01

// Binary Search Tree ADT implementation ... 

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "invertedIndex.h"
#include "invertedIndexBST.h"
#include "fileList.h"

#define MAX_LENGTH 100

// create a new empty tree
InvertedIndexBST newBST (void) {

    return NULL;
}

// make a new node containing a word from a given file
InvertedIndexBST newBSTNode (char *word, char *fileName) {

    InvertedIndexBST new = malloc(sizeof(struct InvertedIndexNode));
    assert(new != NULL);

    new->word = malloc(sizeof(char) * MAX_LENGTH);

    strcpy(new->word, word);

    new->right = NULL;
    new->left = NULL;
    new->fileList = newFileListNode(fileName);

    return new;
}

// free memory associated with Tree
void freeBST(InvertedIndexBST t) {
    
   if (t != NULL) {
      freeBST(t->left);
      freeBST(t->right);
      free(t);
   }

   return;
}

// checks whether a word is already in the tree and returns the node if so
InvertedIndexBST searchBST(InvertedIndexBST t, char *word) {

    if (t == NULL) {
        return NULL;
    }

    else if (strcmp(t->word, word) > 0) {
        return searchBST(t->left, word);
    }

    else if (strcmp(t->word, word) < 0) {
        return searchBST(t->right, word);
    }

    // if strcmp returns 0 and we reached the node containing the word
    else { 
        return t;
    }   

}

// insert a new word into the BST alphabetically
InvertedIndexBST insertBST(InvertedIndexBST t, char *word, char *fileName) {

    // base case: insert at the node t
    if (t == NULL) {
        return newBSTNode(word, fileName);
    }

    else if (strcmp(t->word, word) > 0) {
        t->left = insertBST(t->left, word, fileName);
    }

    else if (strcmp(t->word, word) < 0) {
        t->right = insertBST(t->right, word, fileName);
    }

    return t;
}

// print the words in alphabetical order
void printBST(InvertedIndexBST t) {

    if (t == NULL) {
        return;
    }

    // L
    printBST(t->left);

    // N
    printf("%s\n", t->word);

    // R
    printBST(t->right);

    return;
}