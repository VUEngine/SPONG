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
#include <Collision.h>
#include <Body.h>
#include <macros.h>


//---------------------------------------------------------------------------------------------------------
//												DEFINITIONS
//---------------------------------------------------------------------------------------------------------

PhysicalSpecificationROMSpec CollisionClPhysicalProperties =
{
	// mass
	__F_TO_FIX10_6(0),

	// friction
	__F_TO_FIX10_6(0),

	// bounciness
	__F_TO_FIX10_6(0),

	// maximum velocity
	{0, 0, 0},

	// maximum speed
	__I_TO_FIX10_6(0)
};

CollisionROMSpec CollisionWallCl =
{
	// class allocator
	__TYPE(Collision),

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

	// pixelSize
	// if 0, width and height will be inferred from the first sprite's texture's size
	{100, 100, 16},

	// gameworld's character's type
	kTypeWall,

	// physical specification
	(PhysicalSpecification*)&CollisionClPhysicalProperties,
};

CollisionROMSpec CollisionFloorCl =
{
	// class allocator
	__TYPE(Collision),

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

	// pixelSize
	// if 0, width and height will be inferred from the first sprite's texture's size
	{100, 100, 16},

	// gameworld's character's type
	kTypeFloor,

	// physical specification
	(PhysicalSpecification*)&CollisionClPhysicalProperties,
};

CollisionROMSpec CollisionCeilingCl =
{
	// class allocator
	__TYPE(Collision),

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

	// pixelSize
	// if 0, width and height will be inferred from the first sprite's texture's size
	{100, 100, 16},

	// gameworld's character's type
	kTypeCeiling,

	// physical specification
	(PhysicalSpecification*)&CollisionClPhysicalProperties,
};
