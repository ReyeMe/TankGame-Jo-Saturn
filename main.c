#include <jo/jo.h>
#include "linkedList.h"
#include "Entities/emit.h"
#include "aabb.h"
#include "Entities/tank.h"
#include "Entities/bullet.h"
#include "map.h"
#include "collisions.h"

static jo_camera cam;
static jo_sound soundBum;
static jo_sound soundclick;
static jo_sound soundShoot;

// Game stuff
static bool inGame = false;
static bool inGameMenu = false;
static linked_List bullets;
static linked_List emits;
static map_Data * map = JO_NULL;
static tank_Object * tanks = JO_NULL;
static int selectedMap = 0;
bool inputValid = false;

#define MAP_COUNT 7

static char *mapImages[MAP_COUNT] = 
{
    "PASSAGE.TGA",
    "TOWER.TGA",
    "PLAZA.TGA",
    "ARENA.TGA",
    "DYNA.TGA",
    "RUINS.TGA",
    "SPLIT.TGA"
};

static char *mapFiles[MAP_COUNT] = 
{
    "PASSAGE.TGL",
    "TOWER.TGL",
    "PLAZA.TGL",
    "ARENA.TGL",
    "DYNA.TGL",
    "RUINS.TGL",
    "SPLIT.TGL"
};

static char *maps[MAP_COUNT] = 
{
    "PASSAGE",
    "TOWER",
    "PLAZA",
    "ARENA",
    "DYNA",
    "RUINS",
    "SPLIT"
};

void StartGame(const char *const mapFile)
{
    inGameMenu = false;
    jo_clear_background(JO_COLOR_Black);
    map = Map_Load("MAPS", mapFile);
    tanks = jo_malloc(sizeof(tank_Object) * map->Header.NumOfSpawns);

    int lastChecked = 0;

    for (int tank = 0; tank < map->Header.NumOfSpawns; tank++)
    {
        Tank_Create(&tanks[tank], tank, map->Spawns[tank].PlayerStartAngle, map->Spawns[tank].Location.x, 0, map->Spawns[tank].Location.y);
        
        for (int controller = lastChecked; controller < JO_INPUT_MAX_DEVICE; controller++)
        {
            if (jo_is_input_available(controller))
            {
                tanks[tank].Controller = controller;
                lastChecked = controller + 1;
                break;
            }
        }
    }

    inGame = true;
}

void DrawMapImage()
{
    jo_img mapImage;
    mapImage.data = NULL;
    jo_tga_loader(&mapImage, "MAPS", mapImages[selectedMap], JO_COLOR_Transparent);
    jo_set_background_sprite(&mapImage, 100, 50);
    jo_free_img(&mapImage);
}

void LoadMainMenu(void)
{
    jo_audio_play_sound(&soundclick);

    inGame = false;
    inGameMenu = false;

    if (map != JO_NULL)
    {
        Map_UnLoad(map);
        map = JO_NULL;
        linkedList_Clear_all(&bullets);
        linkedList_Clear_all(&emits);
        jo_free(tanks);
    }

    jo_img logo;
    logo.data = NULL;
    jo_tga_loader(&logo, JO_ROOT_DIR, "LOGO.TGA", JO_COLOR_Transparent);
    jo_set_background_sprite(&logo, 40, 15);
    jo_free_img(&logo);

    DrawMapImage();
}

void draw_loop(void)
{
    jo_clear_screen();

    if (!inputValid)
    {
        jo_printf_with_color(6,20, JO_COLOR_INDEX_Red,"Please connect 2 controllers");
        return;
    }

    jo_3d_camera_look_at(&cam);

    if (inGame)
    {
        Map_Draw(map, &bullets, &emits, tanks);

        if (inGameMenu)
        {
            jo_printf_with_color(6, 15, JO_COLOR_INDEX_White, "Press A to exit to main menu");
        }
    }
    else
    {
        if (selectedMap > 0) jo_printf_with_color(15, 18, JO_COLOR_INDEX_Yellow, "<");
        jo_printf_with_color(17, 18, JO_COLOR_INDEX_White, maps[selectedMap]);
        if (selectedMap < MAP_COUNT - 1) jo_printf_with_color(25, 18, JO_COLOR_INDEX_Yellow, ">");

        jo_printf_with_color(9, 21, JO_COLOR_INDEX_White, "Press start to play...");

        jo_printf_with_color(1,28, JO_COLOR_INDEX_Purple, "Created by Reye (web: reye.me)");
        jo_set_printf_color_index(JO_COLOR_INDEX_White);
    }
}

void game_loop(void)
{
    CdcStat cdStatus;
    CDC_GetCurStat(&cdStatus);

    if (cdStatus.status == CDC_ST_OPEN)
    {
        jo_goto_boot_menu();
    }

    if (!inputValid)
    {
        return;
    }

    if (inGame)
    {
        int tanksAlive = 0;
        int lastAlive = 0;

        for (int tank = 0; tank < map->Header.NumOfSpawns; tank++)
        {
            if (!tanks[tank].IsExploded)
            {
                tanksAlive++;
                lastAlive = tank + 1;
            }
        }

        if (tanksAlive > 1 && jo_is_pad1_available() && jo_is_pad1_key_down(JO_KEY_START))
        {
            inGameMenu = !inGameMenu;
            jo_audio_play_sound(&soundclick);
        }
        
        if (inGameMenu)
        {
            if (jo_is_pad1_key_down(JO_KEY_A))
            {
                LoadMainMenu();
            }
        }
        else
        {
            // Update particles
            linked_List * particles = &emits;

            if (tanksAlive > 1)
            {
                while(particles != JO_NULL)
                {
                    if (particles->Current != JO_NULL)
                    {
                        ((Emit_data*)particles->Current)->FramesAlive++;
                        ((Emit_data*)particles->Current)->Location.x += ((Emit_data*)particles->Current)->Velocity.x;
                        ((Emit_data*)particles->Current)->Location.y += ((Emit_data*)particles->Current)->Velocity.y;
                        ((Emit_data*)particles->Current)->Location.z += ((Emit_data*)particles->Current)->Velocity.z;

                        if (((Emit_data*)particles->Current)->FramesAlive >= ((Emit_data*)particles->Current)->FramesToLive)
                        {
                            linkedList_Remove(&emits, particles->Current);
                        }
                    }

                    particles = particles->Next;
                }

                // Update tanks and bullets
                Bullet_Update_All(&bullets, &emits, tanks, &soundBum, map);

                for (int tank = 0; tank < map->Header.NumOfSpawns; tank++)
                {
                    Tank_Input_Update(&tanks[tank]);

                    if (tanks[tank].FiringAnimationFrames == 1)
                    {
                        Bullet_Create(&bullets, &tanks[tank]);
                        jo_audio_play_sound(&soundShoot);
                    }

                    Tank_Update_movement(&tanks[tank], tanks, map);
                }
            }
            else
            {
                jo_printf_with_color(13, 15, JO_COLOR_INDEX_White, "Player %d wins!", lastAlive);
                jo_printf_with_color(10, 16, JO_COLOR_INDEX_White, "Press B to restart or");
                jo_printf_with_color(7, 17, JO_COLOR_INDEX_White, "press A to go to main menu");
                jo_set_printf_color_index(JO_COLOR_INDEX_White);

                if (jo_is_pad1_key_down(JO_KEY_A))
                {
                    LoadMainMenu();
                }
                else if (jo_is_pad1_key_down(JO_KEY_B))
                {
                    Map_UnLoad(map);
                    map = JO_NULL;
                    linkedList_Clear_all(&bullets);
                    linkedList_Clear_all(&emits);
                    jo_free(tanks);
                    StartGame(mapFiles[selectedMap]);
                }
            }
        }
    }
    else
    {
        if (jo_is_pad1_available())
        {
            bool selectionChanged = false;

            if (selectedMap - 1 >= 0 && jo_is_pad1_key_down(JO_KEY_LEFT))
            {
                jo_audio_play_sound(&soundclick);
                selectedMap--;
                selectionChanged = true;
            }
            else if (selectedMap + 1 < MAP_COUNT && jo_is_pad1_key_down(JO_KEY_RIGHT))
            {
                jo_audio_play_sound(&soundclick);
                selectedMap++;
                selectionChanged = true;
            }

            if (selectedMap < 0)
                selectedMap = 0;
            else if (selectedMap >= MAP_COUNT)
                selectedMap = MAP_COUNT - 1;

            if (selectionChanged)
            {
                selectionChanged = false;
                DrawMapImage();
            }

            if (jo_is_pad1_key_down(JO_KEY_START))
            {
                StartGame(mapFiles[selectedMap]);
            }
        }
    }
}

void check_input()
{
    int count = 0;

    for (int controller = 0; controller < JO_INPUT_MAX_DEVICE; controller++)
    {
        if (jo_is_input_available(controller))
        {
            count++;
        }
    }

    inputValid = true;//count > 1;
}

void jo_main(void)
{
    // Prepare scene
    jo_core_init(JO_COLOR_Black);

    jo_3d_camera_init(&cam);
    jo_3d_camera_set_viewpoint(&cam, 0, -40, -3);

    linkedList_Initialize(&emits);
    linkedList_Initialize(&bullets);
    Tank_Load();
    Bullet_Initialize();

    LoadMainMenu();

    jo_audio_load_pcm("BUM.PCM", JoSoundMono16Bit, &soundBum);
    jo_audio_load_pcm("SHOOT.PCM", JoSoundMono16Bit, &soundShoot);
    jo_audio_load_pcm("CLICK.PCM", JoSoundMono16Bit, &soundclick);

    // Start game
    jo_core_set_restart_game_callback(jo_goto_boot_menu);

    jo_core_add_callback(check_input);
    jo_core_add_callback(draw_loop);
    jo_core_add_callback(game_loop);

    jo_core_run();
}
