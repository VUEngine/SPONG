//---------------------------------------------------------------------------------------------------------
//
//  VolumeMeter
//  • 64×40 pixels
//  • 40 tiles, unreduced, not compressed
//  • 8×5 map, not compressed
//  Size: 644 + 80 = 724
//
//---------------------------------------------------------------------------------------------------------

const uint32 VolumeMeterTiles[161] __attribute__((aligned(4))) =
{
    0x00000000,
    0x00000000,0x00000000,0xAAABFFFF,0xFFFFAAAB,0x00000000,0xFF000000,0x033A033F,0xFF3F033A,
    0x00000000,0x3FFF0000,0x30003000,0x3FFF3000,0x00000000,0x0003FFFF,0x00030003,0xFFFF0003,
    0xFF000000,0x0330033F,0x03300330,0xFF3F0330,0x3FFF0000,0x30003000,0x30003000,0x3FFF3000,
    0x0003FFFF,0x00030003,0x00030003,0xFFFF0003,0x0030003F,0x00300030,0x00300030,0x003F0030,
    0x00000000,0x00000000,0xAAABFFFF,0xFFFFAAAB,0x00000000,0xFF000000,0xAB3AAB3F,0xFF3FAB3A,
    0x00000000,0x3FFF0000,0x3AAA3AAA,0x3FFF3AAA,0x00000000,0x0003FFFF,0x00030003,0xFFFF0003,
    0xFF000000,0x0330033F,0x03300330,0xFF3F0330,0x3FFF0000,0x30003000,0x30003000,0x3FFF3000,
    0x0003FFFF,0x00030003,0x00030003,0xFFFF0003,0x0030003F,0x00300030,0x00300030,0x003F0030,
    0x00000000,0x00000000,0xAAABFFFF,0xFFFFAAAB,0x00000000,0xFF000000,0xAB3AAB3F,0xFF3FAB3A,
    0x00000000,0x3FFF0000,0x3AAA3AAA,0x3FFF3AAA,0x00000000,0xAAABFFFF,0xAAABAAAB,0xFFFFAAAB,
    0xFF000000,0x033A033F,0x033A033A,0xFF3F033A,0x3FFF0000,0x30003000,0x30003000,0x3FFF3000,
    0x0003FFFF,0x00030003,0x00030003,0xFFFF0003,0x0030003F,0x00300030,0x00300030,0x003F0030,
    0x00000000,0x00000000,0xAAABFFFF,0xFFFFAAAB,0x00000000,0xFF000000,0xAB3AAB3F,0xFF3FAB3A,
    0x00000000,0x3FFF0000,0x3AAA3AAA,0x3FFF3AAA,0x00000000,0xAAABFFFF,0xAAABAAAB,0xFFFFAAAB,
    0xFF000000,0xAB3AAB3F,0xAB3AAB3A,0xFF3FAB3A,0x3FFF0000,0x3AAA3AAA,0x3AAA3AAA,0x3FFF3AAA,
    0x0003FFFF,0x00030003,0x00030003,0xFFFF0003,0x0030003F,0x00300030,0x00300030,0x003F0030,
    0x00000000,0x00000000,0xAAABFFFF,0xFFFFAAAB,0x00000000,0xFF000000,0xAB3AAB3F,0xFF3FAB3A,
    0x00000000,0x3FFF0000,0x3AAA3AAA,0x3FFF3AAA,0x00000000,0xAAABFFFF,0xAAABAAAB,0xFFFFAAAB,
    0xFF000000,0xAB3AAB3F,0xAB3AAB3A,0xFF3FAB3A,0x3FFF0000,0x3AAA3AAA,0x3AAA3AAA,0x3FFF3AAA,
    0xAAABFFFF,0xAAABAAAB,0xAAABAAAB,0xFFFFAAAB,0x003A003F,0x003A003A,0x003A003A,0x003F003A,
};

const uint16 VolumeMeterMap[40] __attribute__((aligned(4))) =
{
    0x0000,0x0001,0x0002,0x0003,0x0004,0x0005,0x0006,0x0007,
    0x0008,0x0009,0x000A,0x000B,0x000C,0x000D,0x000E,0x000F,
    0x0010,0x0011,0x0012,0x0013,0x0014,0x0015,0x0016,0x0017,
    0x0018,0x0019,0x001A,0x001B,0x001C,0x001D,0x001E,0x001F,
    0x0020,0x0021,0x0022,0x0023,0x0024,0x0025,0x0026,0x0027,
};
