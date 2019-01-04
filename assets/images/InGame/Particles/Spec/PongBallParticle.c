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

#include <Libgccvb.h>
#include <SolidParticle.h>
#include <ParticleSystem.h>
#include <ObjectAnimatedSprite.h>
#include <AnimatedEntity.h>
#include <macros.h>

//---------------------------------------------------------------------------------------------------------
//												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern BYTE PongBallParticleTiles[];
extern BYTE PongBallParticleMap[];


//---------------------------------------------------------------------------------------------------------
//												DEFINITIONS
//---------------------------------------------------------------------------------------------------------

AnimationFunctionROMSpec PONG_BALL_PARTICLE_DEFAULT_ANIM =
{
	// number of frames of this animation function
	4,

	// frames to play in animation
	{0, 1, 2, 3},

	// number of cycles a frame of animation is displayed
	16,

	// whether to play it in loop or not
	false,

	// method to call on function completion
	NULL,

	// function's name
	"Default",
};

// an animation spec
AnimationDescriptionROMSpec PONG_BALL_PARTICLE_ANIM =
{
	// animation functions
	{
		(AnimationFunction*)&PONG_BALL_PARTICLE_DEFAULT_ANIM,
		NULL,
	}
};

CharSetROMSpec PONG_BALL_PARTICLE_CH =
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
};

TextureROMSpec PONG_BALL_PARTICLE_TX =
{
	// charset spec
	(CharSetSpec*)&PONG_BALL_PARTICLE_CH,

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
};

ObjectSpriteROMSpec PONG_BALL_PARTICLE_SPRITE =
{
	{
		// sprite's type
		__TYPE(ObjectAnimatedSprite),

		// texture spec
		(TextureSpec*)&PONG_BALL_PARTICLE_TX,

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
//											OBJECT PONG_BALL_PARTICLE
//---------------------------------------------------------------------------------------------------------


ObjectSpriteROMSpec* const PONG_BALL_PARTICLE_SPRITES[] =
{
	&PONG_BALL_PARTICLE_SPRITE,
	NULL
};

// particle's spec
ParticleROMSpec PONG_BALL_PARTICLE =
{
	// allocator
	__TYPE(Particle),

	// particle's minimum life span in milliseconds
	100,

	// particle's maximum life span in milliseconds
	1000,

	// particle's minimum mass
	__F_TO_FIX10_6(0.1f),

	// particle's maximum mass
	__F_TO_FIX10_6(0.1f),

	// axis subject to gravity (bitwise or of __X_AXIS, __Y_AXIS, __Z_AXIS, or false to disable)
	__NO_AXIS,

	// function pointer to control particle's behavior
	(void (*)(Particle))NULL,

	// animation description (used only if sprite is animated)
	(AnimationDescription*)&PONG_BALL_PARTICLE_ANIM,

	// name of animation to play
	"Default"
};

ParticleSystemROMSpec PONG_BALL_PARTICLES_PS =
{
	{
		// class allocator
		__TYPE(ParticleSystem),

		// sprites
		(SpriteROMSpec**)NULL,

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

	// reuse expired particles?
	true,

	// minimum generation delay in milliseconds
	20,

	// maximum generation delay in milliseconds
	50,

	// maximum total particles
	6,

	// array of textures
	(const ObjectSpriteSpec**)PONG_BALL_PARTICLE_SPRITES,

	// auto start
	false,

	// particle spec
	(ParticleSpec*)&PONG_BALL_PARTICLE,

	// minimum relative spawn position (x, y, z)
	{__F_TO_FIX10_6(0), __F_TO_FIX10_6(-0.5f), __F_TO_FIX10_6(0)},

	// maximum relative spawn position (x, y, z)
	{__F_TO_FIX10_6(0), __F_TO_FIX10_6(0.5f), __F_TO_FIX10_6(0)},

	// minimum force to apply (x, y, z)
	// (use int values in the spec to avoid overflow)
	{__F_TO_FIX10_6(0), __F_TO_FIX10_6(-12), 0},

	// maximum force to apply (x, y, z)
	// (use int values in the spec to avoid overflow)
	{__F_TO_FIX10_6(0), __F_TO_FIX10_6(12), 0},

	// movement type (__UNIFORM_MOVEMENT or __ACCELERATED_MOVEMENT)
	__ACCELERATED_MOVEMENT
};
