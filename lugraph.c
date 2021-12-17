#define _GNU_SOURCE

#include <stdlib.h>
#include <stdbool.h>

#include "lugraph.h"
#include "queue.h"

struct lugraph
{
  int n;          // the number of vertices
  int *list_size; // the size of each adjacency list
  int *list_cap;  // the capacity of each adjacency list
  int **adj;      // the adjacency lists
};

struct lug_search
{
  const lugraph *g;
  int from;
  int *dist;
  int *pred;
};


#define LUGRAPH_ADJ_LIST_INITIAL_CAPACITY 4

/**
 * Resizes the adjacency list for the given vertex in the given graph.
 * 
 * @param g a pointer to a directed graph
 * @param from the index of a vertex in that graph
 */
void lugraph_list_embiggen(lugraph *g, int from);

/**
 * Prepares a search result for the given graph starting from the given
 * vertex.  It is the responsibility of the caller to destroy the result.
 *
 * @param g a pointer to a directed graph
 * @param from the index of a vertex in that graph
 * @return a pointer to a search result
 */
lug_search *lug_search_create(const lugraph *g, int from);

/**
 * Adds the to vertex to the from vertex's adjacency list.
 *
 * @param g a pointer to a graph, non-NULL
 * @param from the index of a vertex in g
 * @param to the index of a vertex in g
 */
void lugraph_add_half_edge(lugraph *g, int from, int to);

lugraph *lugraph_create(int n)
{
  if (n < 1)
    {
      return NULL;
    }
  
  lugraph *g = malloc(sizeof(lugraph));
  if (g != NULL)
    {
      g->n = n;
      g->list_size = malloc(sizeof(int) * n);
      g->list_cap = malloc(sizeof(int) * n);
      g->adj = malloc(sizeof(int *) * n);
      
      if (g->list_size == NULL || g->list_cap == NULL || g->adj == NULL)
	{
	  free(g->list_size);
	  free(g->list_cap);
	  free(g->adj);
	  free(g);

	  return NULL;
	}

      for (int i = 0; i < n; i++)
	{
	  g->list_size[i] = 0;
	  g->adj[i] = malloc(sizeof(int) * LUGRAPH_ADJ_LIST_INITIAL_CAPACITY);
	  g->list_cap[i] = g->adj[i] != NULL ? LUGRAPH_ADJ_LIST_INITIAL_CAPACITY : 0;
	}
    }

  return g;
}

int lugraph_size(const lugraph *g)
{
  if (g != NULL)
    {
      return g->n;
    }
  else
    {
      return 0;
    }
}

void lugraph_list_embiggen(lugraph *g, int from)
{
  if (g->list_cap[from] != 0)
    {
      int *bigger = realloc(g->adj[from], sizeof(int) * g->list_cap[from] * 2);
      if (bigger != NULL)
	{
	  g->adj[from] = bigger;
	  g->list_cap[from] = g->list_cap[from] * 2;
	}
    }
}

void lugraph_add_edge(lugraph *g, int from, int to)
{
  if (g != NULL && from >= 0 && to >= 0 && from < g->n && to < g->n && from != to)
    {
      lugraph_add_half_edge(g, from, to);
      lugraph_add_half_edge(g, to, from);
    }
}

void lugraph_add_half_edge(lugraph *g, int from, int to)
{
  if (g->list_size[from] == g->list_cap[from])
    {
      lugraph_list_embiggen(g, from);
    }
  
  if (g->list_size[from] < g->list_cap[from])
    {
      g->adj[from][g->list_size[from]++] = to;
    }
}

bool lugraph_has_edge(const lugraph *g, int from, int to)
{
  if (g != NULL && from >= 0 && to >= 0 && from < g->n && to < g->n && from != to)
    {
      int i = 0;
      while (i < g->list_size[from] && g->adj[from][i] != to)
	{
	  i++;
	}
      return i < g->list_size[from];
    }
  else
    {
      return false;
    }
}

int lugraph_degree(const lugraph *g, int v)
{
  if (g != NULL && v >= 0 && v < g->n)
    {
      return g->list_size[v];
    }
  else
    {
      return -1;
    }
}

lug_search *lugraph_bfs(const lugraph *g, int from)
{
  if(g == NULL || from < 0 || from >= g->n) //error checking
  {
    return NULL;
  }

  int curr, sink;
  curr = sink = 0;

  lug_search* s = lug_search_create(g, from);
  queue* q = queueCreate();

  if(s != NULL && q!= NULL)
  {
    enq(q, from);
    s->dist[from] = 0;
    
    while (!queueEmpty(q))
    {
      curr = deq(q);
      for(int i = 0; i < g->list_size[curr]; i++)
      {
        sink = g->adj[curr][i];
        if(s->dist[sink] == -1)
        {
          s->dist[sink] = s->dist[curr] + 1;
          s->pred[sink] =  curr;

          enq(q, sink);
        }

      }

    }

  }

  queueDestroy(q);
  return s;
}


bool lugraph_connected(const lugraph *g, int from, int to)
{
  if(g == NULL || from < 0 || to < 0 || from >= g->n|| to >= g->n) //error checking
  {
    return false;
  }

  lug_search* s = lugraph_bfs(g, from);

  if(s->pred[to] != -1)
  {
    lug_search_destroy(s);
    return true;
  }
  
  lug_search_destroy(s);
  return false;
}

void lugraph_destroy(lugraph *g)
{
  if (g != NULL)
    {
      for (int i = 0; i < g->n; i++)
	{
	  free(g->adj[i]);
	}
      free(g->adj);
      free(g->list_cap);
      free(g->list_size);
      free(g);
    }
}

lug_search *lug_search_create(const lugraph *g, int from)
{
  if (g != NULL && from >= 0 && from < g->n)
    {
      lug_search *s = malloc(sizeof(lug_search));
      
      if (s != NULL)
	{
	  s->g = g;
	  s->from = from;
	  s->dist = malloc(sizeof(int) * g->n);
	  s->pred = malloc(sizeof(int) * g->n);

	  if (s->dist != NULL && s->pred != NULL)
	    {
	      for (int i = 0; i < g->n; i++)
		{
		  s->dist[i] = -1;
		  s->pred[i] = -1;
		}
	    }
	  else
	    {
	      free(s->pred);
	      free(s->dist);
	      free(s);
	      return NULL;
	    }
	}

      return s;
    }
  else
    {
      return NULL;
    }
}

int lug_search_distance(const lug_search *s, int to)
{
  if(s!= NULL && to >= 0 && to < s->g->n)
  {
    return s->dist[to];
  }
  return -1;
}

int *lug_search_path(const lug_search *s, int to, int *len)
{
  if(s == NULL || len == NULL || to < 0 || to >= s->g->n)
  {
    *len = -1;
    return NULL; 
  }

  *len = s->dist[to];

  int size = (*len) + 1 ;
  int *path = calloc(size, sizeof(int));

  for(int i = 0; i < size; i++)
  {
    path[size -  1 - i] = to;
    to = s->pred[to];
  }

  return path;
}

void lug_search_destroy(lug_search *s)
{
  if (s != NULL)
    {
      free(s->dist);
      free(s->pred);
      free(s);
    }
}
