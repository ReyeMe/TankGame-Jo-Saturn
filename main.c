#include <jo/jo.h>
#include "aabb.h"
#include "tank.h"
#include "bullet.h"
#include "map.h"
#include "collisions.h"

static jo_camera cam;

// Game stuff
static bool inGame = false;
static bullet_List bullets;
static map_Data map;
static tank_Object * tanks;

void draw_loop(void)
{
    jo_clear_screen();
    jo_3d_camera_look_at(&cam);

    if (inGame)
        Map_Draw(&map, &bullets, tanks);
}

void game_loop(void)
{
    if (inGame)
    {
        Bullet_Update_All(&bullets, tanks, &map);

        for (int tank = 0; tank < map.Header.NumOfSpawns; tank++)
        {
            Tank_Input_Update(&tanks[tank]);

            if (tanks[tank].FiringAnimationFrames == 1)
                Bullet_Create(&bullets, &tanks[tank]);

            Tank_Update_movement(&tanks[tank], tanks, &map);
        }
    }
}

void jo_main(void)
{
    // Prepare scene
    jo_core_init(JO_COLOR_Black);
    jo_3d_camera_init(&cam);
    jo_3d_camera_set_viewpoint(&cam, 0, -40, -3);

    bullets.Bullet = NULL;
    bullets.Next = NULL;

    Tank_Load_textures();
    Bullet_Initialize();

    map = *Map_Load("MAPS", "DEMO.TGL");
    tanks = jo_malloc(sizeof(tank_Object) * map.Header.NumOfSpawns);

    for (int tank = 0; tank < map.Header.NumOfSpawns; tank++)
    {
        Tank_Create(&tanks[tank], tank, map.Spawns[tank].PlayerStartAngle, map.Spawns[tank].Location.x, 0, map.Spawns[tank].Location.y);
        tanks[tank].Controller = tank;
    }

    inGame = true;

    // Start game
    jo_core_add_callback(draw_loop);
    jo_core_add_callback(game_loop);

    jo_core_run();
}
