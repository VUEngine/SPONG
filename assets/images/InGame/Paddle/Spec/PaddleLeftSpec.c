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

AnimationFunctionROMSpec PaddleRetractedAnimation =
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

AnimationFunctionROMSpec PaddleEjectedAnimation =
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

AnimationFunctionROMSpec PaddleEjectAnimation =
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

AnimationFunctionROMSpec PaddleRetractAnimation =
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

// an animation spec
AnimationDescriptionROMSpec PaddleAnimation =
{
	// animation functions
	{
		(AnimationFunction*)&PaddleRetractedAnimation,
		(AnimationFunction*)&PaddleEjectedAnimation,
		(AnimationFunction*)&PaddleEjectAnimation,
		(AnimationFunction*)&PaddleRetractAnimation,
		NULL,
	}
};

/* Left Sprite */

CharSetROMSpec PaddleLeftLCharset =
{
	// number of chars, depending on allocation type:
	// __ANIMATED_SINGLE*, __ANIMATED_SHARED*: number of chars of a single animation frame (cols * rows)
	// __ANIMATED_MULTI, __NOT_ANIMATED: sum of all chars
	24,

	// allocation type
	// (__ANIMATED_SINGLE, __ANIMATED_SINGLE_OPTIMIZED, __ANIMATED_SHARED, __ANIMATED_SHARED_COORDINATED, __ANIMATED_MULTI or __NOT_ANIMATED)
	__ANIMATED_SINGLE,

	// char spec
	PaddleLeftLTiles,
};

TextureROMSpec PaddleLeftLTexture =
{
	// charset spec
	(CharSetSpec*)&PaddleLeftLCharset,

	// bgmap spec
	PaddleLeftLMap,

	// cols (max 64)
	4,

	// rows (max 64)
	6,

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

	// vertical flip
	false,

	// horizontal flip
	false,
};

BgmapSpriteROMSpec PaddleLeftLSprite =
{
	{
		// sprite's type
		__TYPE(BgmapAnimatedSprite),

		// texture spec
		(TextureSpec*)&PaddleLeftLTexture,

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
	__WORLD_LON,
};

/* Right Sprite */

CharSetROMSpec PaddleLeftRCharset =
{
	// number of chars, depending on allocation type:
	// __ANIMATED_SINGLE*, __ANIMATED_SHARED*: number of chars of a single animation frame (cols * rows)
	// __ANIMATED_MULTI, __NOT_ANIMATED: sum of all chars
	24,

	// allocation type
	// (__ANIMATED_SINGLE, __ANIMATED_SINGLE_OPTIMIZED, __ANIMATED_SHARED, __ANIMATED_SHARED_COORDINATED, __ANIMATED_MULTI or __NOT_ANIMATED)
	__ANIMATED_SINGLE,

	// char spec
	PaddleLeftRTiles,
};

TextureROMSpec PaddleLeftRTexture =
{
	// charset spec
	(CharSetSpec*)&PaddleLeftRCharset,

	// bgmap spec
	PaddleLeftRMap,

	// cols (max 64)
	4,

	// rows (max 64)
	6,

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

	// vertical flip
	false,

	// horizontal flip
	false,
};

BgmapSpriteROMSpec PaddleLeftRSprite =
{
	{
		// sprite's type
		__TYPE(BgmapAnimatedSprite),

		// texture spec
		(TextureSpec*)&PaddleLeftRTexture,

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
	__WORLD_RON,
};

/* Entity */

BgmapSpriteROMSpec* const PaddleLeftSprites[] =
{
	&PaddleLeftLSprite,
	&PaddleLeftRSprite,
	NULL
};


ShapeROMSpec PaddleLeftShapes[] =
{
	// wall collider
	{
		// shape
		__TYPE(Ball),

		// size (x, y, z)
		{12, 28, 12},

		// displacement (x, y, z, p)
		{0, 0, -14, 0},

		// rotation (x, y, z)
		{0, 0, 0},

		// scale (x, y, z)
		{__I_TO_FIX7_9(1), __I_TO_FIX7_9(1), __I_TO_FIX7_9(1)},

		// if true this shape checks for collisions against other shapes
		true,

		// layers in which I live
		kLayerPlayField | kLayerPlayFieldPaddleHelper,

		// layers to ignore when checking for collisions
		kLayerAll & ~(kLayerPlayFieldWalls | kLayerPlayFieldSplitter | kLayerPlayFieldFloor | kLayerPlayFieldFloor),
	},

	// ball collider
	{
		// shape
		__TYPE(Box),

		// size (x, y, z)
		{32, 48, 16},

		// displacement (x, y, z, p)
		{8, 0, 0, 0},

		// rotation (x, y, z)
		{0, 28, 0},

		// scale (x, y, z)
		{__I_TO_FIX7_9(1), __I_TO_FIX7_9(1), __I_TO_FIX7_9(1)},

		// if true this shape checks for collisions against other shapes
		false,

		// layers in which I live
		kLayerPlayField | kLayerPlayFieldPaddle,

		// layers to ignore when checking for collisions
		kLayerAll,
	},

	{NULL, {0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0}, {0, 0, 0}, false, kLayerNone, kLayerNone}
};

PhysicalSpecificationROMSpec PaddlePhysicalProperties =
{
	// mass
	__F_TO_FIX10_6(0.55f),

	// friction
	__F_TO_FIX10_6(0.25f),

	// bounciness
	__F_TO_FIX10_6(0.0f),

	// maximum velocity
	{0, 0, 0},

	// maximum speed
	__I_TO_FIX10_6(8)
};

PaddleROMSpec PaddleLeftEntity =
{
	{
		{
			{
				// class allocator
				__TYPE(Paddle),

				// children
				NULL,

				// behaviors
				NULL,

				// extra
				NULL,

				// sprites
				(SpriteSpec**)PaddleLeftSprites,

				// use z displacement in projection
				false,

				// collision shapes
				(ShapeSpec*)PaddleLeftShapes,

				// size
				// if 0, width and height will be inferred from the first sprite's texture's size
				{0, 0, 0},

				// gameworld's character's type
				kTypePaddle,

				// physical specification
				(PhysicalSpecification*)&PaddlePhysicalProperties,
			},

			// pointer to the animation spec for the item
			(AnimationDescription*)&PaddleAnimation,

			// initial animation
			"Ejected",
		},

		// true to create a body
		true,

		// axes subject to gravity
		__Z_AXIS,

		// axis around which to rotate the entity when syncronizing with body
		__NO_AXIS
	},

	// force
	{
		__I_TO_FIX10_6(80),
		__I_TO_FIX10_6(80),
		0
	}
};
