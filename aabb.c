#include <jo/jo.h>
#include "aabb.h"

void AABB_Create(AABB *box, const jo_fixed l, const jo_fixed b, const jo_fixed r, const jo_fixed t)
{
    box->Left = l;
    box->Right = r;
    box->Bottom = b;
    box->Top = t;
}

void AABB_Create_size(AABB *box, const jo_fixed x, const jo_fixed y, const jo_fixed w, const jo_fixed h)
{
    return AABB_Create(box, x, y, x + w, y + h);
}

void AABB_Create_by_center(AABB *box, const jo_fixed x, const jo_fixed y, const jo_fixed size)
{
    jo_fixed offset = jo_fixed_div(size, JO_FIXED_2);
    return AABB_Create_size(box, x - offset, y - offset, size, size);
}