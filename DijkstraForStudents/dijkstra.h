//
// Created by Muhammad Mohiuddin on 3/29/2021.
//

#ifndef DIJKSTRA_C_DIJKSTRA_H
#define DIJKSTRA_C_DIJKSTRA_H
#define MaxWordSize 20
#define MaxVertices 50
#define White 'w'
#define Gray 'g'
#define Black 'b'
#define Infinity 99999

typedef struct gEdge {
    int child, weight; //'child' is the location of the child vertex
    struct gEdge *nextEdge;
} GEdge, *GEdgePtr;

typedef struct {
    char id[MaxWordSize+1], colour;
    int parent, cost, discover, finish, inDegree;
    GEdgePtr firstEdge;
} GVertex;

typedef struct graph {
    int numV;
    GVertex vertex[MaxVertices+1];
} *Graph;
void buildGraph(FILE *, Graph);
void printGraph(Graph);
void depthFirstTraversal(Graph, int);
void Dijkstra(Graph, int);
void initSingleSource(Graph, int);
void siftDown(Graph, int, int[], int, int, int[]);
void siftUp(Graph, int[], int, int[]);
void printCostPath(Graph);
void followPath(Graph, int);
GVertex newGVertex(char[]);
void addEdge(char[], char[], int, Graph);
GEdgePtr newGEdge(int, int);

#endif //DIJKSTRA_C_DIJKSTRA_H
