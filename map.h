#ifndef __MAP_H__
#define __MAP_H__

/* Fixed -> 4 bytes */
#define WALL_BREAKABLE(flag) ((flag & 0x1) != 0)
#define WALL_BROKEN(flag) ((flag & 0x2) != 0)
#define WALL_IGNORE_BULLET(flag) ((flag & 0x4) != 0)

/**
 * Structure to represent tank spawn point
 */
typedef struct
{
    jo_pos2D_fixed Location;       /**Player start location*/
    unsigned short PlayerStartAngle; /**Player start angle*/
} map_Spawn;

/**
 * Structure to represent map detail
 */
typedef struct
{
    unsigned char Detail;     /**ID of detail model*/
    unsigned char ParentWall; /**Used to remove deatil from rendering if wall gets destroyed*/
    unsigned short Angle;       /**Angle detail model*/
    jo_pos3D_fixed Location;  /**Location of detail model*/
    jo_pos3D_fixed Scale;     /**Scale of detail model*/
} map_Detail;

/**
 * Structure to represent wall in map
 */
typedef struct
{
    AABB Rectangle;      /**Rectangle coordinates*/
    unsigned char Flags; /**Wall flags*/
} map_Wall;

/**
 * Map file header
 */
typedef struct
{
    unsigned char NumOfSpawns;
    unsigned char NumOfWalls;
    unsigned char NumOfDetails;
} map_Header;

/**
 * Structure to represent map data
 */
typedef struct
{
    map_Header Header;
    map_Spawn *Spawns;
    map_Wall *Walls;
    map_Detail *Details;
} map_Data;

/**
 * Load map file
 * @param sub_dir Folder maps are located int
 * @param filename Name of the file
 * @return Map data
 */
map_Data *Map_Load(const char *const sub_dir, const char *const filename);

/**
 * Unload map file
 * @param map Map data
 */
void Map_UnLoad(map_Data *map);

/**
 * Load map file
 * @param map Map geometry to draw
 * @param bulletList Bullets to draw
 * @param emits Particles to draw
 * @param tanks Tanks to draw
 */
void Map_Draw(map_Data *map, linked_List *bulletList, linked_List *emits, tank_Object *tanks);

#endif