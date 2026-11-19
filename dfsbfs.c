#include <stdio.h>

#define MAX_VERTICES 50

int adjacency_matrix[MAX_VERTICES + 1][MAX_VERTICES + 1];
int active_vertex[MAX_VERTICES + 1];
int vertex_count = 0;

int is_valid_vertex(int vertex)
{
	return vertex >= 1 && vertex <= MAX_VERTICES && active_vertex[vertex];
}

void clear_graph(void)
{
	int i, j;

	for (i = 1; i <= MAX_VERTICES; i++) {
		active_vertex[i] = 0;
		for (j = 1; j <= MAX_VERTICES; j++) {
			adjacency_matrix[i][j] = 0;
		}
	}
	vertex_count = 0;
}

int insert_vertex(int vertex)
{
	if (vertex < 1 || vertex > MAX_VERTICES || active_vertex[vertex]) {
		return 0;
	}

	active_vertex[vertex] = 1;
	vertex_count++;
	return 1;
}

void create_graph(void)
{
	int number_of_vertices, i;

	printf("Enter the number of vertices (1-%d): ", MAX_VERTICES);
	scanf("%d", &number_of_vertices);

	if (number_of_vertices < 1 || number_of_vertices > MAX_VERTICES) {
		printf("Invalid number of vertices.\n");
		return;
	}

	clear_graph();
	for (i = 1; i <= number_of_vertices; i++) {
		insert_vertex(i);
	}

	printf("Graph created with vertices 1 to %d.\n", number_of_vertices);
}

void add_vertex(void)
{
	int vertex;

	printf("Enter the vertex to insert (1-%d): ", MAX_VERTICES);
	scanf("%d", &vertex);

	if (insert_vertex(vertex)) {
		printf("Vertex %d inserted successfully.\n", vertex);
	} else {
		printf("Invalid vertex or vertex %d already exists.\n", vertex);
	}
}

void add_edge(void)
{
	int first, second;

	printf("Enter the first vertex: ");
	scanf("%d", &first);
	printf("Enter the second vertex: ");
	scanf("%d", &second);

	if (!is_valid_vertex(first) || !is_valid_vertex(second)) {
		printf("Both vertices must exist in the graph.\n");
	} else if (first == second) {
		printf("Self-loops are not supported.\n");
	} else {
		adjacency_matrix[first][second] = 1;
		adjacency_matrix[second][first] = 1;
		printf("Edge inserted between %d and %d.\n", first, second);
	}
}

void remove_edge(void)
{
	int first, second;

	printf("Enter the first vertex: ");
	scanf("%d", &first);
	printf("Enter the second vertex: ");
	scanf("%d", &second);

	if (!is_valid_vertex(first) || !is_valid_vertex(second)) {
		printf("Both vertices must exist in the graph.\n");
	} else if (adjacency_matrix[first][second] == 0) {
		printf("The edge does not exist.\n");
	} else {
		adjacency_matrix[first][second] = 0;
		adjacency_matrix[second][first] = 0;
		printf("Edge deleted between %d and %d.\n", first, second);
	}
}

void display_matrix(void)
{
	int row, column;

	if (vertex_count == 0) {
		printf("The graph is empty.\n");
		return;
	}

	printf("\nAdjacency matrix:\n    ");
	for (column = 1; column <= MAX_VERTICES; column++) {
		if (active_vertex[column]) {
			printf("%4d", column);
		}
	}
	printf("\n");

	for (row = 1; row <= MAX_VERTICES; row++) {
		if (active_vertex[row]) {
			printf("%4d", row);
			for (column = 1; column <= MAX_VERTICES; column++) {
				if (active_vertex[column]) {
					printf("%4d", adjacency_matrix[row][column]);
				}
			}
			printf("\n");
		}
	}
}

void depth_first_search(int vertex, int visited[])
{
	int next_vertex;

	visited[vertex] = 1;
	printf("%d ", vertex);

	for (next_vertex = 1; next_vertex <= MAX_VERTICES; next_vertex++) {
		if (active_vertex[next_vertex] &&
			adjacency_matrix[vertex][next_vertex] && !visited[next_vertex]) {
			depth_first_search(next_vertex, visited);
		}
	}
}

void iterative_depth_first_search(int start_vertex, int visited[])
{
	int stack[MAX_VERTICES + 1];
	int top = 0;
	int vertex, next_vertex;

	stack[top++] = start_vertex;

	while (top > 0) {
		vertex = stack[--top];

		if (visited[vertex]) {
			continue;
		}

		visited[vertex] = 1;
		printf("%d ", vertex);

		for (next_vertex = MAX_VERTICES; next_vertex >= 1; next_vertex--) {
			if (active_vertex[next_vertex] &&
				adjacency_matrix[vertex][next_vertex] &&
				!visited[next_vertex]) {
				stack[top++] = next_vertex;
			}
		}
	}
}

void iterative_breadth_first_search(int start_vertex, int visited[])
{
	int queue[MAX_VERTICES + 1];
	int front = 0, rear = 0;
	int vertex, next_vertex;

	queue[rear++] = start_vertex;
	visited[start_vertex] = 1;

	while (front < rear) {
		vertex = queue[front++];
		printf("%d ", vertex);

		for (next_vertex = 1; next_vertex <= MAX_VERTICES; next_vertex++) {
			if (active_vertex[next_vertex] &&
				adjacency_matrix[vertex][next_vertex] &&
				!visited[next_vertex]) {
				visited[next_vertex] = 1;
				queue[rear++] = next_vertex;
			}
		}
	}
}

void recursive_breadth_first_search(int queue[], int *front, int *rear,
	int visited[])
{
	int vertex, next_vertex;

	if (*front >= *rear) {
		return;
	}

	vertex = queue[(*front)++];
	printf("%d ", vertex);

	for (next_vertex = 1; next_vertex <= MAX_VERTICES; next_vertex++) {
		if (active_vertex[next_vertex] &&
			adjacency_matrix[vertex][next_vertex] &&
			!visited[next_vertex]) {
			visited[next_vertex] = 1;
			queue[(*rear)++] = next_vertex;
		}
	}

	recursive_breadth_first_search(queue, front, rear, visited);
}

void run_single_traversal(int traversal_type)
{
	int start_vertex, visited[MAX_VERTICES + 1] = {0};

	printf("Enter the starting vertex: ");
	scanf("%d", &start_vertex);

	if (!is_valid_vertex(start_vertex)) {
		printf("The starting vertex does not exist.\n");
		return;
	}

	if (traversal_type == 1) {
		printf("Recursive DFS traversal: ");
		depth_first_search(start_vertex, visited);
	} else if (traversal_type == 2) {
		printf("Iterative DFS traversal: ");
		iterative_depth_first_search(start_vertex, visited);
	} else if (traversal_type == 3) {
		printf("Iterative BFS traversal: ");
		iterative_breadth_first_search(start_vertex, visited);
	} else {
		int queue[MAX_VERTICES + 1];
		int front = 0, rear = 1;

		queue[0] = start_vertex;
		visited[start_vertex] = 1;
		printf("Recursive BFS traversal: ");
		recursive_breadth_first_search(queue, &front, &rear, visited);
	}
	printf("\n");
}

void display_reachable_vertices(void)
{
	int start_vertex, visited[MAX_VERTICES + 1] = {0};

	printf("Enter the source vertex: ");
	scanf("%d", &start_vertex);

	if (!is_valid_vertex(start_vertex)) {
		printf("The source vertex does not exist.\n");
		return;
	}

	printf("Vertices reachable from vertex %d: ", start_vertex);
	depth_first_search(start_vertex, visited);
	printf("\n");
}

void traverse_disconnected_graph(int traversal_type)
{
	int visited[MAX_VERTICES + 1] = {0};
	int vertex;

	printf("Traversal of the complete graph: ");
	for (vertex = 1; vertex <= MAX_VERTICES; vertex++) {
		if (active_vertex[vertex] && !visited[vertex]) {
			if (traversal_type == 1) {
				depth_first_search(vertex, visited);
			} else if (traversal_type == 2) {
				iterative_depth_first_search(vertex, visited);
			} else if (traversal_type == 3) {
				iterative_breadth_first_search(vertex, visited);
			} else {
				int queue[MAX_VERTICES + 1];
				int front = 0, rear = 1;

				queue[0] = vertex;
				visited[vertex] = 1;
				recursive_breadth_first_search(queue, &front, &rear,
					visited);
			}
		}
	}
	printf("\n");
}

void search_vertex(void)
{
	int vertex;

	printf("Enter the vertex to search: ");
	scanf("%d", &vertex);

	if (is_valid_vertex(vertex)) {
		printf("Vertex %d is present in the graph.\n", vertex);
	} else {
		printf("Vertex %d is not present in the graph.\n", vertex);
	}
}

void print_menu(void)
{
	printf("\n--- GRAPH USING ADJACENCY MATRIX ---\n");
	printf("1. Create graph\n");
	printf("2. Insert vertex\n");
	printf("3. Insert edge\n");
	printf("4. Delete edge\n");
	printf("5. Display adjacency matrix\n");
	printf("6. Search for a vertex\n");
	printf("7. Display vertices reachable from a source\n");
	printf("8. Recursive DFS traversal from a vertex\n");
	printf("9. Iterative DFS traversal from a vertex\n");
	printf("10. Recursive BFS traversal from a vertex\n");
	printf("11. Iterative BFS traversal from a vertex\n");
	printf("12. Recursive DFS traversal of a disconnected graph\n");
	printf("13. Iterative DFS traversal of a disconnected graph\n");
	printf("14. Recursive BFS traversal of a disconnected graph\n");
	printf("15. Iterative BFS traversal of a disconnected graph\n");
	printf("0. Exit\n");
	printf("Enter your choice: ");
}

int main(void)
{
	int choice;

	clear_graph();

	do {
		print_menu();
		scanf("%d", &choice);

		switch (choice) {
		case 1:
			create_graph();
			break;
		case 2:
			add_vertex();
			break;
		case 3:
			add_edge();
			break;
		case 4:
			remove_edge();
			break;
		case 5:
			display_matrix();
			break;
		case 6:
			search_vertex();
			break;
		case 7:
			display_reachable_vertices();
			break;
		case 8:
			run_single_traversal(1);
			break;
		case 9:
			run_single_traversal(2);
			break;
		case 10:
			run_single_traversal(4);
			break;
		case 11:
			run_single_traversal(3);
			break;
		case 12:
			traverse_disconnected_graph(1);
			break;
		case 13:
			traverse_disconnected_graph(2);
			break;
		case 14:
			traverse_disconnected_graph(4);
			break;
		case 15:
			traverse_disconnected_graph(3);
			break;
		case 0:
			printf("Exiting...\n");
			break;
		default:
			printf("Invalid choice. Please try again.\n");
		}
	} while (choice != 0);

	return 0;
}
