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

#ifndef MACROS_H_
#define MACROS_H_


//---------------------------------------------------------------------------------------------------------
//												DEFINES
//---------------------------------------------------------------------------------------------------------

// physics
#define NO_FRICTION 		0
#define FLOOR_FRICTION 		0.75f
#define FLOOR_BOUNCINESS 	0.0f

// entity collision types
enum InGameTypes
{
	kPaddle = kNoType + 1,
	kPongBallType,
	kPaddleType,
	kWall,
	kFloor,
	kCeiling
};

// entity collision layers
enum CollisionLayers
{
    kSolidLayer		 					= 1 << (kNoLayer + 0), 	// hex 00000001
	kPlayFieldLayer						= 1 << (kNoLayer + 1), 	// hex 00000002
	kPlayFieldWallsLayer				= 1 << (kNoLayer + 2), 	// hex 00000004
	kPlayFieldFloorLayer				= 1 << (kNoLayer + 4), 	// hex 00000010
	kPlayFieldCeilingLayer				= 1 << (kNoLayer + 5), 	// hex 00000020
	kPlayFieldSplitterLayer				= 1 << (kNoLayer + 6), 	// hex 00000040
	kPlayFieldBallLayer					= 1 << (kNoLayer + 7), 	// hex 00000080
	kPlayFieldPaddleLayer				= 1 << (kNoLayer + 8), 	// hex 00000100
	kPlayFieldPaddleHelperLayer			= 1 << (kNoLayer + 9), 	// hex 00000200
    kAllLayers							= 0xFFFFFFFF,
};

enum PlatformerLevelStateMessageTypes
{
	kLevelSetUp = kLastEngineMessage + 1,
	kFollowPongBall,
	kAddBonusScore,
	kShowScreen,
};

#define PONG_BALL_NAME			"PongBall"
#define PADDLE_LEFT_NAME		"LeftPd"
#define PADDLE_RIGHT_NAME		"RightPd"

#endif
