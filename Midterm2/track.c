#include <stdio.h>
#include <stdlib.h>

float cords_return[4];

typedef struct Node {
    float x;
    float y;
    int left;
    int right;
    struct Node *next;
} Node;

Node *head = NULL;

Node* createNode(float x, float y, int left, int right){
    Node *node = (Node*)malloc(sizeof(Node));
    node -> x = x;
    node -> y = y;
    node -> left = left;
    node -> right = right;
    return node;
}

void addNode(Node *node){
    node -> next = head;
    head = node;
}

void printList() {
    Node *current = head;
    while (current) {
        printf("x cord: %i y cord: %i\n", current->left, current->right);
        current = current->next;
    }
}

float* return_head(int next){
    cords_return[0] = head -> x;
    cords_return[1] = head -> y;
    cords_return[2] = head -> left;
    cords_return[3] = head -> right;
    if(next == 0){
        head = head -> next;
    }
    return cords_return;
}