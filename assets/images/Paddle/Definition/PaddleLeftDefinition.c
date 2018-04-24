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

#include <Entity.h>
#include <Box.h>
#include <Ball.h>
#include <Paddle.h>
#include <macros.h>


//---------------------------------------------------------------------------------------------------------
//												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern BYTE PaddleLeftLTiles[];
extern BYTE PaddleLeftLMap[];
extern BYTE PaddleLeftRTiles[];
extern BYTE PaddleLeftRMap[];


//---------------------------------------------------------------------------------------------------------
//												DEFINITIONS
//---------------------------------------------------------------------------------------------------------

/* Animations */

AnimationFunctionROMDef PADDLE_RETRACTED_ANIM =
{
	// number of frames of this animation function
	1,

	// frames to play in animation
	{0},

	// number of cycles a frame of animation is displayed
	8,

	// whether to play it in loop or not
	false,

	// method to call on function completion
	NULL,

	// function's name
	"Retracted",
};

AnimationFunctionROMDef PADDLE_EJECTED_ANIM =
{
	// number of frames of this animation function
	1,

	// frames to play in animation
	{2},

	// number of cycles a frame of animation is displayed
	8,

	// whether to play it in loop or not
	false,

	// method to call on function completion
	NULL,

	// function's name
	"Ejected",
};

AnimationFunctionROMDef PADDLE_EJECT_ANIM =
{
	// number of frames of this animation function
	2,

	// frames to play in animation
	{1, 2},

	// number of cycles a frame of animation is displayed
	4,

	// whether to play it in loop or not
	false,

	// method to call on function completion
	NULL,

	// function's name
	"Eject",
};

AnimationFunctionROMDef PADDLE_RETRACT_ANIM =
{
	// number of frames of this animation function
	3,

	// frames to play in animation
	{2, 1, 0},

	// number of cycles a frame of animation is displayed
	8,

	// whether to play it in loop or not
	false,

	// method to call on function completion
	NULL,

	// function's name
	"Retract",
};

// an animation definition
AnimationDescriptionROMDef PADDLE_ANIM =
{
	// animation functions
	{
		(AnimationFunction*)&PADDLE_RETRACTED_ANIM,
		(AnimationFunction*)&PADDLE_EJECTED_ANIM,
		(AnimationFunction*)&PADDLE_EJECT_ANIM,
		(AnimationFunction*)&PADDLE_RETRACT_ANIM,
		NULL,
	}
};

/* Left Sprite */

CharSetROMDef PADDLE_LEFT_L_CH =
{
	// number of chars, depending on allocation type:
	// __ANIMATED_SINGLE*, __ANIMATED_SHARED*: number of chars of a single animation frame (cols * rows)
	// __ANIMATED_MULTI, __NOT_ANIMATED: sum of all chars
	16,

	// allocation type
	// (__ANIMATED_SINGLE, __ANIMATED_SINGLE_OPTIMIZED, __ANIMATED_SHARED, __ANIMATED_SHARED_COORDINATED, __ANIMATED_MULTI or __NOT_ANIMATED)
	__ANIMATED_SINGLE,

	// char definition
	PaddleLeftLTiles,
};

TextureROMDef PADDLE_LEFT_L_TX =
{
	// charset definition
	(CharSetDefinition*)&PADDLE_LEFT_L_CH,

	// bgmap definition
	PaddleLeftLMap,

	// cols (max 64)
	4,

	// rows (max 64)
	4,

	// padding for affine/hbias transformations (cols, rows)
	{0, 0},

	// number of frames, depending on charset's allocation type:
	// __ANIMATED_SINGLE*, __ANIMATED_SHARED*, __NOT_ANIMATED: 1
	// __ANIMATED_MULTI: total number of frames
	1,

	// palette number (0-3)
	0,

	// recyclable
	false,
};

BgmapSpriteROMDef PADDLE_LEFT_L_SPRITE =
{
	{
		// sprite's type
		__TYPE(BgmapAnimatedSprite),

		// texture definition
		(TextureDefinition*)&PADDLE_LEFT_L_TX,

		// transparent (__TRANSPARENCY_NONE, __TRANSPARENCY_EVEN or __TRANSPARENCY_ODD)
		__TRANSPARENCY_NONE,

		// displacement
		{0, 0, 0, 0},
	},

	// bgmap mode (__WORLD_BGMAP, __WORLD_AFFINE, __WORLD_OBJECT or __WORLD_HBIAS)
	// make sure to use the proper corresponding sprite type throughout the definition (BgmapSprite or ObjectSprite)
	__WORLD_BGMAP,

	// pointer to affine/hbias manipulation function
	NULL,

	// display mode (__WORLD_ON, __WORLD_LON or __WORLD_RON)
	__WORLD_LON,
};

/* Right Sprite */

CharSetROMDef PADDLE_LEFT_R_CH =
{
	// number of chars, depending on allocation type:
	// __ANIMATED_SINGLE*, __ANIMATED_SHARED*: number of chars of a single animation frame (cols * rows)
	// __ANIMATED_MULTI, __NOT_ANIMATED: sum of all chars
	16,

	// allocation type
	// (__ANIMATED_SINGLE, __ANIMATED_SINGLE_OPTIMIZED, __ANIMATED_SHARED, __ANIMATED_SHARED_COORDINATED, __ANIMATED_MULTI or __NOT_ANIMATED)
	__ANIMATED_SINGLE,

	// char definition
	PaddleLeftRTiles,
};

TextureROMDef PADDLE_LEFT_R_TX =
{
	// charset definition
	(CharSetDefinition*)&PADDLE_LEFT_R_CH,

	// bgmap definition
	PaddleLeftRMap,

	// cols (max 64)
	4,

	// rows (max 64)
	4,

	// padding for affine/hbias transformations (cols, rows)
	{0, 0},

	// number of frames, depending on charset's allocation type:
	// __ANIMATED_SINGLE*, __ANIMATED_SHARED*, __NOT_ANIMATED: 1
	// __ANIMATED_MULTI: total number of frames
	1,

	// palette number (0-3)
	0,

	// recyclable
	false,
};

BgmapSpriteROMDef PADDLE_LEFT_R_SPRITE =
{
	{
		// sprite's type
		__TYPE(BgmapAnimatedSprite),

		// texture definition
		(TextureDefinition*)&PADDLE_LEFT_R_TX,

		// transparent (__TRANSPARENCY_NONE, __TRANSPARENCY_EVEN or __TRANSPARENCY_ODD)
		__TRANSPARENCY_NONE,

		// displacement
		{0, 0, 0, 0},
	},

	// bgmap mode (__WORLD_BGMAP, __WORLD_AFFINE, __WORLD_OBJECT or __WORLD_HBIAS)
	// make sure to use the proper corresponding sprite type throughout the definition (BgmapSprite or ObjectSprite)
	__WORLD_BGMAP,

	// pointer to affine/hbias manipulation function
	NULL,

	// display mode (__WORLD_ON, __WORLD_LON or __WORLD_RON)
	__WORLD_RON,
};

/* Entity */

BgmapSpriteROMDef* const PADDLE_LEFT_SPRITES[] =
{
	&PADDLE_LEFT_L_SPRITE,
	&PADDLE_LEFT_R_SPRITE,
	NULL
};


ShapeROMDef PADDLE_LEFT_AC_SHAPES[] =
{
	// wall collider
	{
		// shape
		__TYPE(Ball),

		// size (x, y, z)
		{12, 12, 12},

		// displacement (x, y, z, p)
		{0, 0, -6, 0},

		// rotation (x, y, z)
		{0, 0, 0},

		// scale (x, y, z)
		{__I_TO_FIX7_9(1), __I_TO_FIX7_9(1), __I_TO_FIX7_9(1)},

		// if true this shape checks for collisions against other shapes
		true,

		// layers in which I live
		kPlayFieldLayer | kPlayFieldPaddleHelperLayer,

		// layers to ignore when checking for collisions
		kAllLayers & ~(kPlayFieldWallsLayer | kPlayFieldSplitterLayer | kPlayFieldFloorLayer | kPlayFieldFloorLayer),
	},

	// ball collider
	{
		// shape
		__TYPE(Box),

		// size (x, y, z)
		{48, 48, 8},

		// displacement (x, y, z, p)
		{0, 0, 0, 0},

		// rotation (x, y, z)
		{0, 28, 0},

		// scale (x, y, z)
		{__I_TO_FIX7_9(1), __I_TO_FIX7_9(1), __I_TO_FIX7_9(1)},

		// if true this shape checks for collisions against other shapes
		false,

		// layers in which I live
		kPlayFieldLayer | kPlayFieldPaddleLayer,

		// layers to ignore when checking for collisions
		kAllLayers,
	},

	{NULL, {0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0}, {0, 0, 0}, false, kNoLayer, kNoLayer}
};

PhysicalSpecificationROMDef PADDLE_AC_PHYSICAL_PROPERTIES =
{
	// mass
	__F_TO_FIX10_6(0.25f),

	// friction
	__F_TO_FIX10_6(0.3f),

	// bounciness
	__F_TO_FIX10_6(0.3f),

	// maximum velocity
	{0, 0, 0}
};

PaddleROMDef PADDLE_LEFT_AC =
{
	{
		{
			{
				// class allocator
				__TYPE(Paddle),

				// sprites
				(SpriteROMDef**)PADDLE_LEFT_SPRITES,

				// collision shapes
				(ShapeDefinition*)PADDLE_LEFT_AC_SHAPES,

				// size
				// if 0, width and height will be inferred from the first sprite's texture's size
				{0, 0, 0},

				// gameworld's character's type
				kPaddleType,

				// physical specification
				(PhysicalSpecification*)&PADDLE_AC_PHYSICAL_PROPERTIES,
			},

			// pointer to the animation definition for the item
			(AnimationDescription*)&PADDLE_ANIM,

			// initial animation
			"Ejected",
		},

		// true to create a body
		true,

		// axes subject to gravity
		__Z_AXIS
	},

	// minimum velocity
	{0, 0, 0},
	// maximum velocity
	{0, 0, 0},
};