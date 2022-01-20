// Interface for Linked List for tf-idf List

// Linked List functions written by UNSW for COMP1511
// Modified by Mehak Arya for COMP2521 Assignment 01

#include <stdio.h>
#include <stdlib.h>

#include "invertedIndex.h"

// create a TfIdf List
TfIdfList createTfIdfList (void);

// create a TfIdf list node
TfIdfList createTfIdfNode (char *fileName, double tf_idf);

// insert a TfIdf list node
TfIdfList insertTfIdfList (TfIdfList l, char *fileName, double tf_idf);

// checks if the filename already exists if so then sums the tf-idf values
int checkDuplicate (TfIdfList l, double tf_idf, char *fileName);