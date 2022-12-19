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

extern uint32 PaddleRightLTiles[];
extern uint32 PaddleRightRTiles[];
extern uint32 PaddleRightLTilesFrameOffsets[];
extern uint32 PaddleRightRTilesFrameOffsets[];
extern uint16 PaddleRightLMap[];
extern uint16 PaddleRightRMap[];


//---------------------------------------------------------------------------------------------------------
//												DEFINITIONS
//---------------------------------------------------------------------------------------------------------

/* Animations */

extern AnimationFunctionROMSpec* const PaddleAnimation[];


/* Left Sprite */

CharSetROMSpec PaddleRightLCharset =
{
	// number of chars, depending on allocation type:
	// __ANIMATED_SINGLE*, __ANIMATED_SHARED*: number of chars of a single animation frame (cols * rows)
	// __ANIMATED_MULTI, __NOT_ANIMATED: sum of all chars
	24,

	// allocation type
	// (__ANIMATED_SINGLE, __ANIMATED_SINGLE_OPTIMIZED, __ANIMATED_SHARED, __ANIMATED_SHARED_COORDINATED, __ANIMATED_MULTI or __NOT_ANIMATED)
	__ANIMATED_SINGLE,

	// char spec
	PaddleRightLTiles,

	// pointer to the frames offsets
	PaddleRightLTilesFrameOffsets,
};

TextureROMSpec PaddleRightLTexture =
{
	// charset spec
	(CharSetSpec*)&PaddleRightLCharset,

	// bgmap spec
	PaddleRightLMap,

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

BgmapSpriteROMSpec PaddleRightLSprite =
{
	{
		// sprite's type
		__TYPE(BgmapAnimatedSprite),

		// texture spec
		(TextureSpec*)&PaddleRightLTexture,

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

CharSetROMSpec PaddleRightRCharset =
{
	// number of chars, depending on allocation type:
	// __ANIMATED_SINGLE*, __ANIMATED_SHARED*: number of chars of a single animation frame (cols * rows)
	// __ANIMATED_MULTI, __NOT_ANIMATED: sum of all chars
	24,

	// allocation type
	// (__ANIMATED_SINGLE, __ANIMATED_SINGLE_OPTIMIZED, __ANIMATED_SHARED, __ANIMATED_SHARED_COORDINATED, __ANIMATED_MULTI or __NOT_ANIMATED)
	__ANIMATED_SINGLE,

	// char spec
	PaddleRightRTiles,

	// pointer to the frames offsets
	PaddleRightRTilesFrameOffsets,
};

TextureROMSpec PaddleRightRTexture =
{
	// charset spec
	(CharSetSpec*)&PaddleRightRCharset,

	// bgmap spec
	PaddleRightRMap,

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

BgmapSpriteROMSpec PaddleRightRSprite =
{
	{
		// sprite's type
		__TYPE(BgmapAnimatedSprite),

		// texture spec
		(TextureSpec*)&PaddleRightRTexture,

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

BgmapSpriteROMSpec* const PaddleRightSprites[] =
{
	&PaddleRightLSprite,
	&PaddleRightRSprite,
	NULL
};

ShapeROMSpec PaddleRightShapes[] =
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
		{-8, 0, 0, 0},

		// rotation (x, y, z)
		{0, -28, 0},

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

extern PhysicalSpecification PaddlePhysicalProperties;

PaddleROMSpec PaddleRightEntity =
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
				(SpriteSpec**)PaddleRightSprites,

				// use z displacement in projection
				false,
			
				// wireframes
				(WireframeSpec**)NULL,

				// collision shapes
				(ShapeSpec*)PaddleRightShapes,

				// size
				// if 0, width and height will be inferred from the first sprite's texture's size
				{0, 0, 0},

				// gameworld's character's type
				kTypePaddle,

				// physical specification
				(PhysicalSpecification*)&PaddlePhysicalProperties,
			},

			// pointer to the animation spec for the item
			(const AnimationFunction**)&PaddleAnimation,

			// initial animation
			"Ejected",
		},

		// true to create a body
		true,

		// axes subject to gravity
		__Z_AXIS,

		// axis around which to rotate the entity when syncronizing with body
		__Z_AXIS
	},

	// force
	{
		__I_TO_FIX10_6(80),
		__I_TO_FIX10_6(80),
		0
	}
};
