/* VUEngine - Virtual Utopia Engine <http://vuengine.planetvb.com/>
 * A universal game engine for the Nintendo Virtual Boy
 *
 * Copyright (C) 2007, 2018 by Jorge Eremiev <jorgech3@gmail.com> and Christian Radke <chris@vr32.de>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
 * LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN
 * NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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

extern EntitySpec COPYRIGHT_EN;
extern EntitySpec RHOMBUS_BACKGROUND_AE;
extern EntitySpec DEMO_BADGE_LE;
extern EntitySpec MAIN_MENU_CHALLENGE_MODE_LE;
extern EntitySpec MAIN_MENU_CREDITS_LE;
extern EntitySpec MAIN_MENU_CURSOR_AE;
extern EntitySpec MAIN_MENU_HIGHSCORES_LE;
extern EntitySpec MAIN_MENU_MARATHON_MODE_LE;
extern EntitySpec MAIN_MENU_OPTIONS_LE;
extern EntitySpec MAIN_MENU_VERSUS_MODE_LE;
extern EntitySpec MAIN_MENU_WAITING_FOR_OTHER_PLAYER_LE;
extern EntitySpec PRESS_START_BUTTON_LE;
extern EntitySpec LOGO_AE;
extern EntitySpec TRANSITION_LAYER_B_AE;
extern EntitySpec LOW_POWER_INDICATOR_LB;

extern u16 TITLE_BGM[][2];


//---------------------------------------------------------------------------------------------------------
//												ASSETS
//---------------------------------------------------------------------------------------------------------

EntityROMSpec MAIN_MENU_CONTAINER =
{
	// class allocator
	__TYPE(Entity),

	// behaviors
	NULL,

	// sprites
	(SpriteSpec**)NULL,

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

PositionedEntityROMSpec MAIN_MENU_CHILDREN[] =
{
	{&MAIN_MENU_CURSOR_AE, 			{0,   0, 0, 0}, 	0, "MMCursor", NULL, NULL, false},
	{&MAIN_MENU_MARATHON_MODE_LE, 	{0, -24, 0, 0}, 	0, NULL, NULL, NULL, false},
	{&MAIN_MENU_CHALLENGE_MODE_LE, 	{0, -12, 0, 0}, 	0, NULL, NULL, NULL, false},
	{&MAIN_MENU_VERSUS_MODE_LE,		{0,   0, 0, 0}, 	0, "MMVersus", NULL, NULL, false},
	{&MAIN_MENU_HIGHSCORES_LE, 		{0,  12, 0, 0}, 	0, NULL, NULL, NULL, false},
	{&MAIN_MENU_OPTIONS_LE, 		{0,  24, 0, 0}, 	0, NULL, NULL, NULL, false},
	{&MAIN_MENU_CREDITS_LE, 		{0,  36, 0, 0}, 	0, NULL, NULL, NULL, false},

	{NULL, {0,0,0,0}, 0, NULL, NULL, NULL, false},
};


//---------------------------------------------------------------------------------------------------------
// 											ENTITY LISTS
//---------------------------------------------------------------------------------------------------------

PositionedEntityROMSpec TITLE_SCREEN_STAGE_ST_ENTITIES[] =
{
	{&RHOMBUS_BACKGROUND_AE,					{192, 112,  0,  0},   	0, NULL, NULL, NULL, false},
	{&LOGO_AE, 									{192,  88,  0,  0}, 	0, NULL, NULL, NULL, false},
	{&DEMO_BADGE_LE, 							{314, 104,  0,  0}, 	0, NULL, NULL, NULL, false},
	{(EntitySpec*)&MAIN_MENU_CONTAINER, 		{192, 156,  0,  0}, 	0, "MainMenu", (struct PositionedEntity*)MAIN_MENU_CHILDREN, NULL, false},
	{&PRESS_START_BUTTON_LE, 					{192, 168,  0,  0}, 	0, "PrssStrt", NULL, NULL, false},
	{&COPYRIGHT_EN, 							{192, 212,  0,  0}, 	0, NULL, NULL, NULL, false},

	{&TRANSITION_LAYER_B_AE,					{192, 112,  0,  0},  	0, "TRNSLYR", NULL, NULL, false},
	{&MAIN_MENU_WAITING_FOR_OTHER_PLAYER_LE, 	{192, 168, -1,  0}, 	0, "Waiting", NULL, NULL, false},

	{NULL, {0,0,0,0}, 0, NULL, NULL, NULL, false},
};

PositionedEntityROMSpec TITLE_SCREEN_STAGE_ST_UI_ENTITIES[] =
{
	{&LOW_POWER_INDICATOR_LB, 		{ 16,  12,  -1,  0},		0, NULL, NULL, NULL, false},

	{NULL, {0,0,0,0}, 0, NULL, NULL, NULL, false},
};


//---------------------------------------------------------------------------------------------------------
// 											PRELOAD LISTS
//---------------------------------------------------------------------------------------------------------

FontROMSpec* const TITLE_SCREEN_STAGE_ST_FONTS[] =
{
	&INDUSTRIAL_FONT,

	NULL
};

CharSetROMSpec* const TITLE_SCREEN_STAGE_ST_CHARSETS[] =
{
	NULL
};

TextureSpec* const TITLE_SCREEN_STAGE_ST_TEXTURES[] =
{
	NULL
};


//---------------------------------------------------------------------------------------------------------
//											STAGE DEFINITION
//---------------------------------------------------------------------------------------------------------

StageROMSpec TITLE_SCREEN_STAGE_ST =
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
		// number of cycles the texture writing is idle
		__TARGET_FPS / 10,

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
		(FontSpec**)TITLE_SCREEN_STAGE_ST_FONTS,

		// char sets to preload
		(CharSetSpec**)TITLE_SCREEN_STAGE_ST_CHARSETS,

		// textures to preload
		(TextureSpec**)TITLE_SCREEN_STAGE_ST_TEXTURES,

		// background music
		(Sound**)NULL,
	},

	// entities
	{
		// ui
		{
			TITLE_SCREEN_STAGE_ST_UI_ENTITIES,
			__TYPE(UIContainer),
		},

		// children
		TITLE_SCREEN_STAGE_ST_ENTITIES,
	},

	// post processing effects
	(PostProcessingEffect*)NULL,
};
