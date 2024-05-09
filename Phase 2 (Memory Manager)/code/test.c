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
  pcb1.memSize = 8;
  struct PCB pcb2;
  pcb2.pid = 2;
  pcb2.memSize = 8;
  struct PCB pcb3;
  pcb3.pid = 3;
  pcb3.memSize = 8;
  struct PCB pcb4;
  pcb4.pid = 4;
  pcb4.memSize = 8;
  struct PCB pcb5;
  pcb5.pid = 5;
  pcb5.memSize = 8;
  struct PCB pcb6;
  pcb6.pid = 6;
  pcb6.memSize = 8;
  struct PCB pcb7;
  pcb7.pid = 7;
  pcb7.memSize = 8;
  struct PCB pcb8;
  pcb8.pid = 8;
  pcb8.memSize = 8;
  struct PCB pcb9;
  pcb9.pid = 9;
  pcb9.memSize = 64;

  // Insert elements into the tree
  struct PCB dummy;
  dummy.pid = -1;
  t.root = createNode(0, 63, dummy, 1); // Create root node

  insertElement(pcb1, &t, t.root, 0);
  insertElement(pcb2, &t, t.root, 0);
  insertElement(pcb3, &t, t.root, 0);
  insertElement(pcb4, &t, t.root, 0);
  insertElement(pcb5, &t, t.root, 0);
  insertElement(pcb6, &t, t.root, 0);
  insertElement(pcb7, &t, t.root, 0);
  insertElement(pcb8, &t, t.root, 0);
  insertElement(pcb9, &t, t.root, 0);
  printf("**********\n");
  printTree(t.root);
  // Insert pcb3
  // Insert pcb2
  // insertElement(pcb3, &t, t.root, 0);
  delete (&t, t.root, 5,0);
  insertElement(pcb9, &t, t.root, 0);
  printf("**********\n");
  printTree(t.root);
  printf("**********\n");
  delete (&t, t.root, 8,0);
  insertElement(pcb9, &t, t.root, 0);
  printf("**********\n");
  printTree(t.root);
  printf("**********\n");
  delete (&t, t.root, 3,0);
  insertElement(pcb9, &t, t.root, 0);
  printf("**********\n");
  printTree(t.root);
  printf("**********\n");
  delete (&t, t.root, 4,0);
  insertElement(pcb9, &t, t.root, 0);
  printf("**********\n");
  printTree(t.root);
  printf("**********\n");
  delete (&t, t.root, 1,0);
  insertElement(pcb9, &t, t.root, 0);
  printf("**********\n");
  printTree(t.root);
  printf("**********\n");
  delete (&t, t.root, 6,0);
  insertElement(pcb9, &t, t.root, 0);
  printf("**********\n");
  printTree(t.root);
  printf("**********\n");
  delete (&t, t.root, 2,0);
  insertElement(pcb9, &t, t.root, 0);
  printf("**********\n");
  printTree(t.root);
  printf("**********\n");
  delete (&t, t.root, 7,0);
  insertElement(pcb9, &t, t.root, 0);
  printf("**********\n");
  printTree(t.root);
  printf("**********\n");

  // Print the tree structure

  return 0;
}
