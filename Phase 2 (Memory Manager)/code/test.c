#include <stdio.h>
#include <stdlib.h>
#include "Tree.h" // Assuming this includes all required declarations

int main()
{
    Tree t;

    t.root = NULL; // Initializing the tree root to NULL

    // Create PCB instances
    struct PCB pcb1;
    pcb1.pid = 1;
    pcb1.memSize = 500;
    struct PCB pcb2;
    pcb2.pid = 2;
    pcb2.memSize = 20;
    struct PCB pcb3;
    pcb3.pid = 3;
    pcb3.memSize = 30;
    struct PCB pcb4;
    pcb4.pid = 4;
    pcb4.memSize = 500;

    // Insert elements into the tree
    struct PCB dummy;
    dummy.pid = -1;
    t.root = createNode(0, 1023, dummy, 1); // Create root node

    insertElement(pcb1, &t, t.root, 0);
    insertElement(pcb2, &t, t.root, 0);
    printf("**********\n");
    printTree(t.root);
    // Insert pcb3
    // Insert pcb2
    // insertElement(pcb3, &t, t.root, 0);
    delete (&t, t.root, 1);
     printf("**********\n");
     printTree(t.root);
      printf("**********\n");
    delete (&t, t.root, 2);
    insertElement(pcb4, &t, t.root, 0);
 delete (&t, t.root, 4);
    printTree(t.root);

    // Print the tree structure

    return 0;
}
