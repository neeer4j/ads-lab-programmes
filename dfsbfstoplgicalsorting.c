
#include <stdio.h>
#define N 20

int a[N][N], vis[N], n;

void dfs(int v) {
    vis[v] = 1;
    printf("%d ", v);
    for (int i = 1; i <= n; i++)
        if (a[v][i] && !vis[i]) dfs(i);
}

void dfsStack(int s) {
    int st[N], top = -1;
    for (int i = 1; i <= n; i++) vis[i] = 0;
    st[++top] = s;
    vis[s] = 1;

    while (top != -1) {
        int v = st[top--];
        printf("%d ", v);
        for (int i = n; i >= 1; i--)
            if (a[v][i] && !vis[i]) {
                vis[i] = 1;
                st[++top] = i;
            }
    }
}

void bfs(int s) {
    int q[N], f = 0, r = 0;
    for (int i = 1; i <= n; i++) vis[i] = 0;
    q[r++] = s;
    vis[s] = 1;

    while (f < r) {
        int v = q[f++];
        printf("%d ", v);
        for (int i = 1; i <= n; i++)
            if (a[v][i] && !vis[i]) {
                vis[i] = 1;
                q[r++] = i;
            }
    }
}

void topo() {
    int d[N] = {0}, q[N], f = 0, r = 0, c = 0;

    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            if (a[i][j]) d[j]++;

    for (int i = 1; i <= n; i++)
        if (!d[i]) q[r++] = i;

    while (f < r) {
        int v = q[f++];
        c++;
        for (int i = 1; i <= n; i++)
            if (a[v][i] && --d[i] == 0) q[r++] = i;
    }

    if (c != n) {
        printf("Topological sorting is impossible: the graph contains a cycle.\n");
        return;
    }

    printf("Topological order: ");
    for (int i = 0; i < n; i++)
        printf("%d ", q[i]);
    printf("\n");
}

int main() {
    int ch, s, edges, from, to;
    printf("GRAPH TRAVERSAL\n");
    printf("---------------\n");

    do {
        printf("\nHow many vertices?: ", N);
        scanf("%d", &n);
        if (n < 1 || n > N)
            printf("Please enter a number between 1 and %d.\n", N);
    } while (n < 1 || n > N);

    do {
        printf("\nHow many directed edges? : ", n * (n - 1));
        scanf("%d", &edges);
        if (edges < 0 || edges > n * (n - 1))
            printf("Please enter a number between 0 and %d.\n", n * (n - 1));
    } while (edges < 0 || edges > n * (n - 1));

    printf("Enter each directed edge as: source destination\n");
    for (int i = 1; i <= edges; i++) {
        do {
            printf("Edge %d: ", i);
            scanf("%d %d", &from, &to);
            if (from < 1 || from > n || to < 1 || to > n) {
                printf("Invalid edge. Vertices must be between 1 and %d.\n", n);
            } else if (from == to) {
                printf("Self-loops are not supported. Enter two different vertices.\n");
            }
        } while (from < 1 || from > n || to < 1 || to > n || from == to);
        a[from][to] = 1;
    }

    do {
        printf("\nChoose an operation:\n");
        printf("1 - DFS Recursive\n");
        printf("2 - DFS using Stack\n");
        printf("3 - BFS using Queue\n");
        printf("4 - Topological Sort\n");
        printf("5 - Exit\n");
        printf("Enter choice: ");
        scanf("%d", &ch);

        if (ch >= 1 && ch <= 3) {
            printf("Starting vertex (1-%d): ", n);
            scanf("%d", &s);
            if (s < 1 || s > n) {
                printf("Invalid vertex. Please try again.\n");
                continue;
            }
        }

        switch (ch) {
            case 1:
                printf("\nDFS (Recursive): ");
                for (int i = 1; i <= n; i++) vis[i] = 0;                dfs(s);
                break;
            case 2:
                printf("\nDFS (Using Stack): ");
                dfsStack(s);
                break;
            case 3:
                printf("\nBFS (Using Queue): ");
                bfs(s);
                break;
            case 4:
                printf("\nTopological Sort: ");
                topo();
                break;
            case 5:
                printf("\nProgram ended.\n");
                break;
            default:
                printf("\nInvalid choice. Enter a number from 1 to 5.\n");
        }
    } while (ch != 5);

    return 0;
}
