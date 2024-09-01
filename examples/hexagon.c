// TODO: townscaper style generation
#include "arrayDynamics.h"

typedef Vec3 VertexPosition;
typedef void VertexAttribute;

// TODO: Graph structures
typedef struct GraphNode_t GraphNode;
typedef struct GraphEdge_t GraphEdge;
// typedef struct GraphFace_t GraphFace;

typedef struct AdjComponent_t AdjComponent;

struct AdjComponent_t {
    u32 a, b;
};

struct HexagonalGraphNode_t {
    u32 connectionCount;
    u32 connectedNodes[6];
};

bool hexGridRelaxed() {
    const int layerCount = 3;
    const float layerDistance = 1.1;

    const int vertexCount = 3*layerCount*(layerCount + 1)+ 1;
    const u32 edgeCount = 3*layerCount*(3*layerCount + 1)*2;

    const u32 vertexBufferSize = sizeof(Vec2) * vertexCount;
    Vec2 *vertices = malloc(vertexBufferSize);
    AdjComponent *edges = malloc(edgeCount * sizeof(AdjComponent));

    if ((vertices == nullptr) || (edges == nullptr)){
        free(vertices);
        free(edges);
        return false;
    }

    // NOTE: vertex placement
    vertices[0] = (Vec2){.data = {0,0}};
    int placedVertices = 1;
    // build hexagon layer by layer
    for (int i = 0; i < layerCount; i++) {
        // treat hexagon as 6 equal triangles
        // for each triangle
        for (int j = 0; j < 6; j++) {
            // create a starting point
            Vec2 edgeStart;
            glm_vec2_rotate((vec2){0, layerDistance * i + 1}, GLM_PI * j / 3, edgeStart.data);

            Vec2 target = edgeStart;

            Vec2 step;
            glm_vec2_rotate(edgeStart.data, GLM_PI * 2 / 3, step.data);
            glm_vec2_normalize(step.data);
            glm_vec2_scale(step.data, layerDistance, step.data);

            // travel trough the edge
            for (int k = 0; k < (i + 1); k++)
            {
                vertices[placedVertices] = target;
                glm_vec2_add(target.data, step.data, target.data);
                placedVertices += 1;
            }
        }
    }

    // NOTE: adding graph edges

    // (think visually) select 2 points (I thought them as spiraling) and connect them
    int placedEdges = 0;
    // vertex index of the selected points

    // NOTE: scaffhold
    u32 up = 1;
    u32 down = 0;
    for (int layer = 0; layer < layerCount; layer++)
    {
        for (int slice = 0; slice < 6; slice++)
        {
            // put edge at the start of the hexagon slice
            edges[placedEdges++] = (AdjComponent){down, up};
            // zig zag edges
            for (int i = 0; i < layer; i++)
            {
                edges[placedEdges++] = (AdjComponent){down, ++up};
                edges[placedEdges++] = (AdjComponent){++down, up};
            }

            up++;
        }
        // fix last edge of the frame
        if (layer > 0)
        {
            edges[placedEdges - 1].a -= 6*layer;
        }
        else 
        {
            down++;
        }
    }

    // NOTE: frame
    for (int layer = 0; layer < layerCount; layer++)
    {
        u32 current =  3*layer*(layer + 1) + 1;
        for (int i = 0; i < 6*(layer+1); i++)
        {
            edges[placedEdges++] = (AdjComponent){current, current + 1};
            current++;
        }
        
        // fix last edge of the frame
        edges[placedEdges - 1].b -= 6*(layer + 1);
    }

    // TODO: delete edges then add quadrilation
    srand(1 + time(nullptr));
    printf("%u\n", placedEdges);
    for (int i = 0; i < 1000; i++)
    {
        // select a random edge
        u32 selectedEdge = rand() % placedEdges;

        // check if it's a quad
        u32 connectionCount = 0;
        for (int i = 0; i < placedEdges; i++)
        {
            if (edges[selectedEdge].a == edges[i].a ||
                edges[selectedEdge].a == edges[i].b ||
                edges[selectedEdge].b == edges[i].a ||
                edges[selectedEdge].b == edges[i].b )
            {
                connectionCount++;
            }
        }

        if (connectionCount >= 11)
        {
            printf("(%u, %u) -> %u\n", edges[selectedEdge].a, edges[selectedEdge].b, connectionCount);
            // delete the edge
            placedEdges--;
            edges[selectedEdge] = edges[placedEdges];
        }
    }
    printf("%u\n", placedEdges);

    // NOTE: rendering
    unsigned int vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, vertices, GL_STREAM_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, placedEdges * sizeof(AdjComponent), (float*)edges, GL_STREAM_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vec2), (void*)0);
    glEnableVertexAttribArray(0);

    glUseProgram(globalShader);
    glDrawElements(GL_LINES, placedEdges * 2, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

    free(vertices);
    free(edges);
    return true;
}

bool drawLayeredHexagon() {
    const int layerCount = 4;
    const float layerDistance = 1.1;

    const int vertexCount = (layerCount + 1)*layerCount*3 + 1;
    const int edgeCount = ((3*layerCount*layerCount) - layerCount) * 3;

    const int triangleCount = layerCount * layerCount * 6;
    const int indexCount = triangleCount * 3;

    const u32 vertexBufferSize = sizeof(Vec2) * vertexCount;
    const u32 indexBufferSize = sizeof(u32) * indexCount;
    Vec2 *vertices = malloc(vertexBufferSize);
    int *indices = malloc(indexBufferSize);

    if ((vertices == nullptr) || (indices == nullptr)){
        free(vertices);
        free(indices);
        return false;
    }

    // place vertices
    //
    // place first vector
    vertices[0] = (Vec2){.data = {0,0}};
    int placedVertices = 1;

    // place the rest
    for (int i = 0; i < layerCount; i++) {
        // for each edge of the hexagon
        for (int j = 0; j < 6; j++) {
            Vec2 edgeStart;
            glm_vec2_rotate((vec2){0, layerDistance * i + 1}, GLM_PI * j / 3, edgeStart.data);

            Vec2 target = edgeStart;

            Vec2 step;
            glm_vec2_rotate(edgeStart.data, GLM_PI * 2 / 3, step.data);
            glm_vec2_normalize(step.data);
            glm_vec2_scale(step.data, layerDistance, step.data);

            // travel trough the edge
            for (int k = 0; k < (i + 1); k++)
            {
                vertices[placedVertices] = target;
                glm_vec2_add(target.data, step.data, target.data);
                placedVertices += 1;
            }
        }
    }

    // create triangles
    // -------------------------------------------------------------------------------------------
    // clockwise
    int passedVertices = 1;
    int placedIndices = 0;
    int up = 1;
    int down = 0;
    for (int i = 0; i < layerCount; i++)
    {
        int verticesInNextLayer = (i + 1) * 6;

        for (int j = 0; j < 6; j++)
        {
            // first triangle
            indices[placedIndices++] = up++;
            indices[placedIndices++] = down++;
            indices[placedIndices++] = up++;

            // generate the rest using strip method
            for (int k = 0; k < i; k++)
            {
                // strip
                indices[placedIndices] = indices[placedIndices - 1];
                indices[placedIndices + 1] = indices[placedIndices - 2];

                // new index
                indices[placedIndices + 2] = down++;
                placedIndices += 3;

                // strip
                indices[placedIndices] = indices[placedIndices - 3];
                indices[placedIndices + 1] = indices[placedIndices - 1];

                // new index
                indices[placedIndices + 2] = up++;
                placedIndices += 3;
            }

            up--;
            down--;
        }

        if (i != 0)
        {
            indices[placedIndices - 4] -= i * 6;
            indices[placedIndices - 2] -= i * 6;
        }
        indices[placedIndices - 1] -= (i + 1) * 6;

        down = passedVertices;
        passedVertices += verticesInNextLayer;
    }

    unsigned int vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, vertices, GL_STREAM_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferSize, indices, GL_STREAM_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vec2), (void*)0);
    glEnableVertexAttribArray(0);

    glUseProgram(globalShader);
    // glDrawElements(GL_LINE_STRIP, placedIndices, GL_UNSIGNED_INT, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, placedIndices, GL_UNSIGNED_INT, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    free(vertices);
    free(indices);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

    return true;
}
