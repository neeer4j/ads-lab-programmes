
#include <stdio.h>
#include <stdlib.h>

struct node {
    int data;
    struct node *left, *right;
};

struct node* create(int x) {
    struct node *newnode = malloc(sizeof(struct node));
    newnode->data = x;
    newnode->left = newnode->right = NULL;
    return newnode;
}

struct node* insert(struct node *root, int x) {
    if (root == NULL)
        return create(x);

    if (x < root->data)
        root->left = insert(root->left, x);
    else if (x > root->data)
        root->right = insert(root->right, x);

    return root;
}

void search(struct node *root, int x) {
    if (root == NULL)
        printf("%d not found\n", x);
    else if (root->data == x)
        printf("%d found\n", x);
    else if (x < root->data)
        search(root->left, x);
    else
        search(root->right, x);
}

struct node* minimum(struct node *root) {
    while (root->left != NULL)
        root = root->left;
    return root;
}

struct node* delete(struct node *root, int x) {
    if (root == NULL)
        return root;

    if (x < root->data)
        root->left = delete(root->left, x);
    else if (x > root->data)
        root->right = delete(root->right, x);
    else {
        if (root->left == NULL) {
            struct node *temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL) {
            struct node *temp = root->left;
            free(root);
            return temp;
        }

        struct node *temp = minimum(root->right);
        root->data = temp->data;
        root->right = delete(root->right, temp->data);
    }

    return root;
}

void inorder(struct node *root) {
    if (root != NULL) {
        inorder(root->left);
        printf("%d ", root->data);
        inorder(root->right);
    }
}

int main() {
    struct node *root = NULL;
    int choice, n, x;

    while (1) {
        printf("\nBINARY SEARCH TREE \n");
        printf("1. Insert Elements\n");
        printf("2. Search Element\n");
        printf("3. Delete Elements\n");
        printf("4. Inorder Traversal\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 5)
            break;

        switch (choice) {
        case 1:
            printf("Enter number of elements: ");
            scanf("%d", &n);

            printf("Enter elements: ");
            for (int i = 0; i < n; i++) {
                scanf("%d", &x);
                root = insert(root, x);
            }
            printf("\n");
            break;

        case 2:
            printf("Enter element to search: ");
            scanf("%d", &x);
            search(root, x);
            break;

        case 3:
            printf("Enter number of elements to delete: ");
            scanf("%d", &n);

            for (int i = 0; i < n; i++) {
                printf("Enter element to delete: ");
                scanf("%d", &x);
                root = delete(root, x);

                printf("Inorder after deletion: ");
                inorder(root);
                printf("\n");
            }
            break;

        case 4:
            printf("Inorder traversal: ");
            inorder(root);
            printf("\n");
            break;

        default:
            printf("Invalid choice\n");
            break;
        }
    }

    return 0;
}
