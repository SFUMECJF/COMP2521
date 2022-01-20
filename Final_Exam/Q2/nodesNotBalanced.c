// COMP2521 Final Exam
// Question 2
// zID: z5259731

// nodesNotBalanced.c ... implementation of nodesNotBalanced function

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "BSTree.h"

int calculateHeight(BSTree n);
int max (int a, int b);
int isBalanced (BSTree n);
int unbalancedNodes (BSTree n);

// REFERENCES
// calculateHeight was written (by me) for COMP2521 Lab 5
// max was taken from the starter code for COMP2521 Lab 5

///////////////////////////////////////////////////////////////////////////////

// returns the number of nodes in t that are not height balanced
int nodesNotBalanced(BSTree t) {

    return unbalancedNodes(t);
}

// calculates the height of a given node
int calculateHeight(BSTree n) {

    if (n == NULL) {
        return -1;
    }
    
    else {
        
        return max(calculateHeight(n->right), calculateHeight(n->left)) + 1;
    }
}

// this function was taken from the starter code for COMP2521 Lab 5
int max (int a, int b) {

    return a > b ? a : b;
}

// returns 0 if the left and right sub-tree of the node are balanced
int isBalanced (BSTree n) {

    if (n == NULL) {

        return 0;
    }

    int heightL = calculateHeight(n->left);
    int heightR = calculateHeight(n->right);

    // if not balanced
    if (abs(heightL - heightR) > 1) {

        return 1;
    }
    
    // if balanced
    else {

        return 0;
    }
}

// traverse the tree and total the output from the isBalanced function
int unbalancedNodes (BSTree n) {

    if (n == NULL) {
        return 0;
    }

    return isBalanced(n) + unbalancedNodes(n->left) + unbalancedNodes(n->right);
}