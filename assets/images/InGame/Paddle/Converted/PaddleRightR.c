//---------------------------------------------------------------------------------------------------------
//
//  PaddleRightR
//  • 32×144 pixels
//  • 72 tiles, unreduced, not compressed
//  • 4×18 map, not compressed
//  • 3 animation frames, 4×6, spritesheet
//  Size: 1156 + 144 = 1300
//
//---------------------------------------------------------------------------------------------------------

const uint32 PaddleRightRTiles[289] __attribute__((aligned(4))) =
{
    0x00000000,
    0x00000000,0xFF000000,0xFAC0AAC0,0xAEC0AEC0,0x00000000,0xFFFF0000,0xFFFFAAAA,0xAAAAAAAA,
    0x00000000,0xFFFF0000,0xFFFFAAAA,0xAAAAAAAA,0x00000000,0x01FF0000,0x05AF05AA,0x059A059A,
    0xAEC0AEC0,0xAEC0AEC0,0xAEC0AEC0,0xAEC0AEC0,0xAAABEFBF,0xAAAAAAAB,0xAAABAAAB,0xAAABAAAA,
    0xEAAAFEFB,0xAAAAEAAA,0xEAAAEAAA,0xEAAAAAAA,0x059A059A,0x059A059A,0x059A059A,0x059A059A,
    0xAEC0AEC0,0xAEC0AEC0,0xAEC0AEC0,0xAEC0AEC0,0xAAAAAAAB,0xAAABAAAB,0xAAABAAAA,0xAAAAAAAB,
    0xAAAAEAAA,0xEAAAEAAA,0xEAAAAAAA,0xAAAAEAAA,0x059A059A,0x059A059A,0x059A059A,0x059A059A,
    0xAEC0AEC0,0xAEC0AEC0,0xAEC0AEC0,0xAEC0AEC0,0xAAABAAAB,0xAAABAAAA,0xAAAAAAAB,0xAAABAAAB,
    0xEAAAEAAA,0xEAAAAAAA,0xAAAAEAAA,0xEAAAEAAA,0x059A059A,0x059A059A,0x059A059A,0x059A059A,
    0xAEC0AEC0,0xAEC0AEC0,0xAEC0AEC0,0xAEC0AEC0,0xAAABAAAA,0xAAAAAAAB,0xAAABAAAB,0xAAAAEFBF,
    0xEAAAAAAA,0xAAAAEAAA,0xEAAAEAAA,0xAAAAFEFB,0x059A059A,0x059A059A,0x059A059A,0x059A059A,
    0x5AC0AEC0,0x5500AAC0,0x00000000,0x00000000,0x5555AAAA,0x5555AAAA,0x00000000,0x00000000,
    0x5555AAAA,0x5555AAAA,0x00000000,0x00000000,0x05A5059A,0x015505AA,0x00000000,0x00000000,
    0x00000000,0xFF000000,0xFAC0AAC0,0xAEC0AEC0,0x00000000,0xFFFF0000,0xAAFFFFAA,0xABEAFAAA,
    0x00000000,0xFFFF0000,0xBFAAAAAF,0xBAAABABE,0x00000000,0x01FD0000,0x059505A5,0x05950595,
    0xAEC0AEC0,0xAEC0AEC0,0xAEC0AEC0,0xAEC0AEC0,0xAAABAAAF,0xAAAAAAAB,0xAAABAAAB,0xAAABAAAA,
    0xAAAABAAA,0xBAAABAAA,0xAAAABAAA,0xBAAABAAA,0x05950595,0x05950595,0x05950595,0x05950595,
    0xAEC0AEC0,0xAEC0AEC0,0xAEC0AEC0,0xAEC0AEC0,0xAAAAAAAB,0xAAABAAAB,0xAAABAAAA,0xAAAAAAAB,
    0xBAAAAAAA,0xBAAABAAA,0xBAAAAAAA,0xAAAABAAA,0x05950595,0x05950595,0x05950595,0x05950595,
    0xAEC0AEC0,0xAEC0AEC0,0xAEC0AEC0,0xAEC0AEC0,0xAAABAAAB,0xAAABAAAA,0xAAAAAAAB,0xAAABAAAB,
    0xBAAABAAA,0xBAAAAAAA,0xBAAABAAA,0xBAAAAAAA,0x05950595,0x05950595,0x05950595,0x05950595,
    0xAEC0AEC0,0xAEC0AEC0,0xAEC0AEC0,0xAEC0AEC0,0xAAABAAAA,0xAAAAAAAB,0xAAABAAAB,0xABEAAAAF,
    0xAAAABAAA,0xBAAABAAA,0xAAAABAAA,0xBAAABAAA,0x05950595,0x05950595,0x05950595,0x05950595,
    0x5AC0AEC0,0x5500AAC0,0x00000000,0x00000000,0xAA55FAAA,0x555555AA,0x00000000,0x00000000,
    0xBFAABABE,0x5555AAA5,0x00000000,0x00000000,0x05950595,0x015505A5,0x00000000,0x00000000,
    0x00000000,0xFF000000,0xFAC0AAC0,0xAEC0AEC0,0x00000000,0xFFFF0000,0xAAFFFFAA,0xABEABEAA,
    0x00000000,0x7FFF0000,0x6FEA6AAF,0x6EAA6EAF,0x00000000,0x01FF0000,0x059505A5,0x05950595,
    0xAEC0AEC0,0xAEC0AEC0,0xAEC0AEC0,0xAEC0AEC0,0xAAABAAAF,0xAAAAAAAB,0xAAABAAAB,0xAAABAAAA,
    0x6AAA6EAA,0x6EAA6EAA,0x6AAA6EAA,0x6EAA6EAA,0x05950595,0x05950595,0x05950595,0x05950595,
    0xAEC0AEC0,0xAEC0AEC0,0xAEC0AEC0,0xAEC0AEC0,0xAAAAAAAB,0xAAABAAAB,0xAAABAAAA,0xAAAAAAAB,
    0x6EAA6AAA,0x6EAA6EAA,0x6EAA6AAA,0x6AAA6EAA,0x05950595,0x05950595,0x05950595,0x05950595,
    0xAEC0AEC0,0xAEC0AEC0,0xAEC0AEC0,0xAEC0AEC0,0xAAABAAAB,0xAAABAAAA,0xAAAAAAAB,0xAAABAAAB,
    0x6EAA6EAA,0x6EAA6AAA,0x6EAA6EAA,0x6EAA6AAA,0x05950595,0x05950595,0x05950595,0x05950595,
    0xAEC0AEC0,0xAEC0AEC0,0xAEC0AEC0,0xAEC0AEC0,0xAAABAAAA,0xAAAAAAAB,0xAAABAAAB,0xABEAAAAF,
    0x6AAA6EAA,0x6EAA6EAA,0x6AAA6EAA,0x6EAA6EAA,0x05950595,0x05950595,0x05950595,0x05950595,
    0x5AC0AEC0,0x5500AAC0,0x00000000,0x00000000,0xAA55BEAA,0x555555AA,0x00000000,0x00000000,
    0x6FEA6EAF,0x55556AA5,0x00000000,0x00000000,0x05950595,0x015505A5,0x00000000,0x00000000,
};

const uint16 PaddleRightRMap[72] __attribute__((aligned(4))) =
{
    0x0000,0x0001,0x0002,0x0003,0x0004,0x0005,0x0006,0x0007,
    0x0008,0x0009,0x000A,0x000B,0x000C,0x000D,0x000E,0x000F,
    0x0010,0x0011,0x0012,0x0013,0x0014,0x0015,0x0016,0x0017,
    0x0018,0x0019,0x001A,0x001B,0x001C,0x001D,0x001E,0x001F,
    0x0020,0x0021,0x0022,0x0023,0x0024,0x0025,0x0026,0x0027,
    0x0028,0x0029,0x002A,0x002B,0x002C,0x002D,0x002E,0x002F,
    0x0030,0x0031,0x0032,0x0033,0x0034,0x0035,0x0036,0x0037,
    0x0038,0x0039,0x003A,0x003B,0x003C,0x003D,0x003E,0x003F,
    0x0040,0x0041,0x0042,0x0043,0x0044,0x0045,0x0046,0x0047,
};

const uint32 PaddleRightRTilesFrameOffsets[3] __attribute__((aligned(4))) =
{
    0x00000001,0x00000061,0x000000C1,
};
