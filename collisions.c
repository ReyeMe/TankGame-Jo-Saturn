#include <jo/jo.h>
#include "tank.h"
#include "bullet.h"
#include "collisions.h"

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
        if (moving->Bottom + entry < wall->Top &&
            moving->Top + entry > wall->Bottom)
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
        if (moving->Left + entry < wall->Right &&
            moving->Right + entry > wall->Left)
        {
            *ey = entryTime;
            vc = true;
        }
    }

    return hc || vc;
}

static void Bullet_Update_Single(bullet_Object *bullet, bullet_List *bullets, tank_Object *tanks, int tanksSize, AABB *walls, int wallsSize)
{
    // Update trail
    for (int trail = BULLET_TRAIL_LENGTH - 1; trail > 0; trail--)
    {
        bullet->Trail[trail].x = bullet->Trail[trail - 1].x;
        bullet->Trail[trail].y = bullet->Trail[trail - 1].y;
        bullet->Trail[trail].z = bullet->Trail[trail - 1].z;
    }

    bullet->Trail[0].x = bullet->Location.x;
    bullet->Trail[0].y = bullet->Location.y;
    bullet->Trail[0].z = bullet->Location.z;

    // Update bullet
    AABB moving;
    AABB_Create_by_center(&moving, bullet->Location.x, bullet->Location.z, JO_FIXED_2);
    jo_fixed vx = bullet->Velocity.x;
    jo_fixed vy = bullet->Velocity.z;

    for (int tankIndex = 0; tankIndex < tanksSize; tankIndex++)
    {
        if (tanks[tankIndex].InGame &&
            !tanks[tankIndex].IsExploded &&
            (bullet->Bounced || (!bullet->Bounced && tanks[tankIndex].Id != bullet->Id)))
        {
            AABB standing;
            jo_fixed sx;
            jo_fixed sy;

            AABB_Create_by_center(
                &standing,
                tanks[tankIndex].Location.x,
                tanks[tankIndex].Location.z,
                PLAYER_BOX_SIZE);

            if (AABB_Swept_collision(&standing, &moving, vx, vy, &sx, &sy))
            {
                tanks[tankIndex].IsExploded = true;
                Bullet_Destroy(bullets, bullet);
                return;
            }
        }
    }

    for (int wallIndex = 0; wallIndex < wallsSize; wallIndex++)
    {
        // Collision response
        jo_fixed sx;
        jo_fixed sy;

        if (AABB_Swept_collision(&walls[wallIndex], &moving, vx, vy, &sx, &sy))
        {
            if (bullet->Bounced)
            {
                // Maybe failing??
                Bullet_Destroy(bullets, bullet);
            }
            else
            {
                bullet->Bounced = true;

                if (sx != JO_FIXED_1)
                {
                    bullet->Velocity.x = -bullet->Velocity.x;
                }

                if (sy != JO_FIXED_1)
                {
                    bullet->Velocity.z = -bullet->Velocity.z;
                }
            }
        }
    }

    bullet->Location.x += vx;
    bullet->Location.z += vy;
}

void Bullet_Update_All(bullet_List *bullets, tank_Object *tanks, int tanksSize, AABB *walls, int wallsSize)
{
    bullet_List *bulletList = bullets;

    while (bulletList != NULL)
    {
        if (bulletList->Bullet != NULL)
        {
            Bullet_Update_Single(bulletList->Bullet, bullets, tanks, tanksSize, walls, wallsSize);
        }

        bulletList = bulletList->Next;
    }
}

void Tank_Update_movement(tank_Object *tank, tank_Object *tanks, int tanksSize, AABB *walls, int wallsSize)
{
    // Movement vector
    jo_fixed oldX = tank->Location.x;
    jo_fixed oldZ = tank->Location.z;
    jo_fixed vx = tank->Velocity.x;
    jo_fixed vy = tank->Velocity.z;

    // Create list of things to check collision against
    AABB moving;
    AABB_Create_by_center(&moving, oldX, oldZ, PLAYER_BOX_SIZE);

    for (int tankIndex = 0; tankIndex < tanksSize; tankIndex++)
    {
        if (tanks[tankIndex].InGame && tanks[tankIndex].Id != tank->Id)
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
                    Tank_Update_movement(&tanks[tankIndex], tanks, tanksSize, walls, wallsSize);
                }

                vx = jo_fixed_mult(vx, sx);
                vy = jo_fixed_mult(vy, sy);
            }
        }
    }

    for (int wallIndex = 0; wallIndex < wallsSize; wallIndex++)
    {
        // Collision response
        jo_fixed sx;
        jo_fixed sy;

        if (AABB_Swept_collision(&walls[wallIndex], &moving, vx, vy, &sx, &sy))
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
