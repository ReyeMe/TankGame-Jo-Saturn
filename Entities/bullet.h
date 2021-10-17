#ifndef __BULLET_H__
#define __BULLET_H__

#define BULLET_SPEED 20659
#define BULLET_SIZE JO_FIXED_1
#define BULLET_SIZE_RENDER 7768

/**
 * Bullet object
 */
typedef struct
{
    int Id;
    bool Bounced;
    tank_Object *FiredBy;
    jo_pos3D_fixed Location;
    jo_vector_fixed Velocity;
    int TrailSpriteId;
    int ExpSpriteId;
    int framesAlive;
} bullet_Object;

/**
 * Create bullet object
 * @param bulletList List of all bullets
 * @param firedBy Tank bullet was fired by
 */
void Bullet_Create(linked_List *bulletList, tank_Object *firedBy);

/**
 * Destroy bullet object
 * @param bulletList List of all bullets
 * @param bullet Bullet object to destroy
 */
void Bullet_Destroy(linked_List *bulletList, bullet_Object *bullet);

/**
 * Initialize bullet
 */
void Bullet_Initialize(void);

/**
 * Draw bullet
 * @param bullet Bullet to draw
 */
void Bullet_Draw(bullet_Object *bullet);

#endif