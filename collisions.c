#include <jo/jo.h>
#include "Entities/emit.h"
#include "linkedList.h"
#include "aabb.h"
#include "Entities/tank.h"
#include "Entities/bullet.h"
#include "map.h"
#include "collisions.h"

bool AABB_Collides(const AABB *box1, const AABB *box2)
{
    return box1->Left < box2->Right &&
           box1->Right > box2->Left &&
           box1->Bottom < box2->Top &&
           box1->Top > box2->Bottom;
}

bool AABB_Swept_collision(const AABB *wall, const AABB *moving, const jo_fixed vx, const jo_fixed vy, jo_fixed *ex, jo_fixed *ey)
{
    jo_fixed entryTime, entry;
    bool vc = false;
    bool hc = false;

    // Default to maximal entry time (1.0f)
    *ex = JO_FIXED_1;
    *ey = JO_FIXED_1;

    // Check for collision on horizontal line
    if (vx != 0 &&
        ((vx > 0 && moving->Right <= wall->Left && moving->Right + vx > wall->Left) ||
         (vx < 0 && moving->Left >= wall->Right && moving->Left + vx < wall->Right)))
    {
        // We got collision
        // Calculate vertical collision at point of horizontal collision
        if (vx > 0)
        {
            entryTime = wall->Left - moving->Right;
        }
        else
        {
            entryTime = wall->Right - moving->Left;
        }

        entryTime = jo_fixed_div(entryTime, vx);
        entry = jo_fixed_mult(vy, entryTime);

        // Check for vertical collision
        if (moving->Bottom + entry <= wall->Top &&
            moving->Top + entry >= wall->Bottom)
        {
            *ex = entryTime;
            hc = true;
        }
    }

    // Check for collision on vertical line
    if (vy != 0 &&
        ((vy > 0 && moving->Top <= wall->Bottom && moving->Top + vy > wall->Bottom) ||
         (vy < 0 && moving->Bottom >= wall->Top && moving->Bottom + vy < wall->Top)))
    {
        // We got collision
        // Calculate horizontal collision at point of vertical collision
        if (vy > 0)
        {
            entryTime = wall->Bottom - moving->Top;
        }
        else
        {
            entryTime = wall->Top - moving->Bottom;
        }

        entryTime = jo_fixed_div(entryTime, vy);
        entry = jo_fixed_mult(vx, entryTime);

        // Check for horizontal collision
        if (moving->Left + entry <= wall->Right &&
            moving->Right + entry >= wall->Left)
        {
            *ey = entryTime;
            vc = true;
        }
    }

    return hc || vc;
}

static void Bullet_Create_explosion(linked_List *emits, bullet_Object *bullet)
{
    for (int angle = 0; angle < 360; angle+=36)
    {
        Emit_data * emit = jo_malloc(sizeof(Emit_data));
        emit->Location = bullet->Location;
        emit->Velocity.x = jo_sin(angle) / 2;
        emit->Velocity.y = 0;
        emit->Velocity.z = jo_cos(angle) / 2;
        emit->FramesToLive = 25;
        emit->FramesAlive = 0;
        emit->BaseScaleDiv = JO_FIXED_2;
        emit->SpriteId = bullet->ExpSpriteId;
        linkedList_Add(emits, emit);
    }
}

static void Bullet_Create_trail(linked_List *emits, bullet_Object *bullet)
{
    Emit_data * emit = jo_malloc(sizeof(Emit_data));
    emit->Location = bullet->Location;
    emit->Velocity.x = 0;
    emit->Velocity.y = 0;
    emit->Velocity.z = 0;
    emit->FramesToLive = 35;
    emit->FramesAlive = 0;
    emit->BaseScaleDiv = JO_FIXED_4;
    emit->SpriteId = bullet->TrailSpriteId;
    linkedList_Add(emits, emit);
}

static void Bullet_Update_Single(bullet_Object *bullet, linked_List *emits, linked_List *bullets, tank_Object *tanks, jo_sound * explosion, map_Data *map)
{
    // Prevent bullets that miss collision to fly out
    if (map->Walls[1].Rectangle.Bottom < bullet->Location.y ||
        map->Walls[2].Rectangle.Bottom > bullet->Location.y ||
        map->Walls[0].Rectangle.Left > bullet->Location.x ||
        map->Walls[3].Rectangle.Left < bullet->Location.x)
    {
        Bullet_Destroy(bullets, bullet);
        return;
    }

    // Update bullet
    if (bullet->framesAlive % 10 == 0)
    {
        Bullet_Create_trail(emits, bullet);
    }

    bullet->framesAlive++;
    jo_fixed vx = bullet->Velocity.x;
    jo_fixed vy = bullet->Velocity.z;
    AABB moving;
    AABB_Create_by_center(&moving, bullet->Location.x, bullet->Location.z, JO_FIXED_1);
    AABB moved;
    AABB_Create_by_center(&moved, bullet->Location.x + vx, bullet->Location.z + vy, JO_FIXED_1);

    for (int tankIndex = 0; tankIndex < map->Header.NumOfSpawns; tankIndex++)
    {
        if (!tanks[tankIndex].IsExploded && (bullet->Bounced || (!bullet->Bounced && tanks[tankIndex].Id != bullet->FiredBy->Id)))
        {
            AABB standing;
            jo_fixed sx;
            jo_fixed sy;

            AABB_Create_by_center(
                &standing,
                tanks[tankIndex].Location.x,
                tanks[tankIndex].Location.z,
                PLAYER_BOX_SIZE);

            if (AABB_Collides(&standing, &moved) ||
                AABB_Collides(&standing, &moving) ||
                AABB_Swept_collision(&standing, &moving, vx, vy, &sx, &sy))
            {
                tanks[tankIndex].IsExploded = true;
                Bullet_Create_explosion(emits, bullet);
                jo_audio_play_sound(explosion);
                Bullet_Destroy(bullets, bullet);
                return;
            }
        }
    }

    int wallIndex = 0;
    int wallIgnore = -1;

    while(wallIndex < map->Header.NumOfWalls)
    {
        // Collision response
        jo_fixed sx;
        jo_fixed sy;

        if (wallIndex != wallIgnore &&
            !WALL_IGNORE_BULLET(map->Walls[wallIndex].Flags) &&
            !WALL_BROKEN(map->Walls[wallIndex].Flags) &&
            AABB_Swept_collision(&map->Walls[wallIndex].Rectangle, &moving, vx, vy, &sx, &sy))
        {
            if (WALL_BREAKABLE(map->Walls[wallIndex].Flags))
            {
                map->Walls[wallIndex].Flags |= 0x2;
                Bullet_Destroy(bullets, bullet);
                Bullet_Create_explosion(emits, bullet);
                jo_audio_play_sound(explosion);
                break;
            }
            else if (bullet->Bounced)
            {
                Bullet_Destroy(bullets, bullet);
                Bullet_Create_explosion(emits, bullet);
                jo_audio_play_sound(explosion);
                break;
            }
            else
            {
                wallIgnore = wallIndex;
                wallIndex = 0;
                bullet->Bounced = true;

                if (sx != JO_FIXED_1)
                {
                    bullet->Velocity.x = -bullet->Velocity.x;
                    bullet->Location.x += jo_fixed_mult(bullet->Velocity.x, sx);
                    vx = -jo_fixed_mult(bullet->Velocity.x, JO_FIXED_1 - sx);
                }

                if (sy != JO_FIXED_1)
                {
                    bullet->Velocity.z = -bullet->Velocity.z;
                    bullet->Location.z += jo_fixed_mult(bullet->Velocity.z, sy);
                    vy = -jo_fixed_mult(bullet->Velocity.z, JO_FIXED_1 - sy);
                }
            }
        }
        else
        {
            wallIndex++;
        }
    }

    bullet->Location.x += vx;
    bullet->Location.z += vy;
}

void Bullet_Update_All(linked_List *bullets, linked_List *emits, tank_Object *tanks, jo_sound * explosion, map_Data *map)
{
    linked_List *bulletList = bullets;

    while (bulletList != JO_NULL)
    {
        if (bulletList->Current != JO_NULL)
        {
            Bullet_Update_Single(bulletList->Current, emits, bullets, tanks, explosion, map);
        }

        bulletList = bulletList->Next;
    }
}

void Tank_Update_movement(tank_Object *tank, tank_Object *tanks, map_Data *map)
{
    // Movement vector
    jo_fixed oldX = tank->Location.x;
    jo_fixed oldZ = tank->Location.z;
    jo_fixed vx = tank->Velocity.x;
    jo_fixed vy = tank->Velocity.z;

    // Create list of things to check collision against
    AABB moving;
    AABB_Create_by_center(&moving, oldX, oldZ, PLAYER_BOX_SIZE);

    for (int tankIndex = 0; tankIndex < map->Header.NumOfSpawns; tankIndex++)
    {
        if (tanks[tankIndex].Id != tank->Id)
        {
            AABB standing;
            AABB_Create_by_center(
                &standing,
                tanks[tankIndex].Location.x,
                tanks[tankIndex].Location.z,
                PLAYER_BOX_SIZE);

            // Collision response
            jo_fixed sx;
            jo_fixed sy;

            if (AABB_Swept_collision(&standing, &moving, vx, vy, &sx, &sy))
            {
                if (tanks[tankIndex].IsExploded)
                {
                    tanks[tankIndex].Velocity.x = vx;
                    tanks[tankIndex].Velocity.z = vy;
                    Tank_Update_movement(&tanks[tankIndex], tanks, map);
                }

                vx = jo_fixed_mult(vx, sx);
                vy = jo_fixed_mult(vy, sy);
            }
        }
    }

    for (int wallIndex = 0; wallIndex < map->Header.NumOfWalls; wallIndex++)
    {
        // Collision response
        jo_fixed sx;
        jo_fixed sy;

        if (!WALL_BROKEN(map->Walls[wallIndex].Flags) && AABB_Swept_collision(&map->Walls[wallIndex].Rectangle, &moving, vx, vy, &sx, &sy))
        {
            vx = jo_fixed_mult(vx, sx);
            vy = jo_fixed_mult(vy, sy);
        }
    }

    tank->Location.x = oldX + vx;
    tank->Location.z = oldZ + vy;
    tank->Velocity.x = 0;
    tank->Velocity.z = 0;
}
