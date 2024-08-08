#pragma once

typedef enum
{
    NODE_TYPE_NULL,
    NODE_TYPE_CHAR_PTR,
    NODE_TYPE_INT,
    NODE_TYPE_FLOAT,
} NodeType;

typedef struct Node
{
    NodeType nodeType;
    void *data;
    struct Node *children; // an dynamically allocated array which contains
                           // children in contiguous memory
    int depth;
    int child_count;
    int child_capacity;
} Node;

void NodeTreePrint(const Node *node);
void freeNodeBranch(Node *current);
bool NodeAddChild(Node *self, void *data, NodeType dataType);
bool NodeChangeCapacity(Node *node, int newCapacity);
bool NodeTreeGetDepth(const Node *current);