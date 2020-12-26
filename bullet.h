#ifndef __BULLET_H__
#define __BULLET_H__

#define BULLET_SPEED 19659
#define BULLET_TRAIL_LENGTH 6
#define BULLET_SIZE JO_FIXED_2
#define BULLET_TRAIL_SIZE_RENDER 6768
#define BULLET_SIZE_RENDER 7768

typedef struct
{
    int Id;
    bool Bounced;
    tank_Object * FiredBy;
    jo_pos3D Trail[BULLET_TRAIL_LENGTH];
    jo_pos3D Location;
    jo_vector Velocity;
} bullet_Object;

typedef struct bullet_List bullet_List;
struct bullet_List
{
    bullet_Object * Bullet;
    bullet_List * Next;
};

void Bullet_Create(bullet_List * bulletList, tank_Object * firedBy);

void Bullet_Destroy(bullet_List * bulletList, bullet_Object * bullet);

void Bullet_Initialize(void);

void Bullet_Draw(bullet_Object * bullet);

#endif