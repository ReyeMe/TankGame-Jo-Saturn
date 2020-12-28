#include <jo/jo.h>
#include "tank.h"

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
	int iterator = 0;

	// Load blue tank
	spriteStartIndex = jo_sprite_add_tga("TBLUE", "THREADS.TGA", JO_COLOR_Transparent);
	jo_sprite_add_tga("TBLUE", "SIDES.TGA", JO_COLOR_Transparent);
	jo_sprite_add_tga("TBLUE", "BACK.TGA", JO_COLOR_Transparent);
	jo_sprite_add_tga("TBLUE", "TOP.TGA", JO_COLOR_Transparent);
	jo_sprite_add_tga("TBLUE", "TBOT.TGA", JO_COLOR_Transparent);
	jo_sprite_add_tga("TBLUE", "TSIDES.TGA", JO_COLOR_Transparent);
	jo_sprite_add_tga("TBLUE", "FFIRE.TGA", JO_COLOR_Black);

	for (iterator = 0; iterator < 21; iterator++)
		AttributesBlueTankBody[iterator].texno += spriteStartIndex;

	for (iterator = 0; iterator < 5; iterator++)
		AttributesBlueTankTower[iterator].texno += spriteStartIndex;

	// Load Red tank
	spriteStartIndex = jo_sprite_add_tga("TRED", "THREADS.TGA", JO_COLOR_Transparent);
	jo_sprite_add_tga("TRED", "SIDES.TGA", JO_COLOR_Transparent);
	jo_sprite_add_tga("TRED", "BACK.TGA", JO_COLOR_Transparent);
	jo_sprite_add_tga("TRED", "TOP.TGA", JO_COLOR_Transparent);
	jo_sprite_add_tga("TRED", "TBOT.TGA", JO_COLOR_Transparent);
	jo_sprite_add_tga("TRED", "TSIDES.TGA", JO_COLOR_Transparent);
	jo_sprite_add_tga("TRED", "FFIRE.TGA", JO_COLOR_Black);

	for (iterator = 0; iterator < 21; iterator++)
		AttributesRedTankBody[iterator].texno += spriteStartIndex;

	for (iterator = 0; iterator < 5; iterator++)
		AttributesRedTankTower[iterator].texno += spriteStartIndex;
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

	return thrust;
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
			jo_3d_mesh_draw(TankColors[tank->Color * 2]);

			if (!tank->IsExploded)
			{
				jo_3d_rotate_matrix_y(tank->TowerAngle);
				jo_3d_mesh_draw(TankColors[(tank->Color * 2) + 1]);

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