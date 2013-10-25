#include "mheads.h"

HDF  *g_cfg = NULL;
HASH *g_datah = NULL;

typedef struct {
    float x;
    float y;
    float z;
} vecx3;


vecx3 vecx3_newx(float x, float y, float z)
{
    vecx3 v;

    v.x = x;
    v.y = y;
    v.z = z;

    return v;
}

int main()
{
    vecx3 x = vecx3_newx(1, 0, 0);
    
    vecx3 v = {1, 1, 1};

    printf("%f, %f, %f \n", v.x, v.y, v.z);

    return 0;
}
