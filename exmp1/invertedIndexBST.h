// Interface for Binary Search Tree

// Linked List functions written by UNSW for COMP2521
// Modified by Mehak Arya for COMP2521 Assignment 01

#include "invertedIndex.h"

// create a new empty tree
InvertedIndexBST newBST (void);

// make a new node containing a word from a given file
InvertedIndexBST newBSTNode (char *word, char *fileName);

// free memory associated with Tree
void freeBST(InvertedIndexBST t);

// checks whether a word is already in the tree and returns the node if so
InvertedIndexBST searchBST(InvertedIndexBST t, char *word);

// insert a new word into the BST alphabetically and returns the root node of the tree
InvertedIndexBST insertBST(InvertedIndexBST t, char *word, char *fileName);

// print the words in alphabetical order
void printBST(InvertedIndexBST t);