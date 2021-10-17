#include <jo/jo.h>
#include "Entities/emit.h"
#include "linkedList.h"
#include "aabb.h"
#include "Entities/tank.h"
#include "Entities/bullet.h"
#include "map.h"

// Temporary quad
static jo_3d_quad solidWall;
static jo_vertice solidWallVertices[] = { {0, 0, 0}, {1, 0, 0}, {1, 0, 1}, {0, 0, 1}};
static jo_3d_quad breakableWall;
static jo_vertice breakableWallVertices[] = { {0, 0, 0}, {1, 0, 0}, {1, 0, 1}, {0, 0, 1}};
static jo_3d_quad fenceWall;
static jo_vertice fenceWallVertices[] = { {0, 0, 0}, {1, 0, 0}, {1, 0, 1}, {0, 0, 1}};

map_Data *Map_Load(const char *const sub_dir, const char *const filename)
{
    // Get file stream
    int counter = 0;
    char *file = jo_fs_read_file_in_dir(filename, sub_dir, JO_NULL);
    char *stream = file;

    // Prepare map structure
    map_Data *map = jo_malloc(sizeof(map_Data));
    map->Spawns = JO_NULL;
    map->Walls = JO_NULL;
    map->Details = JO_NULL;

    // Load file contents
    map->Header.NumOfSpawns = *stream;
    stream++;
    map->Header.NumOfWalls = *stream;
    stream++;
    map->Header.NumOfDetails = *stream;
    stream++;

    // There must be at least 2 spawn points on map
    if (map->Header.NumOfSpawns > 1)
    {
        map->Spawns = jo_malloc(sizeof(map_Spawn) * map->Header.NumOfSpawns);

        for (counter = 0; counter < map->Header.NumOfSpawns; counter++)
        {
            map->Spawns[counter].Location.x = jo_int2fixed(*(char*)stream);
            stream++;
            map->Spawns[counter].Location.y = jo_int2fixed(*(char*)stream);
            stream++;
            map->Spawns[counter].PlayerStartAngle = (*(unsigned char*)stream << 8) | *(unsigned char*)(stream + 1);
            stream += 2;
        }
    }
    else
    {
        jo_free(map);
        jo_free(file);
        return JO_NULL;
    }

    // Add build in walls
    map->Header.NumOfWalls += 4;
    map->Walls = jo_malloc(sizeof(map_Wall) * map->Header.NumOfWalls);
    map->Walls[0].Flags = 0;
    map->Walls[1].Flags = 0;
    map->Walls[2].Flags = 0;
    map->Walls[3].Flags = 0;
    AABB_Create_size(&map->Walls[0].Rectangle, jo_int2fixed(-37), jo_int2fixed(-29), jo_int2fixed(74), JO_FIXED_2);
    AABB_Create_size(&map->Walls[1].Rectangle, jo_int2fixed(-37), jo_int2fixed(27), jo_int2fixed(74), JO_FIXED_2);
    AABB_Create_size(&map->Walls[2].Rectangle, jo_int2fixed(-39), jo_int2fixed(-27), JO_FIXED_2, jo_int2fixed(54));
    AABB_Create_size(&map->Walls[3].Rectangle, jo_int2fixed(37), jo_int2fixed(-27), JO_FIXED_2, jo_int2fixed(54));

    // Add walls from map file
    for (counter = 4; counter < map->Header.NumOfWalls; counter++)
    {
        map->Walls[counter].Rectangle.Left = jo_int2fixed(*(char*)stream);
        stream++;
        map->Walls[counter].Rectangle.Bottom = jo_int2fixed(*(char*)stream);
        stream++;
        map->Walls[counter].Rectangle.Right = jo_int2fixed(*(char*)stream);
        stream++;
        map->Walls[counter].Rectangle.Top = jo_int2fixed(*(char*)stream);
        stream++;
        map->Walls[counter].Flags = *(unsigned char *)stream;
        stream++;
    }

    if (map->Header.NumOfDetails > 0)
    {
        map->Details = jo_malloc(sizeof(map_Detail) * map->Header.NumOfDetails);

        for (counter = 0; counter < map->Header.NumOfDetails; counter++)
        {
            map->Details[counter] = *((map_Detail *)stream);
            stream += sizeof(map_Detail);
        }
    }

    jo_free(file);
    return map;
}

void Map_UnLoad(map_Data *map)
{
    if (map->Spawns != JO_NULL)
        jo_free(map->Spawns);

    if (map->Walls != JO_NULL)
        jo_free(map->Walls);

    if (map->Details != JO_NULL)
        jo_free(map->Details);

    jo_free(map);
}

void Map_Draw(map_Data *map, linked_List *bullets, linked_List *emits, tank_Object *tanks)
{
    // ALPHA ONLY
    static bool run = false;

    if (!run)
    {
        run = true;
        jo_3d_create_plane(&solidWall, solidWallVertices);
        jo_3d_create_plane(&breakableWall, breakableWallVertices);
        jo_3d_create_plane(&fenceWall, fenceWallVertices);
        jo_3d_set_color(&solidWall, JO_COLOR_Red);
        jo_3d_set_color(&breakableWall, JO_COLOR_Yellow);
        jo_3d_set_color(&fenceWall, JO_COLOR_Cyan);
        jo_3d_set_screen_doors(&fenceWall, true);
    }
    // ALPHA ONLY

    linked_List *bulletList = bullets;
    linked_List *emited = emits;

    // Render tanks
    for (int iterator = 0; iterator < map->Header.NumOfSpawns; iterator++)
    {
        Tank_Draw(&tanks[iterator]);
    }

    // Render bullets
    while (bulletList != JO_NULL)
    {
        if (bulletList->Current != JO_NULL)
        {
            Bullet_Draw(bulletList->Current);
        }

        bulletList = bulletList->Next;
    }

    // Render emits
    while (emited != JO_NULL)
    {
        if (emited->Current != JO_NULL)
        {
            jo_3d_push_matrix();
            {
                jo_fixed scale = JO_FIXED_1 - jo_fixed_div(jo_int2fixed(((Emit_data*)emited->Current)->FramesAlive), jo_int2fixed(((Emit_data*)emited->Current)->FramesToLive));
                scale = jo_fixed_div(scale, ((Emit_data*)emited->Current)->BaseScaleDiv);
                jo_3d_translate_matrix_fixed(
                    ((Emit_data*)emited->Current)->Location.x,
                    ((Emit_data*)emited->Current)->Location.y,
                    ((Emit_data*)emited->Current)->Location.z);
                
                jo_3d_push_matrix();
                {
                    jo_3d_rotate_matrix_x(90);
                    jo_3d_set_scale_fixed(scale, scale, scale);
                    jo_3d_draw_sprite(((Emit_data*)emited->Current)->SpriteId);
                }
                jo_3d_pop_matrix();
            }
            jo_3d_pop_matrix();
        }

        emited = emited->Next;
    }

    // Render arena bounds
    for (int iterator = 0; iterator < map->Header.NumOfWalls; iterator++)
    {
        if (!WALL_BROKEN(map->Walls[iterator].Flags))
        {
            jo_3d_push_matrix();
            {
                jo_3d_translate_matrix_fixed(
                    map->Walls[iterator].Rectangle.Left,
                    0,
                    map->Walls[iterator].Rectangle.Bottom);

                jo_3d_set_scale_fixed(
                    map->Walls[iterator].Rectangle.Right - map->Walls[iterator].Rectangle.Left,
                    0,
                    map->Walls[iterator].Rectangle.Top - map->Walls[iterator].Rectangle.Bottom);

                // Just draw quad here for now, will be invisible in final
                if (WALL_BREAKABLE(map->Walls[iterator].Flags))
                {
                    jo_3d_draw(&breakableWall);
                }
                else if (WALL_IGNORE_BULLET(map->Walls[iterator].Flags))
                {
                    jo_3d_draw(&fenceWall);
                }
                else
                {
                    jo_3d_draw(&solidWall);
                }
                
            }
            jo_3d_pop_matrix();
        }
    }
}