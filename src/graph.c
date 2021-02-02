#include "graph.h"

graph *init_graph(graph *g, size_t nvertices, size_t nedges, bool directed) {
  assert(g);

  g->nvertices = nvertices;
  g->nedges = nedges;
  g->directed = directed;

  g->edges = calloc(nvertices, sizeof(edgenode *));
  assert(g->edges);

  g->degree = calloc(nvertices, sizeof(size_t));
  assert(g->degree);

  return g;
}

void destroy_graph(graph *g) {
  assert(g);
  assert(g->edges);
  assert(g->degree);

  for (size_t i = 0; i < g->nvertices; ++i) {
    destroy_list(g->edges[i]);
  }

  free(g->edges);
  free(g->degree);

  g->nvertices = 0;
  g->nedges = 0;
  g->directed = 0;
  g->edges = 0;
  g->degree = 0;
}

void destroy_list(edgenode *v) {
  if (!v) return;
  edgenode *n = v->next;
  free(v);
  destroy_list(n);
}

void insert_edge(graph *g, size_t x, size_t y, double w, bool directed) {
  assert(g);
  assert(x < g->nvertices);
  assert(y < g->nvertices);

  edgenode *p = calloc(1, sizeof(edgenode));
  assert(p);
  p->y = y;
  p->weight = w;
  p->next = g->edges[x];
  g->edges[x] = p;
  g->degree[x]++;

  if (!directed) {
    insert_edge(g, y, x, w, true);
  } else {
    g->nedges++;
  }
}

void print_graph(graph *g) {
  assert(g);
  for (size_t i = 0; i < g->nvertices; i++) {
    printf("%zu:", i);
    edgenode *p = g->edges[i];
    while (p) {
      printf(" %zu", p->y);
      p = p->next;
    }
    printf("\n");
  }
}

void dijkstra(graph *g, size_t source, double dist[], int prev[]) {
  priority_queue *pq = pq_init(calloc(1, sizeof(priority_queue)), g->nvertices + 1);

  dist[source] = 0;

  for (size_t i = 0; i < g->nvertices; i++) {
    if (i != source) {
      dist[i] = INF;
    }
    prev[i] = -1; // verify
    pq_insert(pq, (int) i, dist[i]);
  }

  while (!pq_empty(pq)) {
    size_t u = (size_t) pq_extract_min(pq);
    edgenode *p = g->edges[u];
    while (p) {
      int i = pq_index_of(pq, (int) p->y);
      if (i < 0) {
        p = p->next;
        continue;
      }

      double alt = dist[u] + p->weight;
      if (alt < dist[p->y]) {
        dist[p->y] = alt;
        prev[p->y] = (int) u;

        pq_decrease_priority(pq, (size_t) i, alt);
      }
      p = p->next;
    }
  }

  pq_destroy(pq);
  free(pq);
}

void prim(graph *g, int parents[], double keys[]) {
  size_t source = 0;
  priority_queue *pq = pq_init(calloc(1, sizeof(priority_queue)), MAX);

  for (size_t i = 0; i < g->nvertices; i++) {
    keys[i] = INF;
    parents[i] = -1;
    pq_insert(pq, (int) i, INF);
  }

  // Operation below is safe because we know that source is in the queue
  // i.e. its index is not -1
  pq_decrease_priority(pq, (size_t) pq_index_of(pq, (int) source), 0);
  keys[source] = 0;

  while (!pq_empty(pq)) {
    int u = pq_extract_min(pq);
    edgenode *p = g->edges[u];
    while (p) {
      int i = pq_index_of(pq, (int) p->y);
      if (i >= 0 && p->weight < keys[p->y]) {
        parents[p->y] = u;
        keys[p->y] = p->weight;
        pq_decrease_priority(pq, (size_t) i, p->weight);
      }
      p = p->next;
    }
  }

  pq_destroy(pq);
  free(pq);
}

void distance_distribution(graph *g, hash_table *ht) {
  assert(g);
  assert(ht);

  // assume that ht has been allocated, but not initialized
  ht_init(ht, sizeof(double), sizeof(size_t), g->nedges * 2);
  ht->kcomp = __dbl_kcomp;

  double *dists = calloc(g->nvertices, sizeof(double));
  int *prev = calloc(g->nvertices, sizeof(int));

  for (size_t i = 0; i < g->nvertices - 1; i++) {
    dijkstra(g, i, dists, prev);
    for (size_t j = i + 1; j < g->nvertices; j++) {
      // update distance count for dists[j]
      size_t tmp = 0;
      if (ht_get_value(ht, &dists[j], &tmp)) {
        // add one to existing entry
        size_t new = tmp + 1;
        ht_set_value(ht, &dists[j], &new);
      } else {
        // add new entry
        tmp = 1;
        ht_insert(ht, &dists[j], &tmp);
      }
    }
  }

  free(dists);
  free(prev);
}


