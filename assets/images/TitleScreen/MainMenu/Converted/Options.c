//---------------------------------------------------------------------------------------------------------
//
//  Options
//  • 64×32 pixels
//  • 32 tiles, unreduced, not compressed
//  • 8×4 map, not compressed
//  Size: 516 + 64 = 580
//
//---------------------------------------------------------------------------------------------------------

const uint32 OptionsTiles[129] __attribute__((aligned(4))) =
{
    0x00000000,
    0xF0000000,0xF000F000,0xF000F000,0xF000F000,0xF3FF0000,0xF3C3F3C3,0xF3C3F3C3,0xF3FFF3C3,
    0xF3FF0000,0x03C303C3,0x000303FF,0x00030003,0xF3FF0000,0xF03FF03F,0xF03FF03F,0xF03FF03F,
    0xFFF30000,0xC3F3C3F3,0xC3F3C3F3,0xFFF3C3F3,0xC3F30000,0xFFF3CFF3,0xF3F3FFF3,0xC3F3C3F3,
    0xFFF30000,0x03F3C3F3,0xC003FFF3,0xFFF3C3F3,0x00030000,0x00000003,0x00030003,0x00030003,
    0xFFF00000,0xC3F0C3F0,0xC3F0C3F0,0xFFF0C3F0,0xFFF30000,0xC3F3C3F3,0x03F3FFF3,0x03F303F3,
    0xFFF30000,0x3F033F03,0x3F003F03,0x3F003F00,0xF3F30000,0xF3F0F3F0,0xF3F0F3F0,0xF3F0F3F0,
    0xF3FF0000,0xF3C3F3C3,0xF3C3F3C3,0xF3FFF3C3,0xF3C30000,0xF3FFF3CF,0xF3F3F3FF,0xF3C3F3C3,
    0xF3FF0000,0xF003F3C3,0xF003F03F,0xF3FFF3C3,0x03C30000,0x03FF03CF,0x03F303FF,0x03C303C3,
    0xFFF00000,0xC3F0C3F0,0xC3F0C3F0,0xFFF0C3F0,0xFFF30000,0xC3F3C3F3,0x03F3FFF3,0x03F303F3,
    0xFFF30000,0x03F3C3F3,0x03F003F3,0xFFF0C3F0,0xF3F30000,0xF3F0F3F3,0xF3F0F3F0,0xF3F3F3F3,
    0xF3FF0000,0xF3C3F3C3,0xF3C3F3C3,0xF3FFF3C3,0xF3C30000,0xF3FFF3CF,0xF3F3F3FF,0xF3C3F3C3,
    0xF3FF0000,0xF003F3C3,0x0003F03F,0xF3FFF3C3,0x03FF0000,0x000303C3,0x03C003FF,0x03FF03C3,
    0xF0000000,0xF000F000,0xF000F000,0xF000F000,0xF3FF0000,0xF3C3F3C3,0xF3C3F3C3,0xF3FFF3C3,
    0xF3FF0000,0x03C303C3,0x000303FF,0x00030003,0xF3FF0000,0xF03FF03F,0xF03FF03F,0xF03FF03F,
    0xFFF30000,0xC3F3C3F3,0xC3F3C3F3,0xFFF3C3F3,0xC3F30000,0xFFF3CFF3,0xF3F3FFF3,0xC3F3C3F3,
    0xFFF30000,0x03F3C3F3,0xC003FFF3,0xFFF3C3F3,0x00030000,0x00000003,0x00030003,0x00030003,
};

const uint16 OptionsMap[32] __attribute__((aligned(4))) =
{
    0x0000,0x0001,0x0002,0x0003,0x0004,0x0005,0x0006,0x0007,
    0x0008,0x0009,0x000A,0x000B,0x000C,0x000D,0x000E,0x000F,
    0x0010,0x0011,0x0012,0x0013,0x0014,0x0015,0x0016,0x0017,
    0x0018,0x0019,0x001A,0x001B,0x001C,0x001D,0x001E,0x001F,
};
