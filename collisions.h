#ifndef __COLLISION_H__
#define __COLLISION_H__

/**
 * AABB collision
 * @param box1 First AABB
 * @param box2 Second AABB
 * @return True if AABB collide
 */
bool AABB_Collides(const AABB *box1, const AABB *box2);

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
bool AABB_Swept_collision(const AABB *wall, const AABB *moving, const jo_fixed vx, const jo_fixed vy, jo_fixed *ex, jo_fixed *ey);

/**
 * Update tank movement with collision
 * @param bullets List of all bullets
 * @param tanks List of all tanks in the scene
 * @param map Current map data
 */
void Bullet_Update_All(linked_List *bullets, linked_List *emits, tank_Object *tanks, jo_sound * explosion, map_Data *map);

/**
 * Update tank movement with collision
 * @param tank Tank object to move
 * @param tanks List of all tanks in the scene
 * @param map Current map data
 */
void Tank_Update_movement(tank_Object *tank, tank_Object *tanks, map_Data *map);

#endif