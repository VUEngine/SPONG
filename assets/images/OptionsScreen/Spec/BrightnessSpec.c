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

#include <BgmapAnimatedSprite.h>
#include <macros.h>
#include <LocalizedEntity.h>
#include <Actor.h>


//---------------------------------------------------------------------------------------------------------
//												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern BYTE BrightnessTiles[];
extern BYTE BrightnessMap[];


//---------------------------------------------------------------------------------------------------------
//												DEFINITIONS
//---------------------------------------------------------------------------------------------------------

AnimationFunctionROMSpec OPTIONS_BRIGHTNESS_EN_ANIM =
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
	"0",
};

AnimationFunctionROMSpec OPTIONS_BRIGHTNESS_DE_ANIM =
{
	// number of frames of this animation function
	1,

	// frames to play in animation
	{1},

	// number of cycles a frame of animation is displayed
	8,

	// whether to play it in loop or not
	false,

	// method to call on function completion
	NULL,

	// function's name
	"1",
};

AnimationFunctionROMSpec OPTIONS_BRIGHTNESS_ES_ANIM =
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
	"2",
};

AnimationFunctionROMSpec OPTIONS_BRIGHTNESS_FR_ANIM =
{
	// number of frames of this animation function
	1,

	// frames to play in animation
	{3},

	// number of cycles a frame of animation is displayed
	8,

	// whether to play it in loop or not
	false,

	// method to call on function completion
	NULL,

	// function's name
	"3",
};

// an animation spec
AnimationDescriptionROMSpec OPTIONS_BRIGHTNESS_ANIM =
{
	// animation functions
	{
		(AnimationFunction*)&OPTIONS_BRIGHTNESS_EN_ANIM,
		(AnimationFunction*)&OPTIONS_BRIGHTNESS_DE_ANIM,
		(AnimationFunction*)&OPTIONS_BRIGHTNESS_ES_ANIM,
		(AnimationFunction*)&OPTIONS_BRIGHTNESS_FR_ANIM,
		NULL,
	}
};

CharSetROMSpec OPTIONS_BRIGHTNESS_CH =
{
	// number of chars, depending on allocation type:
	// __ANIMATED_SINGLE*, __ANIMATED_SHARED*: number of chars of a single animation frame (cols * rows)
	// __ANIMATED_MULTI, __NOT_ANIMATED: sum of all chars
	20,

	// allocation type
	// (__ANIMATED_SINGLE, __ANIMATED_SINGLE_OPTIMIZED, __ANIMATED_SHARED, __ANIMATED_SHARED_COORDINATED, __ANIMATED_MULTI or __NOT_ANIMATED)
	__ANIMATED_SINGLE,

	// char spec
	BrightnessTiles,
};

TextureROMSpec OPTIONS_BRIGHTNESS_TX =
{
	// charset spec
	(CharSetSpec*)&OPTIONS_BRIGHTNESS_CH,

	// bgmap spec
	BrightnessMap,

	// cols (max 64)
	10,

	// rows (max 64)
	2,

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

BgmapSpriteROMSpec OPTIONS_BRIGHTNESS_SPRITE =
{
	{
		// sprite's type
		__TYPE(BgmapAnimatedSprite),

		// texture spec
		(TextureSpec*)&OPTIONS_BRIGHTNESS_TX,

		// transparent (__TRANSPARENCY_NONE, __TRANSPARENCY_EVEN or __TRANSPARENCY_ODD)
		__TRANSPARENCY_NONE,

		// displacement
		{0, 0, 0, 0},
	},

	// bgmap mode (__WORLD_BGMAP, __WORLD_AFFINE, __WORLD_OBJECT or __WORLD_HBIAS)
	// make sure to use the proper corresponding sprite type throughout the spec (BgmapSprite or ObjectSprite)
	__WORLD_BGMAP,

	// pointer to affine/hbias manipulation function
	NULL,

	// display mode (__WORLD_ON, __WORLD_LON or __WORLD_RON)
	__WORLD_ON,
};

BgmapSpriteROMSpec* const OPTIONS_BRIGHTNESS_SPRITES[] =
{
	&OPTIONS_BRIGHTNESS_SPRITE,
	NULL
};

LocalizedEntityROMSpec OPTIONS_BRIGHTNESS_LE =
{
	{
		// class allocator
		__TYPE(LocalizedEntity),

		// sprites
		(SpriteROMSpec**)OPTIONS_BRIGHTNESS_SPRITES,

		// collision shapes
		(ShapeSpec*)NULL,

		// size
		// if 0, width and height will be inferred from the first sprite's texture's size
		{0, 0, 0},

		// gameworld's character's type
		kNoType,

		// physical specification
		(PhysicalSpecification*)NULL,
	},

	// pointer to the animation spec for the character
	(AnimationDescription*)&OPTIONS_BRIGHTNESS_ANIM,

	// initial animation
	"0"
};