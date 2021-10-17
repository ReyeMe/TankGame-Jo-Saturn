#include <jo/jo.h>
#include "tank.h"
#include "../modelLoader.h"

/**
 * Tank color table
 * When color is set to 0, tank will be white, PUT ONLY AT THE END OF A LIST!
 */
static jo_color TankColors[TANK_COLOR_COUNT] =
	{
		JO_COLOR_RGB(255, 150, 150),
		JO_COLOR_RGB(150, 150, 255)};

static jo_3d_mesh * TankMeshes[TANK_COLOR_COUNT];

/* Model texture loader
 * @texture Current texture to load
 * @count Total number of textures to load
 * @return Index of first loaded texture
 */
static int ModelTextureLoader(TmfTexture * texture, const char * modelDir, int count)
{
    static int startIndex = -1;

	if (startIndex < 0)
	{
    	for (int index = 0; index < count; index++)
    	{
			jo_printf(1,1, "Loading... %d/%d", index + 1, count);
			slSynch();

    	    if (texture[index].FileName[0] != '\0')
    	    {
    	        int loadedAt = jo_sprite_add_tga(modelDir, texture[index].FileName, JO_COLOR_Black);

    	        if (startIndex == -1)
    	            startIndex = loadedAt;
    	    }
    	}

		jo_clear_screen_line(1);
	}

    return startIndex;
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

void Tank_Load(void)
{
	jo_color *ptr;
	int loadedCount = 0;
	jo_3d_mesh * loaded;

	for (int color = 0; color < TANK_COLOR_COUNT; color++)
	{
		loaded = ML_LoadMeshWithCustomTextureLoader("TANK.TMF", "MODELS", &ModelTextureLoader, &loadedCount);

		if (loadedCount != 4)
		{
			jo_core_error("TANK.TMF is corrupted (%d)", loadedCount);
		}

		// Load gouraud table
		if (TankColors[color])
		{
			ptr = (jo_color *)(JO_VDP1_VRAM + 0x70000 + JO_MULT_BY_8(color));
			*ptr = TankColors[color];
			*(ptr + 1) = TankColors[color];
			*(ptr + 2) = TankColors[color];
			*(ptr + 3) = TankColors[color];
		}

		// First is flash, so skip that
		for (int model = 1; model < loadedCount; model++)
		{
			// Set gouraud
			for (Uint32 iterator = 0; iterator < loaded[model].data.nbPolygon; iterator++)
			{
				// Add gouraud if not a solid color polygon/threads or tank color is white
				if (loaded[model].data.attbl[iterator].texno != 0)
				{
					loaded[model].data.attbl[iterator].gstb = 0xe000 + color;
					JO_ADD_FLAG(loaded[model].data.attbl[iterator].atrb, CL_Gouraud);
				}
			}
		}

		TankMeshes[color] = loaded;
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
			jo_3d_mesh_draw(&(TankMeshes[(int)tank->Color][3]));

			if (!tank->IsExploded)
			{
				jo_3d_rotate_matrix_y(tank->TowerAngle);
				jo_3d_mesh_draw(&(TankMeshes[(int)tank->Color][2]));

				if (tank->FiringAnimationFrames != 0)
				{
					int animationHalf = JO_DIV_BY_2(PLAYER_FIRE_ANIMATION);

					if (tank->FiringAnimationFrames < animationHalf)
						jo_3d_mesh_draw(&(TankMeshes[(int)tank->Color][0]));

					jo_3d_push_matrix();
					{
						jo_3d_translate_matrix_fixed(0, 0, jo_fixed_div(jo_sin(tank->FiringAnimationFrames * 3), JO_FIXED_4));
						jo_3d_mesh_draw(&(TankMeshes[(int)tank->Color][1]));
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
					jo_3d_mesh_draw(&(TankMeshes[(int)tank->Color][1]));
				}
			}
		}
		jo_3d_pop_matrix();
	}
	jo_3d_pop_matrix();
}