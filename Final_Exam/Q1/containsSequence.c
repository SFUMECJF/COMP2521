// COMP2521 Final Exam
// Question 1
// zID: z5259731

// containsSequence.c ... implementation of containsSequence function

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "list.h"

int listLength(List l);
int compareValue (Node *node1, Node *node2);

///////////////////////////////////////////////////////////////////////////////

// returns 1 if seq2 is contained in seq1 in the same order
int containsSequence (List seq1, List seq2) {

	// case 1: empty seq1
	if (seq1->first == NULL) {
		return 0;
	}

	// case 2: empty seq2
	if (seq2->first == NULL) {
		return 1;
	}

	// case 3: seq2 is larger than seq1
	if (listLength(seq2) > listLength(seq1)) {
		return 0;
	}

	Node *curr1 = seq1->first;
	Node *curr2 = seq2->first;

	// traverse through seq2
	while (curr2 != NULL) {

		// if we have reached the end of seq1 but not the end of seq2
		if (curr1 == NULL) {
			return 0;
		}

		// if a value matches, move along seq2
		if (compareValue(curr1, curr2)) {
			curr2 = curr2->next;
		}

		// move along seq1
		curr1 = curr1->next;
	}

	return 1; 
}

// returns 1 if the nodes contain the same value
int compareValue (Node *node1, Node *node2) {

	if (node1->value == node2->value) {
		return 1;
	}

	return 0;
}

// return the length of the list
int listLength (List l) {

	int length = 0;

	Node *curr = l->first;
	while (curr != NULL) {
		length++;
		curr = curr->next;
	}

	return length;
}
