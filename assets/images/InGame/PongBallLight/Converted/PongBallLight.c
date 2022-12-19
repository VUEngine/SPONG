//---------------------------------------------------------------------------------------------------------
//
//  PongBallLight
//  • 24×24 pixels
//  • 4 tiles, reduced by non-unique and flipped tiles, not compressed
//  • 3×3 map, not compressed
//  Size: 68 + 18 = 86
//
//---------------------------------------------------------------------------------------------------------

const uint32 PongBallLightTiles[17] __attribute__((aligned(4))) =
{
    0x00000000,
    0xA0000000,0xAA00A800,0xAAA0AA80,0xAAA8AAA8,0xAAAA2AA8,0xAAAAAAAA,0xAAAAAAAA,0xAAAAAAAA,
    0xAAAAAAA8,0xAAAAAAAA,0xAAAAAAAA,0xAAA8AAAA,0xAAAAAAAA,0xAAAAAAAA,0xAAAAAAAA,0xAAAAAAAA,
};

const uint16 PongBallLightMap[9] __attribute__((aligned(4))) =
{
    0x0000,0x0001,0x2000,0x0002,0x0003,0x2002,0x1000,0x3001,
    0x3000,
};
