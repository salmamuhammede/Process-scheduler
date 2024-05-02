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
    pcb1.memSize = 70;
    struct PCB pcb2;
    pcb2.pid = 2;
    pcb2.memSize = 35;
    struct PCB pcb3;
    pcb3.pid = 3;
    pcb3.memSize = 80;
    struct PCB pcb4;
    pcb4.pid = 4;
    pcb4.memSize = 1;

    // Insert elements into the tree
    struct PCB dummy;
    dummy.pid = -1;
    t.root = createNode(0, 511, dummy, 1); // Create root node
    insertElement(pcb1, &t, t.root);
    insertElement(pcb2, &t, t.root);
   

    // Insert pcb3
    // Insert pcb2
    insertElement(pcb3, &t, t.root);
    //printTree(t.root); // Insert pcb3
    delete (&t,t.root, 1);
    insertElement(pcb4, &t, t.root);

    // Print the tree structure
    printTree(t.root);

    return 0;
}
