/* VUEngine - Virtual Utopia Engine <http://vuengine.planetvb.com/>
 * A universal game engine for the Nintendo Virtual Boy
 *
 * Copyright (C) 2007, 2018 by Jorge Eremiev<jorgech3@gmail.com> and Christian Radke <chris@vr32.de>
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

#include <libgccvb.h>
#include <AnimatedEntity.h>
#include <BgmapAnimatedSprite.h>


//---------------------------------------------------------------------------------------------------------
//												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern BYTE HexagonBackgroundLeftMap[];
extern BYTE HexagonBackgroundLeftTiles[];
extern BYTE HexagonBackgroundRightTiles[];
extern BYTE HexagonBackgroundRightMap[];


//---------------------------------------------------------------------------------------------------------
//												DEFINITIONS
//---------------------------------------------------------------------------------------------------------

/* Animation */

AnimationFunctionROMDef HEXAGON_BACKGROUND_DEFAULT_ANIM =
{
	// number of frames of this animation function
	8,

	// frames to play in animation
	{
		0, 1, 2, 3, 4, 5, 6, 7,
	},

	// number of cycles a frame of animation is displayed
	2,

	// whether to play it in loop or not
	true,

	// method to call on function completion
	NULL,

	// function's name
	"Default",
};

AnimationDescriptionROMDef HEXAGON_BACKGROUND_ANIM =
{
	// animation functions
	{
		(AnimationFunction*)&HEXAGON_BACKGROUND_DEFAULT_ANIM,
		NULL,
	}
};


/* Left */

CharSetROMDef HEXAGON_BACKGROUND_L_CH =
{
	// number of chars, depending on allocation type:
	// __ANIMATED_SINGLE*, __ANIMATED_SHARED*: number of chars of a single animation frame (cols * rows)
	// __ANIMATED_MULTI, __NOT_ANIMATED: sum of all chars
	526,

	// allocation type
	// (__ANIMATED_SINGLE, __ANIMATED_SINGLE_OPTIMIZED, __ANIMATED_SHARED, __ANIMATED_SHARED_COORDINATED, __ANIMATED_MULTI or __NOT_ANIMATED)
	__ANIMATED_SINGLE_OPTIMIZED,

	// char definition
	HexagonBackgroundLeftTiles,
};

TextureROMDef HEXAGON_BACKGROUND_L_TX =
{
	// charset definition
	(CharSetDefinition*)&HEXAGON_BACKGROUND_L_CH,

	// bgmap definition
	HexagonBackgroundLeftMap,

	// cols (max 64)
	48,

	// rows (max 64)
	28,

	// padding for affine transformations
	{0, 0},

	// number of frames, depending on charset's allocation type:
	// __ANIMATED_SINGLE*, __ANIMATED_SHARED*, __NOT_ANIMATED: 1
	// __ANIMATED_MULTI: total number of frames
	__ANIMATED_SINGLE_OPTIMIZED,

	// palette number (0-3)
	0,

	// recyclable
	false,
};

BgmapSpriteROMDef HEXAGON_BACKGROUND_L_SPRITE =
{
	{
		// sprite's type
		__TYPE(BgmapAnimatedSprite),

		// texture definition
		(TextureDefinition*)&HEXAGON_BACKGROUND_L_TX,

		// transparent
		false,

		// displacement
		{0, 0, 0, 0},
	},

	// bgmap mode (__WORLD_BGMAP, __WORLD_AFFINE, __WORLD_OBJECT or __WORLD_HBIAS)
	// make sure to use the proper corresponding sprite type throughout the definition (BgmapSprite or ObjectSprite)
	__WORLD_BGMAP,

	// pointer to affine / hbias manipulation function
	NULL,

	// display mode (__WORLD_ON, __WORLD_LON or __WORLD_RON)
	__WORLD_LON,
};


/* Right */

CharSetROMDef HEXAGON_BACKGROUND_R_CH =
{
	// number of chars, depending on allocation type:
	// __ANIMATED_SINGLE*, __ANIMATED_SHARED*: number of chars of a single animation frame (cols * rows)
	// __ANIMATED_MULTI, __NOT_ANIMATED: sum of all chars
	529,

	// allocation type
	// (__ANIMATED_SINGLE, __ANIMATED_SINGLE_OPTIMIZED, __ANIMATED_SHARED, __ANIMATED_SHARED_COORDINATED, __ANIMATED_MULTI or __NOT_ANIMATED)
	__ANIMATED_SINGLE_OPTIMIZED,

	// char definition
	HexagonBackgroundRightTiles,
};

TextureROMDef HEXAGON_BACKGROUND_R_TX =
{
	// charset definition
	(CharSetDefinition*)&HEXAGON_BACKGROUND_R_CH,

	// bgmap definition
	HexagonBackgroundRightMap,

	// cols (max 64)
	48,

	// rows (max 64)
	28,

	// padding for affine transformations
	{0, 0},

	// number of frames, depending on charset's allocation type:
	// __ANIMATED_SINGLE*, __ANIMATED_SHARED*, __NOT_ANIMATED: 1
	// __ANIMATED_MULTI: total number of frames
	__ANIMATED_SINGLE_OPTIMIZED,

	// palette number (0-3)
	0,

	// recyclable
	false,
};

BgmapSpriteROMDef HEXAGON_BACKGROUND_R_SPRITE =
{
	{
		// sprite's type
		__TYPE(BgmapAnimatedSprite),

		// texture definition
		(TextureDefinition*)&HEXAGON_BACKGROUND_R_TX,

		// transparent
		false,

		// displacement
		{0, 0, 0, 0},
	},

	// bgmap mode (__WORLD_BGMAP, __WORLD_AFFINE, __WORLD_OBJECT or __WORLD_HBIAS)
	// make sure to use the proper corresponding sprite type throughout the definition (BgmapSprite or ObjectSprite)
	__WORLD_BGMAP,

	// pointer to affine / hbias manipulation function
	NULL,

	// display mode (__WORLD_ON, __WORLD_LON or __WORLD_RON)
	__WORLD_RON,
};


/* Entity */

BgmapSpriteROMDef* const HEXAGON_BACKGROUND_SPRITES[] =
{
	&HEXAGON_BACKGROUND_L_SPRITE,
	&HEXAGON_BACKGROUND_R_SPRITE,
	NULL
};

AnimatedEntityROMDef HEXAGON_BACKGROUND_AE =
{
	{
		// class allocator
		__TYPE(AnimatedEntity),

		// sprites
		(SpriteROMDef**)HEXAGON_BACKGROUND_SPRITES,

		// collision shapes
		(ShapeDefinition*)NULL,

		// size
		// if 0, width and height will be inferred from the first sprite's texture's size
		{0, 0, 0},

		// gameworld's character's type
		0,

		// physical specification
		(PhysicalSpecification*)NULL,
	},

	// pointer to the animation definition for the item
	(AnimationDescription*)&HEXAGON_BACKGROUND_ANIM,

	// initial animation
	"Default",
};
