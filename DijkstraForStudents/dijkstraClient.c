/**
 * Program P7.3 from Data structures in C by Kalicharan.
 * This program reads data from a file into a graph and applies Dijkstra algorithm to determine
 * the shortest path from one of the nodes to all the reachable nodes of the entire graph.
 * If a node is unreachable then it displays its cost as 99999 which is a defined constant as infinity
 * Programs was migrated to CLION by M. Mohiuddin on March 29, 21 and was divided into multiple files for
 * convenient code re-usability
 */


#include <stdio.h>
#include "dijkstra.h"
     int main() {
        int numVertices;
        Graph newGraph(int);
        FILE * in = fopen("..//dijkstra.in", "r");
        fscanf(in, "%d", &numVertices);
        Graph G = newGraph(numVertices);
        buildGraph(in, G);
        printGraph(G);
        Dijkstra(G, 1);
        fclose(in);
     } // end main

