#pragma once

#include <stdlib.h>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

#define RETURN_IF_ARRAY_SCALE_FAILS(array, type, newCapacity, returnVal)
#define SCALE_ARRAY(ptr, type, newCapacity) array = (type *)realloc

struct BoundingBoxList_t
{
    int count;
    int capacity;
    struct bbObjectMap
    {
        BoundingBox bb;
        void *ptr;
    } *objects;
};

typedef struct BoundingBoxList_t BBList;

void addObject() {
}

void raycastAddBoundingBox(BBList *bblist, BoundingBox bb)
{
    if (bblist->capacity <= bblist->count)
    {
    }
}

void *raycastGetCollisionObject(Ray r, BBList bblist)
{
    void *nearestObject = NULL;
    float nearestObjectDistance = -1;
    for (int i = 0; i < bblist.count; i++)
    {
        RayCollision c = GetRayCollisionBox(r, bblist.objects[i].bb);

        if (c.hit)
        {
            float d = Vector3DistanceSqr(r.position, c.point);

            if (d < nearestObjectDistance || nearestObjectDistance < 0)
            {
                nearestObjectDistance = d;
                nearestObject = bblist.objects[i].ptr;
            }
        }
    }

    return nearestObject;
}