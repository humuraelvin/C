#include <stdio.h>
#include <stdlib.h>

// Define the structure for a node in the linked list
struct Node {
    int data;
    struct Node* next;
};

// Function to create a new node
struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

// Function to print the linked list
void printList(struct Node* head) {
    struct Node* current = head;
    while (current != NULL) {
        printf("%d -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}

int main() {
    // Create the nodes
    struct Node* head = createNode(1);
    struct Node* second = createNode(2);
    struct Node* third = createNode(3);

    // Link the nodes together
    head->next = second;
    second->next = third;

    // Print the linked list
    printf("Linked List: ");
    printList(head);

    // Free the allocated memory
    free(head);
    free(second);
    free(third);

    return 0;
}



// Explanation:
// Node Structure:

// Each node in the linked list is defined by a struct Node.
// It contains an int data to hold the value and a struct Node* next to point to the next node in the list.
// Creating a Node:

// The createNode function allocates memory for a new node, sets its data, and initializes its next pointer to NULL.
// Printing the List:

// The printList function traverses the linked list starting from the head node, printing each
// node's data followed by an arrow (->). The traversal stops when the next pointer is NULL.
// Main Function:

// Three nodes are created and linked together to form a simple linked list: 1 -> 2 -> 3 -> NULL.
// The linked list is then printed to the console.
// Finally, the allocated memory for the nodes is freed to prevent memory leaks.