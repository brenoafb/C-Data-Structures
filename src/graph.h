#pragma once

#define MAX 1000000
#define INF INT_MAX

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <limits.h>
#include "priority_queue.h" // for Dijkstra, Prim
#include "hash_table.h" // for distance distribution

typedef struct edgenode {
  // next edge
  struct edgenode *next;
  // edge endpoint
  size_t y;
  // edge weight
  double weight;
} edgenode;

void destroy_list(edgenode *v);

typedef struct graph {
  // adjacency list
  edgenode **edges;
  size_t *degree;
  size_t nvertices;
  size_t nedges;
  bool directed;
} graph;

// Initializes graph g with attributes
graph *init_graph(graph *g, size_t nvertices, size_t nedges, bool directed);

// Destroys graph g, frees all memory allocated in init_graph()
void destroy_graph(graph *g);

// Inserts directed or non-directed edge (x, y) with weight w in graph g.
void insert_edge(graph *g, size_t x, size_t y, double w, bool directed);

// Prints graph g
void print_graph(graph *g);

// dijkstra path search.
// dist[i] = d(source, i)
// prev stores paths
void dijkstra(graph *g, size_t source, double dist[], int prev[]);

// prim's algorithm (minimum spanning tree)
// stores tree's edges on vertices
// stores edges' cost on keys
void prim(graph *g, int parents[], double keys[]);

// Calculates distance distribution for all distances.
void distance_distribution(graph *g, hash_table *ht);

