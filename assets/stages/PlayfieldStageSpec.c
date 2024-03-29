/**
 * SPONG
 *
 * © Jorge Eremiev <jorgech3@gmail.com> and Christian Radke <c.radke@posteo.de>
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */


//---------------------------------------------------------------------------------------------------------
//												INCLUDES
//---------------------------------------------------------------------------------------------------------

#include <Stage.h>
#include <Fonts.h>
#include <Collision.h>
#include <macros.h>


//---------------------------------------------------------------------------------------------------------
//											DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern EntitySpec PongBallPb;
extern EntitySpec PaddleLeftEntity;
extern EntitySpec PaddleRightEntity;
extern EntitySpec PlayfieldEntity;
extern EntitySpec LowPowerIndicatorEntity;
extern EntitySpec TransitionLayerBAe;
extern EntitySpec CollisionWallCl;
extern EntitySpec CollisionCeilingCl;
extern EntitySpec CollisionFloorCl;
extern EntitySpec PongBallLightEntity;
extern EntitySpec PongBallParticlesParticleSystem;

extern CharSetSpec PaddleLeftLCharset;
extern CharSetSpec PaddleLeftRCharset;
extern CharSetSpec PaddleRightLCharset;
extern CharSetSpec PaddleRightRCharset;
extern CharSetSpec PlayfieldCharset;

extern TextureSpec PaddleLeftLTexture;
extern TextureSpec PaddleLeftRTexture;
extern TextureSpec PaddleRightLTexture;
extern TextureSpec PaddleRightRTexture;
extern TextureSpec PlayfieldLTexture;
extern TextureSpec PlayfieldRTexture;

extern uint16 GAME_BGM_1[][2];

const CollisionExtraInfo horizontalWallCollision =
{
	{48 * 8, 	4 * 8, 		48 * 8},
	kLayerPlayFieldWalls
};

const CollisionExtraInfo verticalWallCollision =
{
	{4 * 8, 	28 * 8, 	48 * 8},
	kLayerPlayFieldWalls
};

const CollisionExtraInfo ceilingCollision =
{
	{48 * 8, 	28 * 8, 	4 * 8},
	kLayerPlayFieldCeiling
};

const CollisionExtraInfo floorCollision =
{
	{48 * 8, 	28 * 8, 	4 * 8},
	kLayerPlayFieldFloor
};

const CollisionExtraInfo splitterCollision =
{
	{2 * 8, 	28 * 8, 	48 * 8},
	kLayerPlayFieldSplitter
};


//---------------------------------------------------------------------------------------------------------
// 											ENTITY LISTS
//---------------------------------------------------------------------------------------------------------

PositionedEntityROMSpec PongBallChildren[] =
{
	{&PongBallParticlesParticleSystem, 			{0,	0,	 1, 0}, 	0, "Partcls", NULL, NULL, false},

	{NULL, {0,0,0,0}, 0, NULL, NULL, NULL, false},
};


PositionedEntityROMSpec PlayfieldStageEntities[] =
{
	{&PlayfieldEntity, 			{192,	120,	 96+8, 0}, 	0, NULL, NULL, NULL, false},

	{&PaddleLeftEntity, 			{192-96, 112,	 96, 0}, 	0, PADDLE_LEFT_NAME, NULL, NULL, false},
	{&PaddleRightEntity, 			{192+96, 112,	 96, 0}, 	0, PADDLE_RIGHT_NAME, NULL, NULL, false},
	{&PongBallPb, 			{192,	112,	 0, 0}, 	0, PONG_BALL_NAME, (struct PositionedEntity*)PongBallChildren, NULL, false},
	{&PongBallLightEntity,		{192,	112,	 96+8+1, 0}, 	0, NULL, NULL, NULL, false},

	{&CollisionFloorCl,		{192, 112,	 96+16, 0},	0, NULL, NULL, (void*)&floorCollision, false}, // far border
	{&CollisionCeilingCl,		{192,	112,	  -48, 0},	0, NULL, NULL, (void*)&ceilingCollision, false}, // front border
	{&CollisionWallCl,		{ 12, 112,	  0, 0},	0, NULL, NULL, (void*)&verticalWallCollision, false}, // left border
	{&CollisionWallCl,		{384-12, 112,	  0, 0},	0, NULL, NULL, (void*)&verticalWallCollision, false}, // right border
	{&CollisionWallCl,		{192,	16+12,	0, 0},	0, NULL, NULL, (void*)&horizontalWallCollision, false}, // top border
	{&CollisionWallCl,		{192,	224-12,   0, 0},	0, NULL, NULL, (void*)&horizontalWallCollision, false}, // bottom border
	{&CollisionWallCl,		{192,	112,	 48, 0},	0, NULL, NULL, (void*)&splitterCollision, false}, // splitter border

	{&TransitionLayerBAe,	{192,	112,	  0, 0}, 	0, "TRNSLYR", NULL, NULL, false},

	{NULL, {0,0,0,0}, 0, NULL, NULL, NULL, false},
};

PositionedEntityROMSpec PlayfieldStageUiEntities[] =
{
	{&LowPowerIndicatorEntity, 	{ 25,  	 25, 	  -1, 0},	0, NULL, NULL, NULL, false},

	{NULL, {0,0,0,0}, 0, NULL, NULL, NULL, false},
};


//---------------------------------------------------------------------------------------------------------
// 											PRELOAD LISTS
//---------------------------------------------------------------------------------------------------------

FontROMSpec* const PlayfieldStageFonts[] =
{
	&IndustrialFont,

	NULL
};

CharSetROMSpec* const PlayfieldStageCharsets[] =
{
	&PaddleLeftLCharset,
	&PaddleLeftRCharset,
	&PaddleRightLCharset,
	&PaddleRightRCharset,
	&PlayfieldCharset,

	NULL
};

TextureSpec* const PlayfieldStageTextures[] =
{
	&PaddleLeftLTexture,
	&PaddleLeftRTexture,
	&PaddleRightLTexture,
	&PaddleRightRTexture,
	&PlayfieldLTexture,
	&PlayfieldRTexture,

	NULL
};


//---------------------------------------------------------------------------------------------------------
//											STAGE DEFINITION
//---------------------------------------------------------------------------------------------------------

StageROMSpec PlayfieldStage =
{
	// allocator
	__TYPE(Stage),

	// Timer config
	{
		__TIMER_100US,
		10,
		kMS
	},

	// Sound config
	{
		__DEFAULT_PCM_HZ,
		0
	},

	// level
	{
		// size
		{
			// x
			__SCREEN_WIDTH,
			// y
			__SCREEN_HEIGHT,
			// z
			__SCREEN_DEPTH,
		},

		// camera's initial position inside the game world
		{
			// x
			0,
			// y
			0,
			// z
			0,
			// p
			0
		},

		// camera's frustum
		{
			// x0
			0,
			// y0
			0,
			// z0
			-10,
			// x1
			__SCREEN_WIDTH,
			// y1
			__SCREEN_HEIGHT,
			// z1
			__SCREEN_WIDTH * 5
		}
	},

	// streaming
	{
		// load padding
		40,
		// unload padding
		16,
		// streaming amplitude
		24,
		// particle removal delay cycles
		0,
		// deferred
		false,
	},

	// rendering
	{
		// maximum number of texture's rows to write each time the texture writing is active
		64,

		// maximum number of rows to compute on each call to the affine functions
		64,

		// colors config
		{
			// background color
			__COLOR_BLACK,

			// brightness
			// these values times the repeat values specified in the column table (max. 16) make the final
			// brightness values on the respective regions of the screen. maximum brightness is 128.
			{
				// dark red
				__BRIGHTNESS_DARK_RED,
				// medium red
				__BRIGHTNESS_MEDIUM_RED,
				// bright red
				__BRIGHTNESS_BRIGHT_RED,
			},

			// brightness repeat
			(BrightnessRepeatSpec*)NULL,
		},

		// palettes' config
		{
			{
				// bgmap palette 0
				__BGMAP_PALETTE_0,
				// bgmap palette 1
				__BGMAP_PALETTE_1,
				// bgmap palette 2
				__BGMAP_PALETTE_2,
				// bgmap palette 3
				__BGMAP_PALETTE_3,
			},
			{
				// object palette 0
				__OBJECT_PALETTE_0,
				// object palette 1
				__OBJECT_PALETTE_1,
				// object palette 2
				__OBJECT_PALETTE_2,
				// object palette 3
				__OBJECT_PALETTE_3,
			},
		},

		// bgmap segments configuration
		// number of segments reserved for the param table
		2,

		// obj segments sizes (must total 1024)
		{
			// __spt0
			__AVAILABLE_CHAR_OBJECTS / __TOTAL_OBJECT_SEGMENTS,
			// __spt1
			__AVAILABLE_CHAR_OBJECTS / __TOTAL_OBJECT_SEGMENTS,
			// __spt2
			__AVAILABLE_CHAR_OBJECTS / __TOTAL_OBJECT_SEGMENTS,
			// __spt3
			__AVAILABLE_CHAR_OBJECTS / __TOTAL_OBJECT_SEGMENTS,
		},

		// OBJECT segments z coordinates
		// note that each spt's z coordinate much be larger than or equal to the previous one's,
		// since the vip renders obj worlds in reverse order (__spt3 to __spt0)
		{
			// __spt0
			0,
			// __spt1
			32,
			// __spt2
			64,
			// __spt3
			96,
		},

		// optical configuration values
		{
			// maximum view distance's power into the horizon
			__MAXIMUM_X_VIEW_DISTANCE, __MAXIMUM_X_VIEW_DISTANCE,
			// distance of the eyes to the screen
			__CAMERA_NEAR_PLANE,
			// distance from left to right eye (depth sensation)
			__BASE_FACTOR,
			// horizontal view point center
			__HORIZONTAL_VIEW_POINT_CENTER,
			// vertical view point center
			__VERTICAL_VIEW_POINT_CENTER,
			// scaling factor
			__SCALING_MODIFIER_FACTOR,
		},
	},

	// physics
	{
		// gravity
		{
			__F_TO_FIX10_6(__GRAVITY),
			__F_TO_FIX10_6(__GRAVITY),
			__F_TO_FIX10_6(__GRAVITY)
		},

		// friction
		__F_TO_FIX10_6(0.0f),
	},

	// assets
	{
		// fonts to preload
		(FontSpec**)PlayfieldStageFonts,

		// char sets to preload
		(CharSetSpec**)PlayfieldStageCharsets,

		// textures to preload
		(TextureSpec**)PlayfieldStageTextures,

		// background music
		(Sound**)NULL,
	},

	// entities
	{
		// ui
		{
			(PositionedEntity*)PlayfieldStageUiEntities,
			__TYPE(UIContainer),
		},

		// children
		(PositionedEntity*)PlayfieldStageEntities,
	},

	// post processing effects
	(PostProcessingEffect*)NULL,
};
