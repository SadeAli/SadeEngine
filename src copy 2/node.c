#include <stdio.h>
#include <stdlib.h>

#include "node.h"

#define ARRAY_CAPACITY_INCREMENT_STEP 10

int incrementNodeCapacity(Node *node);

int incrementNodeCapacity(Node *node)
{
    size_t newCapacity = node->child_capacity * 2 + 1;
    return NodeChangeCapacity(node, newCapacity);
}

bool NodeAddChild(Node self[static 1], void *data, NodeType dataType)
{
    if (self == NULL)
    {
        fprintf(stderr, "Error: self is NULL\n");
        return false;
    }

    if (self->child_capacity <= self->child_count)
    {
        if (incrementNodeCapacity(self) == 0)
        {
            return false;
        }
    }

    self->children[self->child_count].data = data;
    self->children[self->child_count].nodeType = dataType;
    self->child_count += 1;

    return true;
}

bool NodeChangeCapacity(Node *node, int newCapacity)
{
    Node *children = realloc(node->children, newCapacity * sizeof(Node));

    if (children)
    {
        for (int i = node->child_capacity; i < newCapacity; i++)
        {
            children[i] = (Node){.nodeType = NODE_TYPE_NULL,
                                 .data = NULL,
                                 .children = NULL,
                                 .child_count = 0,
                                 .child_capacity = 0,
                                 .depth = node->depth + 1};
        }

        node->children = children;
        node->child_capacity = newCapacity;
        return false;
    }

    return true;
}

void displayNodeType(const Node *node)
{
    switch (node->nodeType)
    {
    case NODE_TYPE_CHAR_PTR:
        printf("%s", (char *)node->data);
        break;

    default:
        printf("unable to recognize node type!\n");
        break;
    }
}

void NodeTreePrint(const Node *node)
{
    printf(" ");
    displayNodeType(node);
    printf("\n");
    for (int i = 0; i < node->child_count; i++)
    {
        const int depth = node->depth;
        for (int i = 0; i < (depth + 1); i++)
        {
            printf(" | ");
        }

        NodeTreePrint(&node->children[i]);
    }
}

void freeNodeTree(Node *node)
{
    for (int i = 0; i < node->child_count; i++)
    {
        freeNodeTree(&node->children[i]);
    }
}