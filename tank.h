#ifndef __3D_MODELTANK_H__
#define __3D_MODELTANK_H__

#define PLAYER_BOX_SIZE JO_FIXED_4
#define PLAYER_BOX_SIZEH JO_FIXED_2
#define PLAYER_SPEED 6553
#define PLAYER_FIRE_ANIMATION 60

static POINT PointsTankBody[] =
	{
		{-71559, 15080, -135346},
		{71559, 15080, -135346},
		{108423, -27760, -135346},
		{118253, -23691, -30340},
		{-71559, 15080, 94305},
		{118253, -23691, -137209},
		{-118253, 0, 117115},
		{-88762, -23691, -169198},
		{88762, -49152, -169198},
		{118254, -49152, 117115},
		{69101, -80155, -88057},
		{118253, 0, 117115},
		{-69101, -80155, 67963},
		{-69101, -80155, -88057},
		{118253, -49152, -137209},
		{-118253, -49152, -30340},
		{-118253, -23691, -30340},
		{118253, -49152, -30340},
		{-108423, -27760, 94305},
		{-108423, 15080, 94305},
		{-108423, 15080, -135346},
		{-108423, -27760, -135346},
		{108423, 15080, -135346},
		{108423, -27760, 94305},
		{108423, 15080, 94305},
		{71559, 15080, 94305},
		{71559, -27760, 94305},
		{71559, -27760, -135346},
		{-71559, -27760, -135346},
		{-71559, -27760, 94305},
		{69102, -80155, 67963},
		{-118253, -49152, -137209},
		{-118253, -49152, 117115},
		{88762, -23691, -169198},
		{-118253, -23691, -137209},
		{-88762, -49152, -169198},
		{-71559, 15080, 94305},
		{-71559, 15080, -135346},
		{71559, -27760, 94305},
		{71559, 15080, 94305},
		{108423, -27760, -135346},
		{71559, -27760, -135346},
		{-108423, 15080, 94305},
		{-108423, -27760, 94305},
		{108423, 15080, -135346},
		{108423, 15080, 94305},
		{-69101, -80155, -88057},
		{-69101, -80155, -88057},
		{-69101, -80155, -88057},
		{-69101, -80155, 67963},
		{-69101, -80155, 67963},
		{-118253, -23691, -30340},
		{-118253, -23691, -137209},
		{-71559, -27760, 94305},
		{-118253, -49152, -30340},
		{-118253, -49152, 117115},
		{-118253, -49152, 117115},
		{-118253, -49152, 117115},
		{118253, -49152, -30340},
		{118253, -49152, -137209},
		{118253, -49152, -137209},
		{118253, -49152, -137209},
		{-88762, -49152, -169198},
		{-88762, -49152, -169198},
		{-88762, -49152, -169198},
		{88762, -49152, -169198},
		{88762, -49152, -169198},
		{88762, -49152, -169198},
		{-118253, 0, 117115},
		{69102, -80155, 67963},
		{69102, -80155, 67963},
		{118253, -23691, -137209},
		{118254, -49152, 117115},
		{118254, -49152, 117115},
		{118254, -49152, 117115},
		{118253, 0, 117115},
		{108423, -27760, 94305},
		{71559, 15080, -135346},
		{-71559, -27760, -135346},
		{-108423, -27760, -135346},
		{-108423, 15080, -135346},
		{118253, -23691, -30340},
		{69101, -80155, -88057},
		{69101, -80155, -88057},
		{69101, -80155, -88057},
		{-118253, -49152, -137209},
		{-118253, -49152, -137209},
		{-118253, -49152, -137209},
		{88762, -23691, -169198},
		{-88762, -23691, -169198}};

static POINT PointsTankTower[] =
	{
		{34406, -144877, 29491},
		{34406, -144877, -29491},
		{-34406, -144877, 29491},
		{-34406, -144877, -29491},
		{68812, -83249, 58982},
		{68812, -83249, -58982},
		{-68812, -83249, -58982},
		{-68812, -83249, 58982}};

static POINT PointsTankGun[] =
	{
		{9830, -104110, -40452},
		{9830, -123771, -40452},
		{9830, -104110, -170317},
		{9830, -123771, -170317},
		{-9830, -104110, -40452},
		{-9830, -123771, -40452},
		{-9830, -104110, -170317},
		{-9830, -123771, -170317}};

static POINT PointsTankFlash[] =
	{
		{29491, -85184, -172550},
		{29491, -144167, -172550},
		{98304, -16372, -273473},
		{98304, -212980, -273473},
		{-29491, -85184, -172550},
		{-29491, -144167, -172550},
		{-98304, -16372, -273473},
		{-98304, -212980, -273473},
		{20853, -114676, -172550},
		{0, -135529, -172550},
		{0, -93822, -172550},
		{-20853, -114676, -172550},
		{0, -114676, -209177}};

static POLYGON PolygonsTankBody[] =
	{
		{{-65536, 0, 0}, {39, 77, 27, 38}},
		{{65536, 0, 0}, {37, 36, 29, 78}},
		{{0, 0, -65536}, {40, 41, 1, 22}},
		{{0, 0, -65536}, {28, 79, 80, 0}},
		{{65536, 0, 0}, {20, 42, 43, 21}},
		{{-65536, 0, 0}, {45, 44, 2, 76}},
		{{65536, 0, 0}, {71, 81, 58, 59}},
		{{-65536, 0, 0}, {68, 16, 54, 55}},
		{{-34963, -55430, 0}, {49, 57, 86, 46}},
		{{34963, -55430, 0}, {70, 82, 60, 73}},
		{{-19641, -59847, -18107}, {63, 48, 31, 31}},
		{{-48182, 0, 44420}, {5, 88, 66, 61}},
		{{0, 0, 65536}, {75, 6, 56, 72}},
		{{0, -61217, -23389}, {47, 62, 65, 83}},
		{{0, -65536, 0}, {84, 69, 50, 13}},
		{{48182, 0, 44420}, {89, 34, 87, 64}},
		{{-65536, 0, 0}, {51, 52, 85, 15}},
		{{65536, 0, 0}, {3, 11, 74, 17}},
		{{19641, -59847, -18107}, {10, 67, 14, 14}},
		{{0, -55430, 34963}, {30, 9, 32, 12}},
		{{0, 0, 65536}, {33, 7, 35, 8}}};

static POLYGON PolygonsTankTower[] =
	{
		{{57219, -31948, 0}, {4, 0, 1, 5}},
		{{0, -28291, -59113}, {5, 1, 3, 6}},
		{{-57219, -31948, 0}, {6, 3, 2, 7}},
		{{0, -28291, 59113}, {7, 2, 0, 4}},
		{{0, -65536, 0}, {3, 1, 0, 2}}};

static POLYGON PolygonsTankGun[] =
	{
		{{65536, 0, 0}, {0, 1, 3, 2}},
		{{0, 0, -65536}, {2, 3, 7, 6}},
		{{-65536, 0, 0}, {6, 7, 5, 4}},
		{{0, -65536, 0}, {7, 3, 1, 5}}};

static POLYGON PolygonsTankFlash[] =
	{
		{{0, 0, -65536}, {4, 0, 1, 5}},
		{{42985, -42985, -24477}, {9, 12, 8, 8}},
		{{-42985, -42985, -24477}, {11, 12, 9, 9}},
		{{-42985, 42985, -24477}, {12, 11, 10, 10}},
		{{42985, 42985, -24477}, {12, 10, 8, 8}}};

static ATTR AttributesTankGun[] =
	{
		ATTRIBUTE(Dual_Plane, SORT_CEN, No_Texture, C_RGB(70, 70, 70), CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprPolygon, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, No_Texture, C_RGB(10, 10, 10), CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprPolygon, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, No_Texture, C_RGB(70, 70, 70), CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprPolygon, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, No_Texture, C_RGB(70, 70, 70), CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprPolygon, No_Option)};

static ATTR AttributesTankFlash[] =
	{
		ATTRIBUTE(Dual_Plane, SORT_CEN, 6, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHon, sprNoflip, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, No_Texture, C_RGB(255, 255, 0), CL32KRGB | No_Gouraud, CL32KRGB | MESHon, sprPolygon, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, No_Texture, C_RGB(255, 255, 0), CL32KRGB | No_Gouraud, CL32KRGB | MESHon, sprPolygon, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, No_Texture, C_RGB(255, 255, 0), CL32KRGB | No_Gouraud, CL32KRGB | MESHon, sprPolygon, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, No_Texture, C_RGB(255, 255, 0), CL32KRGB | No_Gouraud, CL32KRGB | MESHon, sprPolygon, No_Option)};

static ATTR AttributesBlueTankBody[] =
	{
		ATTRIBUTE(Dual_Plane, SORT_CEN, 0, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprNoflip, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, 0, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprNoflip, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, No_Texture, C_RGB(0, 0, 0), CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprPolygon, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, No_Texture, C_RGB(0, 0, 0), CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprPolygon, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, 0, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprNoflip, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, 0, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprNoflip, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, 1, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprNoflip, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, 1, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprNoflip, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, 3, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprNoflip, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, 3, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprNoflip, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, 3, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprNoflip, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, 1, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprNoflip, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, 2, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprNoflip, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, 3, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprNoflip, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, 4, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprNoflip, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, 1, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprNoflip, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, 1, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprNoflip, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, 1, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprNoflip, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, 3, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprNoflip, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, 3, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprNoflip, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, 1, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprNoflip, No_Option)};

static ATTR AttributesBlueTankTower[] =
	{
		ATTRIBUTE(Dual_Plane, SORT_CEN, 5, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprNoflip, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, 5, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprNoflip, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, 5, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprNoflip, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, 5, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprNoflip, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, 5, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprNoflip, No_Option)};

static ATTR AttributesRedTankBody[] =
	{
		ATTRIBUTE(Dual_Plane, SORT_CEN, 0, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprNoflip, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, 0, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprNoflip, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, No_Texture, C_RGB(0, 0, 0), CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprPolygon, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, No_Texture, C_RGB(0, 0, 0), CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprPolygon, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, 0, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprNoflip, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, 0, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprNoflip, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, 1, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprNoflip, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, 1, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprNoflip, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, 3, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprNoflip, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, 3, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprNoflip, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, 3, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprNoflip, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, 1, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprNoflip, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, 2, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprNoflip, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, 3, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprNoflip, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, 4, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprNoflip, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, 1, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprNoflip, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, 1, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprNoflip, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, 1, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprNoflip, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, 3, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprNoflip, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, 3, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprNoflip, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, 1, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprNoflip, No_Option)};

static ATTR AttributesRedTankTower[] =
	{
		ATTRIBUTE(Dual_Plane, SORT_CEN, 5, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprNoflip, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, 5, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprNoflip, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, 5, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprNoflip, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, 5, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprNoflip, No_Option),
		ATTRIBUTE(Dual_Plane, SORT_CEN, 5, No_Palet, CL32KRGB | No_Gouraud, CL32KRGB | MESHoff, sprNoflip, No_Option)};

static jo_3d_mesh MeshTankGun =
	{
		.data =
			{
				PointsTankGun,
				8,
				PolygonsTankGun,
				4,
				AttributesTankGun}};

static jo_3d_mesh MeshTankFlash =
	{
		.data =
			{
				PointsTankFlash,
				13,
				PolygonsTankFlash,
				5,
				AttributesTankFlash}};

static jo_3d_mesh MeshBlueTankBody =
	{
		.data =
			{
				PointsTankBody,
				90,
				PolygonsTankBody,
				21,
				AttributesBlueTankBody}};

static jo_3d_mesh MeshBlueTankTower =
	{
		.data =
			{
				PointsTankTower,
				8,
				PolygonsTankTower,
				5,
				AttributesBlueTankTower}};

static jo_3d_mesh MeshRedTankBody =
	{
		.data =
			{
				PointsTankBody,
				90,
				PolygonsTankBody,
				21,
				AttributesRedTankBody}};

static jo_3d_mesh MeshRedTankTower =
	{
		.data =
			{
				PointsTankTower,
				8,
				PolygonsTankTower,
				5,
				AttributesRedTankTower}};

/**
 * Tank color table
 */
static jo_3d_mesh *TankColors[] =
	{
		&MeshBlueTankBody,
		&MeshBlueTankTower,
		&MeshRedTankBody,
		&MeshRedTankTower};

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
 * Load tank textures
 */
void Tank_Load_textures(void);

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
