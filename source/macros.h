/**
 * SPONG
 *
 * © Jorge Eremiev <jorgech3@gmail.com> and Christian Radke <c.radke@posteo.de>
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
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
	kTypeParticle
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
	kLayerParticles						= 1 << (kLayerNone + 10), 	// hex 00000400
	kLayerAll							= 0xFFFFFFFF,
};

enum GameMessageTypes
{
	kMessageFollowPongBall = kMessageLastEngine + 1,
	kMessageAddBonusScore,
	kMessageShowScreen,
};

enum CommunicationMessages
{
	kCommunicationMessageSendAndReceiveInput = kMessageLastCommunicationMessage + 1,
	kCommunicationMessageStartVersusMatch
};

#define PONG_BALL_NAME			"PongBall"
#define PADDLE_LEFT_NAME		"LeftPd"
#define PADDLE_RIGHT_NAME		"RightPd"

#endif
