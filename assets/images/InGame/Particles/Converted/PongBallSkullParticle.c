//---------------------------------------------------------------------------------------------------------
//
//  PongBallSkullParticle
//  • 8×32 pixels
//  • 4 tiles, unreduced, not compressed
//  • 1×4 map, not compressed
//  • 4 animation frames, 1×1, spritesheet
//  Size: 68 + 8 = 76
//
//---------------------------------------------------------------------------------------------------------

const uint32 PongBallSkullParticleTiles[17] __attribute__((aligned(4))) =
{
    0x00000000,
    0x00000000,0x3CCF0FFC,0x3FFF3CCF,0x03300FFC,0x00000000,0x288A0AA8,0x2AAA288A,0x02200AA8,
    0x00000000,0x14450554,0x15551445,0x01100554,0x00000000,0x00000000,0x00000000,0x00000000,
};

const uint16 PongBallSkullParticleMap[4] __attribute__((aligned(4))) =
{
    0x0000,0x0001,0x0002,0x0003,
};

const uint32 PongBallSkullParticleTilesFrameOffsets[4] __attribute__((aligned(4))) =
{
    0x00000001,0x00000005,0x00000009,0x0000000D,
};
