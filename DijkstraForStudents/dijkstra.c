//
// Created by Muhammad Mohiuddin on 3/29/2021.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dijkstra.h"
void Dijkstra(Graph G, int s) {
    int heap[MaxVertices + 1], heapLoc[MaxVertices + 1];
    //heapLoc[i] gives the position in heap of vertex i
    //if heapLoc[i] = k, then heap[k] contains i

    initSingleSource(G, s);
    for (int i = 1; i <= G -> numV; i++) heap[i] = heapLoc[i] = i;
    heap[1] = s; heap[s] = 1; heapLoc[s] = 1; heapLoc[1] = s;
    int heapSize = G -> numV;
    while (heapSize > 0) {
        int u = heap[1];
        if (G -> vertex[u].cost == Infinity) break; //no paths to other vertices
        //reorganize heap after removing top item
        siftDown(G, heap[heapSize], heap, 1, heapSize-1, heapLoc);
        GEdgePtr p = G -> vertex[u].firstEdge;
        while (p != NULL) {
            if (G -> vertex[u].cost + p -> weight < G -> vertex[p -> child].cost) {
                G -> vertex[p -> child].cost = G -> vertex[u].cost + p -> weight;
                G -> vertex[p -> child].parent = u;
                siftUp(G, heap, heapLoc[p -> child], heapLoc);
            }
            p = p -> nextEdge;
        }
        --heapSize;
    } //end while

    printCostPath(G);
} //end Dijkstra

void initSingleSource(Graph G, int s) {
    for (int i = 1; i <= G -> numV; i++) {
        G -> vertex[i].cost = Infinity;
        G -> vertex[i].parent = 0;
    }
    G -> vertex[s].cost = 0;
} //end initSingleSource

void printCostPath(Graph G) {
    for (int i = 1; i <= G -> numV; i++) {
        printf("Cost to %s: %2d, Path: ", G -> vertex[i].id, G -> vertex[i].cost);
        followPath(G, i);
        printf("\n");
    }
} //end printCostPath

void followPath(Graph G, int c) {
    if (c != 0) {
        followPath(G, G -> vertex[c].parent);
        if (G -> vertex[c].parent != 0) printf(" -> "); //do not print -> for source
        printf("%s ", G -> vertex[c].id);
    }
} //end followPath

void siftUp(Graph G, int heap[], int n, int heapLoc[]) {
    //sifts up heap[n] so that heap[1..n] contains a heap based on cost
    int siftItem = heap[n];
    int child = n;
    int parent = child / 2;
    while (parent > 0) {
        if (G->vertex[siftItem].cost >= G->vertex[heap[parent]].cost) break;
        heap[child] = heap[parent]; //move down parent
        heapLoc[heap[parent]] = child;
        child = parent;
        parent = child / 2;
    }
    heap[child] = siftItem;
    heapLoc[siftItem] = child;
} //end siftUp

void siftDown(Graph G, int key, int heap[], int root, int last, int heapLoc[]) {
    int smaller = 2 * root;
    while (smaller <= last) { //while there is at least one child
        if (smaller < last) //there is a right child as well; find the smaller
            if (G->vertex[heap[smaller+1]].cost < G->vertex[heap[smaller]].cost)
                smaller++;
        //'smaller' holds the index of the smaller child
        if (G -> vertex[key].cost <= G -> vertex[heap[smaller]].cost) break;
        //cost[key] is bigger; promote heap[smaller]
        heap[root] = heap[smaller];
        heapLoc[heap[smaller]] = root;
        root = smaller;
        smaller = 2 * root;
    } //end while
    heap[root] = key;
    heapLoc[key] = root;
} //end siftDown

Graph newGraph(int n) {
    if (n > MaxVertices) {
        printf("\nToo big. Only %d vertices allowed.\n", MaxVertices);
        exit(1);
    }
    Graph p = (Graph) malloc(sizeof(struct graph));
    p -> numV = n;
    return p;
} //end newGraph

void buildGraph(FILE * in, Graph G) {
    int numEdges, weight;
    char nodeID[MaxWordSize+1], adjID[MaxWordSize+1];
    for (int h = 1; h <= G -> numV; h++) {
        G -> vertex[h] = newGVertex("");      //create a vertex node
        fscanf(in, "%s", G -> vertex[h].id);   //read the name into id
    }
    for (int h = 1; h <= G -> numV; h++) {
        fscanf(in, "%s %d", nodeID, &numEdges); //parent id and numEdges
        for (int k = 1; k <= numEdges; k++) {
            fscanf(in, "%s %d", adjID, &weight); //get child id and weight
            addEdge(nodeID, adjID, weight, G);
        }
    }
} //end buildGraph

GVertex newGVertex(char name[]) {
    GVertex temp;
    strcpy(temp.id, name);
    temp.firstEdge = NULL;
    return temp;
}

void addEdge(char X[], char Y[], int weight, Graph G) {
    //add an edge X -> Y with a given weight
    int h, k;
    //find X in the list of nodes; its location is h
    for (h = 1; h <= G -> numV; h++) if (strcmp(X, G -> vertex[h].id) == 0) break;

    //find Y in the list of nodes; its location is k
    for (k = 1; k <= G-> numV; k++) if (strcmp(Y, G -> vertex[k].id) == 0) break;

    if (h > G -> numV || k > G -> numV) {
        printf("No such edge: %s -> %s\n", X, Y);
        exit(1);
    }

    GEdgePtr ep = newGEdge(k, weight); //create edge vertex
    // add it to the list of edges, possible empty, from X;
    // it is added so that the list is in order by vertex id
    GEdgePtr prev, curr;
    prev = curr = G -> vertex[h].firstEdge;
    while (curr != NULL && strcmp(Y, G -> vertex[curr -> child].id) > 0) {
        prev = curr;
        curr = curr -> nextEdge;
    }

    if (prev == curr) {
        ep -> nextEdge = G -> vertex[h].firstEdge;
        G -> vertex[h].firstEdge = ep;
    }
    else {
        ep -> nextEdge = curr;
        prev -> nextEdge = ep;
    }
} //end addEdge

GEdgePtr newGEdge(int c, int w) {
    //return a pointer to a new GEdge node
    GEdgePtr p = (GEdgePtr) malloc(sizeof (GEdge));
    p -> child = c;
    p -> weight = w;
    p -> nextEdge = NULL;
    return p;
}

void printGraph(Graph G) {
    for (int h = 1; h <= G -> numV; h++) {
        printf("%s: ", G -> vertex[h].id);
        GEdgePtr p = G -> vertex[h].firstEdge;
        while (p != NULL) {
            printf("%s %d ", G -> vertex[p -> child].id, p -> weight);
            p = p -> nextEdge;
        }
        printf("\n");
    }
    printf("\n");
} //end printGraph


