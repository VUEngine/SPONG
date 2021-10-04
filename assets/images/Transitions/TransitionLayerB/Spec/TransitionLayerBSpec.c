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
#include <TransitionEntity.h>
#include <BgmapAnimatedSprite.h>


//---------------------------------------------------------------------------------------------------------
//												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern BYTE TransitionLayerBTiles[];
extern BYTE TransitionLayerBMap[];


//---------------------------------------------------------------------------------------------------------
//												DEFINITIONS
//---------------------------------------------------------------------------------------------------------

// a function which defines the frames to play
AnimationFunctionROMSpec TRANSITION_LAYER_B_HIDDEN_ANIM =
{
	// number of frames of this animation function
	1,

	// frames to play in animation
	{17},

	// number of cycles a frame of animation is displayed
	1,

	// whether to play it in loop or not
	false,

	// method to call on function completion
	NULL,

	// function's name
	"Hidden",
};

// a function which defines the frames to play
AnimationFunctionROMSpec TRANSITION_LAYER_B_FADE_IN_ANIM =
{
	// number of frames of this animation function
	18,
	//9,

	// frames to play in animation
	{17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0},
	//{16, 14, 12, 10, 8, 6, 4, 2, 0},

	// number of cycles a frame of animation is displayed
	1,

	// whether to play it in loop or not
	false,

	// method to call on function completion
	(EventListener)&TransitionEntity_onTransitionInComplete,

	// function's name
	"FadeIn",
};

// a function which defines the frames to play
AnimationFunctionROMSpec TRANSITION_LAYER_B_FADE_OUT_ANIM =
{
	// number of frames of this animation function
	18,
	//10,

	// frames to play in animation
	{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17},
	//{0, 2, 4, 6, 8, 10, 12, 14, 16, 17},

	// number of cycles a frame of animation is displayed
	1,

	// whether to play it in loop or not
	false,

	// method to call on function completion
	(EventListener)&TransitionEntity_onTransitionOutComplete,

	// function's name
	"FadeOut",
};

// an animation spec
AnimationDescriptionROMSpec TRANSITION_LAYER_B_ANIM =
{
	// animation functions
	{
		(AnimationFunction*)&TRANSITION_LAYER_B_HIDDEN_ANIM,
		(AnimationFunction*)&TRANSITION_LAYER_B_FADE_IN_ANIM,
		(AnimationFunction*)&TRANSITION_LAYER_B_FADE_OUT_ANIM,
		NULL,
	}
};

CharSetROMSpec TRANSITION_LAYER_B_CH =
{
	// number of chars, depending on allocation type:
	// __ANIMATED_SINGLE*, __ANIMATED_SHARED*: number of chars of a single animation frame (cols * rows)
	// __ANIMATED_MULTI, __NOT_ANIMATED: sum of all chars
	4,

	// allocation type
	// (__ANIMATED_SINGLE, __ANIMATED_SINGLE_OPTIMIZED, __ANIMATED_SHARED, __ANIMATED_SHARED_COORDINATED, __ANIMATED_MULTI or __NOT_ANIMATED)
	__ANIMATED_SINGLE_OPTIMIZED,

	// char spec
	TransitionLayerBTiles,
};

TextureROMSpec TRANSITION_LAYER_B_TX =
{
	// charset spec
	(CharSetSpec*)&TRANSITION_LAYER_B_CH,

	// bgmap spec
	TransitionLayerBMap,

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
	1,

	// recyclable
	false,

	// vertical flip
	false,

	// horizontal flip
	false,
};

BgmapSpriteROMSpec TRANSITION_LAYER_B_SPRITE =
{
	{
		// sprite's type
		__TYPE(BgmapAnimatedSprite),

		// texture spec
		(TextureSpec*)&TRANSITION_LAYER_B_TX,

		// transparent (__TRANSPARENCY_NONE, __TRANSPARENCY_EVEN or __TRANSPARENCY_ODD)
		__TRANSPARENCY_NONE,

		// displacement
		{0, 0, -64, 0},
	},

	// bgmap mode (__WORLD_BGMAP, __WORLD_AFFINE, __WORLD_OBJECT or __WORLD_HBIAS)
	// make sure to use the proper corresponding sprite type throughout the spec (BgmapSprite or ObjectSprite)
	__WORLD_BGMAP,

	// pointer to affine / hbias manipulation function
	NULL,

	// display mode (__WORLD_ON, __WORLD_LON or __WORLD_RON)
	__WORLD_ON,
};

BgmapSpriteROMSpec* const TRANSITION_LAYER_B_SPRITES[] =
{
	&TRANSITION_LAYER_B_SPRITE,
	NULL
};

TransitionEntityROMSpec TRANSITION_LAYER_B_AE =
{
	{
		// class allocator
		__TYPE(TransitionEntity),

		// children
		NULL,

		// behaviors
		NULL,

		// extra
		NULL,

		// sprites
		(SpriteSpec**)TRANSITION_LAYER_B_SPRITES,

		// use z displacement in projection
		false,

		// collision shapes
		(ShapeSpec*)NULL,

		// size
		// if 0, width and height will be inferred from the first sprite's texture's size
		{0, 0, 0},

		// gameworld's character's type
		0,

		// physical specification
		(PhysicalSpecification*)NULL,
	},

	// pointer to the animation spec for the item
	(AnimationDescription*)&TRANSITION_LAYER_B_ANIM,

	// initial animation
	"Hidden",
};
