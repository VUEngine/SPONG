//---------------------------------------------------------------------------------------------------------
//
//  Checkbox
//  • 16×16 pixels
//  • 4 tiles, unreduced, not compressed
//  • 2×2 map, not compressed
//  Size: 68 + 8 = 76
//
//---------------------------------------------------------------------------------------------------------

const uint32 CheckboxTiles[17] __attribute__((aligned(4))) =
{
    0x00000000,0xFFFC0000,0x00030003,0x00030003,0xFFFC0003,0x000F0000,0x00300030,0x00300030,
    0x000F0030,0x3FFC0000,0xF033C003,0x3FF3FCFC,0xF33C0FC3,0x003F000C,0x0033000F,0x00300030,
    0x000F0030,
};

const uint16 CheckboxMap[4] __attribute__((aligned(4))) =
{
    0x0000,0x0001,0x0002,0x0003,
};
