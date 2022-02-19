//---------------------------------------------------------------------------------------------------------
//
//  BrightnessMeter
//  • 64×40 pixels
//  • 40 tiles, unreduced, not compressed
//  • 8×5 map, not compressed
//  Size: 644 + 80 = 724
//
//---------------------------------------------------------------------------------------------------------

const uint32 BrightnessMeterTiles[161] __attribute__((aligned(4))) =
{
    0x00000000,0xFFFC0000,0x55575557,0x55575557,0xFFFC5557,0xFC0F0000,0x03350335,0x03350335,
    0xFC0F0335,0x0FFF0000,0x30003000,0x30003000,0x0FFF3000,0xFFFC0000,0x00030003,0x00030003,
    0xFFFC0003,0xFC0F0000,0x03300330,0x03300330,0xFC0F0330,0x0FFF0000,0x30003000,0x30003000,
    0x0FFF3000,0xFFFC0000,0x00030003,0x00030003,0xFFFC0003,0x000F0000,0x00300030,0x00300030,
    0x000F0030,0xFFFC0000,0x55575557,0x55575557,0xFFFC5557,0xFC0F0000,0x9B356735,0x9B356735,
    0xFC0F6735,0x0FFF0000,0x39993666,0x39993666,0x0FFF3666,0xFFFC0000,0x00030003,0x00030003,
    0xFFFC0003,0xFC0F0000,0x03300330,0x03300330,0xFC0F0330,0x0FFF0000,0x30003000,0x30003000,
    0x0FFF3000,0xFFFC0000,0x00030003,0x00030003,0xFFFC0003,0x000F0000,0x00300030,0x00300030,
    0x000F0030,0xFFFC0000,0x55575557,0x55575557,0xFFFC5557,0xFC0F0000,0x9B356735,0x9B356735,
    0xFC0F6735,0x0FFF0000,0x39993666,0x39993666,0x0FFF3666,0xFFFC0000,0xAAABAAAB,0xAAABAAAB,
    0xFFFCAAAB,0xFC0F0000,0x033A033A,0x033A033A,0xFC0F033A,0x0FFF0000,0x30003000,0x30003000,
    0x0FFF3000,0xFFFC0000,0x00030003,0x00030003,0xFFFC0003,0x000F0000,0x00300030,0x00300030,
    0x000F0030,0xFFFC0000,0x55575557,0x55575557,0xFFFC5557,0xFC0F0000,0x9B356735,0x9B356735,
    0xFC0F6735,0x0FFF0000,0x39993666,0x39993666,0x0FFF3666,0xFFFC0000,0xAAABAAAB,0xAAABAAAB,
    0xFFFCAAAB,0xFC0F0000,0xEF3ABB3A,0xEF3ABB3A,0xFC0FBB3A,0x0FFF0000,0x3EEE3BBB,0x3EEE3BBB,
    0x0FFF3BBB,0xFFFC0000,0x00030003,0x00030003,0xFFFC0003,0x000F0000,0x00300030,0x00300030,
    0x000F0030,0xFFFC0000,0x55575557,0x55575557,0xFFFC5557,0xFC0F0000,0x9B356735,0x9B356735,
    0xFC0F6735,0x0FFF0000,0x39993666,0x39993666,0x0FFF3666,0xFFFC0000,0xAAABAAAB,0xAAABAAAB,
    0xFFFCAAAB,0xFC0F0000,0xEF3ABB3A,0xEF3ABB3A,0xFC0FBB3A,0x0FFF0000,0x3EEE3BBB,0x3EEE3BBB,
    0x0FFF3BBB,0xFFFC0000,0xFFFFFFFF,0xFFFFFFFF,0xFFFCFFFF,0x000F0000,0x003F003F,0x003F003F,
    0x000F003F,
};

const uint16 BrightnessMeterMap[40] __attribute__((aligned(4))) =
{
    0x0000,0x0001,0x0002,0x0003,0x0004,0x0005,0x0006,0x0007,
    0x0008,0x0009,0x000A,0x000B,0x000C,0x000D,0x000E,0x000F,
    0x0010,0x0011,0x0012,0x0013,0x0014,0x0015,0x0016,0x0017,
    0x0018,0x0019,0x001A,0x001B,0x001C,0x001D,0x001E,0x001F,
    0x0020,0x0021,0x0022,0x0023,0x0024,0x0025,0x0026,0x0027,
};
