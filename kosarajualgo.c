#include <stdio.h>

int graph[20][20], transpose[20][20], visited[20], stack[20], top = -1, n;

void push(int v)
{
    stack[++top] = v;
}

int pop(void)
{
    return stack[top--];
}

void dfs1(int v)
{
    int i;

    visited[v] = 1;
    for (i = 0; i < n; i++) {
        if (graph[v][i] && !visited[i])
            dfs1(i);
    }

    push(v);
}

void dfs2(int v)
{
    int i;

    visited[v] = 1;
    printf("%d ", v);
    for (i = 0; i < n; i++) {
        if (transpose[v][i] && !visited[i])
            dfs2(i);
    }
}

int main(void)
{
    int i, j, count = 0;

    printf("Enter the number of vertices: ");
    if (scanf("%d", &n) != 1 || n < 1 || n > 20) {
        printf("Invalid number of vertices. Enter a value from 1 to 20.\n");
        return 1;
    }

    printf("Enter the adjacency matrix:\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (scanf("%d", &graph[i][j]) != 1 ||
                (graph[i][j] != 0 && graph[i][j] != 1)) {
                printf("Invalid adjacency matrix entry at row %d, column %d. "
                       "Use only 0 or 1.\n", i, j);
                return 1;
            }
        }
    }

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++)
            transpose[j][i] = graph[i][j];
    }

    for (i = 0; i < n; i++)
        visited[i] = 0;

    for (i = 0; i < n; i++) {
        if (!visited[i])
            dfs1(i);
    }

    for (i = 0; i < n; i++)
        visited[i] = 0;

    printf("\nStrongly connected components:\n");
    while (top != -1) {
        int v = pop();

        if (!visited[v]) {
            count++;
            printf("Component %d: ", count);
            dfs2(v);
            printf("\n");
        }
    }

    printf("Total number of strongly connected components: %d\n", count);
    return 0;
}