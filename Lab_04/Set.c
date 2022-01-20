// Set ADT implementation using a binary search tree

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Set.h"

typedef struct node *Node;
struct node {
	int  elem;
	Node left;
	Node right;
};

struct set {
	Node root;
	int  size;
};

static void doSetFree(Node n);
static Node doSetAdd(Set s, Node n, int elem);
static Node newNode(int elem);
static void doSetPrint(Node n);

/**
 * Creates a new empty set
 */
Set  SetNew(void) {
	Set s = malloc(sizeof(Set));
	if (s == NULL) {
		fprintf(stderr, "couldn't allocate set\n");
		exit(EXIT_FAILURE);
	}
	
	return s;
}

/**
 * Frees all resources associated with the given set
 */
void SetFree(Set s) {
	doSetFree(s->root);
	free(s);
}

static void doSetFree(Node n) {

	if (n->left != NULL) {
		doSetFree(n->left);
	}
	if (n->right != NULL) {
		doSetFree(n->right);
	}
	free(n);
}

/**
 * Adds an element to the given set
 * Does not insert duplicates
 */
void SetAdd(Set s, int elem) {

	s->root = doSetAdd(s, s->root, elem);

	s->size++;
	return;

}

static Node doSetAdd(Set s, Node n, int elem) {

	if (n == NULL) {
		n = newNode(elem);
		return n;
	}
	
	// if elem is less than the current node, then move left
	if (elem < n->elem) {
		n->left = doSetAdd(s, n->left, elem);
	}
	// if elem is more than the current node, then move right
	else if (elem > n->elem) {
		n->right = doSetAdd(s, n->right, elem);
	}
	// else, if elem already exists in the tree i.e. duplicate
	else {
		s->size--;
	}

	return n;
}

static Node newNode(int elem) {
	Node n = malloc(sizeof(struct node));
	if (n == NULL) {
		fprintf(stderr, "couldn't allocate node\n");
		exit(EXIT_FAILURE);
	}
	
	n->left = NULL;
	n->right = NULL;
	n->elem = elem;
	return n;
}

/**
 * Returns true if the given element is contained in the set, and false
 * otherwise
 */
bool SetContains(Set s, int elem) {

	// searching for the int elem inside the set s
	Node n = s->root;
	
	// iterative search
	while (n != NULL) {

		int current = n->elem;

		if (elem == current) {
			return true;
		}
		else if (elem < current) {
			n = n->left;
		}
		else if (elem > current) {
			n = n->right;
		}

	}
	
	return false; // couldn't find element
}

/**
 * Returns the size (number of elements) of the given set
 */
int  SetSize(Set s) {
	return s->size;
}

/**
 * Prints the set to stdout
 */
static bool firstElem = true;

void SetPrint(Set s) {
	printf("{");
	
	firstElem = true;
	doSetPrint(s->root);
	
	printf("}\n");
}

static void doSetPrint(Node n) {
	if (n == NULL) return;
	
	doSetPrint(n->left);
	
	// print comma before each element except the first
	if (!firstElem) {
		printf(", ");
	}
	printf("%d", n->elem);
	firstElem = false;
	
	doSetPrint(n->right);
}

