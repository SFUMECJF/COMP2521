// COMP2521 Assignment 1
// Written by Mehak Arya
// zID: z5259731

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <assert.h>

#include "invertedIndex.h"
#include "invertedIndexBST.h"
#include "fileList.h"
#include "linkedList.h"
#include "tfidfList.h"

///////////////////////////////////////////////////////////////////////////////

#define MAX_LENGTH 100

///////////////////////////////////////////////////////////////////////////////

static char *removeLeadingSpace (char *str);
static char *removeTrailingSpace (char *str);
static void addFrequency (FileList l);
static void calculateTf (InvertedIndexBST InvertedIndex, char *fileName,
                       double totalWords);
static void printIndex (InvertedIndexBST InvertedIndex, FILE *file);

///////////////////////////////////////////////////////////////////////////////
//                            PART 1 FUNCTIONS                               //
///////////////////////////////////////////////////////////////////////////////

/**
 * Normalises a given string. See the spec for details. Note: you should
 * modify the given string - do not create a copy of it.
 */
char *normaliseWord (char *str) {
    
    // remove leading spaces (if any)
    while (str[0] == ' ') {
        str = removeLeadingSpace(str);
    }

    // remove trailing spaces
    int length = strlen(str);
    while (str[length - 1] == ' ') {
        str = removeTrailingSpace(str);
        length = strlen(str);
    }

    // convert all characters to lower case
    length = strlen(str);
    for (int i = 0; i < length; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            str[i] = tolower(str[i]);
        }
    }

    // remove these punctuation marks at the end of a word: . , ; ?
    length = strlen(str);
    if (str[length - 1] == '.' || str[length - 1] == ',' ||
        str[length - 1] == ';' || str[length - 1] == '?') {

        str[length - 1] = '\0';
    }

    return str;
}

/**
 * This function opens the collection file with the given name, and then
 * generates an inverted index from those files listed in the collection
 * file, as discussed in the spec. It returns the generated inverted
 * index.
 */
InvertedIndexBST generateInvertedIndex(char *collectionFilename) {

    InvertedIndexBST InvertedIndex = newBST();
    
    // open the collection file with the given name
    FILE *file = fopen(collectionFilename, "r");

    char fileName[MAX_LENGTH];

    // store the individual file names inside a linked list
    List files = createList();
    assert(files != NULL);

    int result = fscanf(file, "%s", fileName);
    while (result > 0) {

        insertNode(files, fileName);
        result = fscanf(file, "%s", fileName);

    }

    fclose(file);

    // open the individual file
    struct node *currNode = files->head;
    while (currNode != NULL) {

        strcpy(fileName, currNode->filename);
        file = fopen(fileName, "r");

        double totalWords = 0;
        char scanWord[MAX_LENGTH];
        char normWord[MAX_LENGTH];

        // scan the words inside the given file
        result = fscanf(file, "%s", scanWord);
        while (result > 0) {

            totalWords++;
            strcpy(normWord, normaliseWord(scanWord));

            // printf("%s\n", normWord);

            // search for the word in the tree in case it already exists
            InvertedIndexBST currWord = searchBST(InvertedIndex, normWord);

            // if the word does not already exist in the BST then insert it
            if (currWord == NULL) {

                InvertedIndex = insertBST(InvertedIndex, normWord, fileName);

            }

            // else, if the word already exists in the BST then...
            else {

                // search for list of occurrences for the given word
                FileList wordFileList = findFileListNode(currWord->fileList,
                                                         fileName);
                
                // if there is no occurrence for this file then insert it
                if (wordFileList == NULL) {

                    insertFileListNode(fileName, currWord);
                }

                // else, if there is an occurrence then increase the frequency
                else {

                    addFrequency(wordFileList);
                }

            }
            
            result = fscanf(file, "%s", scanWord);
        }

        // close the file
        fclose(file);

        // compute the TF values for each word
        calculateTf(InvertedIndex, fileName, totalWords);

        // repeat for the next file
        currNode = currNode->next;
    }

    return InvertedIndex;
}

/**
 * Outputs the given inverted index to a file named invertedIndex.txt.
 * The output should contain one line per word, with the words ordered
 * alphabetically in ascending order. Each list of filenames for a word
 * should be ordered alphabetically in ascending order.
*/
void printInvertedIndex(InvertedIndexBST tree) {

    // creates a invertedIndex.txt and writes to it
    FILE *file = fopen("invertedIndex.txt", "w");

    // if the BST node is empty
    if (tree == NULL) {

        return;
    }

    printIndex(tree, file);

    fclose(file);    

    return;
}

///////////////////////////////////////////////////////////////////////////////
//                            PART 2 FUNCTIONS                               //
///////////////////////////////////////////////////////////////////////////////

/**
 * Returns  an  ordered list where each node contains a filename and the 
 * corresponding tf-idf value for a given searchWord. You only  need  to
 * include documents (files) that contain the given searchWord. The list
 * must  be  in  descending order of tf-idf value. If there are multiple
 * files with same tf-idf, order them by  their  filename  in  ascending
 * order. D is the total number of documents in the collection.
 */
TfIdfList calculateTfIdf(InvertedIndexBST tree, char *searchWord, int D) {

    assert(tree != NULL);

    // find the word in the tree and count how many files it appears in
    InvertedIndexBST foundWord = searchBST(tree, searchWord);
    FileList wordList = foundWord->fileList;

    double files = 0;
    while (wordList != NULL) {

        files++;
        wordList = wordList->next;
    }

    // calculate idf
    double idf = log10(D / files);

    // for each file list, calculate the tf-idf value and insert to tfidf list
    double tf;
    double tf_idf;

    TfIdfList newTfIdf = createTfIdfList();

    FileList currList = foundWord->fileList;
    while (currList != NULL) {

        tf = currList->tf;
        tf_idf = tf * idf;
        newTfIdf = insertTfIdfList(newTfIdf, currList->filename, tf_idf);
        currList = currList->next;
    }

    return newTfIdf;
}

/**
 * Returns  an  ordered list where each node contains a filename and the
 * summation of tf-idf values of all the matching search words for  that
 * file.  You only need to include documents (files) that contain one or
 * more of the given search words. The list must be in descending  order
 * of summation of tf-idf values (tfIdfSum). If there are multiple files
 * with  the  same tf-idf sum, order them by their filename in ascending
 * order. D is the total number of documents in the collection.
 */
TfIdfList retrieve(InvertedIndexBST tree, char *searchWords[], int D) {

    if (searchWords[0] == NULL) {

        return NULL;
    }

    TfIdfList retrievedList = createTfIdfList();


    int index = 0;
    while (searchWords[index] != NULL) {

        TfIdfList wordTfIdfList = calculateTfIdf(tree, searchWords[index], D);

        TfIdfList currList = wordTfIdfList;

        // traverse word tf-idf list
        while (currList != NULL) {

            // insert function will insert in order and sum the tf-idf values
            retrievedList = insertTfIdfList(retrievedList, currList->filename,
                                            currList->tfIdfSum);
            currList = currList->next;
        }

        index++;
    }

    return retrievedList;
}

///////////////////////////////////////////////////////////////////////////////
//                            HELPER FUNCTIONS                               //
///////////////////////////////////////////////////////////////////////////////

/**
 * This function is called when there is a leading space. This function
 * will move all the characters along to remove the leading space. 
 */
static char *removeLeadingSpace (char *str) {

    int length = strlen(str);
    for (int i = 0; i < (length - 1); i++) {
        str[i] = str[i + 1];
    }

    return str;
}

/**
 * This function is called when there is a trailing space. This function
 * will replace the trailing space with a '\0' to mark the end of the string.
 */
static char *removeTrailingSpace (char *str) {

    int length = strlen(str);
    str[length - 1] = '\0';

    return str;
}

/**
 * This function is called when the scanned word is already exists inside the 
 * tree for the current file. It will enter the file list and increase the 
 * frequency by one.
 */
static void addFrequency (FileList l) {

    l->tf = l->tf + 1;
    return;
}

/**  
 * This function is called once all the words in a given file have been 
 * scanned and the totalWords has been computed, so that the TF for each word
 * can be calculated.
 */
static void calculateTf (InvertedIndexBST InvertedIndex, char *fileName,
                       double totalWords) {

    // if the tree is empty, no calculation required
    if (InvertedIndex == NULL) {
        return;
    }

    calculateTf(InvertedIndex->right, fileName, totalWords);

    FileList currList = findFileListNode(InvertedIndex->fileList, fileName);

    if (currList != NULL) {
        currList->tf = currList->tf / totalWords;
    }

    calculateTf(InvertedIndex->left, fileName, totalWords);
    
    return;
}

/**
 * This function is called to print a node of the Inverted Index BST
 * in order LNR (alphabetical).
 */
static void printIndex (InvertedIndexBST InvertedIndex, FILE *file) {

    if (InvertedIndex == NULL) {

        return;
    }

    // L
    printIndex(InvertedIndex->left, file);

    // N

    // print word
    fprintf(file, "%s ", InvertedIndex->word);

    // print filename and tf value
    printFileList(InvertedIndex->fileList, file);
    
    // R
    printIndex(InvertedIndex->right, file);

    return;
}

