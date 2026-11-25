

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum { RED, BLACK } Color;

typedef struct RBNode {
    int key;
    Color color;
    struct RBNode *left;
    struct RBNode *right;
    struct RBNode *parent;
} RBNode;

typedef struct RBTree {
    RBNode *root;
    RBNode *NIL;
} RBTree;

RBTree* createRBTree() {
    RBTree *tree = (RBTree*)malloc(sizeof(RBTree));
    tree->NIL = (RBNode*)malloc(sizeof(RBNode));
    tree->NIL->color = BLACK;
    tree->NIL->left = NULL;
    tree->NIL->right = NULL;
    tree->NIL->parent = NULL;
    tree->root = tree->NIL;
    return tree;
}

/* Rotations */
void leftRotate(RBTree *tree, RBNode *x) {
    RBNode *y = x->right;
    x->right = y->left;
    if (y->left != tree->NIL) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == tree->NIL) {
        tree->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void rightRotate(RBTree *tree, RBNode *x) {
    RBNode *y = x->left;
    x->left = y->right;
    if (y->right != tree->NIL) {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == tree->NIL) {
        tree->root = y;
    } else if (x == x->parent->right) {
        x->parent->right = y;
    } else {
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
}

/* Insertion and Fixup */
void insertFixup(RBTree *tree, RBNode *z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            RBNode *uncle = z->parent->parent->right;
            if (uncle->color == RED) {
                z->parent->color = BLACK;
                uncle->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(tree, z->parent->parent);
            }
        } else {
            RBNode *uncle = z->parent->parent->left;
            if (uncle->color == RED) {
                z->parent->color = BLACK;
                uncle->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(tree, z->parent->parent);
            }
        }
    }
    tree->root->color = BLACK;
}

void insert(RBTree *tree, int key) {
    RBNode *z = (RBNode*)malloc(sizeof(RBNode));
    z->key = key;
    z->color = RED;
    z->left = tree->NIL;
    z->right = tree->NIL;

    RBNode *y = tree->NIL;
    RBNode *x = tree->root;

    while (x != tree->NIL) {
        y = x;
        if (z->key < x->key) {
            x = x->left;
        } else if (z->key > x->key) {
            x = x->right;
        } else {
            printf("Duplicate key %d ignored.\n", key);
            free(z);
            return;
        }
    }

    z->parent = y;
    if (y == tree->NIL) {
        tree->root = z;
    } else if (z->key < y->key) {
        y->left = z;
    } else {
        y->right = z;
    }

    insertFixup(tree, z);
}

/* Deletion Helpers and Fixup */
void transplant(RBTree *tree, RBNode *u, RBNode *v) {
    if (u->parent == tree->NIL) {
        tree->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

RBNode* treeMinimum(RBTree *tree, RBNode *x) {
    while (x->left != tree->NIL) {
        x = x->left;
    }
    return x;
}

RBNode* treeMaximum(RBTree *tree, RBNode *x) {
    while (x->right != tree->NIL) {
        x = x->right;
    }
    return x;
}

void deleteFixup(RBTree *tree, RBNode *x) {
    while (x != tree->root && x->color == BLACK) {
        if (x == x->parent->left) {
            RBNode *w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                leftRotate(tree, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    rightRotate(tree, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                leftRotate(tree, x->parent);
                x = tree->root;
            }
        } else {
            RBNode *w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rightRotate(tree, x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    leftRotate(tree, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rightRotate(tree, x->parent);
                x = tree->root;
            }
        }
    }
    x->color = BLACK;
}

void deleteKey(RBTree *tree, int key) {
    RBNode *z = tree->root;
    while (z != tree->NIL && z->key != key) {
        if (key < z->key) z = z->left;
        else z = z->right;
    }

    if (z == tree->NIL) {
        printf("Key %d not found in the tree.\n", key);
        return;
    }

    RBNode *y = z;
    RBNode *x;
    Color yOriginalColor = y->color;

    if (z->left == tree->NIL) {
        x = z->right;
        transplant(tree, z, z->right);
    } else if (z->right == tree->NIL) {
        x = z->left;
        transplant(tree, z, z->left);
    } else {
        y = treeMinimum(tree, z->right);
        yOriginalColor = y->color;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;
        } else {
            transplant(tree, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(tree, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    free(z);

    if (yOriginalColor == BLACK) {
        deleteFixup(tree, x);
    }
    printf("Key %d deleted successfully.\n", key);
}

/* Search */
RBNode* search(RBTree *tree, int key) {
    RBNode *curr = tree->root;
    while (curr != tree->NIL && curr->key != key) {
        if (key < curr->key) curr = curr->left;
        else curr = curr->right;
    }
    return curr;
}

/* Traversals */
void inorderTraversal(RBTree *tree, RBNode *x) {
    if (x != tree->NIL) {
        inorderTraversal(tree, x->left);
        printf("%d(%c) ", x->key, (x->color == RED) ? 'R' : 'B');
        inorderTraversal(tree, x->right);
    }
}

void preorderTraversal(RBTree *tree, RBNode *x) {
    if (x != tree->NIL) {
        printf("%d(%c) ", x->key, (x->color == RED) ? 'R' : 'B');
        preorderTraversal(tree, x->left);
        preorderTraversal(tree, x->right);
    }
}

void levelOrderTraversal(RBTree *tree) {
    if (tree->root == tree->NIL) {
        printf("Tree is empty.\n");
        return;
    }
    RBNode **queue = (RBNode**)malloc(1000 * sizeof(RBNode*));
    int front = 0, rear = 0;
    queue[rear++] = tree->root;

    while (front < rear) {
        RBNode *curr = queue[front++];
        printf("%d(%c) ", curr->key, (curr->color == RED) ? 'R' : 'B');
        if (curr->left != tree->NIL) queue[rear++] = curr->left;
        if (curr->right != tree->NIL) queue[rear++] = curr->right;
    }
    free(queue);
}

/* Red-Black Tree Property Verification */
int checkBlackHeight(RBTree *tree, RBNode *node) {
    if (node == tree->NIL) {
        return 1;
    }

    /* Property 4: Red node cannot have red children */
    if (node->color == RED) {
        if (node->left->color == RED || node->right->color == RED) {
            return -1;
        }
    }

    int leftBlackHeight = checkBlackHeight(tree, node->left);
    int rightBlackHeight = checkBlackHeight(tree, node->right);

    /* Property 5: Equal black height on all paths */
    if (leftBlackHeight == -1 || rightBlackHeight == -1 || leftBlackHeight != rightBlackHeight) {
        return -1;
    }

    return leftBlackHeight + (node->color == BLACK ? 1 : 0);
}

bool verifyRBProperties(RBTree *tree) {
    if (tree->root == tree->NIL) {
        return true;
    }
    /* Property 2: Root must be black */
    if (tree->root->color != BLACK) {
        return false;
    }
    /* Property 3: NIL leaf sentinel is black */
    if (tree->NIL->color != BLACK) {
        return false;
    }
    return checkBlackHeight(tree, tree->root) != -1;
}

int main() {
    RBTree *tree = createRBTree();
    int choice, key, n;

    while (1) {
        printf("\n=== Red-Black Tree Menu ===\n");
        printf("1. Insert a set of keys\n");
        printf("2. Delete a key\n");
        printf("3. Search for a key\n");
        printf("4. Display Inorder, Preorder, and Level-Order traversals\n");
        printf("5. Display Minimum and Maximum keys\n");
        printf("6. Verify Red-Black Tree properties\n");
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
                printf("Insertion complete. RBT Properties Valid: %s\n",
                       verifyRBProperties(tree) ? "YES" : "NO");
                break;

            case 2:
                printf("Enter key to delete: ");
                scanf("%d", &key);
                deleteKey(tree, key);
                printf("RBT Properties Valid: %s\n",
                       verifyRBProperties(tree) ? "YES" : "NO");
                break;

            case 3:
                printf("Enter key to search: ");
                scanf("%d", &key);
                RBNode *res = search(tree, key);
                if (res != tree->NIL) {
                    printf("Key %d FOUND (Color: %s).\n", key, (res->color == RED) ? "RED" : "BLACK");
                } else {
                    printf("Key %d NOT FOUND.\n", key);
                }
                break;

            case 4:
                if (tree->root == tree->NIL) {
                    printf("Red-Black Tree is empty.\n");
                } else {
                    printf("Inorder Traversal   : ");
                    inorderTraversal(tree, tree->root);
                    printf("\nPreorder Traversal  : ");
                    preorderTraversal(tree, tree->root);
                    printf("\nLevel-Order         : ");
                    levelOrderTraversal(tree);
                    printf("\n");
                }
                break;

            case 5:
                if (tree->root == tree->NIL) {
                    printf("Red-Black Tree is empty.\n");
                } else {
                    printf("Minimum Key: %d\n", treeMinimum(tree, tree->root)->key);
                    printf("Maximum Key: %d\n", treeMaximum(tree, tree->root)->key);
                }
                break;

            case 6:
                if (verifyRBProperties(tree)) {
                    printf("All Red-Black Tree properties are VALID.\n");
                } else {
                    printf("VIOLATION detected in Red-Black Tree properties!\n");
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
