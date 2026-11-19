#include <stdio.h>

#define MAX 100

int parent[MAX + 1];
int present[MAX + 1];
int number_of_sets = 0;

int exists(int element)
{
	return element >= 1 && element <= MAX && present[element];
}

int find_root(int element)
{
	while (parent[element] != element) {
		element = parent[element];
	}
	return element;
}

void make_set(int element)
{
	if (element < 1 || element > MAX || present[element]) {
		printf("Invalid element or element already exists.\n");
		return;
	}

	parent[element] = element;
	present[element] = 1;
	number_of_sets++;
	printf("Element %d created as a singleton set.\n", element);
}

void create_collection(void)
{
	int n, i;

	printf("Enter the number of elements (1-%d): ", MAX);
	scanf("%d", &n);
	if (n < 1 || n > MAX) {
		printf("Invalid number of elements.\n");
		return;
	}

	for (i = 1; i <= MAX; i++) {
		parent[i] = 0;
		present[i] = 0;
	}
	number_of_sets = 0;

	for (i = 1; i <= n; i++) {
		make_set(i);
	}
}

void union_sets(void)
{
	int first, second, first_root, second_root;

	printf("Enter the first element: ");
	scanf("%d", &first);
	printf("Enter the second element: ");
	scanf("%d", &second);

	if (!exists(first) || !exists(second)) {
		printf("Both elements must exist.\n");
		return;
	}

	first_root = find_root(first);
	second_root = find_root(second);
	if (first_root == second_root) {
		printf("Both elements are already in the same set.\n");
		return;
	}

	parent[second_root] = first_root;
	number_of_sets--;
	printf("The two sets were joined.\n");
}

void delete_element(void)
{
	int element, root, replacement, other;

	printf("Enter the element to delete: ");
	scanf("%d", &element);

	if (!exists(element)) {
		printf("Element does not exist.\n");
		return;
	}

	root = find_root(element);
	if (root == element) {
		replacement = 0;
		for (other = 1; other <= MAX; other++) {
			if (other != element && present[other] &&
				find_root(other) == root) {
				replacement = other;
				break;
			}
		}

		if (replacement == 0) {
			number_of_sets--;
		} else {
			for (other = 1; other <= MAX; other++) {
				if (other != element && present[other] &&
					find_root(other) == root) {
					parent[other] = replacement;
				}
			}
			parent[replacement] = replacement;
		}
	}

	present[element] = 0;
	parent[element] = 0;
	printf("Element %d deleted.\n", element);
}

void display_representatives(void)
{
	int element;

	for (element = 1; element <= MAX; element++) {
		if (present[element]) {
			printf("Element %d -> Representative %d\n",
			       element, find_root(element));
		}
	}
}

void display_sets(void)
{
	int root, element;

	printf("Disjoint sets:\n");
	for (root = 1; root <= MAX; root++) {
		if (present[root] && find_root(root) == root) {
			printf("{ ");
			for (element = 1; element <= MAX; element++) {
				if (present[element] && find_root(element) == root) {
					printf("%d ", element);
				}
			}
			printf("}\n");
		}
	}
}

void check_same_set(void)
{
	int first, second;

	printf("Enter the first element: ");
	scanf("%d", &first);
	printf("Enter the second element: ");
	scanf("%d", &second);

	if (!exists(first) || !exists(second)) {
		printf("Both elements must exist.\n");
	} else if (find_root(first) == find_root(second)) {
		printf("Yes, both elements are in the same set.\n");
	} else {
		printf("No, they are in different sets.\n");
	}
}

void display_set_sizes(void)
{
	int root, element, size;

	for (root = 1; root <= MAX; root++) {
		if (present[root] && find_root(root) == root) {
			size = 0;
			for (element = 1; element <= MAX; element++) {
				if (present[element] && find_root(element) == root) {
					size++;
				}
			}
			printf("Representative %d has size %d\n", root, size);
		}
	}
}

void menu(void)
{
	printf("\n--- DISJOINT SET ---\n");
	printf("1. Create n singleton sets\n");
	printf("2. Create a set for one element\n");
	printf("3. Find representative\n");
	printf("4. Union two sets\n");
	printf("5. Check whether two elements are in the same set\n");
	printf("6. Display every representative\n");
	printf("7. Display all sets and members\n");
	printf("8. Insert a new singleton element\n");
	printf("9. Count sets\n");
	printf("10. Display size of every set\n");
	printf("11. Delete an element\n");
	printf("0. Exit\n");
	printf("Enter your choice: ");
}

int main(void)
{
	int choice, element;

	do {
		menu();
		scanf("%d", &choice);

		switch (choice) {
		case 1:
			create_collection();
			break;
		case 2:
		case 8:
			printf("Enter the element: ");
			scanf("%d", &element);
			make_set(element);
			break;
		case 3:
			printf("Enter the element: ");
			scanf("%d", &element);
			if (exists(element)) {
				printf("Representative = %d\n", find_root(element));
			} else {
				printf("Element does not exist.\n");
			}
			break;
		case 4:
			union_sets();
			break;
		case 5:
			check_same_set();
			break;
		case 6:
			display_representatives();
			break;
		case 7:
			display_sets();
			break;
		case 9:
			printf("Number of sets = %d\n", number_of_sets);
			break;
		case 10:
			display_set_sizes();
			break;
		case 11:
			delete_element();
			break;
		case 0:
			printf("Exiting...\n");
			break;
		default:
			printf("Invalid choice.\n");
		}
	} while (choice != 0);

	return 0;
}