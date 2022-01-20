// Simple program to test a sorting function

#include <stdio.h>
#include <stdlib.h>

#define N 10

static void sort(int[], int);
static void show(char *, int[], int);

int main(void)
{
	int i, j, a[N];

	srand(0);

	for (j = 1; j <= 5; j++) {

		// initialise array (pseudo-randomly)
		for (i = 0; i < N; i++) {
			a[i] = rand() % 100;
		}

		// display, sort, then re-display
		printf("Test #%d\n", j);
		show("Sorting", a, N);

		sort(a, N);
		show("Sorted ", a, N);

	}
	return 0;
}

// sort array using bubble sort
static void sort(int a[], int n)
{
	int i, j, nswaps;
	// this for loop runs n number of times where n is the num of numbers
	// being sorted
	for (i = 0; i < n; i++) {
		// number of swaps starts at 0
		nswaps = 0;
		// i starts at the start of the array and j starts at the end,
		// so j is being decremented* not incremented in each iteration
		for (j = n - 1; j > i; j--) {
			if (a[j] < a[j - 1]) {
				int tmp;
				tmp = a[j];
				a[j] = a[j - 1];
				a[j - 1] = tmp;
				nswaps++;
			}
		}
		// number of swaps is 0 if the array is sorted hence we return
		if (nswaps == 0)
			break;
	}
	return;
}

// display array, preceded by label
static void show(char *label, int a[], int n)
{
	int i;
	printf("%s:", label);
	for (i = 0; i < n; i++) {
		printf(" %02d", a[i]);
	}
	printf("\n");
}

