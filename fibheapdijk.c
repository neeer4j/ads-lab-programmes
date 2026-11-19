
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX 100

struct Edge {
    int dest, weight;
    struct Edge *next;
};

struct Node {
    int vertex, degree, mark;
    int dist;
    struct Node *parent, *child, *left, *right;
};

struct Edge *graph[MAX];
struct Node *heap = NULL;
struct Node *nodes[MAX];

int dist[MAX], parent[MAX];
int n;

struct Node* createNode(int v, int d) {
    struct Node *x = malloc(sizeof(struct Node));
    x->vertex = v;
    x->dist = d;
    x->degree = 0;
    x->mark = 0;
    x->parent = x->child = NULL;
    x->left = x->right = x;
    return x;
}

void addEdge(int u, int v, int w) {
    struct Edge *e = malloc(sizeof(struct Edge));
    e->dest = v;
    e->weight = w;
    e->next = graph[u];
    graph[u] = e;
}

void insert(struct Node *x) {
    if (heap == NULL)
        heap = x;
    else {
        x->left = heap;
        x->right = heap->right;
        heap->right->left = x;
        heap->right = x;

        if (x->dist < heap->dist)
            heap = x;
    }
}

void link(struct Node *y, struct Node *x) {
    y->left->right = y->right;
    y->right->left = y->left;

    y->parent = x;
    y->mark = 0;

    if (x->child == NULL) {
        x->child = y;
        y->left = y->right = y;
    }
    else {
        y->left = x->child;
        y->right = x->child->right;
        x->child->right->left = y;
        x->child->right = y;
    }

    x->degree++;
}

void consolidate() {
    struct Node *a[100] = {NULL};
    struct Node *list[100];
    int count = 0;

    if (!heap) return;

    struct Node *x = heap;
    do {
        list[count++] = x;
        x = x->right;
    } while (x != heap);

    for (int i = 0; i < count; i++) {
        x = list[i];
        int d = x->degree;

        while (a[d] != NULL) {
            struct Node *y = a[d];

            if (x->dist > y->dist) {
                struct Node *t = x;
                x = y;
                y = t;
            }

            a[d] = NULL;
            link(y, x);
            d++;
        }
        a[d] = x;
    }

    heap = NULL;

    for (int i = 0; i < 100; i++) {
        if (a[i]) {
            a[i]->left = a[i]->right = a[i];

            if (!heap)
                heap = a[i];
            else {
                a[i]->left = heap;
                a[i]->right = heap->right;
                heap->right->left = a[i];
                heap->right = a[i];

                if (a[i]->dist < heap->dist)
                    heap = a[i];
            }
        }
    }
}

struct Node* extractMin() {
    struct Node *z = heap;

    if (z) {
        if (z->child) {
            struct Node *c = z->child;
            struct Node *start = c;

            do {
                c->parent = NULL;
                c = c->right;
            } while (c != start);

            struct Node *last = z->child->left;
            struct Node *next = z->right;

            z->right->left = last;
            last->right = z->right;
            z->child->left = z;
            z->right = z->child;
            z->left = next->left;
            next->left->right = z;
        }

        z->left->right = z->right;
        z->right->left = z->left;

        if (z == z->right)
            heap = NULL;
        else {
            heap = z->right;
            consolidate();
        }
    }

    return z;
}

void decreaseKey(struct Node *x, int d) {
    x->dist = d;

    struct Node *p = x->parent;

    if (p && x->dist < p->dist) {
        x->left->right = x->right;
        x->right->left = x->left;

        p->degree--;

        if (p->child == x)
            p->child = (x->right == x) ? NULL : x->right;

        x->parent = NULL;
        x->mark = 0;
        insert(x);
    }

    if (x->dist < heap->dist)
        heap = x;
}

void dijkstra(int source) {
    for (int i = 0; i < n; i++) {
        dist[i] = INT_MAX;
        parent[i] = -1;
        nodes[i] = NULL;
    }

    heap = NULL;
    dist[source] = 0;

    for (int i = 0; i < n; i++) {
        nodes[i] = createNode(i, dist[i]);
        insert(nodes[i]);
    }

    while (heap) {
        struct Node *u = extractMin();
        int v = u->vertex;

        if (u->dist == INT_MAX) {
            free(u);
            break;
        }

        struct Edge *e = graph[v];

        while (e) {
            int w = e->dest;

            if (nodes[w] && dist[v] != INT_MAX &&
                dist[v] + e->weight < dist[w]) {

                dist[w] = dist[v] + e->weight;
                parent[w] = v;
                decreaseKey(nodes[w], dist[w]);
            }

            e = e->next;
        }

        nodes[v] = NULL;
        free(u);
    }
}

void showPath(int v) {
    if (v == -1) return;

    showPath(parent[v]);
    printf("%d ", v);
}

int main() {
    int m, u, v, w, source, dest, choice;

    while (1) {
        printf("\n============== FIBONACCI HEAP DIJKSTRA ==============\n");
        printf("1. Enter Graph and Find Shortest Paths\n");
        printf("2. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 2)
            return 0;

        if (choice != 1) {
            printf("Invalid choice\n");
            continue;
        }

        printf("Enter number of vertices: ");
        scanf("%d", &n);

        if (n <= 0 || n > MAX) {
            printf("Invalid number of vertices\n");
            continue;
        }

        heap = NULL;
        for (int i = 0; i < n; i++)
            graph[i] = NULL;

        printf("Enter number of edges: ");
        scanf("%d", &m);

        printf("Enter edges (source destination weight):\n");

        for (int i = 0; i < m; i++) {
            scanf("%d%d%d", &u, &v, &w);

            if (u < 0 || u >= n || v < 0 || v >= n) {
                printf("Invalid edge vertices\n");
                continue;
            }

            if (w < 0) {
                printf("Negative weights not allowed\n");
                continue;
            }

            addEdge(u, v, w);
            addEdge(v, u, w);
        }

        printf("Enter source vertex: ");
        scanf("%d", &source);

        if (source < 0 || source >= n) {
            printf("Invalid source vertex\n");
            continue;
        }

        dijkstra(source);

        printf("\nShortest distances:\n");

        for (int i = 0; i < n; i++) {
            if (dist[i] == INT_MAX)
                printf("%d: Unreachable\n", i);
            else
                printf("%d: %d\n", i, dist[i]);
        }

        printf("\nEnter destination vertex: ");
        scanf("%d", &dest);

        if (dest < 0 || dest >= n || dist[dest] == INT_MAX)
            printf("No path exists\n");
        else {
            printf("Shortest path: ");
            showPath(dest);
            printf("\nDistance: %d\n", dist[dest]);
        }
    }
}
