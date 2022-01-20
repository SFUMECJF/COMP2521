// Schedule ADT implementation

#include <stdio.h>
#include <stdlib.h>

#include "Tree.h"
#include "Time.h"
#include "Schedule.h"

struct schedule {
    Tree times;
    int  count;
};

// Creates a new schedule
Schedule ScheduleNew(void) {
    Schedule s = malloc(sizeof(*s));
    if (s == NULL) {
        fprintf(stderr, "Insufficient memory!\n");
        exit(EXIT_FAILURE);
    }

    s->times = TreeNew();
    s->count = 0;
    return s;
}

// Frees all memory associated with a given schedule
void ScheduleFree(Schedule s) {
    TreeFree(s->times);
    free(s);
}

// Gets the number of times added to the schedule
int  ScheduleCount(Schedule s) {
    return s->count;
}

// Attempts to schedule a new landing time. Returns true if the time was
// successfully added, and false otherwise.
bool ScheduleAdd(Schedule s, Time t) {
    
    Tree tree = s->times;

    Time floor = TreeFloor(tree, t);
    Time ceiling = TreeCeiling(tree, t);

    Time timePlus = TimeAddMinutes(t, 10);
    Time timeMinus = TimeSubtractMinutes(t, 10);

    int cmp1, cmp2;

    // if the floor and ceiling both exist
    if (floor != NULL && ceiling != NULL) {

        cmp1 = TimeCmp(timeMinus, floor);
        cmp2 = TimeCmp(timePlus, ceiling);

        if (cmp1 <= 0 || cmp2 >= 0) {
            return false;
        }

    }

    // if only the floor exists
    else if (floor != NULL && ceiling == NULL) {
        cmp1 = TimeCmp(timeMinus, floor);

        if (cmp1 <= 0) {
            return false;
        }
    }

    // if only the ceiling exists
    else if (floor == NULL && ceiling != NULL) {
        cmp1 = TimeCmp(timePlus, ceiling);

        if (cmp1 >= 0) {
            return false;
        }
    }

    TreeInsert(tree, t);
    s->count++;
    return true;
}

// Shows  all  the landing times in the schedule. If mode is 1, only the
// times are shown. If mode is 2, the underlying data structure is shown
// as well.
void ScheduleShow(Schedule s, int mode) {

    if (mode == 1) {
        TreeList(s->times);
    } else if (mode == 2) {
        TreeShow(s->times);
    }

    return;
}
