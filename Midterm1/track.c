#include <stdio.h>
#include <stdlib.h>

float cords_return[2];

typedef struct Node {
    double x;
    double y;
    struct Node *next;
} Node;

Node *head = NULL;

Node* createNode(double x, double y){
    Node *node = (Node*)malloc(sizeof(Node));
    node -> x = x;
    node -> y = y;
    return node;
}

void addNode(Node *node){
    node -> next = head;
    head = node;
}

void printList() {
    Node *current = head;
    while (current) {
        printf("x cord: %.2lf y cord: %.2lf\n", current->x, current->y);
        current = current->next;
    }
}

float* return_head(){
    cords_return[0] = head -> x;
    cords_return[1] = head -> y;
    head = head -> next;
    return cords_return;
}

/*int main(void){
    addNode(createNode(12.1, 13.4));
    addNode(createNode(1.1, 5.5));
    printList();
}*/