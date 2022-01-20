// Interface for Linked List for tf-idf List

// Linked List functions written by UNSW for COMP1511
// Modified by Mehak Arya for COMP2521 Assignment 01

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "tfidfList.h"
#include "invertedIndex.h"

#define MAX_LENGTH 100

// create a TfIdf List
TfIdfList createTfIdfList (void) {

    return NULL;
}

// create a TfIdf list node
TfIdfList createTfIdfNode (char *fileName, double tf_idf) {

    TfIdfList newTfIdfList = malloc(sizeof(struct TfIdfNode));
    assert(newTfIdfList != NULL);

    newTfIdfList->filename = malloc(sizeof(char) * MAX_LENGTH);
    strcpy(newTfIdfList->filename, fileName);

    newTfIdfList->tfIdfSum = tf_idf;
    newTfIdfList->next = NULL;

    return newTfIdfList;
}

// insert a TfIdf list node
TfIdfList insertTfIdfList (TfIdfList l, char *fileName, double tf_idf) {

    TfIdfList newTfIdfList = createTfIdfNode(fileName, tf_idf);

    // if the list is empty
    if (l == NULL) {
        l = newTfIdfList;
        return l;
    }

    if (checkDuplicate(l, tf_idf, fileName)) {

        return l;
    }

    // if inserting at the head

    // case 1
    if (newTfIdfList->tfIdfSum > l->tfIdfSum) {
        newTfIdfList->next = l;
        l = newTfIdfList;
        return l;
    }

    // case 2
    else if (newTfIdfList->tfIdfSum == l->tfIdfSum &&
             strcmp(newTfIdfList->filename, l->filename) < 0) {

        newTfIdfList->next = l;
        l = newTfIdfList;
        return l;
    }

    // else, search for place of insertion

    TfIdfList currTfIdfList = l;
    TfIdfList prevTfIdfList = NULL;

    while (currTfIdfList != NULL) {

        // insert after prev and before curr
        if (newTfIdfList->tfIdfSum > currTfIdfList->tfIdfSum) {
            prevTfIdfList->next = newTfIdfList;
            newTfIdfList->next = currTfIdfList;
            return l;
        }

        // if tf-idf values are equal, insert in alphabetical order (filename)
        else if (newTfIdfList->tfIdfSum == currTfIdfList->tfIdfSum) {

            if (strcmp(newTfIdfList->filename, currTfIdfList->filename) < 0) {
                prevTfIdfList->next = newTfIdfList;
                newTfIdfList->next = currTfIdfList;
                return l;
            }

        }

        prevTfIdfList = currTfIdfList;
        currTfIdfList = currTfIdfList->next;
    }

    // if the new node is inserted at the end of the list
    if (currTfIdfList == NULL) {

        prevTfIdfList->next = newTfIdfList;
    }

    return l;
}

// checks if the filename already exists in the list and then sums the 
// tf-idf values
int checkDuplicate (TfIdfList l, double tf_idf, char *fileName) {

    TfIdfList currTfIdfList = l;
    TfIdfList prevTfIdfList = NULL;

    // if the head of the list is duplicated
    if (strcmp(l->filename, fileName) == 0) {

        l->tfIdfSum += tf_idf;
        return 1;
    }

    while (currTfIdfList != NULL) {

        if (strcmp(currTfIdfList->filename, fileName) == 0) {

            currTfIdfList->tfIdfSum += tf_idf;

            // remove the current
            prevTfIdfList->next = currTfIdfList->next;

            // re-insert it
            insertTfIdfList(l, currTfIdfList->filename,
                            currTfIdfList->tfIdfSum);
            return 1;
        }

        prevTfIdfList = currTfIdfList;
        currTfIdfList = currTfIdfList->next;
    }

    return 0;
}