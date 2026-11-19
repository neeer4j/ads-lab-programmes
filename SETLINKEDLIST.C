#include <stdio.h>
#include <stdlib.h>

struct Node
{
    int data;
    struct Node *next;
};

struct Node *insert(struct Node *head, int value)
{
    struct Node *newNode;
    struct Node *temp;

    temp = head;
    while (temp != NULL)
    {
        if (temp->data == value)
        {
            printf("This value already exists in the set.\n");
            return head;
        }
        temp = temp->next;
    }

    newNode = (struct Node *)malloc(sizeof(struct Node));
    newNode->data = value;
    newNode->next = NULL;

    if (head == NULL)
        return newNode;

    temp = head;
    while (temp->next != NULL)
        temp = temp->next;

    temp->next = newNode;
    return head;
}

struct Node *deleteElement(struct Node *head, int value)
{
    struct Node *temp;
    struct Node *previous;

    temp = head;
    previous = NULL;

    while (temp != NULL && temp->data != value)
    {
        previous = temp;
        temp = temp->next;
    }

    if (temp == NULL)
    {
        printf("The value was not found in the set.\n");
        return head;
    }

    if (previous == NULL)
        head = temp->next;
    else
        previous->next = temp->next;

    free(temp);
    printf("The value was removed successfully.\n");
    return head;
}

int search(struct Node *head, int value)
{
    while (head != NULL)
    {
        if (head->data == value)
            return 1;
        head = head->next;
    }
    return 0;
}

void display(struct Node *head)
{
    if (head == NULL)
    {
        printf("The set is empty.\n");
        return;
    }

    printf("{ ");
    while (head != NULL)
    {
        printf("%d ", head->data);
        head = head->next;
    }
    printf("}\n");
}

struct Node *copySet(struct Node *source)
{
    struct Node *result = NULL;

    while (source != NULL)
    {
        result = insert(result, source->data);
        source = source->next;
    }
    return result;
}

struct Node *unionSet(struct Node *a, struct Node *b)
{
    struct Node *result = copySet(a);

    while (b != NULL)
    {
        result = insert(result, b->data);
        b = b->next;
    }
    return result;
}

struct Node *intersectionSet(struct Node *a, struct Node *b)
{
    struct Node *result = NULL;

    while (a != NULL)
    {
        if (search(b, a->data))
            result = insert(result, a->data);
        a = a->next;
    }
    return result;
}

struct Node *differenceSet(struct Node *a, struct Node *b)
{
    struct Node *result = NULL;

    while (a != NULL)
    {
        if (!search(b, a->data))
            result = insert(result, a->data);
        a = a->next;
    }
    return result;
}

int count(struct Node *head)
{
    int total = 0;

    while (head != NULL)
    {
        total++;
        head = head->next;
    }
    return total;
}

int equal(struct Node *a, struct Node *b)
{
    if (count(a) != count(b))
        return 0;

    while (a != NULL)
    {
        if (!search(b, a->data))
            return 0;
        a = a->next;
    }
    return 1;
}

int subset(struct Node *a, struct Node *b)
{
    while (a != NULL)
    {
        if (!search(b, a->data))
            return 0;
        a = a->next;
    }
    return 1;
}

void minimumMaximum(struct Node *head)
{
    int minimum, maximum;

    if (head == NULL)
    {
        printf("The set is empty.\n");
        return;
    }

    minimum = maximum = head->data;
    head = head->next;

    while (head != NULL)
    {
        if (head->data < minimum)
            minimum = head->data;
        if (head->data > maximum)
            maximum = head->data;
        head = head->next;
    }

    printf("Lowest value = %d\n", minimum);
    printf("Highest value = %d\n", maximum);
}

void freeSet(struct Node *head)
{
    struct Node *temp;

    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}

void readSet(struct Node **head, char name)
{
    int n, value, i;

    printf("How many elements should be added to Set %c? ", name);
    scanf("%d", &n);

    for (i = 0; i < n; i++)
    {
        printf("Enter value %d: ", i + 1);
        scanf("%d", &value);
        *head = insert(*head, value);
    }
}

int main()
{
    struct Node *setA = NULL;
    struct Node *setB = NULL;
    struct Node *result;
    int choice, value;

    readSet(&setA, 'A');
    readSet(&setB, 'B');

    do
    {
        printf("\n1. Add a value to Set A\n");
        printf("2. Remove a value from Set A\n");
        printf("3. Search in Set A\n");
        printf("4. Display both sets\n");
        printf("5. Union of A and B\n");
        printf("6. Intersection of A and B\n");
        printf("7. Difference A - B\n");
        printf("8. Compare whether the sets are equal\n");
        printf("9. Check whether A is a subset of B\n");
        printf("10. Show the cardinality of both sets\n");
        printf("11. Find the minimum and maximum values\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                printf("Enter value: ");
                scanf("%d", &value);
                setA = insert(setA, value);
                break;
            case 2:
                printf("Enter value: ");
                scanf("%d", &value);
                setA = deleteElement(setA, value);
                break;
            case 3:
                printf("Enter value: ");
                scanf("%d", &value);
                if (search(setA, value)) printf("The value is present in Set A.\n");
                else printf("The value is not present in Set A.\n");
                break;
            case 4:
                printf("Set A: "); display(setA);
                printf("Set B: "); display(setB);
                break;
            case 5:
                result = unionSet(setA, setB); printf("Union result: "); display(result); freeSet(result); break;
            case 6:
                result = intersectionSet(setA, setB); printf("Intersection result: "); display(result); freeSet(result); break;
            case 7:
                result = differenceSet(setA, setB); printf("Difference result: "); display(result); freeSet(result); break;
            case 8:
                if (equal(setA, setB)) printf("The sets are equal.\n"); else printf("The sets are not equal.\n");
                break;
            case 9:
                if (subset(setA, setB)) printf("Set A is contained in Set B.\n"); else printf("Set A is not contained in Set B.\n");
                break;
            case 10:
                printf("Cardinality of A = %d\n", count(setA));
                printf("Cardinality of B = %d\n", count(setB));
                break;
            case 11:
                printf("Set A\n"); minimumMaximum(setA);
                printf("Set B\n"); minimumMaximum(setB);
                break;
            case 0:
                break;
            default:
                printf("Invalid selection.\n");
        }
    } while (choice != 0);

    freeSet(setA);
    freeSet(setB);
    return 0;
}