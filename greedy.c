#include "lugraph.h"
#include "location.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <float.h>


//define a struct for the cities
typedef struct _city
{
    char name[4];
    location loc;
} city;

typedef struct _segment
{
    int from; //index in cities array
    int to; //index in cities array
    double distance;
} segment;

//Merge sort function declarations -- modified versions of Aspnes's functions
bool dist_sorted(const segment d1, const segment d2);
void merge(int n1, const segment a1[], int n2, const segment a2[], segment out[]);
void mergeSort(int n, const segment a[], segment out[]);
void print_pairs(const segment* p, int n);

int main (int argc, char *argv[])
{
    //error checking
    if (argc < 2)
    {
        fprintf(stderr, "TSP: missing filename\n");
        return 1;
    }

    //access & store file
    char *user_input = argv[1];
    FILE *file = fopen(user_input, "r");

    //error checking
    if (file == NULL)
    {
        fprintf(stderr, "TSP: could not open %s\n", user_input);
        return 2;
    }
    
    int n, nth_tri, d, to, from, start;
    n = nth_tri = d = to = from = start = 0;

    fscanf(file, "%i", &n); //store number of cities

    if(n < 2) //error checking
    {
        fprintf(stderr, "TSP: too few cities");
        return 3;
    }

    city cities[n]; //create an array of elements of type city 


    for(int i = 0; i < n; i++) //store city names and initialize values
    {
        fscanf(file,"%s", cities[i].name);
    }

    for(int i = 0; i < n; i++) //store lat and lon values
    {
        fscanf(file, "%lf %lf", &cities[i].loc.lat, &cities[i].loc.lon); 
    }

    nth_tri =  (((n-1) * (n-1)) +(n-1)) / 2;
    
    segment pairs[nth_tri]; //make an array of all pairs of cities
    
    int k = 0; //populate array with all pairs of cities and their distances.
    for(int i = 0; i < n; i++)
    {
        for(int j = i + 1; j < n; j++)
        {
            pairs[k].from = i;
            pairs[k].to = j;
            pairs[k].distance = location_distance(&(cities[i].loc), &(cities[j].loc));

            k++;
        }
    }
    
    mergeSort(nth_tri, pairs, pairs); //sort the pairs by increasing distance using a modified version of Aspnes's merge sort
    
    lugraph* g = lugraph_create(n); //create undirected graph
    lug_search *s = NULL;
    double total_distance = 0;

    for(int i = 0; i < nth_tri; i++) //add edges to graph
    {
        if(lugraph_degree(g, pairs[i].from) < 2 && lugraph_degree(g, pairs[i].to) < 2) //nodes have degree less than two
        {
            s = lugraph_bfs(g, pairs[i].from);
            free(lug_search_path(s, pairs[i].to, &d));

            if (d == -1) //not already connected by a path in graph
            {
                lugraph_add_edge(g, pairs[i].from, pairs[i].to); //add edge to graph
                total_distance += pairs[i].distance; //increment distance
            }

            lug_search_destroy(s);
        }
    }

    //finish the tour by adding an edge between the two vertices with degree 1
    for(int i = 0; i < n; i++)
    {
        if(lugraph_degree(g, i) < 2)
        {
            if(to == 0)
            {
                to = i;
            }
            else
            {
                from = i;
            }
        }
    }
    //lugraph_add_edge(g, to, from);
    total_distance += location_distance(&(cities[to].loc), &(cities[from].loc));

    lug_search *t = lugraph_bfs(g, from);
    int* path = lug_search_path(t, to, &d);
    int tour[n+1];

    for(int i = 0; i < d + 1; i++)
    {
        if(path[i] == 0)
        {
            start = i;
            break;
        }
    }
    
    for(int i = 0; i < n+1; i++)
    {
        tour[i] = path[(start + i) % (d+1)];
    }

    free(path);
    lug_search_destroy(t);
    //iterate through methods
    for (int i = 2; i < argc; i++)
    {
        if(!(strcmp(argv[i], "-greedy") == 0))//error checking
        {
            fprintf(stderr,"TSP: invalid method %s\n", argv[i]);
            return 4;
        }     

        //print output 
        printf("%-16s", "-greedy");
        printf(":");
        printf("%10.2f", total_distance);
        
        for(int i = 0; i < n+1; i++)
        {
            printf(" %s", cities[tour[i]].name);
        }
        printf("\n");

    }

    lugraph_destroy(g);
    fclose(file);
}


/*
* Merge sort functions -- has been modified from Aspnes's notes to work with distances in segment struct
*/
bool dist_sorted(const segment d1, const segment d2)
{
    if(d1.distance < d2.distance)
    {
        return true;
    }
    else return false;
}

/* merge sorted arrays a1 and a2, putting result in out */
void merge(int n1, const segment a1[], int n2, const segment a2[], segment out[])
{
    int i1;
    int i2;
    int iout;

    i1 = i2 = iout = 0;

    while(i1 < n1 || i2 < n2) 
    {
        if(i2 >= n2 || ((i1 < n1) && dist_sorted(a1[i1], a2[i2]))) 
        {
            /* a1[i1] exists and is smaller */
            out[iout++] = a1[i1++];
        } 
        else 
        {
            /* a1[i1] doesn't exist, or is bigger than a2[i2] */
            out[iout++] = a2[i2++];
        }
    }
}

void mergeSort(int n, const segment a[], segment out[])
{
    segment* a1;
    segment* a2;

    if(n < 2)
    {
        /* 0 or 1 elements is already sorted */
        memcpy(out, a, sizeof(segment)*n);
    }
    else
    {
        /* sort into temp arrays */
        a1 = malloc(sizeof(segment) * (n/2));
        a2 = malloc(sizeof(segment) * (n - n/2));

        mergeSort(n/2, a, a1);
        mergeSort(n - n/2, a + n/2, a2);

        merge(n/2, a1, n - n/2, a2, out);

        /* free the temp arrays */
        free(a1);
        free(a2);
    }
}

void print_pairs(const segment* p, int n)
{
    for(int i = 0; i < n; i++)
    {
        printf("%i, %i : %lf\n", p[i].from, p[i].to, p[i].distance);
    }
}