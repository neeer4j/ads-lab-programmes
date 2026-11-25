

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct BTreeNode {
    int *keys;
    int t;
    struct BTreeNode **children;
    int n;
    bool leaf;
} BTreeNode;

typedef struct BTree {
    BTreeNode *root;
    int t;
} BTree;

BTreeNode* createNode(int t, bool leaf) {
    BTreeNode *newNode = (BTreeNode*)malloc(sizeof(BTreeNode));
    newNode->t = t;
    newNode->leaf = leaf;
    newNode->keys = (int*)malloc((2 * t - 1) * sizeof(int));
    newNode->children = (BTreeNode**)malloc((2 * t) * sizeof(BTreeNode*));
    newNode->n = 0;
    return newNode;
}

BTree* createBTree(int t) {
    BTree *tree = (BTree*)malloc(sizeof(BTree));
    tree->t = t;
    tree->root = createNode(t, true);
    return tree;
}

/* Search Operation */
BTreeNode* search(BTreeNode *x, int k) {
    int i = 0;
    while (i < x->n && k > x->keys[i]) {
        i++;
    }
    if (i < x->n && x->keys[i] == k) {
        return x;
    }
    if (x->leaf) {
        return NULL;
    }
    return search(x->children[i], k);
}

/* Node Splitting During Insertion */
void splitChild(BTreeNode *x, int i) {
    int t = x->t;
    BTreeNode *y = x->children[i];
    BTreeNode *z = createNode(t, y->leaf);
    z->n = t - 1;

    for (int j = 0; j < t - 1; j++) {
        z->keys[j] = y->keys[j + t];
    }

    if (!y->leaf) {
        for (int j = 0; j < t; j++) {
            z->children[j] = y->children[j + t];
        }
    }

    y->n = t - 1;

    for (int j = x->n; j >= i + 1; j--) {
        x->children[j + 1] = x->children[j];
    }
    x->children[i + 1] = z;

    for (int j = x->n - 1; j >= i; j--) {
        x->keys[j + 1] = x->keys[j];
    }
    x->keys[i] = y->keys[t - 1];
    x->n = x->n + 1;
}

void insertNonFull(BTreeNode *x, int k) {
    int i = x->n - 1;

    if (x->leaf) {
        while (i >= 0 && x->keys[i] > k) {
            x->keys[i + 1] = x->keys[i];
            i--;
        }
        x->keys[i + 1] = k;
        x->n = x->n + 1;
    } else {
        while (i >= 0 && x->keys[i] > k) {
            i--;
        }
        i++;
        if (x->children[i]->n == 2 * x->t - 1) {
            splitChild(x, i);
            if (x->keys[i] < k) {
                i++;
            }
        }
        insertNonFull(x->children[i], k);
    }
}

void insert(BTree *tree, int k) {
    BTreeNode *r = tree->root;
    if (r->n == 2 * tree->t - 1) {
        BTreeNode *s = createNode(tree->t, false);
        tree->root = s;
        s->children[0] = r;
        splitChild(s, 0);
        insertNonFull(s, k);
    } else {
        insertNonFull(r, k);
    }
}

/* Deletion Helpers */
int findKey(BTreeNode *x, int k) {
    int idx = 0;
    while (idx < x->n && x->keys[idx] < k) {
        idx++;
    }
    return idx;
}

int getPredecessor(BTreeNode *x, int idx) {
    BTreeNode *cur = x->children[idx];
    while (!cur->leaf) {
        cur = cur->children[cur->n];
    }
    return cur->keys[cur->n - 1];
}

int getSuccessor(BTreeNode *x, int idx) {
    BTreeNode *cur = x->children[idx + 1];
    while (!cur->leaf) {
        cur = cur->children[0];
    }
    return cur->keys[0];
}

void borrowFromPrev(BTreeNode *x, int idx) {
    BTreeNode *child = x->children[idx];
    BTreeNode *sibling = x->children[idx - 1];

    for (int i = child->n - 1; i >= 0; --i) {
        child->keys[i + 1] = child->keys[i];
    }

    if (!child->leaf) {
        for (int i = child->n; i >= 0; --i) {
            child->children[i + 1] = child->children[i];
        }
    }

    child->keys[0] = x->keys[idx - 1];

    if (!child->leaf) {
        child->children[0] = sibling->children[sibling->n];
    }

    x->keys[idx - 1] = sibling->keys[sibling->n - 1];

    child->n += 1;
    sibling->n -= 1;
}

void borrowFromNext(BTreeNode *x, int idx) {
    BTreeNode *child = x->children[idx];
    BTreeNode *sibling = x->children[idx + 1];

    child->keys[child->n] = x->keys[idx];

    if (!child->leaf) {
        child->children[child->n + 1] = sibling->children[0];
    }

    x->keys[idx] = sibling->keys[0];

    for (int i = 1; i < sibling->n; ++i) {
        sibling->keys[i - 1] = sibling->keys[i];
    }

    if (!sibling->leaf) {
        for (int i = 1; i <= sibling->n; ++i) {
            sibling->children[i - 1] = sibling->children[i];
        }
    }

    child->n += 1;
    sibling->n -= 1;
}

void merge(BTreeNode *x, int idx) {
    int t = x->t;
    BTreeNode *child = x->children[idx];
    BTreeNode *sibling = x->children[idx + 1];

    child->keys[t - 1] = x->keys[idx];

    for (int i = 0; i < sibling->n; ++i) {
        child->keys[i + t] = sibling->keys[i];
    }

    if (!child->leaf) {
        for (int i = 0; i <= sibling->n; ++i) {
            child->children[i + t] = sibling->children[i];
        }
    }

    for (int i = idx + 1; i < x->n; ++i) {
        x->keys[i - 1] = x->keys[i];
    }

    for (int i = idx + 2; i <= x->n; ++i) {
        x->children[i - 1] = x->children[i];
    }

    child->n += sibling->n + 1;
    x->n--;

    free(sibling->keys);
    free(sibling->children);
    free(sibling);
}

void fill(BTreeNode *x, int idx) {
    int t = x->t;
    if (idx != 0 && x->children[idx - 1]->n >= t) {
        borrowFromPrev(x, idx);
    } else if (idx != x->n && x->children[idx + 1]->n >= t) {
        borrowFromNext(x, idx);
    } else {
        if (idx != x->n) {
            merge(x, idx);
        } else {
            merge(x, idx - 1);
        }
    }
}

void deleteFromNode(BTreeNode *x, int k) {
    int t = x->t;
    int idx = findKey(x, k);

    if (idx < x->n && x->keys[idx] == k) {
        if (x->leaf) {
            for (int i = idx + 1; i < x->n; ++i) {
                x->keys[i - 1] = x->keys[i];
            }
            x->n--;
        } else {
            if (x->children[idx]->n >= t) {
                int pred = getPredecessor(x, idx);
                x->keys[idx] = pred;
                deleteFromNode(x->children[idx], pred);
            } else if (x->children[idx + 1]->n >= t) {
                int succ = getSuccessor(x, idx);
                x->keys[idx] = succ;
                deleteFromNode(x->children[idx + 1], succ);
            } else {
                merge(x, idx);
                deleteFromNode(x->children[idx], k);
            }
        }
    } else {
        if (x->leaf) {
            printf("Key %d does not exist in the tree.\n", k);
            return;
        }

        bool flag = (idx == x->n);
        if (x->children[idx]->n < t) {
            fill(x, idx);
        }

        if (flag && idx > x->n) {
            deleteFromNode(x->children[idx - 1], k);
        } else {
            deleteFromNode(x->children[idx], k);
        }
    }
}

void deleteKey(BTree *tree, int k) {
    if (!tree->root || tree->root->n == 0) {
        printf("The B-Tree is empty.\n");
        return;
    }

    deleteFromNode(tree->root, k);

    if (tree->root->n == 0) {
        BTreeNode *tmp = tree->root;
        if (tree->root->leaf) {
            tree->root = createNode(tree->t, true);
        } else {
            tree->root = tree->root->children[0];
        }
        free(tmp->keys);
        free(tmp->children);
        free(tmp);
    }
}

/* Traversals & Properties */
void inorderTraversal(BTreeNode *x) {
    if (x == NULL) return;
    int i;
    for (i = 0; i < x->n; i++) {
        if (!x->leaf) {
            inorderTraversal(x->children[i]);
        }
        printf("%d ", x->keys[i]);
    }
    if (!x->leaf) {
        inorderTraversal(x->children[i]);
    }
}

void levelOrderTraversal(BTreeNode *root) {
    if (!root || root->n == 0) {
        printf("B-Tree is empty.\n");
        return;
    }
    BTreeNode **queue = (BTreeNode**)malloc(1000 * sizeof(BTreeNode*));
    int front = 0, rear = 0, level = 0;
    queue[rear++] = root;

    while (front < rear) {
        int levelSize = rear - front;
        printf("Level %d: ", level++);
        for (int i = 0; i < levelSize; i++) {
            BTreeNode *curr = queue[front++];
            printf("[");
            for (int j = 0; j < curr->n; j++) {
                printf("%d%s", curr->keys[j], (j < curr->n - 1) ? ", " : "");
            }
            printf("] ");
            if (!curr->leaf) {
                for (int j = 0; j <= curr->n; j++) {
                    queue[rear++] = curr->children[j];
                }
            }
        }
        printf("\n");
    }
    free(queue);
}

int findMin(BTreeNode *root) {
    BTreeNode *curr = root;
    while (!curr->leaf) {
        curr = curr->children[0];
    }
    return curr->keys[0];
}

int findMax(BTreeNode *root) {
    BTreeNode *curr = root;
    while (!curr->leaf) {
        curr = curr->children[curr->n];
    }
    return curr->keys[curr->n - 1];
}

int getHeight(BTreeNode *root) {
    if (!root || root->n == 0) return -1;
    int height = 0;
    BTreeNode *curr = root;
    while (!curr->leaf) {
        height++;
        curr = curr->children[0];
    }
    return height;
}

int countNodes(BTreeNode *root) {
    if (!root || root->n == 0) return 0;
    int count = 1;
    if (!root->leaf) {
        for (int i = 0; i <= root->n; i++) {
            count += countNodes(root->children[i]);
        }
    }
    return count;
}

int main() {
    int t, choice, key, n;
    printf("Enter minimum degree (t >= 2) of B-Tree: ");
    scanf("%d", &t);
    if (t < 2) {
        printf("Minimum degree must be at least 2. Setting t = 2.\n");
        t = 2;
    }

    BTree *tree = createBTree(t);

    while (1) {
        printf("\n=== B-Tree Menu (Minimum Degree t = %d) ===\n", t);
        printf("1. Insert a set of keys\n");
        printf("2. Delete a key\n");
        printf("3. Search for a key\n");
        printf("4. Display Inorder and Level-Order traversals\n");
        printf("5. Display Minimum and Maximum keys\n");
        printf("6. Display Height and Total Number of Nodes\n");
        printf("7. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter number of keys to insert: ");
                scanf("%d", &n);
                printf("Enter %d keys: ", n);
                for (int i = 0; i < n; i++) {
                    scanf("%d", &key);
                    insert(tree, key);
                }
                printf("Keys inserted successfully.\n");
                break;

            case 2:
                printf("Enter key to delete: ");
                scanf("%d", &key);
                deleteKey(tree, key);
                break;

            case 3:
                printf("Enter key to search: ");
                scanf("%d", &key);
                if (tree->root->n > 0 && search(tree->root, key) != NULL) {
                    printf("Key %d FOUND in the B-Tree.\n", key);
                } else {
                    printf("Key %d NOT FOUND in the B-Tree.\n", key);
                }
                break;

            case 4:
                if (tree->root->n == 0) {
                    printf("B-Tree is empty.\n");
                } else {
                    printf("Inorder Traversal: ");
                    inorderTraversal(tree->root);
                    printf("\nLevel-Order Traversal:\n");
                    levelOrderTraversal(tree->root);
                }
                break;

            case 5:
                if (tree->root->n == 0) {
                    printf("B-Tree is empty.\n");
                } else {
                    printf("Minimum Key: %d\n", findMin(tree->root));
                    printf("Maximum Key: %d\n", findMax(tree->root));
                }
                break;

            case 6:
                if (tree->root->n == 0) {
                    printf("B-Tree is empty (Height: -1, Total Nodes: 0).\n");
                } else {
                    printf("Height of B-Tree: %d\n", getHeight(tree->root));
                    printf("Total Number of Nodes: %d\n", countNodes(tree->root));
                }
                break;

            case 7:
                printf("Exiting...\n");
                return 0;

            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}
