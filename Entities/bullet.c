#include <jo/jo.h>
#include "../linkedList.h"
#include "tank.h"
#include "bullet.h"

static int bullet_texture;
static int trail_texture;
static int explosion_texture;

void Bullet_Create(linked_List *bulletList, tank_Object *firedBy)
{
    // Create bullet
    int id = 0;
    int angle = (firedBy->TankAngle + firedBy->TowerAngle) % 360;

    bullet_Object *bullet = jo_malloc(sizeof(bullet_Object));
    bullet->Bounced = false;
    bullet->FiredBy = firedBy;
    bullet->TrailSpriteId = trail_texture;
    bullet->ExpSpriteId = explosion_texture;

    bullet->Velocity.x = -jo_fixed_mult(jo_sin(angle), BULLET_SPEED);
    bullet->Velocity.y = 0;
    bullet->Velocity.z = -jo_fixed_mult(jo_cos(angle), BULLET_SPEED);
    bullet->Location.x = firedBy->Location.x;
    bullet->Location.y = -104110;
    bullet->Location.z = firedBy->Location.z;

    // Add bullet to the list
    linked_List *current = bulletList;

    while (current->Current != JO_NULL)
    {
        id++;
        current = current->Next;
    }

    bullet->Id = id;
    current->Current = bullet;

    // Add next empty item
    if (current->Next == JO_NULL)
    {
        linked_List *newItem = jo_malloc(sizeof(linked_List));
        newItem->Current = JO_NULL;
        newItem->Next = JO_NULL;
        current->Next = newItem;
    }
}

void Bullet_Destroy(linked_List *bulletList, bullet_Object *bullet)
{
    linked_List *last = JO_NULL;
    linked_List *current = bulletList;

    while (current->Next != JO_NULL && ((bullet_Object * )current->Current)->Id != bullet->Id)
    {
        last = current;
        current = current->Next;
    }

    if (((bullet_Object * )current->Current)->Id == bullet->Id)
    {
        jo_free(current->Current);
        current->Current = JO_NULL;

        if (last != JO_NULL)
        {
            linked_List *next = current->Next;
            jo_free(current);
            last->Next = next;
        }
    }
}

void Bullet_Initialize(void)
{
    bullet_texture = jo_sprite_add_tga(JO_ROOT_DIR, "BULLET.TGA", JO_COLOR_Red);
    trail_texture = jo_sprite_add_tga(JO_ROOT_DIR, "TRAIL.TGA", JO_COLOR_Black);
    explosion_texture = jo_sprite_add_tga(JO_ROOT_DIR, "EXP.TGA", JO_COLOR_Black);
}

void Bullet_Draw(bullet_Object *bullet)
{
    jo_3d_push_matrix();
    {
        jo_3d_translate_matrix_fixed(bullet->Location.x, bullet->Location.y, bullet->Location.z);

        jo_3d_push_matrix();
        {
            jo_3d_rotate_matrix_x(90);
            jo_3d_set_scale_fixed(BULLET_SIZE_RENDER, BULLET_SIZE_RENDER, BULLET_SIZE_RENDER);
            jo_3d_draw_sprite(bullet_texture);
        }
        jo_3d_pop_matrix();
    }
    jo_3d_pop_matrix();
}