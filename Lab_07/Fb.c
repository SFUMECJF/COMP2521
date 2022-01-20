
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Fb.h"
#include "Map.h"
#include "Queue.h"

#define MAX_PEOPLE 128

struct fb {
    int   numPeople;

    char *names[MAX_PEOPLE]; // the id of a person is simply the index
                             // that contains their name in this array
    
    Map   nameToId; // maps names to ids
                    // question to think about: why do we have this when
                    // the names array already provides this information?

    bool  friends[MAX_PEOPLE][MAX_PEOPLE];
};

static char *myStrdup(char *s);
static int   nameToId(Fb fb, char *name);

////////////////////////////////////////////////////////////////////////

// Creates a new instance of FriendBook
Fb   FbNew(void) {
    Fb fb = malloc(sizeof(*fb));
    if (fb == NULL) {
        fprintf(stderr, "Insufficient memory!\n");
        exit(EXIT_FAILURE);
    }

    fb->numPeople = 0;
    fb->nameToId = MapNew();

    for (int i = 0; i < MAX_PEOPLE; i++) {
        for (int j = 0; j < MAX_PEOPLE; j++) {
            fb->friends[i][j] = false;
        }
    }

    return fb;
}

void FbFree(Fb fb) {
    for (int i = 0; i < fb->numPeople; i++) {
        free(fb->names[i]);
    }

    MapFree(fb->nameToId);
    free(fb);
}

bool FbAddPerson(Fb fb, char *name) {
    if (fb->numPeople == MAX_PEOPLE) {
        fprintf(stderr, "error: could not add more people\n");
        exit(EXIT_FAILURE);
    }

    if (!MapContains(fb->nameToId, name)) {
        int id = fb->numPeople++;
        fb->names[id] = myStrdup(name);
        MapSet(fb->nameToId, name, id);
        return true;
    } else {
        return false;
    }
}

bool FbHasPerson(Fb fb, char *name) {
    return MapContains(fb->nameToId, name);
}

List FbGetPeople(Fb fb) {
    List l = ListNew();
    for (int id = 0; id < fb->numPeople; id++) {
        ListAppend(l, fb->names[id]);
    }
    return l;
}

bool FbFriend(Fb fb, char *name1, char *name2) {
    int id1 = nameToId(fb, name1);
    int id2 = nameToId(fb, name2);
    assert(id1 != id2);

    if (!fb->friends[id1][id2]) {
        fb->friends[id1][id2] = true;
        fb->friends[id2][id1] = true;
        return true;
    } else {
        return false;
    }
}

bool FbUnfriend(Fb fb, char *name1, char *name2) {
    int id1 = nameToId(fb, name1);
    int id2 = nameToId(fb, name2);
    assert(id1 != id2);

    if (fb->friends[id1][id2]) {
        fb->friends[id1][id2] = false;
        fb->friends[id2][id1] = false;
        return true;
    } else {
        return false;
    }
}

bool FbIsFriend(Fb fb, char *name1, char *name2) {
    int id1 = nameToId(fb, name1);
    int id2 = nameToId(fb, name2);
    return fb->friends[id1][id2];
}

List FbGetFriends(Fb fb, char *name) {
    int id1 = nameToId(fb, name);
    
    List l = ListNew();
    for (int id2 = 0; id2 < fb->numPeople; id2++) {
        if (fb->friends[id1][id2]) {
            ListAppend(l, fb->names[id2]);
        }
    }
    return l;
}

int  FbNumFriends(Fb fb, char *name) {
    int id1 = nameToId(fb, name);
    
    int numFriends = 0;
    for (int id2 = 0; id2 < fb->numPeople; id2++) {
        if (fb->friends[id1][id2]) {
            numFriends++;
        }
    }
    return numFriends;
}

////////////////////////////////////////////////////////////////////////
// TO DO

// returns a list of all mutual friends for two given people
List FbMutualFriends(Fb fb, char *name1, char *name2) {
    
    // list to store mutual friends
    List l = ListNew();

    // check for mutuals
    int numPeople = fb->numPeople;

    for (int i = 0; i < numPeople; i++) {

        char *name3 = fb->names[i];
        if (FbIsFriend(fb, name1, name3) && FbIsFriend(fb, name2, name3)) {

            ListAppend(l, name3);
        }
    }

    return l;
}

// print friend recommendations for the given person only recommending
// people who are friends of friends of the person, in order of highest
// mutuals
void FbFriendRecs1(Fb fb, char *name) {

    // scan through everyone inside fb and use fbmutualfriends function
    // to calculate how many mutuals name shares with V
    // return V and number of mutuals in descending order

    int source = nameToId(fb, name);
    int maxMutuals = fb->numPeople - 2;

    printf("%s's friend recommendations\n", name);

    // print in order of highest mutuals to lowest mutuals
    for (int i = maxMutuals; i > 0; i--) {

        for (int j = 0; j < fb->numPeople; j++) {

            // exclude source and direct friends of the source
            if (j == source || FbIsFriend(fb, name, fb->names[j])) {
                continue;
            }

            // calculate the number of mutuals
            List mutuals = FbMutualFriends(fb, name, fb->names[j]);
            int numMutuals = ListSize(mutuals);

            // print in descending order, i starts at max mutuals and goes to 0
            if (numMutuals == i) {
                printf("\t%-20s%4d mutual friends\n", fb->names[j], numMutuals);
            }

        }

    }

    return;
}

// print friend recommendations for the given person recommending anyone
// that can be reached from the given person, in order of closeness (BFS)
void FbFriendRecs2(Fb fb, char *name) {
    
    printf("%s's friend recommendations\n", name);

    // create and initialize the visited array    
    int *visited = calloc(fb->numPeople, sizeof(int));
    for (int i = 0; i < fb->numPeople; i++) {
        visited[i] = -1;
    }

    // mark source as visited
    int source = nameToId(fb, name);
    visited[source] = 1;
    
    Queue q = QueueNew();
    QueueEnqueue(q, source);

    // while the queue is not empty
    while (QueueSize(q) != 0) {

        Item v = QueueDequeue(q);

        if (visited[v] == -1) {

            if (!fb->friends[source][v]) {

                visited[v] = 1;
                printf("\t%s\n", fb->names[v]);
            } 

        }

        for (int w = 0; w < fb->numPeople; w++) {

            if (fb->friends[v][w] && visited[w] == -1) {
                    QueueEnqueue(q, w);
            }

        }
        
    }

    QueueFree(q);
    free(visited);

    return;
}

////////////////////////////////////////////////////////////////////////
// Helper Functions

static char *myStrdup(char *s) {
    char *copy = malloc((strlen(s) + 1) * sizeof(char));
    if (copy == NULL) {
        fprintf(stderr, "Insufficient memory!\n");
        exit(EXIT_FAILURE);
    }
    return strcpy(copy, s);
}

// Converts a name to an ID. Raises an error if the name doesn't exist.
static int nameToId(Fb fb, char *name) {
    if (!MapContains(fb->nameToId, name)) {
        fprintf(stderr, "error: person '%s' does not exist!\n", name);
        exit(EXIT_FAILURE);
    }
    return MapGet(fb->nameToId, name);
}
