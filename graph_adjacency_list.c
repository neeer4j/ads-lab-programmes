#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 50

typedef struct AdjNode {
	int vertex;
	struct AdjNode *next;
} AdjNode;

AdjNode *adjacency_list[MAX_VERTICES + 1];
int active_vertex[MAX_VERTICES + 1];
int vertex_count;

int is_valid_vertex(int vertex)
{
	return vertex >= 1 && vertex <= MAX_VERTICES && active_vertex[vertex];
}

void free_list(int vertex)
{
	AdjNode *current = adjacency_list[vertex];

	while (current != NULL) {
		AdjNode *next = current->next;
		free(current);
		current = next;
	}
	adjacency_list[vertex] = NULL;
}

void clear_graph(void)
{
	int vertex;

	for (vertex = 1; vertex <= MAX_VERTICES; vertex++) {
		free_list(vertex);
		active_vertex[vertex] = 0;
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

int edge_exists(int first, int second)
{
	AdjNode *current = adjacency_list[first];

	while (current != NULL) {
		if (current->vertex == second) {
			return 1;
		}
		current = current->next;
	}
	return 0;
}

int insert_adjacent(int first, int second)
{
	AdjNode *new_node;

	if (edge_exists(first, second)) {
		return 0;
	}

	new_node = (AdjNode *)malloc(sizeof(AdjNode));
	if (new_node == NULL) {
		printf("Memory allocation failed.\n");
		return 0;
	}

	new_node->vertex = second;
	new_node->next = adjacency_list[first];
	adjacency_list[first] = new_node;
	return 1;
}

int remove_adjacent(int first, int second)
{
	AdjNode *current = adjacency_list[first];
	AdjNode *previous = NULL;

	while (current != NULL) {
		if (current->vertex == second) {
			if (previous == NULL) {
				adjacency_list[first] = current->next;
			} else {
				previous->next = current->next;
			}
			free(current);
			return 1;
		}
		previous = current;
		current = current->next;
	}
	return 0;
}

void create_graph(void)
{
	int number_of_vertices, vertex;

	printf("Enter the number of vertices (1-%d): ", MAX_VERTICES);
	if (scanf("%d", &number_of_vertices) != 1 ||
		number_of_vertices < 1 || number_of_vertices > MAX_VERTICES) {
		printf("Invalid number of vertices.\n");
		return;
	}

	clear_graph();
	for (vertex = 1; vertex <= number_of_vertices; vertex++) {
		insert_vertex(vertex);
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
		printf("Invalid vertex or vertex already exists.\n");
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
		return;
	}
	if (first == second) {
		printf("Self-loops are not supported.\n");
		return;
	}
	if (!insert_adjacent(first, second)) {
		printf("The edge already exists or could not be inserted.\n");
		return;
	}
	insert_adjacent(second, first);
	printf("Edge added between %d and %d.\n", first, second);
}

void delete_edge(void)
{
	int first, second;

	printf("Enter the first vertex: ");
	scanf("%d", &first);
	printf("Enter the second vertex: ");
	scanf("%d", &second);

	if (!is_valid_vertex(first) || !is_valid_vertex(second)) {
		printf("Both vertices must exist in the graph.\n");
		return;
	}
	if (!remove_adjacent(first, second)) {
		printf("The edge does not exist.\n");
		return;
	}
	remove_adjacent(second, first);
	printf("Edge deleted between %d and %d.\n", first, second);
}

void display_adjacency_list(void)
{
	int vertex;
	AdjNode *current;

	if (vertex_count == 0) {
		printf("The graph is empty.\n");
		return;
	}

	printf("\nAdjacency list:\n");
	for (vertex = 1; vertex <= MAX_VERTICES; vertex++) {
		if (!active_vertex[vertex]) {
			continue;
		}
		printf("%d ->", vertex);
		current = adjacency_list[vertex];
		while (current != NULL) {
			printf(" %d", current->vertex);
			current = current->next;
		}
		printf("\n");
	}
}

void print_visited(int visited[])
{
	int vertex;

	printf("Visited array: ");
	for (vertex = 1; vertex <= MAX_VERTICES; vertex++) {
		if (active_vertex[vertex]) {
			printf("%d:%d ", vertex, visited[vertex]);
		}
	}
	printf("\n");
}

void recursive_dfs(int vertex, int visited[], int order[], int *order_count)
{
	AdjNode *current;

	visited[vertex] = 1;
	order[(*order_count)++] = vertex;
	current = adjacency_list[vertex];
	while (current != NULL) {
		if (!visited[current->vertex]) {
			recursive_dfs(current->vertex, visited, order, order_count);
		}
		current = current->next;
	}
}

void iterative_dfs(int start_vertex, int visited[], int order[], int *order_count)
{
	int stack[MAX_VERTICES];
	int top = 0;
	AdjNode *current;

	stack[top++] = start_vertex;
	while (top > 0) {
		int vertex = stack[--top];
		if (visited[vertex]) {
			continue;
		}
		visited[vertex] = 1;
		order[(*order_count)++] = vertex;
		current = adjacency_list[vertex];
		while (current != NULL) {
			if (!visited[current->vertex]) {
				stack[top++] = current->vertex;
			}
			current = current->next;
		}
	}
}

void bfs(int start_vertex, int visited[], int order[], int *order_count)
{
	int queue[MAX_VERTICES];
	int front = 0, rear = 0;
	AdjNode *current;

	queue[rear++] = start_vertex;
	visited[start_vertex] = 1;
	while (front < rear) {
		int vertex = queue[front++];
		order[(*order_count)++] = vertex;
		current = adjacency_list[vertex];
		while (current != NULL) {
			if (!visited[current->vertex]) {
				visited[current->vertex] = 1;
				queue[rear++] = current->vertex;
			}
			current = current->next;
		}
	}
}

void traverse_from(int start_vertex, int method, int visited[], int order[],
	int *order_count)
{
	if (method == 1) {
		recursive_dfs(start_vertex, visited, order, order_count);
		return;
	}
	if (method == 2) {
		iterative_dfs(start_vertex, visited, order, order_count);
		return;
	}
	bfs(start_vertex, visited, order, order_count);
}

void traverse_all_components(int start_vertex, int method, int visited[],
	int order[], int *order_count)
{
	int vertex;

	traverse_from(start_vertex, method, visited, order, order_count);

	for (vertex = 1; vertex <= MAX_VERTICES; vertex++) {
		if (active_vertex[vertex] && !visited[vertex]) {
			traverse_from(vertex, method, visited, order, order_count);
		}
	}
}

void run_traversal(int method)
{
	int start_vertex, visited[MAX_VERTICES + 1] = {0};
	int order[MAX_VERTICES], order_count = 0, index;

	printf("Enter the starting vertex: ");
	scanf("%d", &start_vertex);
	if (!is_valid_vertex(start_vertex)) {
		printf("The starting vertex does not exist.\n");
		return;
	}

	traverse_all_components(start_vertex, method, visited, order, &order_count);
	printf("Visit order: ");
	for (index = 0; index < order_count; index++) {
		printf("%d ", order[index]);
	}
	printf("\n");
	print_visited(visited);
}

int search_vertex(int start_vertex, int target, int method)
{
	int visited[MAX_VERTICES + 1] = {0};
	int order[MAX_VERTICES], order_count = 0;

	traverse_from(start_vertex, method, visited, order, &order_count);
	return visited[target];
}

void search_menu(void)
{
	int start_vertex, target, method;

	printf("Enter the starting vertex: ");
	scanf("%d", &start_vertex);
	printf("Enter the vertex to search for: ");
	scanf("%d", &target);
	printf("Choose search: 1. Recursive DFS  2. Iterative DFS  3. BFS: ");
	scanf("%d", &method);

	if (!is_valid_vertex(start_vertex) || !is_valid_vertex(target) ||
		(method < 1 || method > 3)) {
		printf("Invalid vertex or search method.\n");
		return;
	}

	if (search_vertex(start_vertex, target, method)) {
		printf("Vertex %d is reachable from vertex %d.\n", target, start_vertex);
	} else {
		printf("Vertex %d is not reachable from vertex %d.\n", target, start_vertex);
	}
}

void reachability_menu(void)
{
	int start_vertex, target, visited[MAX_VERTICES + 1] = {0};
	int order[MAX_VERTICES], order_count = 0;

	printf("Enter the source vertex: ");
	scanf("%d", &start_vertex);
	printf("Enter the destination vertex: ");
	scanf("%d", &target);

	if (!is_valid_vertex(start_vertex) || !is_valid_vertex(target)) {
		printf("Both vertices must exist in the graph.\n");
		return;
	}

	bfs(start_vertex, visited, order, &order_count);
	if (visited[target]) {
		printf("Vertex %d is reachable from vertex %d.\n", target, start_vertex);
	} else {
		printf("Vertex %d is not reachable from vertex %d.\n", target, start_vertex);
	}
}

void show_menu(void)
{
	printf("\n--- Graph Using Adjacency List ---\n");
	printf("1. Create graph\n");
	printf("2. Add vertex\n");
	printf("3. Add edge\n");
	printf("4. Delete edge\n");
	printf("5. Display adjacency list\n");
	printf("6. Recursive DFS traversal\n");
	printf("7. Iterative DFS traversal using stack\n");
	printf("8. BFS traversal using queue\n");
	printf("9. Search vertex using DFS/BFS\n");
	printf("10. Check reachability\n");
	printf("0. Exit\n");
	printf("Enter your choice: ");
}

int main(void)
{
	int choice;

	clear_graph();
	while (1) {
		show_menu();
		if (scanf("%d", &choice) != 1) {
			printf("Invalid input.\n");
			break;
		}
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
			delete_edge();
			break;
		case 5:
			display_adjacency_list();
			break;
		case 6:
			run_traversal(1);
			break;
		case 7:
			run_traversal(2);
			break;
		case 8:
			run_traversal(3);
			break;
		case 9:
			search_menu();
			break;
		case 10:
			reachability_menu();
			break;
		case 0:
			clear_graph();
			printf("Goodbye.\n");
			return 0;
		default:
			printf("Invalid choice.\n");
		}
	}

	clear_graph();
	return 0;
}
