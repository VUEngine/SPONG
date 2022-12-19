//---------------------------------------------------------------------------------------------------------
//
//  PongBall
//  • 8×104 pixels
//  • 12 tiles, reduced by non-unique and flipped tiles, not compressed
//  Size: 196
//
//---------------------------------------------------------------------------------------------------------

const uint32 PongBallTiles[49] __attribute__((aligned(4))) =
{
    0x00000000,
    0xA0000000,0xAA00A800,0xAAA0AA80,0xAAA8AAA8,0xAFFA2FF8,0xABFEAFFA,0xABFEABFE,0xABFEABFE,
    0x000A0000,0x00AA002A,0x0EAA03AA,0x3AAA3AAA,0xAAAAAAA8,0xFFFFFFFF,0xFFFFFFFF,0xAAA8AAAA,
    0xABFEABFE,0xFFFFFFFF,0xFFFFFFFF,0xABFEABFE,0xEAAA3AAA,0xFFFFFFFF,0xFFFFFFFF,0x3AAAEAAA,
    0xA0000000,0xAA00A800,0xAAA0AA80,0xEAA8EAA8,0xEBFE3FF8,0xAAFFABFE,0xAAFFAAFF,0xAABFAABF,
    0x000F0000,0x00FA003A,0x0FAA03EA,0x3EAA3EAA,0xEAAAEAA8,0xFFFFFFFF,0xFFFFFFFF,0xEAA8EAAA,
    0xAABFAABF,0xFFFFFFFF,0xFFFFFFFF,0xAABFAABF,0xFEAA3EAA,0xFFFFFFFF,0xFFFFFFFF,0x3EAAFEAA,
};
