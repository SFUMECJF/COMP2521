// Implementation for Linked List containing File Nodes

// Linked List functions written by UNSW for COMP1511
// Modified by Mehak Arya for COMP2521 Assignment 01

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "invertedIndex.h"
#include "fileList.h"

#define MAX_LENGTH 100

FileList newFileList (void) {

    FileList newList = malloc(sizeof(struct FileListNode));

    return newList;
}

// creates a new empty fileList node with the given fileName
FileList newFileListNode (char *fileName) {
    
    // create the node in memory
    FileList newNode = malloc(sizeof(struct FileListNode));
    assert(newNode != NULL);

    // set the fields of the node
    newNode->filename = malloc(sizeof(char) * MAX_LENGTH);
    strcpy(newNode->filename, fileName);

    newNode->tf = 1;
    newNode->next = NULL;

    // return the new node
    return newNode;
}

// creates a new fileList node and inserts it alphabetically into the list
void insertFileListNode (char *fileName, InvertedIndexBST word) {

    FileList newNode = newFileListNode(fileName);

    if (word->fileList == NULL) {
        word->fileList = newFileList();
        word->fileList = newNode;
        return;
    }

    FileList currList = word->fileList;
    FileList prevList = NULL;

    while (currList != NULL &&
           strcmp(newNode->filename, currList->filename) > 0) {
               
        prevList = currList;
        currList = currList->next;
    }

    // if start of list
    if (currList == word->fileList) {
        FileList temp = word->fileList;
        word->fileList = newNode;
        newNode->next = temp;
    }
    
    // else
    else {
        newNode->next = currList;
        prevList->next = newNode;
    }

    return;
}

// finds the fileList node with the given fileName and returns the node
FileList findFileListNode (FileList l, char *fileName) {

    FileList currList = l;

    // printFileList(l, stdout);

    // traverse through the list until end of list or we reached the node
    while (currList != NULL && strcmp(currList->filename, fileName) != 0) {

        currList = currList->next;
    }

    // returns NULL if a fileList for that file doesn't exist
    // else, returns the node

    return currList;
}

// prints the fileList to the given stream
void printFileList (FileList l, FILE *output) {

    FileList currList = l;

    // if the list is empty, do nothing
    if (l == NULL) {

        return;
    }
    
    // else, print the fileList
    while (currList != NULL) {

        fprintf(output, "%s (%f) ", currList->filename, currList->tf);
        currList = currList->next;

    }

    fprintf(output, "\n");

    return;
}