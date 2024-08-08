#include "arrayList.h"
#include "vec.h"
#include <stdio.h>

typedef struct SomeVecRelated_t
{
    int bla;

    ARRAYLIST_COMPATIBLE_STRUCT(vec2) vecList;
    ARRAYLIST_COMPATIBLE_STRUCT(int) intList;

} SomeVecRelated;

void appendExample()
{
    SomeVecRelated x = {0};

    vec2 a = {.data = {10, 30}};
    vec2 b = {.data = {12, 50}};
    vec2 c = {.data = {13, 60}};
    vec2 d = {.data = {15, 10}};

    APPEND_ARRAY_LIST(x.vecList, a);
    APPEND_ARRAY_LIST(x.vecList, b);
    APPEND_ARRAY_LIST(x.vecList, c);
    APPEND_ARRAY_LIST(x.vecList, d);

    APPEND_ARRAY_LIST(x.intList, a.x);
    APPEND_ARRAY_LIST(x.intList, b.y);
    APPEND_ARRAY_LIST(x.intList, a.y);
    APPEND_ARRAY_LIST(x.intList, c.x);
    APPEND_ARRAY_LIST(x.intList, d.y);

    ARRAYLIST_FOREACH(x.vecList, i)
    {
        printf("%f %f\n", x.vecList.data[i].x, x.vecList.data[i].y);
    }

    ARRAYLIST_FOREACH(x.intList, i)
    {
        printf("%d\n", x.intList.data[i]);
    }
}
