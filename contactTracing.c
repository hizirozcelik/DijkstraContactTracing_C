/**
 * Student Name: Hizir Ozcelik
 * Student ID: 991578495
 * Date modified: Aug 11, 2022
 * source file for Assignment 4
 * Created by Muhammad Mohiuddin on 3/23/2021 and revised on 11/11/21. Several functions related to Dijkstra algorithm
 are taken from Program P7.3 from Kalicharan and their full implementation is given below and these MUST NOT BE CHANGED!
For rest of the functions follow the TO DO list.
 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "contactTracing.h"
#include <time.h>

GEdgePtr newGEdge(int, int);

void initSingleSource(Graph G, int s) {
    for (int i = 1; i <= G->numV; i++) {
        G->vertex[i].cost = INT_MAX;
        G->vertex[i].parent = 0;
    }
    G->vertex[s].cost = 0;
} //end initSingleSource
void printCostPath(Graph G) {
    for (int i = 1; i <= G->numV; i++) {
        printf("Cost to %d: %2d, Path: ", G->vertex[i].id, G->vertex[i].cost);
        followPath(G, i);
        printf("\n");
    }
} //end printCostPath
void followPath(Graph G, int c) {
    if (c != 0) {
        followPath(G, G->vertex[c].parent);
        if (G->vertex[c].parent != 0) printf(" -> "); //do not print -> for source
        printf("%d ", G->vertex[c].id);
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
            if (G->vertex[heap[smaller + 1]].cost < G->vertex[heap[smaller]].cost)
                smaller++;
        //'smaller' holds the index of the smaller child
        if (G->vertex[key].cost <= G->vertex[heap[smaller]].cost) break;
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
    if (n > NUM_VERTICES) {
        printf("\nToo big. Only %d vertices allowed.\n", NUM_VERTICES);
        exit(1);
    }
    Graph p = (Graph) malloc(sizeof(struct graph));
    p->numV = n;
    return p;
} //end newGraph

GVertex newGVertex(int id) {
    GVertex temp;
    temp.id = id;
    temp.firstEdge = NULL;
    return temp;
}

void addEdge(int srcID, int destID, int weight, Graph G) {
    //add an edge srcID -> destID with a given weight
    int h, k;
    //find srcID in the list of nodes; its location is h
    for (h = 1; h <= G->numV; h++) if (srcID == G->vertex[h].id) break;
    //find destID in the list of nodes; its location is k
    for (k = 1; k <= G->numV; k++) if (destID == G->vertex[k].id) break;
    if (h > G->numV || k > G->numV) {
        printf("No such edge: %d -> %d\n", srcID, destID);
        exit(1);
    }
    GEdgePtr ep = newGEdge(k, weight); //create edge vertex
    // add it to the list of edges, possible empty, from X;
    // it is added so that the list is in order by vertex id
    GEdgePtr prev, curr;
    prev = curr = G->vertex[h].firstEdge;
    while (curr != NULL && destID > G->vertex[curr->child].id) {
        prev = curr;
        curr = curr->nextEdge;
    }
    if (prev == curr) {
        ep->nextEdge = G->vertex[h].firstEdge;
        G->vertex[h].firstEdge = ep;
    } else {
        ep->nextEdge = curr;
        prev->nextEdge = ep;
    }
} //end addEdge
GEdgePtr newGEdge(int c, int w) {
    //return a pointer to a new GEdge node
    GEdgePtr p = (GEdgePtr) malloc(sizeof(GEdge));
    p->child = c;
    p->weight = w;
    p->nextEdge = NULL;
    return p;
}

/**
 * This function builds a graph comprising of the number of vertices given by the user. Random number of edges varying from
 * 0 to MAX_NUM_EDGES for every vertex to build a random graph representing people and their meetings with others.
 * Hint! this function may call generateRandomYetSortedIntegers function given below
 * @param G is the graph
 */
void buildGraphRandom(Graph G) {

    int temp[MAX_NUM_EDGES] = {0};
    srand(time(NULL));
    for (int i = 1; i <= G->numV; ++i) {
        G->vertex[i].id = i;
    }
    for (int i = 1; i <= G->numV; ++i) {
        int num = rand() % (MAX_NUM_EDGES + 1);
        G->vertex[i].inDegree = num;
        generateRandomYetSortedIntegers(temp, num, G->numV - 1, 1, 1, i);
        for (int j = 0; j < num; ++j) {
            addEdge(i, temp[j], 1, G);
        }
    }

}//buildGraphRandom ends
/**
 *
 * @param region it takes an integer representing region number from the menu
 * @return the corresponding name of the region
 */
char *regionLookup(int region) {

    char *str;
    switch (region) {
        case 1:
            str = "Durham";
            break;
        case 2:
            str = "Peel";
            break;
        case 3:
            str = "York";
            break;
        case 4:
            str = "GTA";
            break;
    }
    return str;
}

/**
 * This function displays data as per the screenshots shown in the assignment specs. Region is provided as an integer only
 * to get its text using mapping function. The name of the region is used only in the header statement on the console
 * @param G
 * @param region
 */
void printGraph(Graph G, int region) {

    printf("------------ Contact tracing database of %d residents of %s -------\n", G->numV, regionLookup(region));
    printf("%-12s%-15s%-19s\n", "Person's", "# of direct", "IDs of people");
    printf("%-12s%-15s%-19s\n", "ID", "contacts", "contacted directly");

    for (int i = 1; i <= G->numV; ++i) {
        printf("%-12d%-15d", G->vertex[i].id, G->vertex[i].inDegree);
        for (int j = 0; j < G->vertex[i].inDegree; ++j) {
            printf("%d\t", G->vertex[i].firstEdge[j].child);
        }
        puts("");
    }
}

/**
 * Write the graph to a text file
 * @param in is the file pointer
 * @param G is the graph
 */
void writeGraphToFile(FILE *in, Graph G) {

    fprintf(in, "%s %s %s\n", "Person's ID, ", "# of direct contacts, ", "IDs of people contacted directly");
    for (int i = 1; i <= G->numV; ++i) {
        fprintf(in, "%d %d ", G->vertex[i].id, G->vertex[i].inDegree);
        for (int j = 0; j < G->vertex[i].inDegree; ++j) {
            fprintf(in, "%d ", G->vertex[i].firstEdge[j].child);
        }
        fprintf(in, "\n");
    }
    fprintf(in, "%d %s %d\n", 0, "END OF FILE", 0);
}

/**
 *
 * @param G is the graph of people and their contact history. This is primarily based on Dijkstra function given in the
 * Dijkstra application, however, you need to only calculate the path between source and one destination, so it must
 * add a check for destination at appropriate place in the Dijkstra code and return if true.
 * @param source source person ID
 * @param destination destination person ID
 * @return
 */
int DijkstraContactTracing(Graph G, int s, int d) {

    int heap[G->numV + 1], heapLoc[G->numV + 1];
    //heapLoc[i] gives the position in heap of vertex i
    //if heapLoc[i] = k, then heap[k] contains i

    initSingleSource(G, s);
    for (int i = 1; i <= G -> numV; i++) heap[i] = heapLoc[i] = i;
    heap[1] = s; heap[s] = 1; heapLoc[s] = 1; heapLoc[1] = s;
    int heapSize = G -> numV;
    while (heapSize > 0) {
        int u = heap[1];
        if (G -> vertex[u].cost == INT_MAX && G->vertex[u].parent == G->vertex[d].parent) return INT_MAX-1; //no paths to other vertices
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
        if(u != d) {
            --heapSize;
        } else{
            return G->vertex[u].cost-1;
        }

    } //end while

} //ends DijkstraContactTracing

/**
 *
 * @param arr is the array that will be populated with random yet sorted list of integers
 * @param cnt is the size of the array
 * @param range is the range or spread of the set of random integers
 * @param offset is the offset of the set of random integers
 * @param distinct all numbers should be distinct then set it to 1 else 0
 * @param exclude any single integer to be excluded else pass INT_MIN or any integer outside the range provided
 * Note! For a distinct set of integers, make sure range > cnt, else the function will set range equal to cnt
 */
void generateRandomYetSortedIntegers(int arr[], int cnt, int range, int offset, int distinct, int exclude) {
    range = cnt > range ? cnt : range;
    for (int dex = 0; dex < cnt; dex++) {
        int randInt = exclude;
        while (randInt == exclude) {
            randInt = rand() % (range + 1) + offset;
        }
        arr[dex] = randInt;
        if (distinct) {
            for (int ctr = 0; ctr < dex; ctr++) {
                if (arr[dex] == arr[ctr]) {
                    dex--;
                    break;
                }
            }
        }
    }
    //quickSort(arr, 0, cnt - 1); // optionally you may provide code for quickSort and call it to sort the array
    int min = 0, temp = 0;

    for (int i = 0; i < cnt; i++) {
        min = i;  // record the position of the smallest
        for (int j = i + 1; j < cnt; j++) {
            // update min when finding a smaller element
            if (arr[j] < arr[min])
                min = j;
        }
        // put the smallest element at position i
        temp = arr[i];
        arr[i] = arr[min];
        arr[min] = temp;
    } // array sorting ends here
} // function generateRandomYetSortedIntegers ends

