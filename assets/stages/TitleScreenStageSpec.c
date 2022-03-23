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
#include <macros.h>


//---------------------------------------------------------------------------------------------------------
//											DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern EntitySpec CopyrightEntity;
extern EntitySpec RhombusBackgroundAe;
extern EntitySpec DemoBadgeLe;
extern EntitySpec MainMenuChallengeModeLe;
extern EntitySpec MainMenuCreditsLe;
extern EntitySpec MainMenuCursorAe;
extern EntitySpec MainMenuHighscoresLe;
extern EntitySpec MainMenuMarathonModeLe;
extern EntitySpec MainMenuOptionsLe;
extern EntitySpec MainMenuVersusModeLe;
extern EntitySpec MainMenuWaitingForOtherPlayerLe;
extern EntitySpec PressStartButtonLe;
extern EntitySpec LogoAe;
extern EntitySpec TransitionLayerBAe;
extern EntitySpec LowPowerIndicatorEntity;

extern uint16 TITLE_BGM[][2];


//---------------------------------------------------------------------------------------------------------
//												ASSETS
//---------------------------------------------------------------------------------------------------------

EntityROMSpec MainMenuContainer =
{
	// class allocator
	__TYPE(Entity),

	// children
	NULL,

	// behaviors
	NULL,

	// extra
	NULL,

	// sprites
	(SpriteSpec**)NULL,

	// use z displacement in projection
	false,

	// collision shapes
	NULL,

	// size
	// if 0, width and height will be inferred from the first sprite's texture's size
	{0, 0, 0},

	// gameworld's character's type
	kTypeNone,

	// physical specification
	NULL,
};

PositionedEntityROMSpec MainMenuChildren[] =
{
	{&MainMenuCursorAe, 			{0,   0, 0, 0}, 	0, "MMCursor", NULL, NULL, false},
	{&MainMenuMarathonModeLe, 	{0, -24, 0, 0}, 	0, NULL, NULL, NULL, false},
	{&MainMenuChallengeModeLe, 	{0, -12, 0, 0}, 	0, NULL, NULL, NULL, false},
	{&MainMenuVersusModeLe,		{0,   0, 0, 0}, 	0, "MMVersus", NULL, NULL, false},
	{&MainMenuHighscoresLe, 		{0,  12, 0, 0}, 	0, NULL, NULL, NULL, false},
	{&MainMenuOptionsLe, 		{0,  24, 0, 0}, 	0, NULL, NULL, NULL, false},
	{&MainMenuCreditsLe, 		{0,  36, 0, 0}, 	0, NULL, NULL, NULL, false},

	{NULL, {0,0,0,0}, 0, NULL, NULL, NULL, false},
};


//---------------------------------------------------------------------------------------------------------
// 											ENTITY LISTS
//---------------------------------------------------------------------------------------------------------

PositionedEntityROMSpec TitleScreenStageEntities[] =
{
	{&RhombusBackgroundAe,					{192, 112,  0,  0},   	0, NULL, NULL, NULL, false},
	{&LogoAe, 									{192,  88,  0,  0}, 	0, NULL, NULL, NULL, false},
	{&DemoBadgeLe, 							{314, 104,  0,  0}, 	0, NULL, NULL, NULL, false},
	{(EntitySpec*)&MainMenuContainer, 		{192, 156,  0,  0}, 	0, "MainMenu", (struct PositionedEntity*)MainMenuChildren, NULL, false},
	{&PressStartButtonLe, 					{192, 168,  0,  0}, 	0, "PrssStrt", NULL, NULL, false},
	{&CopyrightEntity, 							{192, 212,  0,  0}, 	0, NULL, NULL, NULL, false},

	{&TransitionLayerBAe,					{192, 112,  0,  0},  	0, "TRNSLYR", NULL, NULL, false},
	{&MainMenuWaitingForOtherPlayerLe, 	{192, 32, -1,  0}, 	0, "Waiting", NULL, NULL, false},

	{NULL, {0,0,0,0}, 0, NULL, NULL, NULL, false},
};

PositionedEntityROMSpec TitleScreenStageUiEntities[] =
{
	{&LowPowerIndicatorEntity, 		{ 16,  12,  -1,  0},		0, NULL, NULL, NULL, false},

	{NULL, {0,0,0,0}, 0, NULL, NULL, NULL, false},
};


//---------------------------------------------------------------------------------------------------------
// 											PRELOAD LISTS
//---------------------------------------------------------------------------------------------------------

FontROMSpec* const TitleScreenStageFonts[] =
{
	&IndustrialFont,

	NULL
};

CharSetROMSpec* const TitleScreenStageCharsets[] =
{
	NULL
};

TextureSpec* const TitleScreenStageTextures[] =
{
	NULL
};


//---------------------------------------------------------------------------------------------------------
//											STAGE DEFINITION
//---------------------------------------------------------------------------------------------------------

StageROMSpec TitleScreenStage =
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
		16,

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
		1,

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
			0,
			// __spt2
			0,
			// __spt3
			0,
		},

		// optical configuration values
		{
			// maximum view distance's power into the horizon
			__MAXIMUM_X_VIEW_DISTANCE, __MAXIMUM_Y_VIEW_DISTANCE,
			// distance of the eyes to the screen
			__DISTANCE_EYE_SCREEN,
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
			__I_TO_FIX10_6(0),
			__F_TO_FIX10_6(0),
			__I_TO_FIX10_6(0)
		},

		// friction
		__F_TO_FIX10_6(0.1f),
	},

	// assets
	{
		// fonts to preload
		(FontSpec**)TitleScreenStageFonts,

		// char sets to preload
		(CharSetSpec**)TitleScreenStageCharsets,

		// textures to preload
		(TextureSpec**)TitleScreenStageTextures,

		// background music
		(Sound**)NULL,
	},

	// entities
	{
		// ui
		{
			(PositionedEntity*)TitleScreenStageUiEntities,
			__TYPE(UIContainer),
		},

		// children
		(PositionedEntity*)TitleScreenStageEntities,
	},

	// post processing effects
	(PostProcessingEffect*)NULL,
};
