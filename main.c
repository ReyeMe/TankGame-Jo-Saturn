#include <jo/jo.h>
#include "tank.h"
#include "bullet.h"
#include "collisions.h"

// TODO: move to level data, defined here for now
#define NUMBER_OF_TANKS 4
#define NUMBER_OF_WALLS 4

static jo_camera cam;
static tank_Object playerTanks[NUMBER_OF_TANKS];
static AABB walls[NUMBER_OF_WALLS];
static bullet_List bullets;

static jo_3d_quad quad;
static jo_vertice vertices[] =
    {
        {0, 0, 0},
        {1, 0, 0},
        {1, 0, 1},
        {0, 0, 1}};

void my_draw(void)
{
    bullet_List *bulletList = &bullets;

    jo_clear_screen();
    jo_3d_camera_look_at(&cam);

    // Render tanks
    for (int iterator = 0; iterator < NUMBER_OF_TANKS; iterator++)
    {
        if (playerTanks[iterator].InGame)
        {
            Tank_Draw(&playerTanks[iterator]);
        }
    }

    // Render bullets
    while (bulletList != NULL)
    {
        if (bulletList->Bullet != NULL)
        {
            Bullet_Draw(bulletList->Bullet);
        }

        bulletList = bulletList->Next;
    }

    // Render arena bounds
    for (int iterator = 0; iterator < NUMBER_OF_WALLS; iterator++)
    {
        jo_3d_push_matrix();
        {
            jo_3d_translate_matrix_fixed(walls[iterator].Left, 0, walls[iterator].Bottom);
            jo_3d_set_scale_fixed(walls[iterator].Right - walls[iterator].Left, 0, walls[iterator].Top - walls[iterator].Bottom);
            jo_3d_draw(&quad);
        }
        jo_3d_pop_matrix();
    }
}

void my_gamepad(void)
{
    char thrust;
    Bullet_Update_All(&bullets, playerTanks, NUMBER_OF_TANKS, walls, NUMBER_OF_WALLS);

    for (int iterator = 0; iterator < NUMBER_OF_TANKS; iterator++)
    {
        thrust = 0;
        playerTanks[iterator].InGame = true;

        if (jo_is_input_available(iterator) &&
            playerTanks[iterator].InGame &&
            !playerTanks[iterator].IsExploded)
        {
            playerTanks[iterator].IsFiring = jo_is_input_key_pressed(iterator, JO_KEY_A);

            if (jo_is_input_key_pressed(iterator, JO_KEY_UP))
                thrust = -1;
            else if (jo_is_input_key_pressed(iterator, JO_KEY_DOWN))
                thrust = 1;

            if (jo_is_input_key_pressed(iterator, JO_KEY_LEFT))
                playerTanks[iterator].TankAngle += -1;
            else if (jo_is_input_key_pressed(iterator, JO_KEY_RIGHT))
                playerTanks[iterator].TankAngle += 1;

            if (jo_is_input_key_pressed(iterator, JO_KEY_L))
                playerTanks[iterator].TowerAngle += -1;
            else if (jo_is_input_key_pressed(iterator, JO_KEY_R))
                playerTanks[iterator].TowerAngle += 1;

            // Clamp tower angle to safe values
            if (playerTanks[iterator].TowerAngle < 0)
                playerTanks[iterator].TowerAngle = 359;
            else if (playerTanks[iterator].TowerAngle > 359)
                playerTanks[iterator].TowerAngle = 0;

            // Clamp tank angle to safe values
            if (playerTanks[iterator].TankAngle < 0)
                playerTanks[iterator].TankAngle = 359;
            else if (playerTanks[iterator].TankAngle > 359)
                playerTanks[iterator].TankAngle = 0;

            if (playerTanks[iterator].IsFiring && playerTanks[iterator].FiringAnimationFrames == 0)
            {
                playerTanks[iterator].FiringAnimationFrames = 1;
                Bullet_Create(&bullets, &playerTanks[iterator]);
            }

            // Move tank
            if (thrust != 0)
            {
                playerTanks[iterator].Velocity.x = jo_fixed_mult(jo_sin(playerTanks[iterator].TankAngle) * thrust, PLAYER_SPEED);
                playerTanks[iterator].Velocity.z = jo_fixed_mult(jo_cos(playerTanks[iterator].TankAngle) * thrust, PLAYER_SPEED);
            }

            Tank_Update_movement(&playerTanks[iterator], playerTanks, NUMBER_OF_TANKS, walls, NUMBER_OF_WALLS);
        }
        else
        {
            playerTanks[iterator].IsExploded = true;
        }
    }
}

void jo_main(void)
{
    // Prepare scene
    jo_core_init(JO_COLOR_Black);
    Tank_Load_textures();
    jo_3d_camera_init(&cam);
    jo_3d_camera_set_viewpoint(&cam, 0, -40, -3);

    // Create tanks
    Tank_Create(&playerTanks[0], 0, 0, jo_int2fixed(-10), 0, jo_int2fixed(-10));
    Tank_Create(&playerTanks[1], 1, 0, jo_int2fixed(0), 0, jo_int2fixed(0));
    Tank_Create(&playerTanks[2], 0, 0, jo_int2fixed(5), 0, jo_int2fixed(-5));
    Tank_Create(&playerTanks[3], 1, 0, jo_int2fixed(-10), 0, jo_int2fixed(10));

    // Create walls
    AABB_Create_size(&walls[0], jo_int2fixed(-37), jo_int2fixed(-28), jo_int2fixed(74), jo_int2fixed(2));
    AABB_Create_size(&walls[1], jo_int2fixed(-37), jo_int2fixed(28), jo_int2fixed(74), jo_int2fixed(2));
    AABB_Create_size(&walls[2], jo_int2fixed(-39), jo_int2fixed(-26), jo_int2fixed(2), jo_int2fixed(54));
    AABB_Create_size(&walls[3], jo_int2fixed(37), jo_int2fixed(-26), jo_int2fixed(2), jo_int2fixed(54));

    bullets.Bullet = NULL;
    bullets.Next = NULL;

    jo_3d_create_plane(&quad, vertices);
    jo_3d_set_color(&quad, JO_COLOR_DarkRed);
    quad.attribute.sort = SORT_MAX;
    quad.attribute.atrb = CL32KRGB | MESHon;

    Bullet_Initialize();

    // Start game
    jo_core_add_callback(my_draw);
    jo_core_add_callback(my_gamepad);

    jo_core_run();
}
