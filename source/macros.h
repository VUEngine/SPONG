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
	kTypeCeiling = kTypeNone + 1,
	kTypeFloor,
	kTypePaddle,
	kTypePongBall,
	kTypeWall,
};

// entity collision layers
enum CollisionLayers
{
    kLayerSolid		 					= 1 << (kLayerNone + 0), 	// hex 00000001
	kLayerPlayField						= 1 << (kLayerNone + 1), 	// hex 00000002
	kLayerPlayFieldWalls				= 1 << (kLayerNone + 2), 	// hex 00000004
	kLayerPlayFieldFloor				= 1 << (kLayerNone + 4), 	// hex 00000010
	kLayerPlayFieldCeiling				= 1 << (kLayerNone + 5), 	// hex 00000020
	kLayerPlayFieldSplitter				= 1 << (kLayerNone + 6), 	// hex 00000040
	kLayerPlayFieldBall					= 1 << (kLayerNone + 7), 	// hex 00000080
	kLayerPlayFieldPaddle				= 1 << (kLayerNone + 8), 	// hex 00000100
	kLayerPlayFieldPaddleHelper			= 1 << (kLayerNone + 9), 	// hex 00000200
    kLayerAll							= 0xFFFFFFFF,
};

enum SpongMessageTypes
{
	kMessageFollowPongBall = kMessageLastEngine + 1,
	kMessageAddBonusScore,
	kMessageShowScreen,
};

#define PONG_BALL_NAME			"PongBall"
#define PADDLE_LEFT_NAME		"LeftPd"
#define PADDLE_RIGHT_NAME		"RightPd"

#endif
