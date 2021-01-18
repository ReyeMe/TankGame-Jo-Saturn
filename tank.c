#include <jo/jo.h>
#include "tank.h"
#include "tankMesh.h"

/**
 * Tank color table
 * When color is set to 0, tank will be white, PUT ONLY AT THE END OF A LIST!
 */
static jo_color TankColors[] =
	{
		JO_COLOR_RGB(255, 80, 80),
		JO_COLOR_RGB(100, 100, 255)};

/**
 * Contains loaded meshes for all tank color variants 
 */
static jo_3d_mesh Tank_variants[TANK_COLOR_COUNT * 2];

/**
 * Creates mesh body for specified tank color
 * @param startTextureIndex Starting index of tank textures
 * @param color Tank color
 */
static void Tank_Create_Mesh_Body(const int startTextureIndex, const unsigned char color)
{
	int iterator = 0;
	int meshIndex = color * 2;
	Tank_variants[meshIndex].data.pntbl = PointsTankBody;
	Tank_variants[meshIndex].data.nbPoint = 90;
	Tank_variants[meshIndex].data.pltbl = PolygonsTankBody;
	Tank_variants[meshIndex].data.nbPolygon = 21;
	Tank_variants[meshIndex].data.attbl = jo_malloc(sizeof(ATTR) * 21);

	for (iterator = 0; iterator < 21; iterator++)
	{
		Tank_variants[meshIndex].data.attbl[iterator] = AttributesTankBody[iterator];
		Tank_variants[meshIndex].data.attbl[iterator].texno += startTextureIndex;

		// Add gouraud if not a solid color polygon/threads or tank color is white
		if (AttributesTankBody[iterator].texno != 0 && TankColors[color] != 0)
		{
			Tank_variants[meshIndex].data.attbl[iterator].gstb = 0xe000 + color;
			JO_ADD_FLAG(Tank_variants[meshIndex].data.attbl[iterator].atrb, CL_Gouraud);
		}
	}
}

/**
 * Creates mesh body for specified tank tower color
 * @param startTextureIndex Starting index of tank textures
 * @param color Tank tower color
 */
static void Tank_Create_Mesh_Tower(const int startTextureIndex, const unsigned char color)
{
	int iterator = 0;
	int meshIndex = (color * 2) + 1;
	Tank_variants[meshIndex].data.pntbl = PointsTankTower;
	Tank_variants[meshIndex].data.nbPoint = 8;
	Tank_variants[meshIndex].data.pltbl = PolygonsTankTower;
	Tank_variants[meshIndex].data.nbPolygon = 5;
	Tank_variants[meshIndex].data.attbl = jo_malloc(sizeof(ATTR) * 5);

	for (iterator = 0; iterator < 5; iterator++)
	{
		Tank_variants[meshIndex].data.attbl[iterator] = AttributesTankTower[iterator];
		Tank_variants[meshIndex].data.attbl[iterator].texno += startTextureIndex;

		// Add gouraud if tank color is not white
		if (TankColors[color] != 0)
		{
			Tank_variants[meshIndex].data.attbl[iterator].gstb = 0xe000 + color;
			JO_ADD_FLAG(Tank_variants[meshIndex].data.attbl[iterator].atrb, CL_Gouraud);
		}
	}
}

void Tank_Create(tank_Object *tank, const char color, const short tankAngle, const jo_fixed x, const jo_fixed y, const jo_fixed z)
{
	static int id = 0;
	tank->Color = color;
	tank->Id = id++;
	tank->TankAngle = tankAngle;
	tank->TowerAngle = 0;
	tank->Location.x = x;
	tank->Location.y = y;
	tank->Location.z = z;
	tank->Velocity.x = 0;
	tank->Velocity.y = 0;
	tank->Velocity.z = 0;
	tank->IsExploded = false;
	tank->IsFiring = false;
	tank->FiringAnimationFrames = 0;
	tank->Controller = 0;
}

void Tank_Load_textures(void)
{
	int spriteStartIndex = 0;
	int color = 0;
	jo_color *ptr;

	// Load textures
	spriteStartIndex = jo_sprite_add_tga("TANK", "THREADS.TGA", JO_COLOR_Transparent);
	jo_sprite_add_tga("TANK", "SIDES.TGA", JO_COLOR_Transparent);
	jo_sprite_add_tga("TANK", "BACK.TGA", JO_COLOR_Transparent);
	jo_sprite_add_tga("TANK", "TOP.TGA", JO_COLOR_Transparent);
	jo_sprite_add_tga("TANK", "TBOT.TGA", JO_COLOR_Transparent);
	jo_sprite_add_tga("TANK", "TSIDES.TGA", JO_COLOR_Transparent);
	jo_sprite_add_tga("TANK", "FFIRE.TGA", JO_COLOR_Black);

	for (color = 0; color < TANK_COLOR_COUNT; color++)
	{
		// Load gouraud table
		// If color is set to 0, do not create gouraud entry, it won't be used in resulting mesh
		if (TankColors[color] != 0)
		{
			ptr = (jo_color *)(JO_VDP1_VRAM + 0x70000 + JO_MULT_BY_8(color));
			*ptr = TankColors[color];
			*(ptr + 1) = TankColors[color];
			*(ptr + 2) = TankColors[color];
			*(ptr + 3) = TankColors[color];
		}

		// Create tank meshes with correct attributes
		Tank_Create_Mesh_Body(spriteStartIndex, color);
		Tank_Create_Mesh_Tower(spriteStartIndex, color);
	}

	for (color = 0; color < TANK_COLOR_COUNT; color++)
	{
		Tank_Create_Mesh_Body(spriteStartIndex, color);
		Tank_Create_Mesh_Tower(spriteStartIndex, color);
	}
}

void Tank_Input_Update(tank_Object *tank)
{
	char thrust = 0;

	if (jo_is_input_available(tank->Controller) && !tank->IsExploded)
	{
		tank->IsFiring = jo_is_input_key_pressed(tank->Controller, JO_KEY_A);

		if (jo_is_input_key_pressed(tank->Controller, JO_KEY_UP))
			thrust = -1;
		else if (jo_is_input_key_pressed(tank->Controller, JO_KEY_DOWN))
			thrust = 1;

		if (jo_is_input_key_pressed(tank->Controller, JO_KEY_LEFT))
			tank->TankAngle += -1;
		else if (jo_is_input_key_pressed(tank->Controller, JO_KEY_RIGHT))
			tank->TankAngle += 1;

		if (jo_is_input_key_pressed(tank->Controller, JO_KEY_L))
			tank->TowerAngle += -1;
		else if (jo_is_input_key_pressed(tank->Controller, JO_KEY_R))
			tank->TowerAngle += 1;

		// Clamp tower angle to safe values
		if (tank->TowerAngle < 0)
			tank->TowerAngle = 359;
		else if (tank->TowerAngle > 359)
			tank->TowerAngle = 0;

		// Clamp tank angle to safe values
		if (tank->TankAngle < 0)
			tank->TankAngle = 359;
		else if (tank->TankAngle > 359)
			tank->TankAngle = 0;

		if (tank->IsFiring && tank->FiringAnimationFrames == 0)
		{
			tank->FiringAnimationFrames = 1;
		}

		// Move tank
		if (thrust != 0)
		{
			tank->Velocity.x = jo_fixed_mult(jo_sin(tank->TankAngle) * thrust, PLAYER_SPEED);
			tank->Velocity.z = jo_fixed_mult(jo_cos(tank->TankAngle) * thrust, PLAYER_SPEED);
		}
	}
}

void Tank_Draw(tank_Object *tank)
{
	// Draw tank
	jo_3d_push_matrix();
	{
		jo_3d_translate_matrix_fixed(tank->Location.x, tank->Location.y, tank->Location.z);

		jo_3d_push_matrix();
		{
			jo_3d_rotate_matrix_y(tank->TankAngle);
			jo_3d_mesh_draw(&(Tank_variants[tank->Color * 2]));

			if (!tank->IsExploded)
			{
				jo_3d_rotate_matrix_y(tank->TowerAngle);
				jo_3d_mesh_draw(&(Tank_variants[(tank->Color * 2) + 1]));

				if (tank->FiringAnimationFrames != 0)
				{
					int animationHalf = JO_DIV_BY_2(PLAYER_FIRE_ANIMATION);

					if (tank->FiringAnimationFrames < animationHalf)
						jo_3d_mesh_draw(&MeshTankFlash);

					jo_3d_push_matrix();
					{
						jo_3d_translate_matrix_fixed(0, 0, jo_fixed_div(jo_sin(tank->FiringAnimationFrames * 3), JO_FIXED_4));
						jo_3d_mesh_draw(&MeshTankGun);
					}
					jo_3d_pop_matrix();

					tank->FiringAnimationFrames += tank->FiringAnimationFrames < animationHalf ? 3 : 1;

					if (tank->FiringAnimationFrames >= PLAYER_FIRE_ANIMATION)
					{
						tank->FiringAnimationFrames = 0;
					}
				}
				else
				{
					jo_3d_mesh_draw(&MeshTankGun);
				}
			}
		}
		jo_3d_pop_matrix();
	}
	jo_3d_pop_matrix();
}