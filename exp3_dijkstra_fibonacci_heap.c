

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct FibNode {
    int key;
    int vertex;
    int degree;
    int mark;
    struct FibNode *parent;
    struct FibNode *child;
    struct FibNode *left;
    struct FibNode *right;
} FibNode;

typedef struct FibHeap {
    FibNode *min;
    int numNodes;
} FibHeap;

typedef struct Edge {
    int dest;
    int weight;
    struct Edge *next;
} Edge;

typedef struct Graph {
    int numVertices;
    Edge **adjLists;
} Graph;

/* Graph Creation */
Graph* createGraph(int vertices) {
    Graph *graph = (Graph*)malloc(sizeof(Graph));
    graph->numVertices = vertices;
    graph->adjLists = (Edge**)malloc(vertices * sizeof(Edge*));
    for (int i = 0; i < vertices; i++) {
        graph->adjLists[i] = NULL;
    }
    return graph;
}

void addEdge(Graph *graph, int src, int dest, int weight) {
    Edge *newEdge = (Edge*)malloc(sizeof(Edge));
    newEdge->dest = dest;
    newEdge->weight = weight;
    newEdge->next = graph->adjLists[src];
    graph->adjLists[src] = newEdge;
}

/* Fibonacci Heap Operations */
FibHeap* createFibHeap() {
    FibHeap *heap = (FibHeap*)malloc(sizeof(FibHeap));
    heap->min = NULL;
    heap->numNodes = 0;
    return heap;
}

void addToRootList(FibNode *node, FibNode *root) {
    node->left = root;
    node->right = root->right;
    root->right->left = node;
    root->right = node;
}

void removeFromList(FibNode *node) {
    node->left->right = node->right;
    node->right->left = node->left;
}

FibNode* insertFibHeap(FibHeap *heap, int key, int vertex) {
    FibNode *node = (FibNode*)malloc(sizeof(FibNode));
    node->key = key;
    node->vertex = vertex;
    node->degree = 0;
    node->mark = 0;
    node->parent = NULL;
    node->child = NULL;
    node->left = node;
    node->right = node;

    if (heap->min == NULL) {
        heap->min = node;
    } else {
        addToRootList(node, heap->min);
        if (node->key < heap->min->key) {
            heap->min = node;
        }
    }
    heap->numNodes++;
    return node;
}

void fibHeapLink(FibNode *y, FibNode *x) {
    removeFromList(y);
    y->parent = x;
    if (x->child == NULL) {
        x->child = y;
        y->left = y;
        y->right = y;
    } else {
        addToRootList(y, x->child);
    }
    x->degree++;
    y->mark = 0;
}

void consolidate(FibHeap *heap) {
    int maxDegree = 32;
    FibNode **A = (FibNode**)calloc(maxDegree, sizeof(FibNode*));

    int numRoots = 0;
    FibNode *curr = heap->min;
    if (curr != NULL) {
        do {
            numRoots++;
            curr = curr->right;
        } while (curr != heap->min);
    }

    FibNode **rootList = (FibNode**)malloc(numRoots * sizeof(FibNode*));
    curr = heap->min;
    for (int i = 0; i < numRoots; i++) {
        rootList[i] = curr;
        curr = curr->right;
    }

    for (int i = 0; i < numRoots; i++) {
        FibNode *x = rootList[i];
        int d = x->degree;
        while (A[d] != NULL) {
            FibNode *y = A[d];
            if (x->key > y->key) {
                FibNode *temp = x;
                x = y;
                y = temp;
            }
            fibHeapLink(y, x);
            A[d] = NULL;
            d++;
        }
        A[d] = x;
    }

    free(rootList);
    heap->min = NULL;

    for (int i = 0; i < maxDegree; i++) {
        if (A[i] != NULL) {
            if (heap->min == NULL) {
                heap->min = A[i];
                A[i]->left = A[i];
                A[i]->right = A[i];
            } else {
                addToRootList(A[i], heap->min);
                if (A[i]->key < heap->min->key) {
                    heap->min = A[i];
                }
            }
        }
    }
    free(A);
}

FibNode* extractMin(FibHeap *heap) {
    FibNode *z = heap->min;
    if (z != NULL) {
        if (z->child != NULL) {
            FibNode *child = z->child;
            FibNode *nextChild;
            do {
                nextChild = child->right;
                addToRootList(child, heap->min);
                child->parent = NULL;
                child = nextChild;
            } while (child != z->child);
        }

        removeFromList(z);
        if (z == z->right) {
            heap->min = NULL;
        } else {
            heap->min = z->right;
            consolidate(heap);
        }
        heap->numNodes--;
    }
    return z;
}

void cut(FibHeap *heap, FibNode *x, FibNode *y) {
    if (x == x->right) {
        y->child = NULL;
    } else {
        if (y->child == x) {
            y->child = x->right;
        }
        removeFromList(x);
    }
    y->degree--;
    addToRootList(x, heap->min);
    x->parent = NULL;
    x->mark = 0;
}

void cascadingCut(FibHeap *heap, FibNode *y) {
    FibNode *z = y->parent;
    if (z != NULL) {
        if (y->mark == 0) {
            y->mark = 1;
        } else {
            cut(heap, y, z);
            cascadingCut(heap, z);
        }
    }
}

void decreaseKey(FibHeap *heap, FibNode *x, int newKey) {
    if (newKey > x->key) {
        return;
    }
    x->key = newKey;
    FibNode *y = x->parent;
    if (y != NULL && x->key < y->key) {
        cut(heap, x, y);
        cascadingCut(heap, y);
    }
    if (x->key < heap->min->key) {
        heap->min = x;
    }
}

void printPath(int pred[], int dest) {
    if (pred[dest] != -1) {
        printPath(pred, pred[dest]);
        printf(" -> ");
    }
    printf("%d", dest);
}

void dijkstra(Graph *graph, int src, int dest) {
    int V = graph->numVertices;
    int *dist = (int*)malloc(V * sizeof(int));
    int *pred = (int*)malloc(V * sizeof(int));
    FibNode **heapNodes = (FibNode**)malloc(V * sizeof(FibNode*));

    FibHeap *heap = createFibHeap();

    for (int i = 0; i < V; i++) {
        dist[i] = (i == src) ? 0 : INT_MAX;
        pred[i] = -1;
        heapNodes[i] = insertFibHeap(heap, dist[i], i);
    }

    while (heap->numNodes > 0) {
        FibNode *minNode = extractMin(heap);
        int u = minNode->vertex;
        free(minNode);

        if (dist[u] == INT_MAX) {
            break;
        }

        Edge *edge = graph->adjLists[u];
        while (edge != NULL) {
            int v = edge->dest;
            int weight = edge->weight;

            if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pred[v] = u;
                decreaseKey(heap, heapNodes[v], dist[v]);
            }
            edge = edge->next;
        }
    }

    printf("\n--- Shortest Distances from Source Vertex %d ---\n", src);
    for (int i = 0; i < V; i++) {
        if (dist[i] == INT_MAX) {
            printf("Vertex %d : UNREACHABLE\n", i);
        } else {
            printf("Vertex %d : %d\n", i, dist[i]);
        }
    }

    printf("\n--- Unreachable Vertices ---\n");
    int unreachableCount = 0;
    for (int i = 0; i < V; i++) {
        if (dist[i] == INT_MAX) {
            printf("%d ", i);
            unreachableCount++;
        }
    }
    if (unreachableCount == 0) {
        printf("None (All vertices are reachable)");
    }
    printf("\n");

    printf("\n--- Shortest Path from %d to %d ---\n", src, dest);
    if (dest < 0 || dest >= V) {
        printf("Invalid destination vertex.\n");
    } else if (dist[dest] == INT_MAX) {
        printf("No path exists from %d to %d (Destination is unreachable).\n", src, dest);
    } else {
        printf("Distance: %d\nPath: ", dist[dest]);
        printPath(pred, dest);
        printf("\n");
    }

    free(dist);
    free(pred);
    free(heapNodes);
    free(heap);
}

int main() {
    int V, E, src, dest;
    printf("Enter number of vertices: ");
    scanf("%d", &V);
    printf("Enter number of edges: ");
    scanf("%d", &E);

    Graph *graph = createGraph(V);

    printf("Enter edges (source destination weight):\n");
    for (int i = 0; i < E; i++) {
        int u, v, w;
        scanf("%d %d %d", &u, &v, &w);
        if (u >= 0 && u < V && v >= 0 && v < V && w >= 0) {
            addEdge(graph, u, v, w);
        } else {
            printf("Invalid edge input. Try again.\n");
            i--;
        }
    }

    printf("Enter source vertex (0 to %d): ", V - 1);
    scanf("%d", &src);
    printf("Enter destination vertex (0 to %d): ", V - 1);
    scanf("%d", &dest);

    dijkstra(graph, src, dest);

    return 0;
}
