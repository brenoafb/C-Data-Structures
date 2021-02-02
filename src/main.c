#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>
#include <float.h>
#include "graph.h"
#include "priority_queue.h"
#include "hash_table.h"

#define DBL_EQ(x, y) (fabs(x - y) <= DBL_EPSILON)
#define OPPOS 2 // operation position
#define FPOS 1  // input file position
#define PATH 0  // program's path
#define MST 1   // mst operation index
#define DIST 2  // distribution operation index
#define TEST 3  // test operation index

// Counts the number of lines in file f.
size_t lines(FILE *f);

// Reads graph from file at 'filename'.
// Stops program's execution if anything goes wrong.
graph *read_graph(const char *restrict filename, graph *g);

// Prints path between a and b with distance do fp
// dists and prev are output of dijkstra
void path(size_t nvertices, size_t a, size_t b,
    double dists[], int prev[], FILE *fp);

int main(int argc, const char *argv[]) {
  if (argc < 3) {
    printf("No arguments supplied.\nUsage: %s filename operation [arguments]\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  const char *filename = argv[FPOS];

  const char *operations[] = {"path", "mst", "distribution", "test"};

  // Carry out operation.
  if (strncmp(argv[OPPOS], operations[PATH], strlen(operations[PATH])) == 0) {
    if (argc < 5) {
      printf("Insufficient arguments supplied. Please supply start and end points\nUsage: %s path input start end\n", argv[0]);
      exit(EXIT_FAILURE);
    }

    graph *g = read_graph(filename, calloc(1, sizeof(graph)));

    // indicates whether to calculate distance between a and all other points
    bool all = (argv[OPPOS + 2][0] == '.');

    // determine start vertex
    int a = atoi(argv[OPPOS + 1]) - 1;

    if (a+1 <= 0 || a+1 > (int) g->nvertices) {
      printf("Invalid vertex '%s'. Exiting\n", argv[OPPOS + 1]);
      exit(EXIT_FAILURE);
    }

    FILE *fp = stdout;
    if (argc > 5) {
      const char *output_filename = argv[5];
      fp = fopen(output_filename, "w");
      if (!fp) {
        printf("Could not open file '%s' for writing. Exiting.\n", output_filename);
        exit(EXIT_FAILURE);
      }
    }

    double *dists = calloc(g->nvertices, sizeof(double));
    int *prev = calloc(g->nvertices, sizeof(int));

    // calculate distances and paths
    dijkstra(g, (size_t) a, dists, prev);

    if (fp != stdout) {
      printf("Writing to file.\n");
    }

    if (!all) {
      // determine endpoint b
      int b = atoi(argv[OPPOS + 2]) - 1;
      if (b+1 <= 0 || b+1 > (int) g->nvertices) {
        printf("Invalid vertex '%s'. Exiting\n", argv[OPPOS + 2]);
        exit(EXIT_FAILURE);
      }
      // path and distance between a and b
      path(g->nvertices, (size_t) a, (size_t) b, dists, prev, fp);
    } else {
      // calculate distances and paths
      dijkstra(g, (size_t) a, dists, prev);

      // print all paths and distances
      for (size_t i = 0; i < g->nvertices; i++) {
        if ((int) i == a) continue;
        // print path
        path(g->nvertices, (size_t) a, (size_t) i, dists, prev, fp);
      }
    }

    if (fp != stdout) {
      fclose(fp);
      printf("Done.\n");
    }

    free(dists);
    free(prev);
    destroy_graph(g);
    free(g);
  } else if (strncmp(argv[OPPOS], operations[MST], strlen(operations[MST])) == 0) {
    if (argc < 3) {
      printf("Insufficient arguments supplied. Please supply output filename\nUsage: %s mst input file [output file]\n", argv[0]);
      exit(EXIT_FAILURE);
    }

    graph *g = read_graph(filename, calloc(1, sizeof(graph)));

    int *parents = calloc(g->nvertices, sizeof(int));
    double *keys = calloc(g->nvertices, sizeof(double));

    prim(g, parents, keys);

    // determine where to print output (stdout or file)
    FILE *fp = stdout;
    if (argc > 3) {
      fp = fopen(argv[3], "w");
      if (!fp) {
        printf("Could not open file '%s' for writing. Exiting.\n", argv[3]);
        exit(EXIT_FAILURE);
      }

    }

    if (fp != stdout) {
      printf("Writing mst to file.\n");
    }

    fprintf(fp, "%zu\n", g->nvertices);

    double total_cost = 0.0;
    for (size_t i = 0; i < g->nvertices; i++) {
      if (parents[i] == -1) continue;
      total_cost += keys[i];
      fprintf(fp, "%zu %d %f\n", i + 1, parents[i] + 1, keys[i]);
    }

    fprintf(fp, "Total cost: %f\n", total_cost);

    // done printing
    if (fp != stdout) {
      fclose(fp);
      printf("Done.\n");
    }

    // clean up
    destroy_graph(g);
    free(parents);
    free(g);
  } else if (strncmp(argv[OPPOS], operations[DIST], strlen(operations[DIST])) == 0) {

    // determine where to print output (file of stdout)

    FILE *fp = stdout;

    if (argc > 3) {
      const char *output_filename = argv[OPPOS + 1];
      fp = fopen(output_filename, "w");
      if (!fp) {
        printf("Could not open file '%s' for writing. Exiting.\n", output_filename);
        exit(EXIT_FAILURE);
      }
    }

    graph *g = read_graph(filename, calloc(1, sizeof(graph)));
    hash_table *ht = calloc(1, sizeof(hash_table));
    distance_distribution(g, ht);

    // get data on a easier to iterate on format
    double *dists = calloc(ht->count, sizeof(double));
    size_t *counts = calloc(ht->count, sizeof(size_t));
    ht_arrays(ht, (uint8_t *) dists, (uint8_t *) counts);

    // Print results

    fprintf(fp, "Distance distribution:\n");

    // total possible unordered vertex pairs
    double total = (double) g->nvertices * ((double) g->nvertices - 1.0) / 2.0;

    // Begin printing
    if (fp != stdout) {
      printf("Writing to file.\n");
    }

    for (size_t i = 0; i < ht->count; i++) {
      double frac = (double) counts[i]/ total;
      fprintf(fp, "%f: %f\n", dists[i], frac);
    }

    // Done printing
    if (fp != stdout) {
      printf("Done\n");
      fclose(fp);
    }

    // Clean up
    free(dists);
    free(counts);
    ht_destroy(ht);
    free(ht);
    destroy_graph(g);
    free(g);
  } else {
    printf("Invalid option '%s'\n", argv[OPPOS]);
  }

  return 0;
}

size_t lines(FILE *f) {
  size_t lines = 0;
  while(!feof(f)) {
    int ch = fgetc(f);
    if(ch == '\n') {
      lines++;
    }
  }
  rewind(f);
  return lines;
}

graph *read_graph(const char *restrict filename, graph *g) {
  assert(g);

  FILE *f = fopen(filename, "r");
  if (!f) {
    printf("Error: could not read file '%s'. Exiting.\n", filename);
    exit(EXIT_FAILURE);
  }

  size_t nedges = lines(f) - 1;

  int nvertices;

  fscanf(f, "%d", &nvertices);

  if (nvertices <= 0) {
    printf("Invalid vertex count '%d'. Exiting.\n", nvertices);
    exit(EXIT_FAILURE);
  }

  init_graph(g, (size_t) nvertices, nedges, false);

  for (size_t i = 0; i < nedges; i++) {
    int x, y;
    double w;
    fscanf(f, "%d %d %lf", &x, &y, &w);
    // assumes 1 indexing of vertices and positive weights
    // check if values are inside bounds
    if (x <= 0) {
      printf("Error processing edge (%d, %d, %f). Invalid point '%d'. Exiting.\n", x, y, w, x);
      exit(EXIT_FAILURE);
    }

    if (y <= 0) {
      printf("Error processing edge (%d, %d, %f). Invalid point '%d'. Exiting.\n", x, y, w, y);
      exit(EXIT_FAILURE);
    }

    if (w < 0.0) {
      printf("Error processing edge (%d, %d, %f). Invalid weight '%f'. Exiting.\n", x, y, w, w);
      exit(EXIT_FAILURE);
    }

    insert_edge(g, (size_t) x-1, (size_t) y-1, w, false); // -1 (0 index for storage)
  }

  fclose(f);

  return g;
}

void path(size_t nvertices, size_t a, size_t b,
  double dists[], int prev[], FILE *fp) {
  assert(a < nvertices);
  assert(b < nvertices);
  fprintf(fp, "d(%zu, %zu) = %f, [", a + 1, b + 1, dists[b]);

  int curr = (int) b;
  while (curr >= 0 && curr != (int) a) {
    fprintf(fp, "%d, ", curr + 1);
    curr = prev[curr];
  }
  fprintf(fp, "%zu]\n", a + 1);
}

