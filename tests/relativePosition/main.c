#include <stdio.h>

typedef struct {
    float x;
    float y;
} Vector2;

typedef struct Node2D {
    Vector2 position;
    struct Node2D *parent;
} Node2D;

Vector2 create_vector2(float x, float y) {
    Vector2 v;
    v.x = x;
    v.y = y;
    return v;
}

Node2D create_node2d(Vector2 position, Node2D *parent) {
    Node2D node;
    node.position = position;
    node.parent = parent;
    return node;
}

Vector2 get_global_position(Node2D *node) {
    Vector2 global_position = node->position;
    if (node->parent != NULL) {
        Vector2 parent_global_position = get_global_position(node->parent);
        global_position.x += parent_global_position.x;
        global_position.y += parent_global_position.y;
    }
    return global_position;
}

int main() {
    Node2D root = create_node2d(create_vector2(100, 100), NULL);
    Node2D child = create_node2d(create_vector2(50, 50), &root);
    Node2D grandchild = create_node2d(create_vector2(25, 25), &child);

    Vector2 root_pos = get_global_position(&root);
    Vector2 child_pos = get_global_position(&child);
    Vector2 grandchild_pos = get_global_position(&grandchild);

    printf("Root Position: (%f, %f)\n", root_pos.x, root_pos.y);
    printf("Child Position: (%f, %f)\n", child_pos.x, child_pos.y);
    printf("Grandchild Position: (%f, %f)\n", grandchild_pos.x, grandchild_pos.y);

    return 0;
}
