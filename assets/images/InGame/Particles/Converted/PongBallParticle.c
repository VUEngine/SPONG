//---------------------------------------------------------------------------------------------------------
//
//  PongBallParticle
//  • 8×40 pixels
//  • 5 tiles, unreduced, not compressed
//  • 1×5 map, not compressed
//  • 5 animation frames, 1×1, spritesheet
//  Size: 84 + 10 = 94
//
//---------------------------------------------------------------------------------------------------------

const uint32 PongBallParticleTiles[21] __attribute__((aligned(4))) =
{
    0x00000000,0x00C00000,0x3FF003C0,0x03C00FFC,0x00000300,0x00000000,0x03F00300,0x00C00FC0,
    0x00000000,0x00000000,0x03C00000,0x000003C0,0x00000000,0x00000000,0x00C00000,0x00000000,
    0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
};

const uint16 PongBallParticleMap[5] __attribute__((aligned(4))) =
{
    0x0000,0x0001,0x0002,0x0003,0x0004,
};

const uint32 PongBallParticleTilesFrameOffsets[5] __attribute__((aligned(4))) =
{
    0x00000001,0x00000005,0x00000009,0x0000000D,0x00000011,
};
