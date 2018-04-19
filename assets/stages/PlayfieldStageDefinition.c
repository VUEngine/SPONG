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

extern EntityDefinition PONG_BALL_AC;
extern EntityDefinition PADDLE_IM;
extern EntityDefinition PLAYFIELD_IM;
extern EntityDefinition TRANSITION_LAYER_B_AG;
extern EntityDefinition COLLISION_CL;


extern CharSetDefinition PADDLE_CH;
extern CharSetDefinition PLAYFIELD_CH;

extern TextureDefinition PADDLE_TX;
extern TextureDefinition PLAYFIELD_TX;

extern u16 GAME_BGM_1[][2];

extern const PixelSize collision_48_28_2;
extern const PixelSize collision_2_28_48;
extern const PixelSize collision_48_2_48;

//---------------------------------------------------------------------------------------------------------
// 											ENTITY LISTS
//---------------------------------------------------------------------------------------------------------

PositionedEntityROMDef PLAYFIELD_STAGE_ST_ENTITIES[] =
{
//	{&PLAYFIELD_IM, 		{192, 112, 0, 0}, 	0, NULL, NULL, NULL, false},
//	{&PADDLE_IM, 			{ 52, 112, 0, 0}, 	0, "PLAYER1", NULL, NULL, false},
	{&PONG_BALL_AC, 		{192, 112, 32, 0}, 	0, "PongBall", NULL, NULL, true},
//	{&PADDLE_IM, 			{332, 112, 0, 0}, 	0, "PLAYER2", NULL, NULL, false},
//	{&TRANSITION_LAYER_B_AG,	{192, 112, 0, -1}, 	0, "TRNSLYR", NULL, NULL, false},

	{&COLLISION_CL,			{192, 112,   0, 0},	0, NULL, NULL, (void*)&collision_48_28_2, false}, // front border
	{&COLLISION_CL,			{192, 112, 256, 0},	0, NULL, NULL, (void*)&collision_48_28_2, false}, // far border
	{&COLLISION_CL,			{  0, 112, 128, 0},	0, NULL, NULL, (void*)&collision_2_28_48, false}, // left border
	{&COLLISION_CL,			{384, 112, 128, 0},	0, NULL, NULL, (void*)&collision_2_28_48, false}, // right border
	{&COLLISION_CL,			{192,   0, 128, 0},	0, NULL, NULL, (void*)&collision_48_2_48, false}, // top border
	{&COLLISION_CL,			{192, 224,   0, 0},	0, NULL, NULL, (void*)&collision_48_2_48, false}, // bottom border

	{NULL, {0,0,0,0}, 0, NULL, NULL, NULL, false},
};

PositionedEntityROMDef PLAYFIELD_STAGE_ST_UI_ENTITIES[] =
{
	{NULL, {0,0,0,0}, 0, NULL, NULL, NULL, false},
};


//---------------------------------------------------------------------------------------------------------
// 											PRELOAD LISTS
//---------------------------------------------------------------------------------------------------------

FontROMDef* const PLAYFIELD_STAGE_ST_FONTS[] =
{
	&INDUSTRIAL_FONT,

	NULL
};

CharSetROMDef* const PLAYFIELD_STAGE_ST_CHARSETS[] =
{
	&PADDLE_CH,
	&PLAYFIELD_CH,

	NULL
};

TextureDefinition* const PLAYFIELD_STAGE_ST_TEXTURES[] =
{
	&PADDLE_TX,
	&PLAYFIELD_TX,

	NULL
};


//---------------------------------------------------------------------------------------------------------
//											STAGE DEFINITION
//---------------------------------------------------------------------------------------------------------

StageROMDef PLAYFIELD_STAGE_ST =
{
	// allocator
	__TYPE(Stage),

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
		false
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
				8,
				// medium red
				__BRIGHTNESS_MEDIUM_RED,
				// bright red
				__BRIGHTNESS_BRIGHT_RED,
			},

			// brightness repeat
			(BrightnessRepeatDefinition*)NULL,
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
			__MAXIMUM_X_VIEW_DISTANCE, __MAXIMUM_X_VIEW_DISTANCE,
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
			__F_TO_FIX10_6(0),
			__F_TO_FIX10_6(0),
			__F_TO_FIX10_6(__GRAVITY)
		},

		// friction
		__F_TO_FIX10_6(0.1f),
	},

	// assets
	{
		// fonts to preload
		(FontDefinition**)PLAYFIELD_STAGE_ST_FONTS,

		// char sets to preload
		(CharSetDefinition**)PLAYFIELD_STAGE_ST_CHARSETS,

		// textures to preload
		(TextureDefinition**)PLAYFIELD_STAGE_ST_TEXTURES,

		// background music
		(const u16 (*)[])GAME_BGM_1,
	},

	// entities
	{
		// ui
		{
			PLAYFIELD_STAGE_ST_UI_ENTITIES,
			__TYPE(UiContainer),
		},

		// children
		PLAYFIELD_STAGE_ST_ENTITIES,
	},

	// post processing effects
	(PostProcessingEffect*)NULL,
};
