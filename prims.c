
#include <stdio.h>
#define N 20
#define INF 99999

int main() {
    int a[N][N], vis[N] = {0};
    int n, s, u, v, min, cost = 0;

    printf("Vertices: ");
    scanf("%d", &n);

    printf("Adjacency matrix:\n");
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            scanf("%d", &a[i][j]);
            if (i != j && a[i][j] == 0)
                a[i][j] = INF;
        }

    printf("Starting vertex: ");
    scanf("%d", &s);
    vis[s] = 1;

    for (int k = 0; k < n-1; k++) {
        min = INF;
        u = -1;

        for (int i = 0; i < n; i++)
            if (vis[i])
                for (int j = 0; j < n; j++)
                    if (!vis[j] && a[i][j] < min) {
                        min = a[i][j];
                        u = i;
                        v = j;
                    }

        if (u == -1) {
            printf("No spanning tree exists");
            return 0;
        }

        printf("%d - %d : %d\n", u, v, min);
        vis[v] = 1;
        cost += min;
    }

    printf("Minimum cost = %d\n", cost);
    return 0;
}
