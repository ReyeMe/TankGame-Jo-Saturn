#include <jo/jo.h>
#include "tank.h"

void Tank_Create(tank_Object * tank, const char color, const short tankAngle, const jo_fixed x, const jo_fixed y, const jo_fixed z)
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
	tank->InGame = false;
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
	
	for(iterator = 0; iterator < 21; iterator++)
		AttributesBlueTankBody[iterator].texno += spriteStartIndex;

	for(iterator = 0; iterator < 5; iterator++)
		AttributesBlueTankTower[iterator].texno += spriteStartIndex;

	// Load Red tank
	spriteStartIndex = jo_sprite_add_tga("TRED", "THREADS.TGA", JO_COLOR_Transparent);
	jo_sprite_add_tga("TRED", "SIDES.TGA", JO_COLOR_Transparent);
	jo_sprite_add_tga("TRED", "BACK.TGA", JO_COLOR_Transparent);
	jo_sprite_add_tga("TRED", "TOP.TGA", JO_COLOR_Transparent);
	jo_sprite_add_tga("TRED", "TBOT.TGA", JO_COLOR_Transparent);
	jo_sprite_add_tga("TRED", "TSIDES.TGA", JO_COLOR_Transparent);
	jo_sprite_add_tga("TRED", "FFIRE.TGA", JO_COLOR_Black);

	for(iterator = 0; iterator < 21; iterator++)
		AttributesRedTankBody[iterator].texno += spriteStartIndex;
		
	for(iterator = 0; iterator < 5; iterator++)
		AttributesRedTankTower[iterator].texno += spriteStartIndex;
}

void Tank_Draw(tank_Object * tank)
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
						jo_3d_translate_matrix_fixed(0,0,jo_fixed_div(jo_sin(tank->FiringAnimationFrames * 3), JO_FIXED_4));
						jo_3d_mesh_draw(&MeshTankGun);
    				}
    				jo_3d_pop_matrix();

					tank->FiringAnimationFrames+= tank->FiringAnimationFrames < animationHalf ? 3 : 1;

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