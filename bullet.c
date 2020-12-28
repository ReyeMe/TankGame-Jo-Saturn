#include <jo/jo.h>
#include "tank.h"
#include "bullet.h"

static int trail_texture;
static int bullet_texture;

void Bullet_Create(bullet_List *bulletList, tank_Object *firedBy)
{
    // Create bullet
    int id = 0;
    int angle = (firedBy->TankAngle + firedBy->TowerAngle) % 360;
    jo_fixed lx = -jo_fixed_mult(jo_sin(angle), jo_fixed_div(PLAYER_BOX_SIZEH, JO_FIXED_2));
    jo_fixed lz = -jo_fixed_mult(jo_cos(angle), jo_fixed_div(PLAYER_BOX_SIZEH, JO_FIXED_2));

    bullet_Object *bullet = jo_malloc(sizeof(bullet_Object));
    bullet->Bounced = false;
    bullet->FiredBy = firedBy;

    bullet->Velocity.x = -jo_fixed_mult(jo_sin(angle), BULLET_SPEED);
    bullet->Velocity.y = 0;
    bullet->Velocity.z = -jo_fixed_mult(jo_cos(angle), BULLET_SPEED);
    bullet->Location.x = firedBy->Location.x + lx;
    bullet->Location.y = -104110;
    bullet->Location.z = firedBy->Location.z + lz;

    for (int trail = 0; trail < BULLET_TRAIL_LENGTH; trail++)
    {
        bullet->Trail[trail].x = bullet->Location.x;
        bullet->Trail[trail].y = bullet->Location.y;
        bullet->Trail[trail].z = bullet->Location.z;
    }

    // Add bullet to the list
    bullet_List *current = bulletList;

    while (current->Bullet != NULL)
    {
        id++;
        current = current->Next;
    }

    bullet->Id = id;
    current->Bullet = bullet;

    // Add next empty item
    if (current->Next == NULL)
    {
        bullet_List *newItem = jo_malloc(sizeof(bullet_List));
        newItem->Bullet = NULL;
        newItem->Next = NULL;
        current->Next = newItem;
    }
}

void Bullet_Destroy(bullet_List *bulletList, bullet_Object *bullet)
{
    bullet_List *last = NULL;
    bullet_List *current = bulletList;

    while (current->Next != NULL && current->Bullet->Id != bullet->Id)
    {
        last = current;
        current = current->Next;
    }

    if (current->Bullet->Id == bullet->Id)
    {
        jo_free(current->Bullet);
        current->Bullet = NULL;

        if (last != NULL)
        {
            bullet_List *next = current->Next;
            jo_free(current);
            last->Next = next;
        }
    }
}

void Bullet_Initialize(void)
{
    bullet_texture = jo_sprite_add_tga(JO_ROOT_DIR, "BULLET.TGA", JO_COLOR_Red);
    trail_texture = jo_sprite_add_tga(JO_ROOT_DIR, "TRAIL.TGA", JO_COLOR_Black);
}

void Bullet_Draw(bullet_Object *bullet)
{
    jo_fixed trailSize = BULLET_TRAIL_SIZE_RENDER;

    for (int trail = 0; trail < BULLET_TRAIL_LENGTH; trail++)
    {
        jo_3d_push_matrix();
        {
            jo_3d_translate_matrix_fixed(bullet->Trail[trail].x, bullet->Trail[trail].y, bullet->Trail[trail].z);

            jo_3d_push_matrix();
            {
                jo_3d_rotate_matrix_x(90);
                jo_3d_set_scale_fixed(trailSize, trailSize, trailSize);
                jo_3d_draw_sprite(trail_texture);
            }
            jo_3d_pop_matrix();
        }
        jo_3d_pop_matrix();

        trailSize -= 846;
    }

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