#ifndef __COLLISION_H__
#define __COLLISION_H__

/**
 * Structure to represent axis-alligned bounding box
 */
typedef struct
{
    jo_fixed Left; /**Distance from origin to left edge */
    jo_fixed Bottom; /**Distance from origin to bottom edge */
    jo_fixed Right; /**Distance from origin to right edge */
    jo_fixed Top; /**Distance from origin to top edge */
} AABB;

/**
 * Create AABB
 * @param box Address to AABB alocated struct
 * @param l Distance from origin to left side
 * @param b Distance from origin to bottom side
 * @param r Distance from origin to right side
 * @param t Distance from origin to top side
 */
void AABB_Create(AABB * box, const jo_fixed l, const jo_fixed b, const jo_fixed r, const jo_fixed t);

/**
 * Create AABB by size
 * @param box Address to AABB alocated struct
 * @param x Distance from origin to left side
 * @param y Distance from origin to bottom side
 * @param w Rectangle width
 * @param h Rectangle height
 */
void AABB_Create_size(AABB * box, const jo_fixed x, const jo_fixed y, const jo_fixed w, const jo_fixed h);

/**
 * Create AABB by center
 * @param box Address to AABB alocated struct
 * @param x X location
 * @param y Y location
 * @param size Diameter
 */
void AABB_Create_by_center(AABB * box, const jo_fixed x, const jo_fixed y, const jo_fixed size);

/**
 * AABB collision
 * @param box1 First AABB
 * @param box2 Second AABB
 * @return True if AABB collide
 */
bool AABB_Collides(const AABB * box1, const AABB * box2);

/**
 * Swept AABB collision
 * @param wall Static AABB
 * @param moving Moving AABB
 * @param vx Horizontal velocity
 * @param vy Vertical velocity
 * @param ex Horizontal entry time
 * @param ey Vertical entry time
 * @return True if AABB collide
 */
bool AABB_Swept_collision(const AABB * wall, const AABB * moving, const jo_fixed vx, const jo_fixed vy, jo_fixed * ex, jo_fixed * ey);

void Bullet_Update_All(bullet_List * bullets, tank_Object * tanks, int tanksSize, AABB * walls, int wallsSize);

/**
 * Update tank movement with collision
 * @param tank Tank object to move
 * @param thrust Movement direction (1,-1)
 * @param playerTanks List of all tanks in the scene
 * @param tanksSize Number of tanks in the scene
 * @param walls List of all walls in the scene
 * @param wallsSize Number of all walls in the scene
 */
void Tank_Update_movement(tank_Object * tank, tank_Object * tanks, int tanksSize, AABB * walls, int wallsSize);

#endif