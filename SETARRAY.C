#include <stdio.h>
#define MAX 100
#define RESULT_MAX (MAX * 2)
int search(int set[], int size, int value)
{
	int i;

	for (i = 0; i < size; i++) {
		if (set[i] == value) {
			return i;
		}
	}

	return -1;
}

int insert(int set[], int *size, int capacity, int value)
{
	if (*size == capacity) {
		return 0;
	}

	if (search(set, *size, value) != -1) {
		return 0;
	}

	set[*size] = value;
	*size = *size + 1;
	return 1;
}

void display(int set[], int size)
{
	int i;

	printf("{ ");
	for (i = 0; i < size; i++) {
		printf("%d ", set[i]);
	}
	printf("}\n");
}

void createSet(int set[], int *size)
{
	int n, i, value;

	printf("How many values do you want to add? ");
	scanf("%d", &n);

	for (i = 0; i < n; i++) {
		printf("Value %d: ", i + 1);
		scanf("%d", &value);

		if (insert(set, size, MAX, value) == 0) {
			printf("That value already exists, or the set is full.\n");
		}
	}
}

void deleteElement(int set[], int *size, int value)
{
	int position, i;

	position = search(set, *size, value);
	if (position == -1) {
		printf("The requested value does not exist.\n");
		return;
	}

	for (i = position; i < *size - 1; i++) {
		set[i] = set[i + 1];
	}

	*size = *size - 1;
	printf("The value has been removed successfully.\n");
}

void unionSet(int first[], int firstSize, int second[], int secondSize,
	int result[], int *resultSize)
{
	int i;

	*resultSize = 0;
	for (i = 0; i < firstSize; i++) {
		insert(result, resultSize, RESULT_MAX, first[i]);
	}
	for (i = 0; i < secondSize; i++) {
		insert(result, resultSize, RESULT_MAX, second[i]);
	}
}

void intersectionSet(int first[], int firstSize, int second[], int secondSize,
	int result[], int *resultSize)
{
	int i;

	*resultSize = 0;
	for (i = 0; i < firstSize; i++) {
		if (search(second, secondSize, first[i]) != -1) {
			insert(result, resultSize, RESULT_MAX, first[i]);
		}
	}
}

void differenceSet(int first[], int firstSize, int second[], int secondSize,
	int result[], int *resultSize)
{
	int i;

	*resultSize = 0;
	for (i = 0; i < firstSize; i++) {
		if (search(second, secondSize, first[i]) == -1) {
			insert(result, resultSize, RESULT_MAX, first[i]);
		}
	}
}

int equalSet(int first[], int firstSize, int second[], int secondSize)
{
	int i;

	if (firstSize != secondSize) {
		return 0;
	}

	for (i = 0; i < firstSize; i++) {
		if (search(second, secondSize, first[i]) == -1) {
			return 0;
		}
	}

	return 1;
}

int subsetOf(int first[], int firstSize, int second[], int secondSize)
{
	int i;

	for (i = 0; i < firstSize; i++) {
		if (search(second, secondSize, first[i]) == -1) {
			return 0;
		}
	}

	return 1;
}

int main()
{
	int setA[MAX], setB[MAX], result[MAX];
	int sizeA = 0, sizeB = 0, resultSize;
	int choice, value, setChoice;

	printf("Create Set A\n");
	createSet(setA, &sizeA);

	printf("\nCreate Set B\n");
	createSet(setB, &sizeB);

	do {
		printf("1. Add an element\n");
		printf("2. Remove an element\n");
		printf("3. Find an element\n");
		printf("4. View a set\n");
		printf("5. Union of A and B\n");
		printf("6. Intersection of A and B\n");
		printf("7. Difference A - B\n");
		printf("8. Compare whether A and B are equal\n");
		printf("9. Check if A is a subset of B\n");
		printf("10. Find the size of a set\n");
		printf("0. Exit\n");
		printf("Enter your choice: ");
		scanf("%d", &choice);

		switch (choice) {
		case 1:
			printf("Enter 1 to add to A or 2 to add to B: ");
			scanf("%d", &setChoice);
			printf("Enter the element: ");
			scanf("%d", &value);
			if (setChoice == 1) {
				if (insert(setA, &sizeA, MAX, value) == 0) {
					printf("That value already exists, or the set is full.\n");
				}
			} else if (setChoice == 2) {
				if (insert(setB, &sizeB, value) == 0) {
					printf("That value already exists, or the set is full.\n");
				}
			} else {
				printf("Invalid set selection.\n");
			}
			break;

		case 2:
			printf("Enter 1 to remove from A or 2 to remove from B: ");
			scanf("%d", &setChoice);
			printf("Enter the element: ");
			scanf("%d", &value);
			if (setChoice == 1) {
				deleteElement(setA, &sizeA, value);
			} else if (setChoice == 2) {
				deleteElement(setB, &sizeB, value);
			} else {
				printf("Invalid set selection.\n");
			}
			break;

		case 3:
			printf("Enter 1 to search in A or 2 to search in B: ");
			scanf("%d", &setChoice);
			printf("Enter the element: ");
			scanf("%d", &value);
			if ((setChoice == 1 && search(setA, sizeA, value) != -1) ||
				(setChoice == 2 && search(setB, sizeB, value) != -1)) {
				printf("The element exists in the selected set.\n");
			} else {
				printf("The requested value does not exist.\n");
			}
			break;

		case 4:
			printf("Enter 1 to view A or 2 to view B: ");
			scanf("%d", &setChoice);
			if (setChoice == 1) {
				display(setA, sizeA);
			} else {
				display(setB, sizeB);
			}
			break;

		case 5:
			unionSet(setA, sizeA, setB, sizeB, result, &resultSize);
			printf("Union result = ");
			display(result, resultSize);
			break;

		case 6:
			intersectionSet(setA, sizeA, setB, sizeB, result, &resultSize);
			printf("Intersection result = ");
			display(result, resultSize);
			break;

		case 7:
			differenceSet(setA, sizeA, setB, sizeB, result, &resultSize);
			printf("Difference A - B = ");
			display(result, resultSize);
			break;

		case 8:
			if (equalSet(setA, sizeA, setB, sizeB)) {
				printf("The two sets match exactly.\n");
			} else {
				printf("The two sets are not the same.\n");
			}
			break;

		case 9:
			if (subsetOf(setA, sizeA, setB, sizeB)) {
				printf("Set A is contained in Set B.\n");
			} else {
				printf("Set A is not contained in Set B.\n");
			}
			break;

		case 10:
			printf("Enter 1 to check the size of A or 2 for B: ");
			scanf("%d", &setChoice);
			if (setChoice == 1) {
				printf("Size of A = %d\n", sizeA);
			} else {
				printf("Size of B = %d\n", sizeB);
			}
			break;

		case 0:
			printf("Exiting the program.\n");
			break;

		default:
			printf("That option is not valid.\n");
		}
	} while (choice != 0);

	return 0;
}
