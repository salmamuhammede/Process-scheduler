#ifndef TREE_H
#define TREE_H
#include "headers.h"
#include "PCB.h"
// Define the structure for the tree node
typedef struct Node
{
    int start;
    int end;
    int empty;
    struct Node *r;
    struct Node *l;
    struct Node *parent;
    struct PCB P;
    int size;
    int child;
    int left;
    int right;
} Node;

// Function to create a new node
Node *createNode(int s, int e, struct PCB p, int emp)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }

    newNode->start = s;
    newNode->end = e;
    newNode->r = NULL;
    newNode->l = NULL;
    newNode->P = p;
    newNode->size = e - s + 1;
    newNode->empty = emp;
    newNode->child = 0;
    newNode->left = 0;
    newNode->right = 0;
    return newNode;
}
typedef struct Tree
{
    Node *root;

} Tree;
void FindMinLeaf(Node *root, int value, Node **minLeaf)
{
    if (root == NULL)
    {

        return;
    }

    // If current node is a leaf and greater than the given value
    if (root->l == NULL && root->r == NULL && root->empty == 1 && root->size >= value)
    {

        if (*minLeaf == NULL || root->size < (*minLeaf)->size)
        {
            *minLeaf = root;
        }
    }

    // Recursively search left and right subtrees

    FindMinLeaf(root->l, value, minLeaf);
    FindMinLeaf(root->r, value, minLeaf);
}

void WriteStringToFile(const char *filename, const char *str)
{
    FILE *file = fopen(filename, "a");
    if (file == NULL)
    {
        printf("Failed to open file: %s\n", filename);
        return;
    }
    fprintf(file, "%s", str);

    fclose(file);
}

int insertElement(struct PCB p, Tree *t, Node *root, int time)

{
    if (t->root == NULL)
    {
        printf("hallllllllllllllllllll\n");
        struct PCB dummy;
        dummy.pid = -1;
        t->root = createNode(0, 1023, dummy, 1);
    }

    Node *newNode = NULL;
    FindMinLeaf(t->root, p.memSize, &newNode);

    if (newNode != NULL)
    {

        insert(p, t, newNode, p.memSize, time);
        return 1;
    }
    else
    {
        return 0;
    }
}
void insert(struct PCB p, Tree *t, Node *root, int value, int time)
{
    if ((root->size == 8 && value <= root->size) || (value <= root->size && value > (root->size) / 2))
    {
        root->P = p;
        root->empty = 0;
        char str[100];
        snprintf(str, sizeof(str), "At time %d allocated %d bytes for process %d from %d to %d\n", time, p.memSize, p.pid, root->start, root->end);
        WriteStringToFile("memory.log", str);
        while (root != NULL && root->parent != NULL)
        {
            root->parent->child = 1;
            root = root->parent;
        }
    }
    else
    {
        struct PCB dummy;
        dummy.pid = -1;
        root->l = createNode(root->start, (root->end + root->start) / 2, dummy, 1);
        root->r = createNode((root->end + root->start) / 2 + 1, root->end, dummy, 1);
        root->l->parent = root;
        root->r->parent = root;
        root->l->left = 1;
        root->r->right = 1;
        insert(p, t, root->l, value, time);
    }
}

Node *Search(Node *root, int value)
{
    if (root == NULL)
    {
        // If the tree is empty, the value isn't found.
        return NULL;
    }

    if (root->P.pid == value)
    {
        // If the current node's value matches, it's found.
        return root;
    }

    // Recursively search both left and right subtrees.
    Node *leftResult = Search(root->l, value);
    Node *rightResult = Search(root->r, value);

    // Return the found node from either subtree (if any).
    if (leftResult != NULL)
    {
        return leftResult;
    }
    else if (rightResult != NULL)
    {
        return rightResult;
    }

    // If not found in either subtree, return NULL.
    return NULL;
}

void killparent(Tree *t, Node *root)
{
    if (root->r == NULL)
        printf("right null\n");
    if (root->l == NULL)
    {
        printf("left NULL\n");
    }
    if ((root->r == NULL && root->l == NULL))
    {
        printf("delete because of nulls");
        delete (t, root, root->P.pid, 1);
    }
    else if ((root->r == NULL && root->l != NULL && root->l->empty == 1 && root->l->child == 0))
    {
        printf("delete because of right null and left empty");
        delete (t, root, root->P.pid, 1);
    }
    else if ((root->l == NULL && root->r != NULL && root->r->empty == 1 && root->r->child == 0))
    {
        printf("delete because of left null and right empty");
        delete (t, root, root->P.pid, 1);
    }
    else if ((root->l != NULL && root->l->empty == 1 && root->l->child == 0 && root->r != NULL && root->r->empty == 1 && root->r->child == 0))
    {
        printf("delete because of right  and left empty");
        delete (t, root, root->P.pid, 1);
    }
}
void printTreeHelper(Node *root, const char *relation)
{
    if (root == NULL)
        return;

    // Recursive call on the left child
    printTreeHelper(root->l, "left");

    // Print current node information
    printf("Node [%d, %d] Size: %d Empty: %d PCB memSize: %d havechild:%d pid:%d (%s child)\n",
           root->start, root->end, root->size, root->empty, root->P.memSize, root->child, root->P.pid, relation);

    // Recursive call on the right child
    printTreeHelper(root->r, "right");
}

void printTree(Node *root)
{
    printTreeHelper(root, "root");
}

int delete(Tree *t, Node *root, int value, int flag)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    Node *parent = (Node *)malloc(sizeof(Node));
    if (flag == 0)
        newNode = Search(root, value);
    else
    {

        newNode = root;
    }
    if (newNode != NULL)
    {

        parent = newNode->parent;

        if (parent != NULL)
        {
            if (parent->l != NULL && parent->l->P.pid == newNode->P.pid && newNode->left == 1)
            {
                printf("%d\n", newNode->P.pid);

                printf("hallo\n");
                free(parent->l);
                parent->l = NULL;
            }
            else if (parent->r != NULL && parent->r->P.pid == newNode->P.pid && newNode->right == 1)
            {
                printf("%d\n", newNode->P.pid);
                printf("hi\n");
                free(parent->r);
                parent->r = NULL;
            }

            killparent(t, parent);
        }
        else
        {
            free(t->root);
            t->root = NULL;
        }

        return 1;
    }
    else
        return 0;
}

#endif
