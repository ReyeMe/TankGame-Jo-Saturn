#ifndef __AABB_H__
#define __AABB_H__

/**
 * Structure to represent axis-alligned bounding box
 */
typedef struct
{
    jo_fixed Left;   /**Distance from origin to left edge */
    jo_fixed Bottom; /**Distance from origin to bottom edge */
    jo_fixed Right;  /**Distance from origin to right edge */
    jo_fixed Top;    /**Distance from origin to top edge */
} AABB;

/**
 * Create AABB
 * @param box Address to AABB alocated struct
 * @param l Distance from origin to left side
 * @param b Distance from origin to bottom side
 * @param r Distance from origin to right side
 * @param t Distance from origin to top side
 */
void AABB_Create(AABB *box, const jo_fixed l, const jo_fixed b, const jo_fixed r, const jo_fixed t);

/**
 * Create AABB by size
 * @param box Address to AABB alocated struct
 * @param x Distance from origin to left side
 * @param y Distance from origin to bottom side
 * @param w Rectangle width
 * @param h Rectangle height
 */
void AABB_Create_size(AABB *box, const jo_fixed x, const jo_fixed y, const jo_fixed w, const jo_fixed h);

/**
 * Create AABB by center
 * @param box Address to AABB alocated struct
 * @param x X location
 * @param y Y location
 * @param size Diameter
 */
void AABB_Create_by_center(AABB *box, const jo_fixed x, const jo_fixed y, const jo_fixed size);

#endif