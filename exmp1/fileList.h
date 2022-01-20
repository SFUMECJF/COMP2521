// Implementation for Linked List containing File Nodes

// Linked List functions written by UNSW for COMP1511
// Modified by Mehak Arya for COMP2521 Assignment 01

#include "invertedIndex.h"

// creates a new fileList
FileList newFileList (void);

// creates a new empty fileList node
FileList newFileListNode (char *fileName);

// creates a new fileList node and inserts it after the given file name
void insertFileListNode (char *fileName, InvertedIndexBST word);

// finds the fileList node with the given fileName and returns the node
FileList findFileListNode (FileList l, char *fileName);

// prints the fileList to the given stream
void printFileList (FileList l, FILE *output);