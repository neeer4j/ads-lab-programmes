#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct node {
    int key, degree;
    struct node *parent, *child, *sibling;
};

typedef struct node Node;

Node* create(int x) {
    Node *n = malloc(sizeof(Node));
    n->key = x;
    n->degree = 0;
    n->parent = n->child = n->sibling = NULL;
    return n;
}

Node* merge(Node *h1, Node *h2) {
    Node *head = NULL, **tail = &head;

    while (h1 && h2) {
        if (h1->degree <= h2->degree) {
            *tail = h1;
            h1 = h1->sibling;
        } else {
            *tail = h2;
            h2 = h2->sibling;
        }
        tail = &((*tail)->sibling);
    }

    *tail = h1 ? h1 : h2;
    return head;
}

void link(Node *y, Node *z) {
    y->parent = z;
    y->sibling = z->child;
    z->child = y;
    z->degree++;
}

Node* unionHeap(Node *h1, Node *h2) {
    Node *head = merge(h1, h2);

    if (!head) return NULL;

    Node *prev = NULL, *curr = head;
    Node *next = curr->sibling;

    while (next) {
        if (curr->degree != next->degree ||
            (next->sibling &&
             next->sibling->degree == curr->degree)) {
            prev = curr;
            curr = next;
        }
        else if (curr->key <= next->key) {
            curr->sibling = next->sibling;
            link(next, curr);
        }
        else {
            if (prev == NULL)
                head = next;
            else
                prev->sibling = next;

            link(curr, next);
            curr = next;
        }

        next = curr->sibling;
    }

    return head;
}

Node* insert(Node *heap, int x) {
    return unionHeap(heap, create(x));
}

Node* findMin(Node *heap) {
    if (!heap) return NULL;

    Node *min = heap;
    while (heap) {
        if (heap->key < min->key)
            min = heap;
        heap = heap->sibling;
    }
    return min;
}

Node* extractMin(Node **heap) {
    if (!*heap) return NULL;

    Node *min = *heap, *prev = NULL;
    Node *temp = *heap, *p = NULL;

    while (temp) {
        if (temp->key < min->key) {
            min = temp;
            p = prev;
        }
        prev = temp;
        temp = temp->sibling;
    }

    if (p == NULL)
        *heap = min->sibling;
    else
        p->sibling = min->sibling;

    Node *child = min->child, *rev = NULL;

    while (child) {
        Node *next = child->sibling;
        child->sibling = rev;
        child->parent = NULL;
        rev = child;
        child = next;
    }

    *heap = unionHeap(*heap, rev);
    min->sibling = NULL;
    return min;
}

Node* search(Node *root, int x) {
    while (root) {
        if (root->key == x)
            return root;

        Node *found = search(root->child, x);
        if (found) return found;

        root = root->sibling;
    }
    return NULL;
}

void decreasekey(Node *heap,int old,int new){
    Node *n=search(heap,old);
    if(!n){
        printf("Element not found");
        return;

    }
    if(new>old){
        printf("New key must be smaller:");
        return ;
    }

n->key=new;
Node *p=n->parent;
while (p&& n->key < p->key)
{
    int t=n->key;
    n->key=p->key;
    p->key=t;
    n=p;
    p=n->parent;
}
printf("Key decreased Successfully");
}
void deleteNode(Node **heap,int x){
    Node *n=search(*heap,x);
    if(!n){
        printf("Element not found");
        return;
    }
    decreasekey(*heap,x,INT_MIN);
    Node *temp=extractMin(heap);
    free(temp);
    printf("Element deleted");
}

void displayTree(Node *root,int level){
    if(!root)return;
    for (int i=0;i<level;i++)
    printf("");
    printf("%d\n",root->key);
    Node *child=root->child;
    while(child){
        displayTree(child,level+1);
        child=child->sibling;

    }
}
void display(Node *heap){
    if(!heap){
        printf("Heap is empty");
        return;
    }

printf("Root List:");
Node *temp = heap;
while(temp){
    printf("%d(B %d))",temp->key,temp->degree);
    temp=temp->sibling;
}
printf("Binomial Trees:\n");
temp=heap;
while(temp){
    printf("B%d \n",temp->degree);
    displayTree(temp,0);
    temp=temp->sibling;
}
}

int main() {
    Node *heap = NULL;
    int ch, n, x, y;

    while (1) {
        printf("\n=== BINOMIAL HEAP ===\n");
        printf("1. Create Empty Heap\n");
        printf("2. Insert Elements\n");
        printf("3. Display Heap\n");
        printf("4. Find Minimum\n");
        printf("5. Merge Two Heaps\n");
        printf("6. Extract Minimum\n");
        printf("7. Decrease Key\n");
        printf("8. Delete Element\n");
        printf("9. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &ch);

        switch (ch) {
        case 1:
            heap = NULL;
            printf("Empty heap created\n");
            break;

        case 2:
            printf("Enter number of elements: ");
            scanf("%d", &n);

            for (int i = 0; i < n; i++) {
                printf("Enter element: ");
                scanf("%d", &x);
                heap = insert(heap, x);
            }
            break;

        case 3:
            display(heap);
            break;

        case 4: {
            Node *m = findMin(heap);
            if (m)
                printf("Minimum element: %d\n", m->key);
            else
                printf("Heap is empty\n");
            break;
        }

        case 5: {
            Node *h2 = NULL;

            printf("Enter number of elements in second heap: ");
            scanf("%d", &n);

            for (int i = 0; i < n; i++) {
                printf("Enter element: ");
                scanf("%d", &x);
                h2 = insert(h2, x);
            }

            heap = unionHeap(heap, h2);
            printf("Heaps merged successfully\n");
            break;
        }

        case 6: {
            Node *m = extractMin(&heap);
            if (m) {
                printf("Extracted minimum: %d\n", m->key);
                free(m);
            } else
                printf("Heap is empty\n");
            break;
        }

        case 7:
            printf("Enter old key and new key: ");
            scanf("%d%d", &x, &y);
            decreasekey(heap, x, y);
            break;

        case 8:
            printf("Enter element to delete: ");
            scanf("%d", &x);
            deleteNode(&heap, x);
            break;

        case 9:
            return 0;

        default:
            printf("Invalid choice\n");
        }
    }
}

