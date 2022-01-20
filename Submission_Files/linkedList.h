// Interface for Linked List for Files

// Linked List functions written by UNSW for COMP1511
// Modified by Mehak Arya for COMP2521 Assignment 01

#include <stdio.h>
#include <stdlib.h>

#define MAX_LENGTH 100

typedef struct node {
	char filename[MAX_LENGTH];
	struct node *next;
} node;

typedef struct list {
    struct node *head;
} list;

typedef struct list *List;

// create a new linked list
List createList (void);

// create a new linked list node
struct node *createNode (char *fileName);

// insert a new linked list node
void insertNode (List l, char *fileName);