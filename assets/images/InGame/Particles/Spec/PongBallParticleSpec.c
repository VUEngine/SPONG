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

#include <Libgccvb.h>
#include <SolidParticle.h>
#include <ParticleSystem.h>
#include <ObjectAnimatedSprite.h>
#include <AnimatedEntity.h>
#include <macros.h>

//---------------------------------------------------------------------------------------------------------
//												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern uint32 PongBallParticleTiles[];
extern uint32 PongBallParticleTilesFrameOffsets[];
extern uint16 PongBallParticleMap[];


//---------------------------------------------------------------------------------------------------------
//												DEFINITIONS
//---------------------------------------------------------------------------------------------------------

AnimationFunctionROMSpec PongBallParticleDefaultAnimation =
{
	// number of frames of this animation function
	4,

	// frames to play in animation
	{0, 1, 2, 3},

	// number of cycles a frame of animation is displayed
	32,

	// whether to play it in loop or not
	false,

	// method to call on function completion
	NULL,

	// function's name
	"Default",
};

// an animation spec
AnimationFunctionROMSpec* const PongBallParticleAnimation[] =
{
    (AnimationFunction*)&PongBallParticleDefaultAnimation,
    NULL,
};

CharSetROMSpec PongBallParticleCharset =
{
	// number of chars, depending on allocation type:
	// __ANIMATED_SINGLE*, __ANIMATED_SHARED*: number of chars of a single animation frame (cols * rows)
	// __ANIMATED_MULTI, __NOT_ANIMATED: sum of all chars
	5,

	// allocation type
	// (__ANIMATED_SINGLE, __ANIMATED_SINGLE_OPTIMIZED, __ANIMATED_SHARED, __ANIMATED_SHARED_COORDINATED, __ANIMATED_MULTI or __NOT_ANIMATED)
	__ANIMATED_MULTI,

	// char spec
	PongBallParticleTiles,

	// pointer to the frames offsets
	PongBallParticleTilesFrameOffsets,
};

TextureROMSpec PongBallParticleTexture =
{
	// charset spec
	(CharSetSpec*)&PongBallParticleCharset,

	// bgmap spec
	PongBallParticleMap,

	// cols (max 64)
	1,

	// rows (max 64)
	1,

	// padding for affine/hbias transformations (cols, rows)
	{0, 0},

	// number of frames, depending on charset's allocation type:
	// __ANIMATED_SINGLE*, __ANIMATED_SHARED*, __NOT_ANIMATED: 1
	// __ANIMATED_MULTI: total number of frames
	5,

	// palette number (0-3)
	0,

	// recyclable
	false,

	// vertical flip
	false,

	// horizontal flip
	false,

};

ObjectSpriteROMSpec PongBallParticleSprite =
{
	{
		// sprite's type
		__TYPE(ObjectAnimatedSprite),

		// texture spec
		(TextureSpec*)&PongBallParticleTexture,

		// transparent (__TRANSPARENCY_NONE, __TRANSPARENCY_EVEN or __TRANSPARENCY_ODD)
		__TRANSPARENCY_NONE,

		// displacement
		{0, 0, 0, 0},
	},

	// bgmap mode (__WORLD_BGMAP, __WORLD_AFFINE, __WORLD_OBJECT or __WORLD_HBIAS)
	// make sure to use the proper corresponding sprite type throughout the spec (BgmapSprite or ObjectSprite)
	__WORLD_OBJECT,

	// display mode (__WORLD_ON, __WORLD_LON or __WORLD_RON)
	__WORLD_ON,
};

//---------------------------------------------------------------------------------------------------------
//											OBJECT PongBallParticle
//---------------------------------------------------------------------------------------------------------


SpriteSpec* const PongBallParticleSprites[] =
{
	(SpriteSpec*)&PongBallParticleSprite,
	NULL
};

// particle's spec
SolidParticleROMSpec PongBallParticle =
{
	{
		{
			// allocator
			__TYPE(SolidParticle),

			// particle's minimum life span in milliseconds
			500,

			// particle's life span delta in milliseconds (maximum = minimum + delta)
			700,

			// function pointer to control particle's behavior
			//(void (*)(Particle))&dustParticleBehavior,
			NULL,

			// animation description (used only if sprite is animated)
			(const AnimationFunction**)&PongBallParticleAnimation,

			// name of animation to play
			"Default"
		},

		// particle's minimum mass
		__F_TO_FIX10_6(0.1f),

		// particle's mass delta (maximum = minimum + delta)
		__F_TO_FIX10_6(0),

		// axis subject to gravity (bitwise or of __X_AXIS, __Y_AXIS, __Z_AXIS, or false to disable)
		__Z_AXIS,
	},

	/// ball's radius
	__PIXELS_TO_METERS(4),

	/// friction for physics
	__F_TO_FIX10_6(1),

	/// bounciness for physics
	__F_TO_FIX10_6(0),

	/// object's in-game type
	kTypeParticle,

	/// layers in which I live
	kLayerParticles,

	/// layers to ignore when checking for collisions
	kLayerAll & (~kLayerPlayFieldFloor),

	/// disable collision detection when the particle stops
	true
};

ParticleSystemROMSpec PongBallParticlesParticleSystem =
{
	{
		// class allocator
		__TYPE(ParticleSystem),

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
			
		// wireframes
		(WireframeSpec**)NULL,

		// collision shapes
		(ShapeSpec*)NULL,

		// size
		// if 0, width and height will be inferred from the first sprite's texture's size
		{0, 0, 0},

		// gameworld's character's type
		kTypeNone,

		// physical specification
		(PhysicalSpecification*)NULL,
	},

	// reuse expired particles?
	true,

	// minimum generation delay in milliseconds
	50,

	// maximum generation delay in milliseconds
	80,

	// maximum number of alive particles
	12,

	// maximum number of particles to spawn in each cycle
	1,

	// array of sprites
	(const SpriteSpec**)PongBallParticleSprites,

	// array of wireframes
	(const WireframeSpec**)NULL,

	// auto start
	false,

	// particle spec
	(ParticleSpec*)&PongBallParticle,

	// minimum relative spawn position (x, y, z)
	{__PIXELS_TO_METERS(0), __PIXELS_TO_METERS(-1), __PIXELS_TO_METERS(0)},

	// maximum relative spawn position (x, y, z)
	{__PIXELS_TO_METERS(0), __PIXELS_TO_METERS(1), __PIXELS_TO_METERS(0)},

	// minimum force to apply (x, y, z)
	// (use int values in the spec to avoid overflow)
	{__F_TO_FIXED(0.5f), __F_TO_FIXED(0.5f), 0},

	// maximum force to apply (x, y, z)
	// (use int values in the spec to avoid overflow)
	{__F_TO_FIXED(2), __F_TO_FIXED(2), 0},

	// movement type (__UNIFORM_MOVEMENT or __ACCELERATED_MOVEMENT)
	__ACCELERATED_MOVEMENT,

	// use particle system movement vector for the force to apply to the particles
	true
};
