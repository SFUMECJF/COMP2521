// Implementation of the undirected weighted graph ADT

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Graph.h"
#include "Queue.h"

// graph representation (adjacency matrix)
typedef struct GraphRep {
	int nV;      // #vertices
	int nE;      // #edges
	int **edges; // matrix of weights (0 == no edge)
} GraphRep;

static int validVertex(Graph g, Vertex v);
static int makePath (int length, int *temp, int *visited, Vertex src,
					 Vertex dest);
// static void printPath (int *path, int length);

////////////////////////////////////////////////////////////////////////

Graph GraphNew(int nV)
{
	assert(nV > 0);

	Graph new = malloc(sizeof(*new));
	assert(new != NULL);
	new->nV = nV;
	new->nE = 0;

	new->edges = calloc(nV, sizeof(int *));
	assert(new->edges != NULL);
	for (int v = 0; v < nV; v++) {
		new->edges[v] = calloc(nV, sizeof(int));
		assert(new->edges[v] != 0);
	}

	return new;
}

void GraphFree(Graph g)
{
	assert(g != NULL);
	for (int v = 0; v < g->nV; v++)
		free(g->edges[v]);
	free(g->edges);
	free(g);
}

////////////////////////////////////////////////////////////////////////

void GraphInsertEdge(Graph g, Vertex v, Vertex w, int weight)
{
	assert(g != NULL && validVertex(g, v) && validVertex(g, w));

	if (g->edges[v][w] != 0 && g->edges[w][v] != 0)
		return; // an edge already exists; do nothing.

	g->edges[v][w] = weight;
	g->edges[w][v] = weight;
	g->nE++;
}

void GraphRemoveEdge(Graph g, Vertex v, Vertex w)
{
	assert(g != NULL && validVertex(g, v) && validVertex(g, w));
	if (g->edges[v][w] == 0 && g->edges[w][v] == 0)
		return; // the edge doesn't exist; do nothing.

	g->edges[v][w] = 0;
	g->edges[w][v] = 0;
	g->nE--;
}

////////////////////////////////////////////////////////////////////////

/**
 * Finds  the  shortest path (in terms of the number of hops) from `src`
 * to `dest` such that no edge on the path has weight larger than `max`.
 * Stores  the  path  in the given `path` array including both `src` and
 * `dest` and returns the number of vertices stored in the  path  array.
 * Returns 0 if there is no such path.
 */
int findPath(Graph g, Vertex src, Vertex dest, int max, int *path)
{
	// breadth-first search
	assert(g != NULL);

	// indicates whether we have reached the destination or not
	int found = 0;

	// visited array
    int *visited = calloc(g->nV, sizeof(int));
	for (int i = 0; i < g->nV; i++) {
		visited[i] = -1;
	}
	// 'index' is visited from 'value'
	visited[src] = src;
 
	Queue q = QueueNew();
	QueueEnqueue(q, src);

	// BFS traversal
	while (!QueueIsEmpty(q)) {

		Vertex v = QueueDequeue(q);

		if (v == dest) {
			found = 1;
		} else {

			for (int w = 0; w < g->nV; w++) {

				if (g->edges[v][w] > 0 && g->edges[v][w] <= max &&
					visited[w] == -1) {

					visited[w] = v;
					QueueEnqueue(q, w);
				}
			}
		}
	}

	// if a path was found
	if (found) {

		// store path in temp array from dest to source
		int *temp = calloc(g->nV, sizeof(int));
		int length = makePath(0, temp, visited, src, dest);

		// reverse temp array and store in path array
		int i, j;
		for (i = length - 1, j = 0; i >= 0; i--, j++) {

			path[j] = temp[i];
		}
		free(temp);
		return length;
	}

	// if no path was found
	free(visited);
	return 0;
}

// helper function to store the path from dest to source inside of an array
static int makePath (int length, int *temp, int *visited, Vertex src,
					 Vertex dest) {

	// add current 'dest' to the array and update length
	temp[length] = dest;
	length++;

	// base case, if the path is complete then return
	if (dest == src) {
		return length;
	}

	// recursive call, updating dest to it's predecessor (back-tracking)
	length = makePath(length, temp, visited, src, visited[dest]);

	return length;
}

////////////////////////////////////////////////////////////////////////

void GraphShow(Graph g, char **names)
{
	assert(g != NULL);
	printf("#vertices=%d, #edges=%d\n\n", g->nV, g->nE);
	int v, w;
	for (v = 0; v < g->nV; v++) {
		printf("%d %s\n", v, names[v]);
		for (w = 0; w < g->nV; w++) {
			if (g->edges[v][w]) {
				printf("\t%s (%d)\n", names[w], g->edges[v][w]);
			}
		}
		printf("\n");
	}
}

////////////////////////////////////////////////////////////////////////
// Helper Functions

static int validVertex(Graph g, Vertex v)
{
	return (g != NULL && v >= 0 && v < g->nV);
}