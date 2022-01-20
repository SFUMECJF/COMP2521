// Implementation for Linked List for Files

// Linked List functions written by UNSW for COMP1511
// Modified by Mehak Arya for COMP2521 Assignment 01

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "linkedList.h"

#define MAX_LENGTH 100

// create a new linked list
List createList (void) {

    List newList = malloc(sizeof(*newList));
    assert(newList != NULL);

    newList->head = NULL;
    return newList;
}

// create a new linked list node
struct node *createNode (char *fileName) {
    
    struct node *newNode = malloc(sizeof(*newNode));
    assert(newNode != NULL);

    strcpy(newNode->filename, fileName);

    newNode->next = NULL;

    return newNode;
}

// insert a new linked list node
void insertNode (List l, char *fileName) {

    struct node *newNode = createNode(fileName);

    if (l->head == NULL) {
        l->head = newNode;
        return;
    }

    // else
    struct node *currNode = l->head;
    struct node *prevNode = NULL;

    while (currNode != NULL) {
        prevNode = currNode;
        currNode = currNode->next;
    }

    prevNode->next = newNode;

    return;
}