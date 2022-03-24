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
#include <Ball.h>
#include <PongBall.h>
#include <macros.h>


//---------------------------------------------------------------------------------------------------------
//												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern uint32 PongBallTiles[];
extern uint16 PongBallLMap[];
extern uint16 PongBallRMap[];


//---------------------------------------------------------------------------------------------------------
//												DEFINITIONS
//---------------------------------------------------------------------------------------------------------

CharSetROMSpec PongBallCharset =
{
	// number of chars, depending on allocation type:
	// __ANIMATED_SINGLE*, __ANIMATED_SHARED*: number of chars of a single animation frame (cols * rows)
	// __ANIMATED_MULTI, __NOT_ANIMATED: sum of all chars
	13,

	// allocation type
	// (__ANIMATED_SINGLE, __ANIMATED_SINGLE_OPTIMIZED, __ANIMATED_SHARED, __ANIMATED_SHARED_COORDINATED, __ANIMATED_MULTI or __NOT_ANIMATED)
	__NOT_ANIMATED,

	// char spec
	PongBallTiles,

	// pointer to the frames offsets
	NULL,
};

/* Left */

TextureROMSpec PongBallLTexture =
{
	// charset spec
	(CharSetSpec*)&PongBallCharset,

	// bgmap spec
	PongBallLMap,

	// cols (max 64)
	3,

	// rows (max 64)
	3,

	// padding for affine/hbias transformations (cols, rows)
	{12, 12},

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

BgmapSpriteROMSpec PongBallLSprite =
{
	{
		// sprite's type
		__TYPE(BgmapSprite),

		// texture spec
		(TextureSpec*)&PongBallLTexture,

		// transparent (__TRANSPARENCY_NONE, __TRANSPARENCY_EVEN or __TRANSPARENCY_ODD)
		__TRANSPARENCY_NONE,

		// displacement
		{0, 0, 0, 0},
	},

	// bgmap mode (__WORLD_BGMAP, __WORLD_AFFINE, __WORLD_OBJECT or __WORLD_HBIAS)
	// make sure to use the proper corresponding sprite type throughout the spec (BgmapSprite or ObjectSprite)
	__WORLD_AFFINE,

	// pointer to affine/hbias manipulation function
	NULL,

	// display mode (__WORLD_ON, __WORLD_LON or __WORLD_RON)
	__WORLD_LON,
};

/* Right */

TextureROMSpec PongBallRTexture =
{
	// charset spec
	(CharSetSpec*)&PongBallCharset,

	// bgmap spec
	PongBallRMap,

	// cols (max 64)
	3,

	// rows (max 64)
	3,

	// padding for affine/hbias transformations (cols, rows)
	{12, 12},

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

BgmapSpriteROMSpec PongBallRSprite =
{
	{
		// sprite's type
		__TYPE(BgmapSprite),

		// texture spec
		(TextureSpec*)&PongBallRTexture,

		// transparent (__TRANSPARENCY_NONE, __TRANSPARENCY_EVEN or __TRANSPARENCY_ODD)
		__TRANSPARENCY_NONE,

		// displacement
		{0, 0, 0, 0},
	},

	// bgmap mode (__WORLD_BGMAP, __WORLD_AFFINE, __WORLD_OBJECT or __WORLD_HBIAS)
	// make sure to use the proper corresponding sprite type throughout the spec (BgmapSprite or ObjectSprite)
	__WORLD_AFFINE,

	// pointer to affine/hbias manipulation function
	NULL,

	// display mode (__WORLD_ON, __WORLD_LON or __WORLD_RON)
	__WORLD_RON,
};

/* Entity */

BgmapSpriteROMSpec* const PongBallSprites[] =
{
	&PongBallLSprite,
	&PongBallRSprite,
	NULL
};

ShapeROMSpec PongBallShapes[] =
{
	// ball
	{
		// shape
		__TYPE(Ball),

		// size (x, y, z)
		{24, 24, 24},

		// displacement (x, y, z, p)
		{0, 0, 0, 0},

		// rotation (x, y, z)
		{0, 0, 0},

		// scale (x, y, z)
		{__I_TO_FIX7_9(1), __I_TO_FIX7_9(1), __I_TO_FIX7_9(1)},

		// if true this shape checks for collisions against other shapes
		true,

		// layers in which I live
		kLayerPlayField | kLayerPlayFieldBall,

		// layers to ignore when checking for collisions
		kLayerPlayFieldBall | kLayerPlayFieldSplitter | kLayerPlayFieldPaddleHelper | kLayerParticles,
	},

	{NULL, {0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0}, {0, 0, 0}, false, kLayerNone, kLayerNone}
};

PhysicalSpecificationROMSpec PongBallPhysicalProperties =
{
	// mass
	__F_TO_FIX10_6(0.4f),

	// friction
	__F_TO_FIX10_6(0.0f),

	// bounciness
	__F_TO_FIX10_6(1.0f),

	// maximum velocity
	{__I_TO_FIX10_6(10), __I_TO_FIX10_6(9), __I_TO_FIX10_6(13)},

	// maximum speed
	__I_TO_FIX10_6(0)
};

PongBallROMSpec PongBallPb =
{
	{
		{
			{
				// class allocator
				__TYPE(PongBall),

				// children
				NULL,

				// behaviors
				NULL,

				// extra
				NULL,

				// sprites
				(SpriteSpec**)PongBallSprites,

				// use z displacement in projection
				false,

				// collision shapes
				(ShapeSpec*)PongBallShapes,

				// size
				// if 0, width and height will be inferred from the first sprite's texture's size
				{0, 0, 0},

				// gameworld's character's type
				kTypePongBall,

				// physical specification
				(PhysicalSpecification*)&PongBallPhysicalProperties,
			},

			// pointer to the animation spec for the character
			(AnimationDescription*)NULL,

			// initial animation
			NULL,
		},

		// true to create a body
		true,

		// axes subject to gravity
		__Z_AXIS,

		// axis around which to rotate the entity when syncronizing with body
		__Z_AXIS
	},

	// minimum velocity
	{0, 0, 0},
	
	// maximum velocity
	{__I_TO_FIX10_6(10), __I_TO_FIX10_6(9), __I_TO_FIX10_6(13)},

	// maximum velocity
	{__I_TO_FIX10_6(14), __I_TO_FIX10_6(14), __I_TO_FIX10_6(0)}
};
