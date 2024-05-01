#ifndef TREE_H
#define TREE_H
#include "headers.h"
#include "PCB.h"
// Define the structure for the tree node
typedef struct Node {
    int start;
    int end;
    int empty;
    struct Node* r;
    struct Node* l;
    struct PCB P;
} Node;

// Function to create a new node
Node* createNode(int s,int e, struct PCB p) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    newNode->start = s;
    newNode->end = e;
    newNode->r = NULL;
    newNode->l = NULL;
    newNode->P=p;
    return newNode;
}
typedef struct Tree {
    Node*root;
    
} Tree;

void insertElement(struct PCB p,Tree* t,Node* root)

{
    if(t->root==NULL)
    {
        struct PCB dummy;
        dummy.pid=-1;
        t->root=createNode(0,1023,dummy);
        root=t->root;
    }
    else
    {
        if(root->end-root->start>p.memSize)
        {

        }
    }
    
}


#endif