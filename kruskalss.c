#include <stdio.h>

struct edge {
	int src, dest, weight;
};

int parent[20], rank[20];

int find(int n)
{
	if (parent[n] != n)
		parent[n] = find(parent[n]);
	return parent[n];
}

void unionset(int x, int y)
{
	int root_x = find(x);
	int root_y = find(y);

	if (root_x == root_y)
		return;

	if (rank[root_x] < rank[root_y])
		parent[root_x] = root_y;
	else if (rank[root_x] > rank[root_y])
		parent[root_y] = root_x;
	else {
		parent[root_y] = root_x;
		rank[root_x]++;
	}
}

void sortedge(struct edge e[], int m)
{
	int i, j;
	struct edge temp;

	for (i = 0; i < m - 1; i++) {
		for (j = 0; j < m - i - 1; j++) {
			if (e[j].weight > e[j + 1].weight) {
				temp = e[j];
				e[j] = e[j + 1];
				e[j + 1] = temp;
			}
		}
	}
}

int main(void)
{
	struct edge e[50];
	struct edge accepted[20], rejected[50];
	int n, m, i, cost = 0, count = 0, rejected_count = 0;

	printf("Enter the number of nodes: ");
	if (scanf("%d", &n) != 1 || n < 1 || n > 20) {
		printf("Invalid input\n");
		return 1;
	}

	printf("Enter the number of edges: ");
	if (scanf("%d", &m) != 1 || m < 1 || m > 50) {
		printf("Invalid input\n");
		return 1;
	}

	printf("Enter source, destination and weight:\n");
	for (i = 0; i < m; i++) {
		if (scanf("%d %d %d", &e[i].src, &e[i].dest,
			  &e[i].weight) != 3 ||
		    e[i].src < 0 || e[i].src >= n ||
		    e[i].dest < 0 || e[i].dest >= n ||
		    e[i].weight < 1) {
			printf("Invalid input for edge %d. Use vertices 0 to %d "
			       "and provide all %d values.\n",
			       i + 1, n - 1, 3);
			return 1;
		}
	}

	for (i = 0; i < n; i++) {
		parent[i] = i;
		rank[i] = 0;
	}

	sortedge(e, m);

	for (i = 0; i < m; i++) {
		int u = e[i].src;
		int v = e[i].dest;

		if (find(u) != find(v)) {
			unionset(u, v);
			accepted[count] = e[i];
			cost += e[i].weight;
			count++;
		} else {
			rejected[rejected_count] = e[i];
			rejected_count++;
		}
	}

	printf("\nAccepted edges:\n");
	for (i = 0; i < count; i++)
		printf("%d - %d (weight %d)\n",
		       accepted[i].src, accepted[i].dest, accepted[i].weight);

	printf("\nRejected edges:\n");
	for (i = 0; i < rejected_count; i++)
		printf("%d - %d (weight %d): forms a cycle\n",
		       rejected[i].src, rejected[i].dest, rejected[i].weight);

	if (count != n - 1)
		printf("No valid MST found.\n");
	else {
		printf("Valid MST found.\n");
		printf("Minimum cost = %d\n", cost);
	}

	return 0;
}
