#ifndef __3D_MODELTANK_H__
#define __3D_MODELTANK_H__

#define PLAYER_BOX_SIZE JO_FIXED_4 + JO_FIXED_1
#define PLAYER_BOX_SIZEH JO_FIXED_2 + JO_FIXED_1_DIV_2
#define PLAYER_SPEED 7553
#define PLAYER_FIRE_ANIMATION 80
#define TANK_COLOR_COUNT 2

/**
 * Structure to represent tank model
 */
typedef struct
{
	jo_3d_mesh * tower;
	jo_3d_mesh * body;
	jo_3d_mesh * gun;
	jo_3d_mesh * flash;
} tank_mesh;


/**
 * Structure to represent player or computer controled tank
 */
typedef struct
{
	int Id;					   /**Tank ID*/
	char Color;				   /**Tank color ID*/
	bool IsExploded;		   /**Is tank exploded?*/
	bool IsFiring;			   /**Is tank firing?*/
	short TowerAngle;		   /**Current tower angle*/
	short TankAngle;		   /**Current tank angle*/
	jo_pos3D_fixed Location;   /**Current tank location*/
	jo_vector Velocity;		   /**Current tank velocity*/
	int FiringAnimationFrames; /**Current number of animation frames passed*/
	int Controller;			   /**Controller ID*/
} tank_Object;

/**
 * Create tank object
 * @param tank Address to tank_Object alocated struct
 * @param color Tank color ID
 * @param tankAngle start tank angle
 * @param x X location
 * @param y Y location (not used in-game)
 * @param z Z location
 */
void Tank_Create(tank_Object *tank, const char color, const short tankAngle, const jo_fixed x, const jo_fixed y, const jo_fixed z);

/**
 * Load tank model
 */
void Tank_Load(void);

/**
 * Update tank based on gamepad input
 * @param tank Tank object
 */
void Tank_Input_Update(tank_Object *tank);

/**
 * Draw tank object
 * @param tank Tank object to draw
 */
void Tank_Draw(tank_Object *tank);

#endif
