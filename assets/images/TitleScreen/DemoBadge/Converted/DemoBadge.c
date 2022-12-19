//---------------------------------------------------------------------------------------------------------
//
//  DemoBadge
//  • 40×64 pixels
//  • 40 tiles, unreduced, not compressed
//  • 5×8 map, not compressed
//  • 4 animation frames, 5×2, spritesheet
//  Size: 644 + 80 = 724
//
//---------------------------------------------------------------------------------------------------------

const uint32 DemoBadgeTiles[161] __attribute__((aligned(4))) =
{
    0x00000000,
    0xAAA80000,0xAAAAAAAA,0x00AAAAAA,0x80AA80AA,0xAAAA0000,0xAAAAAAAA,0x00A0AAAA,0x80828082,
    0xAAAA0000,0xAAAAAAAA,0x8080AAAA,0x00AA0082,0xAAAA0000,0xAAAAAAAA,0x020AAAAA,0x02000202,
    0x2AAA0000,0xAAAAAAAA,0xAA00AAAA,0xAA0AAA0A,0x80AA80AA,0x00AA80AA,0xAAAAAAAA,0xAAA8AAAA,
    0x80820082,0x00A08082,0xAAAAAAAA,0xAAAAAAAA,0x80AA00A8,0x80808082,0xAAAAAAAA,0xAAAAAAAA,
    0x02080200,0x020A020A,0xAAAAAAAA,0xAAAAAAAA,0xAA0AAA0A,0xAA00AA0A,0xAAAAAAAA,0x2AAAAAAA,
    0xAAA80000,0xAAAAAAAA,0x00AAAAAA,0x80AA80AA,0xAAAA0000,0xAAAAAAAA,0x00A0AAAA,0x80828082,
    0xAAAA0000,0xAAAAAAAA,0x8080AAAA,0x00AA0082,0xAAAA0000,0xAAAAAAAA,0x020AAAAA,0x02000202,
    0x2AAA0000,0xAAAAAAAA,0xAA00AAAA,0xAA0AAA0A,0x80AA80AA,0x00AA80AA,0xAAAAAAAA,0xAAA8AAAA,
    0x80820082,0x00A08082,0xAAAAAAAA,0xAAAAAAAA,0x80AA00A8,0x80808082,0xAAAAAAAA,0xAAAAAAAA,
    0x02080200,0x020A020A,0xAAAAAAAA,0xAAAAAAAA,0xAA0AAA0A,0xAA00AA0A,0xAAAAAAAA,0x2AAAAAAA,
    0xAAA80000,0xAAAAAAAA,0x00AAAAAA,0x80AA80AA,0xAAAA0000,0xAAAAAAAA,0x00A0AAAA,0x80828082,
    0xAAAA0000,0xAAAAAAAA,0x8080AAAA,0x00AA0082,0xAAAA0000,0xAAAAAAAA,0x020AAAAA,0x02000202,
    0x2AAA0000,0xAAAAAAAA,0xAA00AAAA,0xAA0AAA0A,0x80AA80AA,0x00AA80AA,0xAAAAAAAA,0xAAA8AAAA,
    0x80820082,0x00A08082,0xAAAAAAAA,0xAAAAAAAA,0x80AA00A8,0x80808082,0xAAAAAAAA,0xAAAAAAAA,
    0x02080200,0x020A020A,0xAAAAAAAA,0xAAAAAAAA,0xAA0AAA0A,0xAA00AA0A,0xAAAAAAAA,0x2AAAAAAA,
    0xAAA80000,0xAAAAAAAA,0x00AAAAAA,0x80AA80AA,0xAAAA0000,0x2AAAAAAA,0x00A0AAAA,0x80828082,
    0xAAAA0000,0xAA80AAAA,0x8080AAAA,0x00AA0082,0xAAAA0000,0xAAAAAAAA,0x020AAAAA,0x02000202,
    0x2AAA0000,0xAAAAAAAA,0xAA00AAAA,0xAA0AAA0A,0x80AA80AA,0x00AA80AA,0xAAAAAAAA,0xAAA8AAAA,
    0x80820082,0x00A08082,0xAAAAAAAA,0xAAAAAAAA,0x80AA00A8,0x80808082,0xAAAAAAAA,0xAAAAAAAA,
    0x02080200,0x020A020A,0xAAAAAAAA,0xAAAAAAAA,0xAA0AAA0A,0xAA00AA0A,0xAAAAAAAA,0x2AAAAAAA,
};

const uint16 DemoBadgeMap[40] __attribute__((aligned(4))) =
{
    0x0000,0x0001,0x0002,0x0003,0x0004,0x0005,0x0006,0x0007,
    0x0008,0x0009,0x000A,0x000B,0x000C,0x000D,0x000E,0x000F,
    0x0010,0x0011,0x0012,0x0013,0x0014,0x0015,0x0016,0x0017,
    0x0018,0x0019,0x001A,0x001B,0x001C,0x001D,0x001E,0x001F,
    0x0020,0x0021,0x0022,0x0023,0x0024,0x0025,0x0026,0x0027,
};

const uint32 DemoBadgeTilesFrameOffsets[4] __attribute__((aligned(4))) =
{
    0x00000001,0x00000029,0x00000051,0x00000079,
};
