typedef unsigned int u32;

#include <dynamicArray.h>

// HEDAER
typedef struct Node_t Node;
struct Node_t {
    Node *children;
    u32 dataIndex;
    u32 numChildren;
    u32 capacity;
};

bool node_addChild(Node parent, u32 childIndex);

// SOURCE
bool node_addChild(Node parent, u32 childDataIndex) {
    return array_append((void **)&parent.children, &parent.numChildren, &parent.capacity, sizeof(u32), &childDataIndex, 1);
}
